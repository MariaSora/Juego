#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Level2.h"
#include "Physics.h"
#include "Scene.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Level2::Level2(bool startEnabled) : Module(startEnabled), level2Loaded(false)
{
    name.Create("level2");
}

// Destructor
Level2::~Level2()
{}

// Called before render is available
bool Level2::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

bool Level2::Start() {

    if (!active) {
        return true;
    }
    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += name;
    bool ret = Load(mapPath);

    pathfinding = new PathFinding();
    pathfinding4 = new PathFinding(); 
    pathfinding2 = new PathFinding(); 
    pathfinding3 = new PathFinding(); 

    uchar* navigationMap = NULL;
    CreateNavigationMap(level2Data.width, level2Data.height, &navigationMap);
    pathfinding->SetNavigationMap((uint)level2Data.width, (uint)level2Data.height, navigationMap);
    pathfinding4->SetNavigationMap((uint)level2Data.width, (uint)level2Data.height, navigationMap);
    pathfinding2->SetNavigationMap((uint)level2Data.width, (uint)level2Data.height, navigationMap);
    pathfinding3->SetNavigationMap((uint)level2Data.width, (uint)level2Data.height, navigationMap);

    RELEASE_ARRAY(navigationMap); 

    return ret;
}

bool Level2::Update(float dt)
{
    bool ret = true;

    if (level2Loaded == false)
        return false;

    ListItem<Level2Layer*>* mapLayerItem;
    mapLayerItem = level2Data.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties_level2.GetProperty("Draw") != NULL && mapLayerItem->data->properties_level2.GetProperty("Draw")->value
            && (mapLayerItem->data->properties_level2.GetProperty("Parallax") == NULL || mapLayerItem->data->properties_level2.GetProperty("Parallax")->value == false)) {

            iPoint playerPos = app->scene->GetPlayer()->position;
            int xToTiledLeft = MAX((playerPos.x / 16) - 30, 0);
            int xToTiledRight = MIN((playerPos.x / 16) + 30, mapLayerItem->data->width);

            for (int x = xToTiledLeft; x < xToTiledRight; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet_level2* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = Level2ToWorld(x, y);


                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);
                }
            }
        }

        if (mapLayerItem->data->properties_level2.GetProperty("Parallax") != NULL && mapLayerItem->data->properties_level2.GetProperty("Parallax")->value)
        {
    
            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet_level2* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = Level2ToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r,
                        mapLayerItem->data->parallax);
                   
                }
            } 
          
        }
        mapLayerItem = mapLayerItem->next;

    }
    app->fonts->drawText(std::to_string(app->scene->player->points).c_str(), { 0,0,0,255 }, (-app->render->camera.x * app->scene->speedUI) + 420, 10, app->fonts->font);
 
    return true;

}

iPoint Level2::Level2ToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * level2Data.tileWidth;
    ret.y = y * level2Data.tileHeight;

    return ret;
}

iPoint Level2::WorldToLevel2(int x, int y)
{
    iPoint ret(0, 0);

    ret.x = x / level2Data.tileWidth;
    ret.y = y / level2Data.tileHeight;

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet_level2::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet_level2* Level2::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet_level2*>* item = level2Data.tilesets.start;
    TileSet_level2* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Level2::CleanUp()
{
    if (!active) {
        return true;
    }

    LOG("Unloading map");

    pathfinding->CleanUp();
    pathfinding2->CleanUp();

    ListItem<TileSet_level2*>* item;
    item = level2Data.tilesets.start;

    while (item != NULL)
    {
        RELEASE(item->data);
        item = item->next;
    }
    level2Data.tilesets.Clear();

    // Remove all layers
    ListItem<Level2Layer*>* layerItem;
    layerItem = level2Data.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data->data);
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Level2::Load(SString mapFileName)
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());
 
    if (result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName.GetString(), result.description());
        ret = false;
    }

    if (ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }

    if (ret == true)
    {
        ret = LoadAllObjectGroup(mapFileXML.child("map"));
    }

    //Dibuja las colisiones en el mapa
    if (ret == true) {
        ret = LoadCollisionsObject();
    }

    if (ret == true)
    {
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d", level2Data.width, level2Data.height);
        LOG("tile_width : %d tile_height : %d", level2Data.tileWidth, level2Data.tileHeight);

        LOG("Tilesets----");

        ListItem<TileSet_level2*>* tileset;
        tileset = level2Data.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d", tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        LOG("Layers----");

        ListItem<Level2Layer*>* mapLayer;
        mapLayer = level2Data.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    // Find the navigation layer
        ListItem<Level2Layer*>* mapLayerItem;
        mapLayerItem = level2Data.maplayers.start;
        navigationLayer = mapLayerItem->data;

        //Search the layer in the map that contains information for navigation
        while (mapLayerItem != NULL) {
            if (mapLayerItem->data->properties_level2.GetProperty("Navigation") != NULL && mapLayerItem->data->properties_level2.GetProperty("Navigation")->value) {
                navigationLayer = mapLayerItem->data;
                break;
            }
            mapLayerItem = mapLayerItem->next;
        }


    if (mapFileXML) mapFileXML.reset();

    level2Loaded = ret;

    return ret;
}

