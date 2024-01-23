#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "GuiControlSlider.h"
#include "Log.h"

GuiControlSlider::GuiControlSlider(uint32 id, const char* text, SDL_Rect bounds, int minValue, int maxValue) : GuiControl(GuiControlType::SLIDER, id), minValue(minValue), maxValue(maxValue)
{
	this->bounds = bounds;
	this->text = text; 

	canClick = true;
	drawBasic = false;

	currentPos = bounds.x; 
	currentValue = minValue;
}

GuiControlSlider::~GuiControlSlider()
{

}

bool GuiControlSlider::Update(float dt)
{
	app->render->DrawText(text.GetString(), bounds.x - 110, bounds.y-10, 80, 30, { 0,0,0 });
	if (state != GuiControlState::DISABLED)
	{
		LOG("x rect2: %d", rect2.x);
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);
		rect2 = {currentPos, bounds.y - 5, bounds.w - 100, bounds.h + 10 };
		//If the position of the mouse is inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
		
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				isPressed = true;
				currentValue = int(((maxValue - minValue) * ((mouseX - bounds.x) / float(bounds.w))) + minValue);
				currentPos = mouseX; 
				rect2 = { mouseX, bounds.y - 5, bounds.w - 100, bounds.h + 10 };
				state = GuiControlState::PRESSED;
				LOG("current: %d", currentValue);
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				isPressed = false;
				NotifyObserver();
			}
		}
		else {
			isPressed = false;
			state = GuiControlState::NORMAL;

		}

		//L15: DONE 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawRectangle(bounds, 255, 255, 255, 255, true, false);
			app->render->DrawRectangle(rect2, 200, 200, 200, 255, true, false);
			break;
		case GuiControlState::NORMAL:
			app->render->DrawRectangle(bounds, 255, 255, 255, 255, true, false);
			app->render->DrawText(std::to_string(currentValue).c_str(), bounds.x + bounds.w + 15, bounds.y - 2, 20, 15, { 0,0,0 });
			app->render->DrawRectangle(rect2, 0, 0, 0, 255, true, false);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawRectangle(bounds, 255, 255, 255, 255, true, false);
			app->render->DrawText(std::to_string(currentValue).c_str(), bounds.x + bounds.w + 15, bounds.y - 2, 20, 15, { 0,0,0 });
			app->render->DrawRectangle(rect2, 100, 255, 137, 255, true, false);
			break;
		}

		//app->render->DrawText(text.GetString(), bounds.x + 5, bounds.y + 2, bounds.w - 10, bounds.h - 3);

	}

	return false;
}

bool GuiControlSlider::Draw()
{
	return false;
}
