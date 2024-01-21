#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"
#include "Pathfinding.h"

#include "PugiXml\src\pugixml.hpp"

// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet_level2
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
enum Level2Types
{
	LEVEL2TYPE_UNKNOWN = 0,
	LEVEL2TYPE_ORTHOGONAL,
	LEVEL2TYPE_STAGGERED
};

struct Properties_level2
{
	struct Property_level2
	{
		SString name;
		bool value;
	};

	List<Property_level2*> propertyList;

	~Properties_level2()
	{
		//...
		ListItem<Property_level2*>* property;
		property = propertyList.start;

		while (property != NULL)
		{
			RELEASE(property->data);
			property = property->next;
		}

		propertyList.Clear();
	}

	Property_level2* GetProperty(const char* name);

	//List<Property*> list;
};

struct Level2Layer
{
	SString	name;
	int id;
	int width;
	int height;
	float parallax;
	uint* data;

	Properties_level2 properties_level2;

	Level2Layer() : data(NULL)
	{}

	~Level2Layer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};


struct Level2Object{

	uint id;
	uint x;
	uint y;
	uint width;
	uint height;
	List<uint> point;

};

struct Level2Objects
{
	SString name;
	int id;
	int x;
	int y;
	int width;
	int height;
	List<Level2Object*> objects;

	Properties_level2 properties_level2;

};

struct Level2Data
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet_level2*> tilesets;
	Level2Types type;


	List<Level2Objects*> mapObjects;
	List<Level2Layer*> maplayers;
};

class Level2 : public Module
{
public:

	Level2(bool startEnabled);

	// Destructor
	virtual ~Level2();

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

	iPoint Level2ToWorld(int x, int y) const;
	iPoint Level2::WorldToLevel2(int x, int y);

private:

	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSet(pugi::xml_node mapFile);
	bool LoadLayer(pugi::xml_node& node, Level2Layer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);
	bool LoadObject(pugi::xml_node& node, Level2Objects* layer);
	bool LoadAllObjectGroup(pugi::xml_node mapNode);
	bool LoadCollisionsObject();
	TileSet_level2* GetTilesetFromTileId(int gid) const;
	bool LoadProperties(pugi::xml_node& node, Properties_level2& properties);
	
	void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

	int GetTileWidth();
	int GetTileHeight();
public:

	int level = 1;
	SString name2;
	SString path;
	SString mapPath;
	PhysBody* platform;

	PathFinding* pathfinding;
	PathFinding* pathfinding4;
	PathFinding* pathfinding2;
	PathFinding* pathfinding3;

	bool active = false;
	
private:
	Level2Data level2Data; 
	bool level2Loaded;
	Level2Layer* navigationLayer;
	int blockedGid = 1;

	PhysBody* pbody;
};

#endif // __LEVEL2_H__