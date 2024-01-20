#ifndef __GUICONTROLPOPUP_H__
#define __GUICONTROLPOPUP_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlPopUp : public GuiControl
{

public:

	GuiControlPopUp(uint32 id);
	virtual ~GuiControlPopUp();

	// Called each loop iteration
	bool Update(float dt);

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICONTROLPOPUP_H__