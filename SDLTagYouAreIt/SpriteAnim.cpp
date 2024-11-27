#include "SpriteAnim.h"
#include <iostream>

SpriteAnim::SpriteAnim(SpriteSheet* spriteSheet, float frameTime)
    : spriteSheet(spriteSheet), startFrame(0), endFrame(0),
    currentFrame(0), elapsedTime(0.0f), frameTime(frameTime) {}

void SpriteAnim::update(float deltaTime) {
    
    elapsedTime += deltaTime;

    
    if (elapsedTime >= frameTime) {
        elapsedTime -= frameTime; 
        currentFrame++;

        
        if (currentFrame > endFrame) {
            currentFrame = startFrame;
        }
    }

    
}




void SpriteAnim::render(SDL_Renderer* renderer, int x, int y, float scale, float angle, SDL_Color tint) const {
    SDL_Rect srcRect = spriteSheet->getFrame(currentFrame); 
    SDL_Rect dstRect = {
        x - static_cast<int>(srcRect.w * scale) / 2, 
        y - static_cast<int>(srcRect.h * scale) / 2, 
        static_cast<int>(srcRect.w * scale),        
        static_cast<int>(srcRect.h * scale)         
    };

    
    SDL_SetTextureColorMod(spriteSheet->getTexture(), tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(spriteSheet->getTexture(), 255); 

    SDL_RenderCopyEx(renderer, spriteSheet->getTexture(), &srcRect, &dstRect, angle, nullptr, SDL_FLIP_NONE);

}




void SpriteAnim::setAnimationRange(int start, int end) {
    startFrame = start;
    endFrame = end;
    currentFrame = startFrame;
}
