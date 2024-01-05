#ifndef __FONTS_H__
#define __FONTS_H__

#include "Module.h"
#include "SDL_ttf/include/SDL_ttf.h"

class Fonts : public Module
{
public:
	Fonts();
	~Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before quitting
	bool CleanUp();

	bool drawText(const char* string, SDL_Color color, int x, int y);

public:
	/*typedef struct text {
		int x;
		int y;
		SDL_Texture* texture;
	}text;

	p2List<text*> text_list;*/

	TTF_Font* font;
	SDL_Texture* texture;
};

#endif // __FONTS_H__