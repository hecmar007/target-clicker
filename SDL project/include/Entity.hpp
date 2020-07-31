#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>

//Everything we spawn is an entity
class Entity {
public:
	Entity(float p_x, float p_y, SDL_Texture* p_tex);
	float getX();
	float getY();
	SDL_Texture* getTex();
	SDL_Rect getCurrentFrame();
	bool isDeleted();
	void setDeleted(bool p_deleted);
private:
	float x, y;
	SDL_Rect currentFrame;
	SDL_Texture* tex;
	bool deleted;
}; 