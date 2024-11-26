#ifndef NPC_H
#define NPC_H

#include <SDL.h>
#include <cmath>

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

private:
    SDL_Rect rect;
    float posX, posY;
    float angle;
    int speed;
    bool tagged;
    bool removable;

    // Add renderer as a member
    SDL_Renderer* renderer;
};

#endif // NPC_H
