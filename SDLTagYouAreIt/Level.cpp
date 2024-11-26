#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Level::Level(SDL_Renderer* renderer) : renderer(renderer), player(nullptr) {}

Level::~Level() {
    delete player;
}

void Level::init(int numNPCs) {
    // Initialize player
    if (!player) {
        player = new Player(renderer, 1920 / 2, 1080 / 2, 100);
    }

    // Initialize NPCs
    npcs.clear();
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int i = 0; i < numNPCs; ++i) {
        int x = (1920 / 2) + (std::rand() % 300 - 150);
        int y = (1080 / 2) + (std::rand() % 300 - 150);
        npcs.emplace_back(renderer, x, y, 60);
    }
}

void Level::update(float deltaTime) {
    if (player) {
        player->update(deltaTime);
    }

    for (auto& npc : npcs) {
        npc.update(deltaTime, player->getX(), player->getY());
        if (npc.checkTagged(player->getX(), player->getY())) {
            npc.tag();
        }
    }

    // Remove tagged NPCs
    npcs.erase(std::remove_if(npcs.begin(), npcs.end(), [](const NPC& npc) {
        return npc.isRemovable();
        }), npcs.end());
}

void Level::render(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = { 255, 255, 255, 255 };

    // Render player
    if (player) {
        player->render(renderer);
    }

    // Render NPCs
    for (const auto& npc : npcs) {
        npc.render(renderer);
    }

    // Render additional information
    std::string keyInfoText = "[I] Increase NPC Speed  [D] Decrease NPC Speed  [S] Save  [L] Load  [ESC] Quit";
    SDL_Texture* keyInfoTexture = renderText(keyInfoText, font, white, renderer);
    SDL_Rect keyInfoRect = { 10, 50, 1200, 30 };
    SDL_RenderCopy(renderer, keyInfoTexture, nullptr, &keyInfoRect);
    SDL_DestroyTexture(keyInfoTexture);

    std::stringstream playerSpeedStream;
    playerSpeedStream << "Player Speed: " << 100; // Hardcoded as 100
    SDL_Texture* playerSpeedTexture = renderText(playerSpeedStream.str(), font, white, renderer);
    SDL_Rect playerSpeedRect = { 10, 90, 300, 30 };
    SDL_RenderCopy(renderer, playerSpeedTexture, nullptr, &playerSpeedRect);
    SDL_DestroyTexture(playerSpeedTexture);

    std::stringstream enemyCountStream;
    enemyCountStream << "Enemies Remaining: " << npcs.size();
    SDL_Texture* enemyCountTexture = renderText(enemyCountStream.str(), font, white, renderer);
    SDL_Rect enemyCountRect = { 10, 130, 300, 30 };
    SDL_RenderCopy(renderer, enemyCountTexture, nullptr, &enemyCountRect);
    SDL_DestroyTexture(enemyCountTexture);
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
    int numNPCs = static_cast<int>(npcs.size());
    file.write(reinterpret_cast<char*>(&numNPCs), sizeof(numNPCs));

    // Save each NPC's data
    for (const auto& npc : npcs) {
        float posX = npc.getX();
        float posY = npc.getY();
        int speed = npc.getSpeed();
        file.write(reinterpret_cast<char*>(&posX), sizeof(posX));
        file.write(reinterpret_cast<char*>(&posY), sizeof(posY));
        file.write(reinterpret_cast<char*>(&speed), sizeof(speed));
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

    // Clear and reinitialize NPCs
    npcs.clear();
    for (int i = 0; i < numNPCs; ++i) {
        float posX, posY;
        int speed;
        file.read(reinterpret_cast<char*>(&posX), sizeof(posX));
        file.read(reinterpret_cast<char*>(&posY), sizeof(posY));
        file.read(reinterpret_cast<char*>(&speed), sizeof(speed));
        npcs.emplace_back(renderer, static_cast<int>(posX), static_cast<int>(posY), speed);
    }

    file.close();
    std::cout << "Level loaded from " << filename << std::endl;
}

bool Level::isGameOver() const {
    return npcs.empty();
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
