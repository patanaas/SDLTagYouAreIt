#ifndef TGA_LOADER_H
#define TGA_LOADER_H

#include <SDL.h>

class TGALoader {
public:
    static SDL_Texture* loadTGA(SDL_Renderer* renderer, const char* filepath);
};

#endif // TGA_LOADER_H
