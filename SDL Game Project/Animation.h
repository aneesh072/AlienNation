#pragma once
#include <SDL.h>

class Animation
{
public:
	//basic animation information
	int numberOfFrames;
	int frameWidth, frameHeight;
	float frameDuration; //how long to stay on screen per frame
	SDL_Texture* spriteSheet; //reference existing texture
	SDL_Renderer* renderer; //reference to our renderer from main

	//current state of animation
	int currentFrame; //which frame is showing
	float frameTimer; //how long has this current frame been on screen

	//constructor
	Animation(SDL_Texture* spriteSheet, SDL_Renderer* renderer, int numberOfFrames, int frameWidth, int frameHeight,float frameDuration);

	void update(float dt);
	void draw(int x, int y);
	void draw(int x, int y, float scale);
	void draw(int x, int y, bool flip);
};

