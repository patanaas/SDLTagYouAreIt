#include "NPC.h"
#include <cmath>

NPC::NPC() : texture(nullptr), position(0, 0), isTagged(false), speed(60.0f) {}

NPC::~NPC() {
    delete texture;
}

void NPC::Load(const std::string& filePath) {
    texture = new Texture();
    texture->Load(filePath);
}

void NPC::Update(const Point& playerPosition, float deltaTime) {
    if (isTagged) return;

    float dx = playerPosition.X - position.X;
    float dy = playerPosition.Y - position.Y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        position.X += (dx / distance) * speed * deltaTime;
        position.Y += (dy / distance) * speed * deltaTime;
    }
}

void NPC::Serialize(std::ostream& stream) {
    stream.write(reinterpret_cast<char*>(&position), sizeof(position));
    stream.write(reinterpret_cast<char*>(&isTagged), sizeof(isTagged));
}

void NPC::Deserialize(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&position), sizeof(position));
    stream.read(reinterpret_cast<char*>(&isTagged), sizeof(isTagged));
}

Rect NPC::GetRect() const {
    return Rect(position.X, position.Y, texture->GetImageInfo()->width, texture->GetImageInfo()->height);
}
