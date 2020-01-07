#include "Hero.h"



Hero::Hero()
{
	animation = NULL;
	faceRight = true;
}


Hero::~Hero()
{
}

void Hero::update(float dt)
{
	
	//use basic game object vector movement
	updateMovement(dt);

	//update facing directio based on velocity
	if (velocity.x > 0)
		faceRight = true;
	if (velocity.x < 0)
		faceRight = false;

	//update our animation
	if(velocity.x != 0 || velocity.y != 0)
		animation->update(dt);
}

void Hero::draw()
{
	if (animation != NULL) {
		if (faceRight)
			animation->draw(pos.x, pos.y);
		else
			animation->draw(pos.x, pos.y, true);
	}
}
