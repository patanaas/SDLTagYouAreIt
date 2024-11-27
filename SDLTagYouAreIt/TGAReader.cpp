#include "TGAReader.h"
#include <fstream>
#include <iostream>
#include <vector>

#pragma pack(push, 1)
struct TGAHeader {
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t imageType;
    uint16_t colorMapStart;
    uint16_t colorMapLength;
    uint8_t colorMapDepth;
    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t pixelDepth;
    uint8_t imageDescriptor;
};
#pragma pack(pop)

SDL_Texture* TGAReader::loadTGA(const std::string& filename, SDL_Renderer* renderer) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open TGA file: " << filename << std::endl;
        return nullptr;
    }

    TGAHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(TGAHeader));

    if (header.imageType != 2 || header.pixelDepth != 32) {
        std::cerr << "Unsupported TGA format. Only uncompressed 32-bit TGA supported." << std::endl;
        return nullptr;
    }

    size_t imageSize = header.width * header.height * 4;
    std::vector<uint8_t> imageData(imageSize);

    file.seekg(header.idLength, std::ios::cur);
    file.read(reinterpret_cast<char*>(imageData.data()), imageSize);

    if (!(header.imageDescriptor & 0x20)) {
        for (size_t y = 0; y < header.height / 2; ++y) {
            for (size_t x = 0; x < header.width * 4; ++x) {
                std::swap(imageData[y * header.width * 4 + x],
                    imageData[(header.height - 1 - y) * header.width * 4 + x]);
            }
        }
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
        imageData.data(),
        header.width,
        header.height,
        32,
        header.width * 4,
        0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
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
