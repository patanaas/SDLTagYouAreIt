#ifndef NPCWARRIORIMPL_H
#define NPCWARRIORIMPL_H

#include "NPCWarrior.h"
#include "StandardIncludes.h"
#include "PlayerWarrior.h"

class NPCWarriorImpl : public NPCWarrior {
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    Point position;
    float speed;

public:
    NPCWarriorImpl(SDL_Renderer* renderer, PlayerWarrior& player);
    ~NPCWarriorImpl();

    void update(float deltaTime);  // Implement pure virtual function
    void render();  // Implement pure virtual function
};


#endif // !NPCWARRIORIMPL_H