bool Level2::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        level2Data.height = map.attribute("height").as_int();
        level2Data.width = map.attribute("width").as_int();
        level2Data.tileHeight = map.attribute("tileheight").as_int();
        level2Data.tileWidth = map.attribute("tilewidth").as_int();
        level2Data.type = LEVEL2TYPE_UNKNOWN;
    }

    return ret;
}

bool Level2::LoadTileSet(pugi::xml_node mapFile) {

    bool ret = true;

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet_level2* set = new TileSet_level2();

        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        SString texPath = path;
        texPath += tileset.child("image").attribute("source").as_string();
        set->texture = app->tex->Load(texPath.GetString());

        level2Data.tilesets.Add(set);
    }

    return ret;
}

bool Level2::LoadLayer(pugi::xml_node& node, Level2Layer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();
    if (node.attribute("parallaxx"))
    {
        layer->parallax = node.attribute("parallaxx").as_float();
    }
    else
    {
        layer->parallax = 1.0f;
    }

    LoadProperties(node, layer->properties_level2);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

bool Level2::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        Level2Layer* mapLayer = new Level2Layer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        level2Data.maplayers.Add(mapLayer);
    }

    return ret;
}

bool Level2::LoadObject(pugi::xml_node& node, Level2Objects* mapObjects)
{
    bool ret = true;

    //Load the attributes
    mapObjects->id = node.attribute("id").as_int();
    mapObjects->name = node.attribute("name").as_string();
    mapObjects->width = node.attribute("width").as_int();
    mapObjects->width = node.attribute("height").as_int();
    mapObjects->x = node.attribute("x").as_int();
    mapObjects->y = node.attribute("y").as_int();

    LoadProperties(node, mapObjects->properties_level2);

    ////Reserve the memory for the data 
    //layer->data = new uint[layer->width * layer->height];
    //memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node object;
    int i = 0;
    for (object = node.child("object"); object && ret; object = object.next_sibling("object"))
    {
        Level2Object* objectAux = new Level2Object{
            object.attribute("id").as_uint(),
            object.attribute("x").as_uint(),
            object.attribute("y").as_uint(),
            object.attribute("width").as_uint(),
            object.attribute("height").as_uint(),
            List<uint>()

        };

        mapObjects->objects.Add(objectAux);

        i++;
    }

    return ret;
}

bool Level2::LoadAllObjectGroup(pugi::xml_node mapNode)
{
    bool ret = true;

    for (pugi::xml_node objectNode = mapNode.child("objectgroup"); objectNode && ret; objectNode = objectNode.next_sibling("objectgroup"))
    {
        //Load the layer
        Level2Objects* level2Objects = new Level2Objects();
        ret = LoadObject(objectNode, level2Objects);

        //add the layer to the map
        level2Data.mapObjects.Add(level2Objects);
    }

    return ret;
}

bool Level2::LoadCollisionsObject()
{
    ListItem<Level2Objects*>* mapObjectsItem;
    mapObjectsItem = level2Data.mapObjects.start;
    bool ret = true;


    while (mapObjectsItem != NULL) { //Capa de objetos
        for (int i = 0; i < mapObjectsItem->data->objects.Count(); i++) { //Cada objeto individual

            Level2Object* object = mapObjectsItem->data->objects[i];

            if (mapObjectsItem->data->id == 8) //normal floor
            {
                PhysBody* c1 = app->physics->CreateRectangle(object->x + object->width / 2, object->y + object->height / 2, object->width, object->height, STATIC);
                c1->ctype = ColliderType::PLATFORM;
                platform = c1;
            }

        }
        mapObjectsItem = mapObjectsItem->next;
    }


    return ret;
}

bool Level2::LoadProperties(pugi::xml_node& node, Properties_level2& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties_level2::Property_level2* p = new Properties_level2::Property_level2();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.propertyList.Add(p);
    }

    return ret;
}

Properties_level2::Property_level2* Properties_level2::GetProperty(const char* name)
{
    ListItem<Property_level2*>* item = propertyList.start;
    Property_level2* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}

int Level2::GetTileWidth() {
    return level2Data.tileWidth;
}

int Level2::GetTileHeight() {
    return level2Data.tileHeight;
}

// L13: Create navigationMap map for pathfinding
void Level2::CreateNavigationMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;

    //Sets the size of the map. The navigation map is a unidimensional array 
    uchar* navigationMap = new uchar[navigationLayer->width * navigationLayer->height];
    //reserves the memory for the navigation map
    memset(navigationMap, 1, navigationLayer->width * navigationLayer->height);

    for (int x = 0; x < level2Data.width; x++)
    {
        for (int y = 0; y < level2Data.height; y++)
        {
            //i is the index of x,y coordinate in a unidimensional array that represents the navigation map
            int i = (y * navigationLayer->width) + x;

            //Gets the gid of the map in the navigation layer
            int gid = navigationLayer->Get(x, y);

            //If the gid is a blockedGid is an area that I cannot navigate, so is set in the navigation map as 0, all the other areas can be navigated
            //!!!! make sure that you assign blockedGid according to your map
            if (gid == blockedGid) navigationMap[i] = 0;
            else navigationMap[i] = 1;
        }
    }

    *buffer = navigationMap;
    width = level2Data.width;
    height = level2Data.height;

}