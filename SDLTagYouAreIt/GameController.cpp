#include "GameController.h"
#include <iostream>
#include <cmath> // for sqrt and pow
#include "NPCWarriorImpl.h"

GameController::GameController()
    : window(nullptr), renderer(nullptr), isRunning(false), player(nullptr), npcSpeed(60.0f), tagCount(0) {}

GameController::~GameController() {
    cleanUp();
}

bool GameController::initialize(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize player and NPCs
    player = new PlayerWarrior(renderer);
    for (int i = 0; i < 5; ++i) {
        //npcs.push_back(new NPCWarrior(renderer));
        npcs.push_back(new NPCWarriorImpl(renderer, *player));
    }

    isRunning = true;
    return true;
}

void GameController::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void GameController::update(float deltaTime) {
    // Update player
    player->update();

    // Update NPCs
    for (auto& npc : npcs) {
        npc->update(npcSpeed);

        // Check collision with player
        float distance = sqrt(pow(npc->getPosition().X - player->getPosition().X, 2) +
            pow(npc->getPosition().Y - player->getPosition().Y, 2));
        if (distance < 64.0f && !npc->isTagged()) {
            npc->setTagged(true);
            ++tagCount;
        }
    }
}

void GameController::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear screen with black color
    SDL_RenderClear(renderer);

    // Render player and NPCs
    player->render();
    for (auto& npc : npcs) {
        npc->render();
    }

    // Draw HUD or score if needed

    SDL_RenderPresent(renderer);
}

void GameController::cleanUp() {
    delete player;
    for (auto& npc : npcs) {
        delete npc;
    }
    npcs.clear();

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    SDL_Quit();
}

void GameController::run() {
    const float targetDeltaTime = 1.0f / 60.0f; // 60 FPS
    Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        handleEvents();
        update(deltaTime);
        render();

        SDL_Delay(static_cast<Uint32>(targetDeltaTime * 1000.0f));
    }
}
