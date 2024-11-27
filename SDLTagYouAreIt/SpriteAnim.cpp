#include "SpriteAnim.h"
#include <iostream>

SpriteAnim::SpriteAnim(SpriteSheet* spriteSheet, float frameTime)
    : spriteSheet(spriteSheet), startFrame(0), endFrame(0),
    currentFrame(0), elapsedTime(0.0f), frameTime(frameTime) {}

void SpriteAnim::update(float deltaTime) {
    // Accumulate elapsed time
    elapsedTime += deltaTime;

    // Advance frame only if enough time has passed
    if (elapsedTime >= frameTime) {
        elapsedTime -= frameTime; // Subtract frameTime instead of resetting to 0
        currentFrame++;

        // Loop back to the start frame if we exceed the end frame
        if (currentFrame > endFrame) {
            currentFrame = startFrame;
        }
    }

    // Debugging log for frame updates
    std::cout << "currentFrame: " << currentFrame << ", elapsedTime: " << elapsedTime << std::endl;
}




void SpriteAnim::render(SDL_Renderer* renderer, int x, int y, float scale, float angle, SDL_Color tint) const {
    SDL_Rect srcRect = spriteSheet->getFrame(currentFrame); // Extract current frame
    SDL_Rect dstRect = {
        x - static_cast<int>(srcRect.w * scale) / 2, // Center horizontally
        y - static_cast<int>(srcRect.h * scale) / 2, // Center vertically
        static_cast<int>(srcRect.w * scale),        // Scale width
        static_cast<int>(srcRect.h * scale)         // Scale height
    };

    // Ensure consistent color mod and alpha
    SDL_SetTextureColorMod(spriteSheet->getTexture(), tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(spriteSheet->getTexture(), 255); // Full opacity

    SDL_RenderCopyEx(renderer, spriteSheet->getTexture(), &srcRect, &dstRect, angle, nullptr, SDL_FLIP_NONE);
    std::cout << "currentFrame: " << currentFrame
        << ", elapsedTime: " << elapsedTime << std::endl;

}




void SpriteAnim::setAnimationRange(int start, int end) {
    startFrame = start;
    endFrame = end;
    currentFrame = startFrame;
}
