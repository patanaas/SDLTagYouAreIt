#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#pragma once
#include <SDL.h>
#include <vector>
#include "PlayerWarrior.h"
#include "NPCWarrior.h"

class GameController {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;

    PlayerWarrior* player;           // Player object
    std::vector<NPCWarrior*> npcs;   // List of NPCs
    float npcSpeed;                  // Speed of NPC movement
    int tagCount;                    // Number of NPCs tagged

public:
    GameController();
    ~GameController();

    bool initialize(const char* title, int width, int height);
    void handleEvents();
    void update(float deltaTime);
    void render();
    void cleanUp();

    void run();
};



#endif // !GAME_CONTROLLER_H

