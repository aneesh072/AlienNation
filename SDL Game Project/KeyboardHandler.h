#pragma once
#include "InputHandler.h"
class KeyboardHandler :
	public InputHandler
{
public:
	KeyboardHandler();
	~KeyboardHandler();

	//override methods from inputHandler
	void update(SDL_Event* event);
	void updateHeld();
};

