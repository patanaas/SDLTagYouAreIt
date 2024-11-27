#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include "SpriteAnim.h"
#include <string>

class Player {
public:
    Player(SDL_Renderer* renderer, const std::string& spritePath);
    virtual ~Player();

    void handleEvent(const SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    int getX() const { return rect.x; }
    int getY() const { return rect.y; }
    void setPosition(int x, int y) { rect.x = x; rect.y = y; }

private:
    SDL_Rect rect;
    int speed;
    int dx, dy;

    SpriteSheet* spriteSheet;
    SpriteAnim* anim;
};

#endif // PLAYER_H
