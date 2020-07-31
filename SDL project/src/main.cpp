#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>
#include <stdlib.h>
#include <math.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Target.hpp"

//Used to check if a new target at new_x, new_y position collides with any target in the targets vector
bool checkColliding(int new_x, int new_y, std::vector<Target> targets) {
	bool overlapping = false;
	for(auto itr = targets.begin(); itr != targets.end(); ++itr) {
		//If the distance between the two centers is less than 32, the targets will collide
		if(sqrt(pow((*itr).getX()-new_x,2) + pow((*itr).getY()-new_y,2)) <= 32 && !(*itr).isDeleted()) {
			overlapping = true;
			break;
		}
	}
	return overlapping;
}

int main(int argc, char* args[]) {

	//Random initialization
	srand(time(NULL));

	//SDL, TTF and SDL_Image initialization
	if(SDL_Init(SDL_INIT_VIDEO) > 0 || TTF_Init() == -1) {
		std::cout << "Something has gone wrong with SDL or TTF init... " << SDL_GetError() << std::endl;
		return 1;
	}

	if(!IMG_Init(IMG_INIT_PNG)) {
		std::cout << "Something has gone wrong with sdl image... " << SDL_GetError() << std::endl;
		return 2;
	}

	RenderWindow window("Target Clicker", 1280, 720);

	//Initialize all the text
	int score = 0;
	std::string scoreText = "Score: " + std::to_string(score);
	int lives = 3;
	std::string livesText = "Lives left: " + std::to_string(lives);

	SDL_Color White = {255, 255, 255};
	TTF_Font* font = TTF_OpenFont("res/dev/Arrakis.ttf", 24);
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, scoreText.c_str(), White);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(window.getRenderer(), surfaceMessage);

	SDL_Rect Message_rect;
	Message_rect.x = 20;
	Message_rect.y = 0;
	Message_rect.w = 150;
	Message_rect.h = 100;

	SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, livesText.c_str(), White);
	SDL_Texture* Message2 = SDL_CreateTextureFromSurface(window.getRenderer(), surfaceMessage2);

	SDL_Rect Message_rect2;
	Message_rect2.x = 1060;
	Message_rect2.y = 0;
	Message_rect2.w = 200;
	Message_rect2.h = 100;

	SDL_Surface* surfaceMessage3 = TTF_RenderText_Solid(font, "Right click to restart!", White);
	SDL_Texture* Message3 = SDL_CreateTextureFromSurface(window.getRenderer(), surfaceMessage3);

	SDL_Rect Message_rect3; //create a rect
	Message_rect3.x = 340;  //controls the rect's x coordinate 
	Message_rect3.y = 295; // controls the rect's y coordinte
	Message_rect3.w = 600; // controls the width of the rect
	Message_rect3.h = 150; // controls the height of the rect

	SDL_Texture* targetTexture = window.loadTexture("res/gfx/target.png");
	//All the targets in the game are stored in this vector
	std::vector<Target> targets;

	bool gameRunning = true;
	bool isAlive =true;

	SDL_Event event;

	int x, y;
	auto t_start = std::chrono::high_resolution_clock::now();
	int created = 0;
	//Main game loop
	while(gameRunning) {
		//Check for events
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				gameRunning = false;
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && isAlive) {
				for(auto itr = targets.begin(); itr != targets.end(); ++itr) {
					if(!(*itr).isDeleted()) {
						SDL_GetMouseState(&x, &y);
						//If the distance between the click and the target's center is less than 16, then it's a hit
						if(sqrt(pow((*itr).getX()+(*itr).getCurrentFrame().w/2-x/2,2) + pow((*itr).getY()+(*itr).getCurrentFrame().h/2-y/2,2)) <= 16) {
							(*itr).setDeleted(true);
							score++;
							scoreText = "Score: " + std::to_string(score);
							surfaceMessage = TTF_RenderText_Solid(font, scoreText.c_str(), White);
							Message = SDL_CreateTextureFromSurface(window.getRenderer(), surfaceMessage);
						}
					}
				}
			}
			//Restart the game
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT && !isAlive) {
				isAlive = true;
				created = 0;
				t_start = std::chrono::high_resolution_clock::now();
				score = 0;
				lives = 3;

				livesText = "Lives left: " + std::to_string(lives);
				surfaceMessage2 = TTF_RenderText_Solid(font, livesText.c_str(), White);
				Message2 = SDL_CreateTextureFromSurface(window.getRenderer(), surfaceMessage2);
				targets.clear();

				scoreText = "Score: " + std::to_string(score);
				surfaceMessage = TTF_RenderText_Solid(font, scoreText.c_str(), White);
				Message = SDL_CreateTextureFromSurface(window.getRenderer(), surfaceMessage);
			}
		}

		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();

		int attempt_count = 0;

		//Every half a second an attempt to create a new target is made
		if((int)elapsed_time_ms % 500 == 0 && isAlive) {
			created++;
			attempt_count = 0;
			int new_x, new_y;
			do {
				new_x = rand()%(296*2);
				new_y = rand()%(156*2);

				attempt_count++;
			} while(checkColliding(new_x, new_y, targets) && attempt_count < 100);
			// Only spawn one target, and there is a one in five chance to spawn 2 targets
			if(created <=1) {
				targets.push_back(Target(new_x,new_y,targetTexture));
				int random = rand()%(5);
				if(random == 0) {
					new_x = rand()%(296*2);
					new_y = rand()%(156*2);
					if(!checkColliding(new_x, new_y, targets))
						targets.push_back(Target(new_x,new_y,targetTexture));
				}
			}
		}
		else {
			created = 0;
		}

		//If there is no space left to draw in the screen, the game ends
		if(attempt_count >= 100) {
			isAlive = false;
		}

		window.clear();

		for(auto itr = targets.begin(); itr != targets.end(); ++itr) {

			if(!(*itr).isDeleted() && isAlive) {
				//If the target has existed for 5 seconds, delete it
				if(std::chrono::duration<double, std::milli>(t_end-(*itr).getCreated()).count() >= 5000) {

					(*itr).setDeleted(true);
					lives--;
					if(lives <= 0) {
						isAlive = false;
						lives = 0;
					}
					livesText = "Lives left: " + std::to_string(lives);
					surfaceMessage2 = TTF_RenderText_Solid(font, livesText.c_str(), White);
					Message2 = SDL_CreateTextureFromSurface(window.getRenderer(), surfaceMessage2);

				} else {
					window.render(*itr);
				}
			}
		}
		//Render the text 
		SDL_RenderCopy(window.getRenderer(), Message, NULL, &Message_rect);
		SDL_RenderCopy(window.getRenderer(), Message2, NULL, &Message_rect2);

		if(!isAlive) {
			SDL_RenderCopy(window.getRenderer(), Message3, NULL, &Message_rect3);
		}

		window.display();
	}

	//Clear memory
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(surfaceMessage2);
	SDL_DestroyTexture(Message2);
	window.cleanUp();
	SDL_Quit();

	return 0;
}