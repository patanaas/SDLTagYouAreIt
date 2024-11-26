#include "Character.h"

Character::Character(SpriteSheet* _sprite, Position _position, float _speed)
    : sprite(_sprite), position(_position), speed(_speed), isTagged(false) {}

Player::Player(SpriteSheet* _sprite, Position _position, float _speed)
    : Character(_sprite, _position, _speed) {}

void Player::Update(float deltaTime)
{
    InputController* input = &InputController::Instance();
    Keyboard* kb = input->KB();
    SDL_Event event;

    // Handle player movement
    if (SDL_PollEvent(&event))
    {
        if (kb->KeyDown(event, SDLK_LEFT))
        {
            position.X -= speed * deltaTime;
            sprite->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
        }
        else if (kb->KeyDown(event, SDLK_RIGHT))
        {
            position.X += speed * deltaTime;
            sprite->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
        }
        else if (kb->KeyDown(event, SDLK_UP))
        {
            position.Y -= speed * deltaTime;
            sprite->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
        }
        else if (kb->KeyDown(event, SDLK_DOWN))
        {
            position.Y += speed * deltaTime;
            sprite->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
        }
        else
        {
            sprite->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
        }
    }
}

NPC::NPC(SpriteSheet* _sprite, Position _position, float _speed)
    : Character(_sprite, _position, _speed) {}

void NPC::Update(float deltaTime)
{
    // NPC behavior: move closer to or away from player
    if (!isTagged)
    {
        Player* player = dynamic_cast<Player*>(&GameController::Instance());
        float distance = std::sqrt(std::pow(position.X - player->position.X, 2) + std::pow(position.Y - player->position.Y, 2));

        if (distance < NPC_CLOSE_DISTANCE)
        {
            // Move away from player
            position.X += (position.X - player->position.X) / distance * speed * deltaTime;
            position.Y += (position.Y - player->position.Y) / distance * speed * deltaTime;
        }
        else if (distance > NPC_FAR_DISTANCE)
        {
            // Move towards player
            position.X -= (position.X - player->position.X) / distance * speed * deltaTime;
            position.Y -= (position.Y - player->position.Y) / distance * speed * deltaTime;
        }
    }
}
