#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SDL.h>
#include <vector>

class SpriteSheet {
public:
    SpriteSheet(SDL_Texture* texture, int frameWidth, int frameHeight);

    SDL_Rect getFrame(int index) const;
    int getFrameCount() const { return static_cast<int>(frames.size()); }
    SDL_Texture* getTexture() const { return texture; } // Added getter for texture

private:
    std::vector<SDL_Rect> frames;
    SDL_Texture* texture;
};

#endif // SPRITESHEET_H
