#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse is inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			isFocused = true;
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				isPressed = true;
				state = GuiControlState::PRESSED;
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
				isPressed = false;
			}
		}
		else {
			state = GuiControlState::NORMAL;
			isFocused = false;
			isPressed = false;
		}

		//L15: DONE 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
			app->render->DrawText(text.GetString(), bounds.x + 5, bounds.y + 2, bounds.w - 10, bounds.h - 3, { 200,200,236 });
			break;
		case GuiControlState::NORMAL:
		/*	app->render->DrawRectangle(bounds, 255, 250, 246, 255, true, false);*/
			app->render->DrawText(text.GetString(), bounds.x + 5, bounds.y + 2, bounds.w - 10, bounds.h - 3, {0,0,0});
	
			break;
		case GuiControlState::FOCUSED:
	/*		app->render->DrawRectangle(bounds, 255, 165, 236, 255, true, false);*/
			app->render->DrawText(text.GetString(), bounds.x + 5, bounds.y + 2, bounds.w - 10, bounds.h - 3, {255,165,236});
			break;
		case GuiControlState::PRESSED:
		/*	app->render->DrawRectangle(bounds, 255, 0, 175, 255, true, false);*/
			app->render->DrawText(text.GetString(), bounds.x + 5, bounds.y + 2, bounds.w - 10, bounds.h - 3, { 255,0,175 });
			break;
		}

		//app->render->DrawText(text.GetString(), bounds.x + 5, bounds.y + 2, bounds.w - 10, bounds.h - 3);

	}

	return false;
}

