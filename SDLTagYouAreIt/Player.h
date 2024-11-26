#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

class Player {
public:
    Player(SDL_Renderer* renderer, int x, int y, int speed);
    ~Player();

    void handleEvent(const SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    // Methods to get and set position
    int getX() const { return rect.x; }
    int getY() const { return rect.y; }
    void setPosition(int x, int y) { rect.x = x; rect.y = y; }

private:
    SDL_Rect rect;
    int speed;
    int dx, dy;
};

#endif // PLAYER_H
