#pragma once
#include "GameObject.h"
#include "Animation.h"

class Hero : public GameObject
{
public:
	Animation* animation;
	bool faceRight;

	Hero();
	~Hero();

	//override update n draw
	void update(float dt);
	void draw();
};

