#include "Entity.hpp"

Entity::Entity(float p_x, float p_y, SDL_Texture* p_tex) : x(p_x), y(p_y), tex(p_tex) {
	deleted = false;
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 32;
	currentFrame.h = 32;
}

//Getters and setters
float Entity::getX() {
	return x;
}

float Entity::getY() {
	return y;
}

SDL_Texture* Entity::getTex() {
	return tex;
}

SDL_Rect Entity::getCurrentFrame() {
	return currentFrame;
}

bool Entity::isDeleted() {
	return deleted;
}

void Entity::setDeleted(bool p_deleted) {
	deleted = p_deleted;
}