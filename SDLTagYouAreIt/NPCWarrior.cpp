#include "NPCWarrior.h"
#include <iostream>
#include <fstream>

NPCWarrior::NPCWarrior(SDL_Renderer* renderer)
    : position({ rand() % 800, rand() % 600 }), tagged(false), texture(nullptr), renderer(renderer) {
    // Load the NPC texture
    SDL_Surface* surface = SDL_LoadBMP("npc.bmp");
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else {
        std::cerr << "Failed to load NPC texture: " << SDL_GetError() << std::endl;
    }
}

NPCWarrior::~NPCWarrior() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void NPCWarrior::update(float speed) {
    // Randomly move the NPC (example logic)
    position.X += (rand() % 3 - 1) * speed * 0.016f; // Random direction
    position.Y += (rand() % 3 - 1) * speed * 0.016f;

    // Clamp position within screen bounds (assuming 800x600 resolution)
    if (position.X < 0) position.X = 0;
    if (position.Y < 0) position.Y = 0;
    if (position.X > 800) position.X = 800;
    if (position.Y > 600) position.Y = 600;
}

void NPCWarrior::render() {
    if (texture) {
        SDL_Rect dstRect = { static_cast<int>(position.X), static_cast<int>(position.Y), 64, 64 };
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    }
}

void NPCWarrior::Serialize(std::ofstream& _stream) {
    if (_stream.is_open()) {
        _stream << position.X << " " << position.Y << " " << tagged << std::endl;
    }
}

void NPCWarrior::Deserializable(std::ifstream& _stream) {
    if (_stream.is_open()) {
        _stream >> position.X >> position.Y >> tagged;
    }
}
