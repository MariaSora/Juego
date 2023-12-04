#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Pathfinding.h"
#include "DynArray.h"
#include "Queue.h"

#include "PugiXml\src\pugixml.hpp"

// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;
	SDL_Rect GetRect(uint gid) const
	{
		SDL_Rect rect = { 0 };
		int relativeIndex = gid - firstgid;

		rect.w = tileWidth;
		rect.h = tileHeight;
		rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
		rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

		return rect;
	}
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	Property* GetProperty(const char* name);

	List<Property*> list;
};

struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	float parallax;
	uint* data;
	uint* tiles;
	Properties properties;

	uint Get(int x, int y) const
	{
		return tiles[(y * width) + x];
	}
};


struct MapObject {

	uint id;
	uint x;
	uint y;
	uint width;
	uint height;
	List<uint> point;

};

struct MapObjects
{
	SString    name;
	int id;
	int x;
	int y;
	int width;
	int height;
	List<MapObject*> objects;

	Properties properties;

};

struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
	MapTypes type;


	List<MapObjects*> mapObjects;
	List<MapLayer*> maplayers;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
	bool Load(SString mapFileName);

	iPoint MapToWorld(int x, int y) const;
	iPoint Map::WorldToMap(int x, int y);

	void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSet(pugi::xml_node mapFile);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);
	bool LoadObject(pugi::xml_node& node, MapObjects* layer);
	bool LoadAllObjectGroup(pugi::xml_node mapNode);
	bool LoadCollisionsObject();
	bool PortalZone();
	TileSet* GetTilesetFromTileId(int gid) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public: 

	MapData mapData;
	SString name;
	SString path;
	PhysBody* platform;
	PhysBody* stairs;
	PathFinding* pathfinding; 

private:
	MapLayer* navigationLayer;
	bool mapLoaded;
	int blockedGid = 49;
};

#endif // __MAP_H__