#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "GuiControlSlider.h"

GuiControlSlider::GuiControlSlider(uint32 id, SDL_Rect bounds, int minValue, int maxValue) : GuiControl(GuiControlType::SLIDER, id), minValue(minValue), maxValue(maxValue), currentValue(minValue)
{
	this->bounds = bounds;

	canClick = true;
	drawBasic = false;

	initialPos.x = bounds.x;
	initialPos.x = bounds.y;
}

GuiControlSlider::~GuiControlSlider()
{

}

bool GuiControlSlider::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);
		SDL_Rect rect = { mouseX, bounds.y, bounds.w, bounds.y };

		//If the position of the mouse is inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				//mover posición boton respecto la posición del ratón
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		//L15: DONE 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawRectangle(rect, 200, 200, 200, 255, true, false);
			break;
		case GuiControlState::NORMAL:
			app->render->DrawRectangle(rect, 255, 250, 246, 255, true, false);
			break;
		case GuiControlState::FOCUSED:
			app->render->DrawRectangle(rect, 255, 165, 236, 255, true, false);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawRectangle(rect, 255, 0, 175, 255, true, false);
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
