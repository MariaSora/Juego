#ifndef __GUICONTROLCHECKBOX_H__
#define __GUICONTROLCHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlCheckBox : public GuiControl
{

public:

	GuiControlCheckBox(uint32 id, SDL_Rect bounds);
	virtual ~GuiControlCheckBox();

	// Called each loop iteration
	bool Update(float dt);
	bool isPressed = false; 
	bool isFocused = false; 
	bool no = false; 

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICONTROLCHEKCBOX_H__