#include "GuiControlCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiControlCheckBox::GuiControlCheckBox(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;

	canClick = true;
	drawBasic = false;
}

GuiControlCheckBox::~GuiControlCheckBox()
{

}

bool GuiControlCheckBox::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse is inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && isPressed && !no) {
				NotifyObserver();
				isPressed = false;
				no = false; 
				state = GuiControlState::NORMAL;
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && isPressed && no) {
				state = GuiControlState::PRESSED;
				no = false; 
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && !isPressed && no) {
				isPressed = true;
				no = true;
				state = GuiControlState::PRESSED;
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !isPressed && !no) {
				state = GuiControlState::NORMAL; 
				no = true; 
			}
		}

		//L15: DONE 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
			break;
		case GuiControlState::NORMAL:
			app->render->DrawRectangle(bounds, 255, 250, 250, 255, true, false);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawRectangle(bounds, 135, 0, 255, 255, true, false);
			break;
		}

	}

	return false;
}

