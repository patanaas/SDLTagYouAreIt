#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <memory>
#include <SDL.h>
#include "PlayerWarrior.h"
#include "NPC.h"

class Level {
private:
    SDL_Renderer* renderer;
    PlayerWarrior playerWarrior;
    std::vector<std::unique_ptr<NPC>> npcs;
    int npcSpeed = 60;
    int npcTagCount = 0;

public:
    Level(SDL_Renderer* renderer);
    void handleKeyPress(SDL_Keycode key);
    void update();
    void render();
    void saveLevel(const std::string& filename);
    void loadLevel(const std::string& filename);
};


#endif // !LEVEL_H
