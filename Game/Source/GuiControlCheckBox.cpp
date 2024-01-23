#include "GuiControlCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"

GuiControlCheckBox::GuiControlCheckBox(uint32 id, const char* text, SDL_Rect bounds) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text; 

	canClick = true;
	drawBasic = false;
}

GuiControlCheckBox::~GuiControlCheckBox()
{

}

bool GuiControlCheckBox::Update(float dt)
{
	app->render->DrawText(text.GetString(), bounds.x - 190, bounds.y, 150, 30, { 0,0,0 });
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse is inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && isChecked && !no) {
				LOG("NOT PRESSED: %d", isChecked);
				NotifyObserver();
				isChecked = false;
				no = false; 
				state = GuiControlState::NORMAL;
			}
			else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && isChecked && no) {
				LOG("PRESSED: %d", isChecked);
				isChecked = true;
				no = false; 
				state = GuiControlState::PRESSED;
			}
			else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && !isChecked && no) {
				LOG("PRESSED: %d", isChecked);
				isChecked = true;
				no = true;
				state = GuiControlState::PRESSED;
			}
			else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !isChecked && !no) {
				LOG("NOT PRESSED: %d", isChecked);
				NotifyObserver();
				isChecked = false;
				no = true; 
				state = GuiControlState::NORMAL;
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
			app->render->DrawRectangle(bounds, 100, 255, 137, 255, true, false);
			break;
		}

	}

	return false;
}

