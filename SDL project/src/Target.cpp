#include "Target.hpp"

Target::Target(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	time_created = std::chrono::high_resolution_clock::now();
}

std::chrono::system_clock::time_point Target::getCreated() {
	return time_created;
}