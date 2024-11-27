#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#include "SpriteSheet.h"

class SpriteAnim {
public:
    SpriteAnim(SpriteSheet* spriteSheet, float frameTime);

    void update(float deltaTime);
    void render(SDL_Renderer* renderer, int x, int y, float scale, float angle, SDL_Color tint) const;

    void setAnimationRange(int start, int end);
    void reset() { currentFrame = startFrame; elapsedTime = 0.0f; }

    // Add these getters
    int getStartFrame() const { return startFrame; }
    int getEndFrame() const { return endFrame; }
    int getCurrentFrame() const { return currentFrame; }
    int getElapsedTime() const { return elapsedTime; }
private:
    SpriteSheet* spriteSheet;
    int startFrame, endFrame;
    int currentFrame;
    float elapsedTime, frameTime;
};

#endif // SPRITEANIM_H
