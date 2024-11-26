#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Player.h"
#include "NPC.h"
#include "ObjectPool.h"
#include "StackAllocator.h"

class Level {
public:
    Level(SDL_Renderer* renderer);
    ~Level();

    void init(int numNPCs);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, TTF_Font* font);

    void save(const std::string& filename);
    void load(const std::string& filename);

    bool isGameOver() const;

    Player* getPlayer() { return player; }
    std::vector<NPC*>& getNPCs() { return activeNPCs; }

private:
    Player* player;
    std::vector<NPC*> activeNPCs; // Active NPCs in the game
    ObjectPool<NPC> npcPool;      // Object pool for NPCs
    StackAllocator stackAllocator; // Stack allocator for temporary memory
    SDL_Renderer* renderer;

    SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);
};

#endif // LEVEL_H
