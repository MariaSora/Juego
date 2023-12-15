#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"
#include "Pathfinding.h"

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
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	List<Property*> propertyList;

	~Properties()
	{
		//...
		ListItem<Property*>* property;
		property = propertyList.start;

		while (property != NULL)
		{
			RELEASE(property->data);
			property = property->next;
		}

		propertyList.Clear();
	}

	Property* GetProperty(const char* name);

	//List<Property*> list;
};

struct MapLayer
{
	SString	name;
	int id;
	int width;
	int height;
	float parallax;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
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

private:

	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSet(pugi::xml_node mapFile);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);
	bool LoadObject(pugi::xml_node& node, MapObjects* layer);
	bool LoadAllObjectGroup(pugi::xml_node mapNode);
	bool LoadCollisionsObject();
	TileSet* GetTilesetFromTileId(int gid) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	
	void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

	int GetTileWidth();
	int GetTileHeight();
public:

	SString name;
	SString path;
	PhysBody* platform;
	PhysBody* stairs;

	PathFinding* pathfinding;
	PathFinding* pathfinding2;

private:
	MapData mapData; 
	bool mapLoaded;
	MapLayer* navigationLayer;
	int blockedGid = 1;
};

#endif // __MAP_H__