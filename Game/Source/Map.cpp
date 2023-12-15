#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

bool Map::Start() {

    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += name;
    bool ret = Load(mapPath);

    pathfinding = new PathFinding();
    pathfinding2 = new PathFinding(); 

    uchar* navigationMap = NULL;
    CreateNavigationMap(mapData.width, mapData.height, &navigationMap); 
    pathfinding->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap); 
    pathfinding2->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap); 
    RELEASE_ARRAY(navigationMap); 

    return ret;
}

bool Map::Update(float dt)
{
    bool ret = true;

    if (mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value
            && (mapLayerItem->data->properties.GetProperty("Parallax") == NULL || mapLayerItem->data->properties.GetProperty("Parallax")->value == false)) {

            iPoint playerPos = app->scene->GetPlayer()->position;
            int xToTiledLeft = MAX((playerPos.x / 16) - 30, 0);
            int xToTiledRight = MIN((playerPos.x / 16) + 30, mapLayerItem->data->width);

            for (int x = xToTiledLeft; x < xToTiledRight; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);
                }
            }
        }

        if (mapLayerItem->data->properties.GetProperty("Parallax") != NULL && mapLayerItem->data->properties.GetProperty("Parallax")->value)
        {
    
            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

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

    return true;
}

iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    ret.x = x / mapData.tileWidth;
    ret.y = y / mapData.tileHeight;

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

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
bool Map::CleanUp()
{
    LOG("Unloading map");

    pathfinding->CleanUp();
    pathfinding2->CleanUp();

    ListItem<TileSet*>* item;
    item = mapData.tilesets.start;

    while (item != NULL)
    {
        RELEASE(item->data);
        item = item->next;
    }
    mapData.tilesets.Clear();

    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data->data);
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
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
        LOG("width : %d height : %d", mapData.width, mapData.height);
        LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d", tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        LOG("Layers----");

        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    // Find the navigation layer
        ListItem<MapLayer*>* mapLayerItem;
        mapLayerItem = mapData.maplayers.start;
        navigationLayer = mapLayerItem->data;

        //Search the layer in the map that contains information for navigation
        while (mapLayerItem != NULL) {
            if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value) {
                navigationLayer = mapLayerItem->data;
                break;
            }
            mapLayerItem = mapLayerItem->next;
        }


    if (mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMap(pugi::xml_node mapFile)
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
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
        mapData.type = MAPTYPE_UNKNOWN;
    }

    return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile) {

    bool ret = true;

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

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

        mapData.tilesets.Add(set);
    }

    return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
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

    LoadProperties(node, layer->properties);

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

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

bool Map::LoadObject(pugi::xml_node& node, MapObjects* mapObjects)
{
    bool ret = true;

    //Load the attributes
    mapObjects->id = node.attribute("id").as_int();
    mapObjects->name = node.attribute("name").as_string();
    mapObjects->width = node.attribute("width").as_int();
    mapObjects->width = node.attribute("height").as_int();
    mapObjects->x = node.attribute("x").as_int();
    mapObjects->y = node.attribute("y").as_int();

    LoadProperties(node, mapObjects->properties);

    ////Reserve the memory for the data 
    //layer->data = new uint[layer->width * layer->height];
    //memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node object;
    int i = 0;
    for (object = node.child("object"); object && ret; object = object.next_sibling("object"))
    {
        MapObject* objectAux = new MapObject{
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

bool Map::LoadAllObjectGroup(pugi::xml_node mapNode)
{
    bool ret = true;

    for (pugi::xml_node objectNode = mapNode.child("objectgroup"); objectNode && ret; objectNode = objectNode.next_sibling("objectgroup"))
    {
        //Load the layer
        MapObjects* mapObjects = new MapObjects();
        ret = LoadObject(objectNode, mapObjects);

        //add the layer to the map
        mapData.mapObjects.Add(mapObjects);
    }

    return ret;
}

bool Map::LoadCollisionsObject()
{
    ListItem<MapObjects*>* mapObjectsItem;
    mapObjectsItem = mapData.mapObjects.start;
    bool ret = true;


    while (mapObjectsItem != NULL) { //Capa de objetos
        for (int i = 0; i < mapObjectsItem->data->objects.Count(); i++) { //Cada objeto individual

            MapObject* object = mapObjectsItem->data->objects[i];

            if (mapObjectsItem->data->id == 8) //normal floor
            {
                PhysBody* c1 = app->physics->CreateRectangle(object->x + object->width / 2, object->y + object->height / 2, object->width, object->height, STATIC);
                c1->ctype = ColliderType::PLATFORM;
                platform = c1;
            }

            if (mapObjectsItem->data->id == 10) //stairs
            {
                PhysBody* c2 = app->physics->CreateRectangle(object->x + object->width / 2, object->y + object->height / 2, object->width, object->height, STATIC);
                c2->ctype = ColliderType::STAIRS;
                c2->body->GetFixtureList()[0].SetSensor(true);
                stairs = c2;
            }

        }
        mapObjectsItem = mapObjectsItem->next;
    }


    return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.propertyList.Add(p);
    }

    return ret;
}

Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = propertyList.start;
    Property* p = NULL;

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

int Map::GetTileWidth() {
    return mapData.tileWidth;
}

int Map::GetTileHeight() {
    return mapData.tileHeight;
}

// L13: Create navigationMap map for pathfinding
void Map::CreateNavigationMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;

    //Sets the size of the map. The navigation map is a unidimensional array 
    uchar* navigationMap = new uchar[navigationLayer->width * navigationLayer->height];
    //reserves the memory for the navigation map
    memset(navigationMap, 1, navigationLayer->width * navigationLayer->height);

    for (int x = 0; x < mapData.width; x++)
    {
        for (int y = 0; y < mapData.height; y++)
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
    width = mapData.width;
    height = mapData.height;

}