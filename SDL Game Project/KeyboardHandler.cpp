#include "KeyboardHandler.h"



KeyboardHandler::KeyboardHandler()
{
}


KeyboardHandler::~KeyboardHandler()
{
}

void KeyboardHandler::update(SDL_Event * event)
{
	//button presses for our hero
	if (event->type == SDL_KEYDOWN)
	{
		//reset hero position if press R
		if (event->key.keysym.scancode == SDL_SCANCODE_R) {
			hero->pos.x = 200;
			hero->pos.y = 200;
		}
		//TODO add attacks or open menus, or jump or whatever
	}
}

void KeyboardHandler::updateHeld()
{
	//check the state of keyboard keys
	//first we get all states in an array where each key is 1=pressed or 0=not pressed
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	//check individual keys from this point
	//move chara up
	if (keystates[SDL_SCANCODE_UP])
		hero->velocity.y = -80;
	//down
	if (keystates[SDL_SCANCODE_DOWN])
		hero->velocity.y = 80;
	//move chara up
	if (keystates[SDL_SCANCODE_LEFT])
		hero->velocity.x = -80;
	//down
	if (keystates[SDL_SCANCODE_RIGHT])
		hero->velocity.x = 80;
	//check if not holding up or down
	if (!keystates[SDL_SCANCODE_UP] && !keystates[SDL_SCANCODE_DOWN])
		hero->velocity.y = 0;
	//if not holding left or right
	if (!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT])
		hero->velocity.x = 0;


}
