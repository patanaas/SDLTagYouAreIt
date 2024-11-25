#include "Utility.h"
#include <SDL_ttf.h>

void drawHUD(SDL_Renderer* renderer, float playerSpeed, float npcSpeed, int tagCount) {
    // Draw information on screen (e.g., using TTF_RenderText for text)
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
