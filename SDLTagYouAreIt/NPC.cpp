#include "NPC.h"
#include "TGAReader.h"
#include <iostream>
#include <SDL_mixer.h>

// Static member to load sound only once
Mix_Chunk* NPC::tagSound = nullptr;

bool NPC::initializeSound() {
    if (!tagSound) {
        tagSound = Mix_LoadWAV("../Assets/Audio/Effects/Whoosh.wav");
        if (!tagSound) {
            std::cerr << "Failed to load tag sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
            return false;
        }
    }
    return true;
}

void NPC::cleanupSound() {
    if (tagSound) {
        Mix_FreeChunk(tagSound);
        tagSound = nullptr;
    }
}

NPC::NPC()
    : renderer(nullptr), posX(0), posY(0), speed(0), angle(0), tagged(false),
    removable(false), spriteSheet(nullptr), anim(nullptr), state(NPCState::Idle),
    taggedTime(0.0f), taggedRemovalDelay(2.0f) { 
    rect = { 0, 0, 50, 50 };
}

NPC::NPC(SDL_Renderer* renderer, int x, int y, int speed)
    : renderer(renderer), posX(x), posY(y), speed(speed), angle(0), tagged(false),
    removable(false), spriteSheet(nullptr), anim(nullptr), state(NPCState::Idle),
    taggedTime(0.0f), taggedRemovalDelay(10.0f) { 
    rect = { x, y, 50, 50 }; 

    SDL_Texture* npcTexture = TGAReader::loadTGA("../Assets/Textures/Warrior.tga", renderer);
    if (!npcTexture) {
        std::cerr << "Failed to load Warrior.tga for NPC" << std::endl;
        return;
    }
    spriteSheet = new SpriteSheet(npcTexture, 30, 45); 
    if (!spriteSheet) {
        std::cerr << "Failed to create SpriteSheet." << std::endl;
        return;
    }

    anim = new SpriteAnim(spriteSheet, 0.15f); 
    if (!anim) {
        std::cerr << "Failed to create SpriteAnim." << std::endl;
        delete spriteSheet;
        return;
    }

    anim->setAnimationRange(0, 9); 
}

NPC::~NPC() {
    delete anim;
    delete spriteSheet;
}

void NPC::update(float deltaTime, int playerX, int playerY) {
    if (tagged) {
        if (state != NPCState::Tagged) {
            state = NPCState::Tagged;
            anim->setAnimationRange(20, 29); 
            anim->reset();
            taggedTime = 0.0f; 
        }

        anim->update(deltaTime);

        
        taggedTime += deltaTime;

        
        if (taggedTime >= taggedRemovalDelay) {
            removable = true;
        }
        return;
    }

    if (deltaTime <= 0.0f) {
        std::cerr << "Invalid deltaTime: " << deltaTime << std::endl;
        return;
    }

    anim->update(deltaTime);

    
    int dx = playerX - static_cast<int>(posX);
    int dy = playerY - static_cast<int>(posY);
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 140) {
        if (state != NPCState::Running) {
            state = NPCState::Running;
            anim->setAnimationRange(10, 19);
            anim->reset();
        }
        posX -= speed * deltaTime * (dx / distance);
        posY -= speed * deltaTime * (dy / distance);
    }
    else if (distance > 160) {
        if (state != NPCState::Idle) {
            state = NPCState::Idle;
            anim->setAnimationRange(0, 9); 
            anim->reset();
        }
        posX += speed * deltaTime * (dx / distance);
        posY += speed * deltaTime * (dy / distance);
    }

    rect.x = static_cast<int>(posX);
    rect.y = static_cast<int>(posY);
}

void NPC::render(SDL_Renderer* renderer) const {
    if (!anim || !renderer) {
        std::cerr << "Error: Animation or renderer is null." << std::endl;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    SDL_Color tint = { 0, 255, 0, 255 };
    if (state == NPCState::Running) {
        tint = { 0, 255, 0, 255 }; 
    }
    else if (state == NPCState::Tagged) {
        tint = { 255, 0, 0, 255 }; 
    }

    SDL_SetTextureColorMod(spriteSheet->getTexture(), tint.r, tint.g, tint.b);

    anim->render(renderer, static_cast<int>(posX), static_cast<int>(posY), 1.25f, 0.0f, tint);

    SDL_SetTextureColorMod(spriteSheet->getTexture(), 255, 255, 255);
    
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
    //if (!tagged) { // Play the sound only the first time it's tagged
    //    Mix_PlayChannel(-1, tagSound, 0);
    //    tagged = true;
    //}
    ////tagged = true;
    //removable = true; // Mark for removal
    if (!tagged) {
        if (tagSound) {
            int channel = Mix_PlayChannel(-1, tagSound, 0);
            if (channel == -1) {
                std::cerr << "Unable to play sound effect: " << Mix_GetError() << std::endl;
            }
        }
        tagged = true;
        removable = true;
    }
    tagged = true;
    state = NPCState::Tagged;
    anim->setAnimationRange(20, 29); // Death animation frames
    anim->reset();
}
