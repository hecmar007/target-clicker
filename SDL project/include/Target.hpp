#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>

#include "Entity.hpp"

//The class Targets inherits from Entity, as a target is an Entity where time when it was created is also
//tracked to check when to delete it
class Target : public Entity
{
public:
	Target(float p_x, float p_y, SDL_Texture* p_tex);
	std::chrono::system_clock::time_point getCreated();
private:
	std::chrono::system_clock::time_point time_created;
};