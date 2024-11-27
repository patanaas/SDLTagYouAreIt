#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(SDL_Texture* texture, int frameWidth, int frameHeight)
    : texture(texture) {
    int texWidth, texHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);

    for (int y = 0; y < texHeight / frameHeight; ++y) {
        for (int x = 0; x < texWidth / frameWidth; ++x) {
            SDL_Rect frame = { x * frameWidth, y * frameHeight, frameWidth, frameHeight };
            frames.push_back(frame);
        }
    }
}



SDL_Rect SpriteSheet::getFrame(int index) const {
    if (index >= 0 && index < static_cast<int>(frames.size())) {
        return frames[index];
    }
    return { 0, 0, 0, 0 }; 
}
