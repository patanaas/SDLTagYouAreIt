#include "NPC.h"
#include "TGAReader.h"
#include <iostream>
#include <cmath>

NPC::NPC()
    : renderer(nullptr), posX(0), posY(0), speed(0), angle(0), tagged(false), removable(false), spriteSheet(nullptr), anim(nullptr) {
    rect = { 0, 0, 50, 50 }; // Default size
    std::cerr << "Warning: NPC default constructor used. Renderer is not set!" << std::endl;
}

NPC::NPC(SDL_Renderer* renderer, int x, int y, int speed)
    : renderer(renderer), posX(x), posY(y), speed(speed), angle(0), tagged(false), removable(false), spriteSheet(nullptr), anim(nullptr) {
    rect = { x, y, 50, 50 }; // Initialize rect (used for positioning)

    // Load the sprite sheet
    SDL_Texture* npcTexture = TGAReader::loadTGA("../Assets/Textures/Warrior.tga", renderer);
    if (!npcTexture) {
        std::cerr << "Failed to load Warrior.tga for NPC" << std::endl;
        return;
    }

    // Initialize sprite sheet and animation
    spriteSheet = new SpriteSheet(npcTexture, 30, 45); // Frame size (30x45) needs to match your sprite sheet
    if (!spriteSheet) {
        std::cerr << "Failed to create SpriteSheet." << std::endl;
        return;
    }

    anim = new SpriteAnim(spriteSheet, 0.15f); // 0.15 seconds per frame
    if (!anim) {
        std::cerr << "Failed to create SpriteAnim." << std::endl;
        delete spriteSheet;
        return;
    }

    anim->setAnimationRange(0, 9); // Default animation range (e.g., idle animation)
    std::cout << "NPC initialized at position (" << x << ", " << y << ") with speed " << speed << std::endl;
}

NPC::~NPC() {
    delete anim;
    delete spriteSheet;
    std::cout << "NPC destroyed." << std::endl;
}

void NPC::update(float deltaTime, int playerX, int playerY) {
    if (tagged) return; // Stop moving if tagged

    if (deltaTime <= 0.0f) {
        std::cerr << "Invalid deltaTime: " << deltaTime << std::endl;
        return;
    }

    anim->update(deltaTime); // Update the animation frame

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

    std::cout << "NPC position updated to (" << posX << ", " << posY << ")" << std::endl;
}

void NPC::render(SDL_Renderer* renderer) const {
    if (!anim || !renderer) {
        std::cerr << "Error: Animation or renderer is null." << std::endl;
        // Render a fallback rectangle for debugging
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for visibility
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    // Apply a green tint to the NPC sprite
    SDL_Color greenTint = { 0, 255, 0, 255 }; // Green
    SDL_SetTextureColorMod(spriteSheet->getTexture(), greenTint.r, greenTint.g, greenTint.b);

    anim->render(renderer, static_cast<int>(posX), static_cast<int>(posY), 1.25f, 0.0f, greenTint);

    // Reset the texture color modulation to white (no tint)
    SDL_SetTextureColorMod(spriteSheet->getTexture(), 255, 255, 255);
    std::cout << "NPC rendered at position (" << posX << ", " << posY << ") with green tint" << std::endl;
}


bool NPC::checkTagged(int playerX, int playerY) {
    int dx = playerX - static_cast<int>(posX);
    int dy = playerY - static_cast<int>(posY);
    float distance = std::sqrt(dx * dx + dy * dy);
    bool isTagged = distance < 30; // Tagged if within 30 units
    if (isTagged) {
        std::cout << "NPC tagged by player at distance " << distance << std::endl;
    }
    return isTagged;
}

void NPC::tag() {
    tagged = true;
    removable = true; // Mark for removal
    std::cout << "NPC tagged and marked for removal." << std::endl;
}
