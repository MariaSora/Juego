#include "App.h"

#include "Defs.h"
#include "Log.h"
#include "Fonts.h"

#include "SDL_ttf/include/SDL_ttf.h"
#pragma comment(lib, "../Game/Source/External/SDL_ttf/libx86/SDL2_ttf.lib")

Fonts::Fonts() : Module()
{
	name.Create("fonts");
}

Fonts::~Fonts()
{
}

bool Fonts::Awake(pugi::xml_node& config)
{
	LOG("Init ttf library");
	bool ret = true;

	// load support for the PNG image format
	//int flags = TTF_HINTING_MONO;
	int init = TTF_Init();

	//if ((init & flags) != flags)
	if (init < 0)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", TTF_GetError());
		ret = false;
	}
	else
	{
		font = TTF_OpenFont("../Output/Assets/Fonts/ds-digib.ttf", 30);
		if (font == NULL)
		{
			LOG("Couldn't load %d pt font from %s: %s\n",
				18, "Output/Assets/Manrope-VariableFont_wght.ttf", SDL_GetError());
			ret = false;
		}
		TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
		TTF_SetFontOutline(font, 0);
		TTF_SetFontKerning(font, 0);
		TTF_SetFontHinting(font, TTF_HINTING_LIGHT_SUBPIXEL);
	}

	return ret;
}

bool Fonts::CleanUp()
{
	LOG("Freeing fonts and ttf library");

	TTF_CloseFont(font);
	TTF_Quit();

	return false;
}

bool Fonts::drawText(const char* string, SDL_Color color, int x, int y)
{
	SDL_Surface* text_surface = NULL;
	text_surface = TTF_RenderText_Solid(font, string, color);

	texture = SDL_CreateTextureFromSurface(app->render->renderer, text_surface);
	app->render->DrawTexture(texture, x, y);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(texture);
	return true;
}