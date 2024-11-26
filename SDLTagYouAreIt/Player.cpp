#include "Player.h"

Player::Player(SDL_Renderer* renderer, int x, int y, int speed) : rect{ x, y, 50, 50 }, speed(speed), dx(0), dy(0) {}

Player::~Player() {}

void Player::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP: dy = -1; break;
        case SDLK_DOWN: dy = 1; break;
        case SDLK_LEFT: dx = -1; break;
        case SDLK_RIGHT: dx = 1; break;
        }
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        case SDLK_UP: case SDLK_DOWN: dy = 0; break;
        case SDLK_LEFT: case SDLK_RIGHT: dx = 0; break;
        }
    }
}

void Player::update(float deltaTime) {
    rect.x += dx * speed * deltaTime;
    rect.y += dy * speed * deltaTime;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
