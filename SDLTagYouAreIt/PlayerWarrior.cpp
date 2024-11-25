#include "PlayerWarrior.h"
#include "SpriteSheet.h"
//#include <SDL_image.h>

PlayerWarrior::PlayerWarrior(SDL_Renderer* renderer)
    : renderer(renderer), position(960, 540), speed(100.0f) {
    SpriteSheet* rockSheet = SpriteSheet::Pool->GetResource();
    rockSheet->Load("../Assets/Textures/Warrior.tga");// Replace with your asset path
}

PlayerWarrior::~PlayerWarrior()
{
}

void PlayerWarrior::update() {
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    if (keystate[SDL_SCANCODE_UP]) position.y -= speed * 0.016f;
    if (keystate[SDL_SCANCODE_DOWN]) position.y += speed * 0.016f;
    if (keystate[SDL_SCANCODE_LEFT]) position.x -= speed * 0.016f;
    if (keystate[SDL_SCANCODE_RIGHT]) position.x += speed * 0.016f;
}

void PlayerWarrior::render() {
    SDL_Rect dst = { static_cast<int>(position.x), static_cast<int>(position.y), 64, 64 };
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
}

void PlayerWarrior::Serialize(std::ostream& _stream) 
{
    _stream.write(reinterpret_cast<const char*>(&position), sizeof(position));
}

void PlayerWarrior::Deserializable(std::istream& _stream) 
{
    _stream.read(reinterpret_cast<char*>(&position), sizeof(position));
}
