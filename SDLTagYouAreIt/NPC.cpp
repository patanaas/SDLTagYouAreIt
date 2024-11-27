#include "NPC.h"
#include <iostream>
#include <SDL_mixer.h>

// Static member to load sound only once
//Mix_Chunk* NPC::tagSound = nullptr;

// Load sound once, not in constructor
void NPC::initializeSound() {
    if (!tagSound) {
        tagSound = Mix_LoadWAV("../Assets/Audio/Effects/Whoosh.wav");
        if (!tagSound) {
            std::cerr << "Failed to load tag sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }
}

NPC::NPC()
    : renderer(nullptr), posX(0), posY(0), speed(0), angle(0), tagged(false), removable(false) {
    rect = { 0, 0, 50, 50 }; // Initialize with default size
}

NPC::NPC(SDL_Renderer* renderer, int x, int y, int speed)
    : posX(x), posY(y), speed(speed), angle(0), tagged(false), removable(false) {
    rect = { x, y, 50, 50 }; // Initialize rect with integer positions
    tagSound = Mix_LoadWAV("../Assets/Audio/Effects/Whoosh.wav");
    if (!tagSound) {
        std::cerr << "Failed to load tag sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

NPC::~NPC() 
{
    if (tagSound) {
        Mix_FreeChunk(tagSound);
        tagSound = nullptr;
    }
}

void NPC::update(float deltaTime, int playerX, int playerY) {
    if (tagged) return; // Stop moving if tagged

    // Calculate the distance to the player
    int dx = playerX - static_cast<int>(posX);
    int dy = playerY - static_cast<int>(posY);
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 140) {
        // Move away from the player
        posX -= speed * deltaTime * (dx / distance);
        posY -= speed * deltaTime * (dy / distance);
    }
    else if (distance > 160) {
        // Move toward the player
        posX += speed * deltaTime * (dx / distance);
        posY += speed * deltaTime * (dy / distance);
    }

    // Update the integer rect position for rendering
    rect.x = static_cast<int>(posX);
    rect.y = static_cast<int>(posY);
}

void NPC::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, tagged ? 255 : 0, tagged ? 0 : 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool NPC::checkTagged(int playerX, int playerY) {
    int dx = playerX - static_cast<int>(posX);
    int dy = playerY - static_cast<int>(posY);
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < 30;
}

//void NPC::tag() {
//    if (!tagged) {
//        //if (tagSound) {
//            // Use a dynamic channel selection
//            int channel = Mix_PlayChannel(-1, tagSound, 0);
//            if (channel == -1) {
//                std::cerr << "Unable to play sound effect: " << Mix_GetError() << std::endl;
//            }
//        //}
//        tagged = true;
//    }
//}

void NPC::tag() {
    if (!tagged) { // Play the sound only the first time it's tagged
        Mix_PlayChannel(-1, tagSound, 0);
        tagged = true;
    }
    //tagged = true;
    removable = true; // Mark for removal
}
