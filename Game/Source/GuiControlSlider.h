#ifndef __GUICONTROLSLIDER_H__
#define __GUICONTROLSLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(uint32 id, SDL_Rect bounds, int minValue, int maxValue);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw();

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	int minValue;
	int maxValue;
	int currentValue;

	iPoint initialPos;
};

#endif // __GUICONTROLBUTTON_H__