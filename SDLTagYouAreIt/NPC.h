#ifndef NPC_H
#define NPC_H

#include <SDL.h>
#include <cmath>
#include <SDL_mixer.h>
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
    virtual ~NPC();

    void update(float deltaTime, int playerX, int playerY);
    void render(SDL_Renderer* renderer) const;
    bool checkTagged(int playerX, int playerY);
    void tag();
    static bool initializeSound();
    static void cleanupSound();

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
    static Mix_Chunk* tagSound;
    
    NPCState state;
    float taggedTime;  // Time elapsed since tagged
    float taggedRemovalDelay;  // Delay before removal

    // Add renderer as a member
    SDL_Renderer* renderer;
    SpriteSheet* spriteSheet;
    SpriteAnim* anim;
};

#endif // NPC_H
