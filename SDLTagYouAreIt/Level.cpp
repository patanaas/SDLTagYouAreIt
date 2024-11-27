#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Level::Level(SDL_Renderer* renderer)
    : renderer(renderer), player(nullptr), npcPool(100), stackAllocator(1024 * 1024) // Preallocate 100 NPCs, 1MB stack
{}

Level::~Level() {
    delete player;
}

void Level::init(int numNPCs) {
    // Initialize player
    if (!player) {
        player = new Player(renderer, "../Assets/Textures/Warrior.tga");
    }

    // Reset active NPCs and release all NPCs to the pool
    activeNPCs.clear();
    npcPool.reset();

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int i = 0; i < numNPCs; ++i) {
        NPC* npc = npcPool.acquire();
        if (npc) {
            int x = (1920 / 2) + (std::rand() % 300 - 150);
            int y = (1080 / 2) + (std::rand() % 300 - 150);
            npc->~NPC(); // Explicitly call the destructor for the existing object
            new (npc) NPC(renderer, x, y, 60); // Use placement new to construct the object properly

            activeNPCs.push_back(npc);
        }
        else {
            std::cerr << "NPC pool exhausted!" << std::endl;
        }
    }
}

void Level::update(float deltaTime) {
    if (player) {
        player->update(deltaTime);
    }

    for (auto it = activeNPCs.begin(); it != activeNPCs.end();) {
        NPC* npc = *it;

        // Temporary storage for computations using the stack allocator
        float* distance = static_cast<float*>(stackAllocator.allocate(sizeof(float)));
        *distance = std::sqrt(std::pow(npc->getX() - player->getX(), 2) +
            std::pow(npc->getY() - player->getY(), 2));

        npc->update(deltaTime, player->getX(), player->getY());
        if (npc->checkTagged(player->getX(), player->getY())) {
            npc->tag();
            npcPool.release(npc); // Return NPC to pool
            it = activeNPCs.erase(it);
        }
        else {
            ++it;
        }
    }

    // Clear stack allocator for next frame
    stackAllocator.clear();
}

void Level::render(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = { 255, 255, 255, 255 };

    // Render player
    if (player) {
        player->render(renderer);
    }

    // Render NPCs
    for (const auto& npc : activeNPCs) {
        npc->render(renderer);
    }

    // Render additional information
    std::string keyInfoText = "[I] Increase NPC Speed  [D] Decrease NPC Speed  [S] Save  [L] Load  [ESC] Quit";
    SDL_Texture* keyInfoTexture = renderText(keyInfoText, font, white, renderer);
    SDL_Rect keyInfoRect = { 10, 50, 1200, 30 };
    SDL_RenderCopy(renderer, keyInfoTexture, nullptr, &keyInfoRect);
    SDL_DestroyTexture(keyInfoTexture);

    // Render player speed
    std::stringstream playerSpeedStream;
    playerSpeedStream << "Player Speed: " << 100; // Hardcoded as 100
    SDL_Texture* playerSpeedTexture = renderText(playerSpeedStream.str(), font, white, renderer);
    SDL_Rect playerSpeedRect = { 10, 90, 300, 30 };
    SDL_RenderCopy(renderer, playerSpeedTexture, nullptr, &playerSpeedRect);
    SDL_DestroyTexture(playerSpeedTexture);

    // Calculate and render average NPC speed
    int totalSpeed = 0;
    for (const auto& npc : activeNPCs) {
        totalSpeed += npc->getSpeed();
    }
    int averageSpeed = activeNPCs.empty() ? 0 : totalSpeed / activeNPCs.size();

    std::stringstream npcSpeedStream;
    npcSpeedStream << "NPC Average Speed: " << averageSpeed;
    SDL_Texture* npcSpeedTexture = renderText(npcSpeedStream.str(), font, white, renderer);
    SDL_Rect npcSpeedRect = { 10, 130, 300, 30 };
    SDL_RenderCopy(renderer, npcSpeedTexture, nullptr, &npcSpeedRect);
    SDL_DestroyTexture(npcSpeedTexture);

    // Render enemies remaining
    std::stringstream enemyCountStream;
    enemyCountStream << "Enemies Remaining: " << activeNPCs.size();
    SDL_Texture* enemyCountTexture = renderText(enemyCountStream.str(), font, white, renderer);
    SDL_Rect enemyCountRect = { 10, 170, 300, 30 };
    SDL_RenderCopy(renderer, enemyCountTexture, nullptr, &enemyCountRect);
    SDL_DestroyTexture(enemyCountTexture);
}


bool Level::isGameOver() const {
    return activeNPCs.empty();
}

SDL_Texture* Level::renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
    }

    return texture;
}

void Level::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }

    // Save player position
    int playerX = player->getX();
    int playerY = player->getY();
    file.write(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    file.write(reinterpret_cast<char*>(&playerY), sizeof(playerY));

    // Save NPC count
    int numNPCs = static_cast<int>(activeNPCs.size());
    file.write(reinterpret_cast<char*>(&numNPCs), sizeof(numNPCs));

    // Save each NPC's data
    for (const auto& npc : activeNPCs) {
        float posX = npc->getX();
        float posY = npc->getY();
        int speed = npc->getSpeed();
        bool tagged = npc->isTagged();

        file.write(reinterpret_cast<char*>(&posX), sizeof(posX));
        file.write(reinterpret_cast<char*>(&posY), sizeof(posY));
        file.write(reinterpret_cast<char*>(&speed), sizeof(speed));
        file.write(reinterpret_cast<char*>(&tagged), sizeof(tagged));
    }

    file.close();
    std::cout << "Level saved to " << filename << std::endl;
}

void Level::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return;
    }

    // Load player position
    int playerX, playerY;
    file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
    player->setPosition(playerX, playerY);

    // Load NPC count
    int numNPCs;
    file.read(reinterpret_cast<char*>(&numNPCs), sizeof(numNPCs));

    // Clear active NPCs and reset the pool
    activeNPCs.clear();
    npcPool.reset();

    // Load each NPC's data
    for (int i = 0; i < numNPCs; ++i) {
        NPC* npc = npcPool.acquire();
        if (npc) {
            float posX, posY;
            int speed;
            bool tagged;

            file.read(reinterpret_cast<char*>(&posX), sizeof(posX));
            file.read(reinterpret_cast<char*>(&posY), sizeof(posY));
            file.read(reinterpret_cast<char*>(&speed), sizeof(speed));
            file.read(reinterpret_cast<char*>(&tagged), sizeof(tagged));

            // Reinitialize the NPC
            *npc = NPC(renderer, static_cast<int>(posX), static_cast<int>(posY), speed);
            if (tagged) {
                npc->tag();
            }

            activeNPCs.push_back(npc);
        }
    }

    file.close();
    std::cout << "Level loaded from " << filename << std::endl;
}
