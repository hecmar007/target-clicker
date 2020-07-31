#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>

#include "Entity.hpp"

//This class handles all the window/renderer stuff
class RenderWindow 
{
public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void cleanUp();
	void clear();
	void render(Entity& p_entity);
	void display();
	SDL_Renderer* getRenderer();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};