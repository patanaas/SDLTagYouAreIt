#include "Player.h"
#include "Keyboard.h"

Player::Player() : texture(nullptr), position(0, 0) {}

Player::~Player() {
    delete texture;
}

void Player::Load(const std::string& filePath) {
    texture = new Texture();
    texture->Load(filePath);
}

void Player::Update(float deltaTime) {
    if (Keyboard::Instance().IsKeyDown(SDL_SCANCODE_W)) position.Y -= 100 * deltaTime;
    if (Keyboard::Instance().IsKeyDown(SDL_SCANCODE_S)) position.Y += 100 * deltaTime;
    if (Keyboard::Instance().IsKeyDown(SDL_SCANCODE_A)) position.X -= 100 * deltaTime;
    if (Keyboard::Instance().IsKeyDown(SDL_SCANCODE_D)) position.X += 100 * deltaTime;
}

void Player::Serialize(std::ostream& stream) {
    stream.write(reinterpret_cast<char*>(&position), sizeof(position));
}

void Player::Deserialize(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&position), sizeof(position));
}

Rect Player::GetRect() const {
    return Rect(position.X, position.Y, texture->GetImageInfo()->width, texture->GetImageInfo()->height);
}
