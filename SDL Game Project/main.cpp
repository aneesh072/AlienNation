#include <SDL.h>
#include <SDL_image.h>//useful image load and processing functions. Supports lots of image types e.g png,jpg
#include <iostream>
#include <string>
#include "Animation.h"
#include "GameObject.h"
#include "Hero.h"
#include <list>
#include "KeyboardHandler.h"

using namespace std;

//for SDL to work, need old 'main' signature, not void main()
int main(int argc, char **argv)
{
	//initialise SDL and its subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		return -1; //exit program early
	}
	else
	{
		cout << "SDL initialised!!!" << endl;
	}
	//Initialise SDL image
	if (!(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
	{
		//if didn't work, report error
		cout << "SDL IMAGE ERROR: " << IMG_GetError() << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//params: 
	//	window title, x and y pos, width, height, flags for creation
	SDL_Window* window = SDL_CreateWindow("Alien Nation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		600, 700, SDL_WINDOW_SHOWN);//|SDL_WINDOW_FULLSCREEN

	if (window != NULL) {
		cout << "window created!" << endl;
	}
	else
	{
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		SDL_Quit(); //cleanup SDL memory
		return -1; //exit the program
	}

	//create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		SDL_Quit(); //cleanup SDL memory
		return -1; //exit the program
	}

	//LOAD UP ASSETS TO DRAW
	//load image up as a surface. Surfaces are stored in RAM so we can access their pixel data from code
	SDL_Surface* bgSurface = SDL_LoadBMP("assets/BG.bmp");
	//convert surface to texture. Textures are stored in gfx cards vram and used for Hardware rendering (yay!)
	//we cannot directly access the pixel data of a texture
	SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
	//dont need surface anymore, free memory up
	SDL_FreeSurface(bgSurface);

	//the region of the image that will be drawn
	SDL_Rect sourceRectangle;
	//get width and height from texture and store them into our rectangle
	//params: texture to query, format pointer, access pointer, width pointer, height pointer
	SDL_QueryTexture(bgTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h);
	sourceRectangle.x = 0;
	sourceRectangle.y = 0;
	sourceRectangle.w = 600;
	sourceRectangle.h = 700;

	//drawing the source rectangle region to fit into this rectangle
	//includes scaling up and down
	SDL_Rect destinationRectangle;
	destinationRectangle.x = 0;
	destinationRectangle.y = 0;
	destinationRectangle.w = sourceRectangle.w;
	destinationRectangle.h = sourceRectangle.h;



	//LOAD Run Sprite Sheet
	SDL_Texture* runSpriteSheet = IMG_LoadTexture(renderer, "assets/hero.bmp");
	//this time source rect is copying 1 frame at a time
	SDL_Rect runClipRect = { 150,150,200,200 }; //x:0, y:0, w:32, h: 32
	SDL_Rect runDestRect = { 150,150, 200, 200 };

	//LOAD run surface up so we can colour key
	SDL_Surface* runSurface = IMG_Load("assets/hero.bmp");
	//set colour key( this colour goes fully transparent)
	//params: surface, set or unset(1/0), uint for colour (using mapRGB to find index of this colour in image)

	SDL_SetColorKey(runSurface, 1, SDL_MapRGB(runSurface->format, 255, 255, 255));
	//turn into texture
	SDL_Texture* runSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, runSurface);
	//clean up surface
	SDL_FreeSurface(runSurface);


	Animation anim1(runSpriteSheet, renderer, 4, 32, 32, 0.2);//0.2 = 200millisecond frame duration
	Animation anim2(runSpriteSheetWithNoBG, renderer, 4, 32, 32, 0.8);
	Animation anim3(runSpriteSheet, renderer, 4, 32, 32, 0.05);

	//SETUP INITIAL GAME OBJECTS
	list<GameObject*> gameObjects;
	//build a hero
	Hero* hero = new Hero();
	hero->animation = &anim2;
	hero->pos.x = 200;
	hero->pos.y = 200;
	hero->velocity.x = 200;
	hero->renderer = renderer;
	//add to the list
	gameObjects.push_back(hero);

	//INPUT HANDLERS
	KeyboardHandler keyboardHandler;
	keyboardHandler.hero = hero;

	//set last update to current number of milliseconds since start of game
	Uint32 lastUpdate = SDL_GetTicks();

	bool loop = true;
	//GAME LOOP
	while (loop) {
		//TIME MANAGEMENT STUFF
		//difference of current time minus the last update time
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert timeDiff to deltaTime (timeDiff as a fraction of a second)
		float DT = timeDiff / 1000.0;//e.g timeDiff = 200ms, then 200/1000 = 0.2 for DT
		//we want to measure timeDiff between gameLoop iterations, so we need to update lastUpdate
		lastUpdate = SDL_GetTicks();

		//set render draw colour with RGBA value (values between 0-255)
		SDL_SetRenderDrawColor(renderer, 255, 0, 168, 255);
		//clear the screen with current draw colour
		SDL_RenderClear(renderer);

		//change render draw colour
		SDL_SetRenderDrawColor(renderer, 0, 228, 255, 255);
		//make a rectangle to draw to the screen (x,y,w,h)
		SDL_Rect rect = { 100, 50, 300, 240 };

		//draw rect to screen with current draw colour
		SDL_RenderFillRect(renderer, &rect);

		//draw knight image
		SDL_RenderCopy(renderer, bgTexture, &sourceRectangle, &destinationRectangle);

		//render glob to screen
		SDL_RenderCopy(renderer, runSpriteSheet, &runClipRect, &runDestRect);
		int numberOfFrames = 4;
		//quick animation algorithm. each frame runs for 100ms on the screen
		runClipRect.x = runClipRect.w*((SDL_GetTicks() / 100) % numberOfFrames);

		//ANIMATIONS
		/*anim1.update(DT);
		anim2.update(DT);
		anim3.update(DT);
		anim1.draw(50, 50);
		anim2.draw(50, 100, 4.0f);
		anim3.draw(50, 250, true);*/

		//INPUT HANDLING
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			//check if user clicked close on window
			if (e.type == SDL_QUIT) {
				loop = false;
			}
			//check if user pressed a button
			if (e.type == SDL_KEYDOWN) {
				//see if ESC key pressed
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					loop = false;
				}
			}

			keyboardHandler.update(&e);
		}

		keyboardHandler.updateHeld();

		//UPDATE AND DRAW ALL GAME OBJECTS
		for each (GameObject* gObj in gameObjects)
		{
			gObj->update(DT);
			gObj->draw();
		}

		//swap the buffers, show our current frame we've been drawing
		SDL_RenderPresent(renderer);

		////sdl_getTicks returns number of milliseconds since start of program running
		//if (SDL_GetTicks() > 5000)
		//{
		//	loop = false;
		//}
	}
	//wait for 5 seconds (5000ms)
	//SDL_Delay(5000);

	//CLEANUP
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();



	system("pause");
	return 0;
}