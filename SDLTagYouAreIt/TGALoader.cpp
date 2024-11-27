#include "TGALoader.h"
#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct TGAHeader {
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t dataTypeCode;
    uint16_t colorMapOrigin;
    uint16_t colorMapLength;
    uint8_t colorMapDepth;
    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t bitsPerPixel;
    uint8_t imageDescriptor;
};
#pragma pack(pop)

SDL_Texture* TGALoader::loadTGA(SDL_Renderer* renderer, const char* filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open TGA file: " << filepath << std::endl;
        return nullptr;
    }

    TGAHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(TGAHeader));

    if (header.dataTypeCode != 2 || (header.bitsPerPixel != 24 && header.bitsPerPixel != 32)) {
        std::cerr << "Unsupported TGA format" << std::endl;
        return nullptr;
    }

    int pixelSize = header.bitsPerPixel / 8;
    int imageSize = header.width * header.height * pixelSize;
    std::vector<uint8_t> pixelData(imageSize);
    file.read(reinterpret_cast<char*>(pixelData.data()), imageSize);
    file.close();

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
        pixelData.data(),
        header.width,
        header.height,
        header.bitsPerPixel,
        header.width * pixelSize,
        0x000000FF, 0x0000FF00, 0x00FF0000, header.bitsPerPixel == 32 ? 0xFF000000 : 0
    );

    if (!surface) {
        std::cerr << "Failed to create SDL surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create SDL texture: " << SDL_GetError() << std::endl;
    }

    return texture;
}
