#ifndef TGAREADER_H
#define TGAREADER_H

#include <string>
#include <SDL.h>

class TGAReader {
public:
    static SDL_Texture* loadTGA(const std::string& filename, SDL_Renderer* renderer);
};

#endif // TGAREADER_H
