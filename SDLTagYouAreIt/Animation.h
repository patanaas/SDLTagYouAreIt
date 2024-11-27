#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include <vector>

class Animation {
public:
    Animation(SDL_Texture* spriteSheet, int frameWidth, int frameHeight, int numFrames, float frameTime, int startY);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, int x, int y, float scale = 1.0f, float rotation = 0.0f, SDL_Color tint = { 255, 255, 255, 255 }, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
    SDL_Texture* spriteSheet;
    std::vector<SDL_Rect> frames;
    int currentFrame;
    float frameTime;
    float elapsedTime;
};

#endif // ANIMATION_H
