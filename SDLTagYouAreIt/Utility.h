#ifndef UTILITY_H
#define UTILITY_H

#include <SDL.h>
#include <string>

void drawHUD(SDL_Renderer* renderer, float playerSpeed, float npcSpeed, int tagCount);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path);


#endif // !UTILITY_H
