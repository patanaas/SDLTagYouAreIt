#include "Animation.h"

Animation::Animation(SDL_Texture* spriteSheet, int frameWidth, int frameHeight, int numFrames, float frameTime, int sheetWidth)
    : spriteSheet(spriteSheet), currentFrame(0), frameTime(frameTime), elapsedTime(0.0f) {
    int framesPerRow = sheetWidth / frameWidth;
    for (int i = 0; i < numFrames; ++i) {
        int row = i / framesPerRow;
        int col = i % framesPerRow;
        SDL_Rect frame = { col * frameWidth, row * frameHeight, frameWidth, frameHeight };
        frames.push_back(frame);
    }
}

void Animation::update(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        elapsedTime = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
}

void Animation::render(SDL_Renderer* renderer, int x, int y, float scale, float rotation, SDL_Color tint, SDL_RendererFlip flip) {
    SDL_Rect srcRect = frames[currentFrame];
    SDL_Rect dstRect = {
        x,
        y,
        static_cast<int>(srcRect.w * scale),
        static_cast<int>(srcRect.h * scale)
    };

    SDL_SetTextureColorMod(spriteSheet, tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(spriteSheet, tint.a);

    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &dstRect, rotation, nullptr, flip);
}
