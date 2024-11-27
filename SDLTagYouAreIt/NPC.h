#ifndef NPC_H
#define NPC_H

#include <SDL.h>
#include <cmath>
#include "SpriteSheet.h"
#include "SpriteAnim.h"

enum class NPCState {
    Idle,
    Running,
    Tagged
};

class NPC {
public:
    NPC(); // Default constructor
    NPC(SDL_Renderer* renderer, int x, int y, int speed);
    ~NPC();

    void update(float deltaTime, int playerX, int playerY);
    void render(SDL_Renderer* renderer) const;
    bool checkTagged(int playerX, int playerY);
    void tag();

    bool isTagged() const { return tagged; }
    bool isRemovable() const { return removable; }

    // Getters and setters for saving/loading
    float getX() const { return posX; }
    float getY() const { return posY; }
    int getSpeed() const { return speed; }
    void setSpeed(int newSpeed) { speed = newSpeed; }
    NPCState getState() const { return state; }
private:
    SDL_Rect rect;
    float posX, posY;
    float angle;
    int speed;
    bool tagged;
    bool removable;
    NPCState state;

    // Add renderer as a member
    SDL_Renderer* renderer;
    SpriteSheet* spriteSheet;
    SpriteAnim* anim;
};

#endif // NPC_H
