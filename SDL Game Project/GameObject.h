#pragma once
#include <SDL.h>
#include "Vector.h"

class GameObject
{

public:
	SDL_Renderer* renderer;
	
	Vector pos; //xy position
	Vector velocity; //how fast and where to move from this pos

	GameObject();
	~GameObject();

	void setPosition(Vector pos);
	void setVelocity(Vector pos);

	virtual void update(float dt);
	virtual void updateMovement(float dt);
	virtual void draw();
};

