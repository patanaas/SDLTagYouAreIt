#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <algorithm>
#include <sstream>
#include "Player.h"
#include "NPC.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int FRAME_RATE = 80;
const int FRAME_DELAY = 1000 / FRAME_RATE;
const int NUM_NPCS = 10;

// Function to render text to the screen
SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
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

// Save the level to a binary file
void saveLevel(const std::string& filename, const Player& player, const std::vector<NPC>& npcs) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }

    // Save player position
    int playerX = player.getX();
    int playerY = player.getY();
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

// Load the level from a binary file
void loadLevel(const std::string& filename, Player& player, std::vector<NPC>& npcs, SDL_Renderer* renderer) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return;
    }

    // Load player position
    int playerX, playerY;
    file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
    player.setPosition(playerX, playerY);

    // Load NPC count
    int numNPCs;
    file.read(reinterpret_cast<char*>(&numNPCs), sizeof(numNPCs));

    // Clear existing NPCs
    npcs.clear();

    // Load each NPC's data
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

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow(
        "SDLTagYouAreIt",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("../Assets/Fonts/arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }

    // Seed random number generator for NPC positions
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Initialize player
    Player player(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100);

    // Initialize NPCs
    std::vector<NPC> npcs;
    for (int i = 0; i < NUM_NPCS; ++i) {
        int x = (SCREEN_WIDTH / 2) + (std::rand() % 300 - 150);
        int y = (SCREEN_HEIGHT / 2) + (std::rand() % 300 - 150);
        npcs.emplace_back(renderer, x, y, 60); // Speed is 60
    }

    // Main loop flag
    bool running = true;
    SDL_Event event;

    // Game loop
    while (running) {
        auto start = std::chrono::high_resolution_clock::now();

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            player.handleEvent(event);

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_i: // Increase NPC speed
                    for (auto& npc : npcs) {
                        npc.setSpeed(std::min(npc.getSpeed() + 10, 60)); // Max speed: 60
                    }
                    break;
                case SDLK_d: // Decrease NPC speed
                    for (auto& npc : npcs) {
                        npc.setSpeed(std::max(npc.getSpeed() - 10, 0)); // Min speed: 0
                    }
                    break;
                case SDLK_s: // Save level
                    saveLevel("level.bin", player, npcs);
                    break;
                case SDLK_l: // Load level
                    loadLevel("level.bin", player, npcs, renderer);
                    break;
                }
            }
        }

        // Update player
        player.update(FRAME_DELAY / 1000.0f);

        // Update NPCs
        for (auto& npc : npcs) {
            npc.update(FRAME_DELAY / 1000.0f, player.getX(), player.getY());
            if (npc.checkTagged(player.getX(), player.getY())) {
                npc.tag();
            }
        }

        // Remove tagged NPCs
        npcs.erase(std::remove_if(npcs.begin(), npcs.end(), [](const NPC& npc) {
            return npc.isRemovable();
            }), npcs.end());

        // End game if all NPCs are tagged
        if (npcs.empty()) {
            running = false;
        }

        // Render objects
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        player.render(renderer);

        for (const auto& npc : npcs) {
            npc.render(renderer);
        }

        // Render Key Press Information
        SDL_Color white = { 255, 255, 255, 255 };
        std::string keyInfoText = "[I] Increase NPC Speed  [D] Decrease NPC Speed  [S] Save  [L] Load  [ESC] Quit";
        SDL_Texture* keyInfoTexture = renderText(keyInfoText, font, white, renderer);
        SDL_Rect keyInfoRect = { 10, 50, 1200, 30 };
        SDL_RenderCopy(renderer, keyInfoTexture, nullptr, &keyInfoRect);
        SDL_DestroyTexture(keyInfoTexture);

        // Render Player Speed
        std::stringstream playerSpeedStream;
        playerSpeedStream << "Player Speed: " << 100;
        SDL_Texture* playerSpeedTexture = renderText(playerSpeedStream.str(), font, white, renderer);
        SDL_Rect playerSpeedRect = { 10, 90, 300, 30 };
        SDL_RenderCopy(renderer, playerSpeedTexture, nullptr, &playerSpeedRect);
        SDL_DestroyTexture(playerSpeedTexture);

        // Render Enemy Count
        std::stringstream enemyCountStream;
        enemyCountStream << "Enemies Remaining: " << npcs.size();
        SDL_Texture* enemyCountTexture = renderText(enemyCountStream.str(), font, white, renderer);
        SDL_Rect enemyCountRect = { 10, 130, 300, 30 };
        SDL_RenderCopy(renderer, enemyCountTexture, nullptr, &enemyCountRect);
        SDL_DestroyTexture(enemyCountTexture);

        SDL_RenderPresent(renderer);

        // Cap frame rate
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> elapsed = end - start;
        if (elapsed.count() < FRAME_DELAY) {
            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DELAY) - elapsed);
        }
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
