#pragma once
#include <SDL.h>
#include "Hero.h"

class InputHandler
{
public:
	Hero* hero;

	InputHandler();
	~InputHandler();

	//abstract function
	//NOTE: abstract function has no func body and makes class abstract
	virtual void update(SDL_Event* event) = 0;

	virtual void updateHeld()
	{
		//get sub classes to fill in if appropriate
	}
};

