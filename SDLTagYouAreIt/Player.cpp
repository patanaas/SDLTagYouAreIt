#include "Player.h"
#include "TGAReader.h"
#include <iostream>

const int FRAME_WIDTH = 30;
const int FRAME_HEIGHT = 45;
const float PLAYER_SCALE = 1.25f;
const float FRAME_TIME = 0.15f;


const int IDLE_START = 0;
const int IDLE_END = 10;
const int RUN_START = 15;
const int RUN_END = 29;

Player::Player(SDL_Renderer* renderer, const std::string& spritePath)
    : speed(100), dx(0), dy(0), spriteSheet(nullptr), anim(nullptr) {
    rect = { 1920 / 2, 1080 / 2, static_cast<int>(FRAME_WIDTH * PLAYER_SCALE), static_cast<int>(FRAME_HEIGHT * PLAYER_SCALE) };

    SDL_Texture* warriorTexture = TGAReader::loadTGA(spritePath, renderer);
    if (!warriorTexture) {
        std::cerr << "Failed to load Warrior.tga" << std::endl;
        return;
    }

    spriteSheet = new SpriteSheet(warriorTexture, FRAME_WIDTH, FRAME_HEIGHT);
    anim = new SpriteAnim(spriteSheet, FRAME_TIME);

    anim->setAnimationRange(IDLE_START, IDLE_END); 
}


Player::~Player() {
    delete anim;
    delete spriteSheet;
}

void Player::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:    dy = -1; break;
        case SDLK_DOWN:  dy = 1; break;
        case SDLK_LEFT:  dx = -1; break;
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
    bool isIdle = (dx == 0 && dy == 0);

    if (isIdle) {
        if (anim->getStartFrame() != IDLE_START || anim->getEndFrame() != IDLE_END) {
            anim->setAnimationRange(IDLE_START, IDLE_END);
            anim->reset(); 
        }
    }
    else {

        if (anim->getStartFrame() != RUN_START || anim->getEndFrame() != RUN_END) {
            anim->setAnimationRange(RUN_START, RUN_END);
            anim->reset(); 
        }
    }
    anim->update(deltaTime);

    rect.x += static_cast<int>(dx * speed * deltaTime);
    rect.y += static_cast<int>(dy * speed * deltaTime);
}




void Player::render(SDL_Renderer* renderer) {
    float angle = (dx > 0) ? 0 : (dx < 0) ? 180 : 0; 
    SDL_Color blueTint = { 0, 0, 255, 255 };

    anim->render(renderer, rect.x, rect.y, PLAYER_SCALE, angle, blueTint);
}
