#include <iostream>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) :window(NULL), renderer(NULL) {

	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if(window == NULL) {
		std::cout << "Window failed" << std::endl;
	}

	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) {
	SDL_Texture* texture = nullptr;
	texture = IMG_LoadTexture(renderer, p_filePath);
	if(texture == nullptr)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;
	return texture;
}

void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity) {

	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getX() * 2;
	dst.y = p_entity.getY() * 2;
	dst.w = p_entity.getCurrentFrame().w * 2;
	dst.h = p_entity.getCurrentFrame().h * 2;

	SDL_RenderCopy(renderer,p_entity.getTex(), &src, &dst);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

SDL_Renderer* RenderWindow::getRenderer() {
	return renderer;
}