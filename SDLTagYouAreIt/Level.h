#ifndef LEVEL_H
#define LEVEL_H

#include "Serializable.h"
#include "SpriteSheet.h"
#include "BasicStructs.h"
#include "Keyboard.h"
#include <vector>

class Level : public Serializable {
public:
    // Constructors/Destructors
    Level();
    ~Level();

    // Accessors
    SpriteSheet* GetPlayer() { return m_player; }
    std::vector<SpriteSheet*>& GetNPCs() { return m_npcs; }
    int GetNPCTaggedCount() { return m_npcTaggedCount; }
    float GetNPCSpeed() { return m_npcSpeed; }

    void SetNPCSpeed(float speed) { m_npcSpeed = speed; }
    void IncrementNPCTaggedCount() { m_npcTaggedCount++; }

    // Methods
    void Initialize(); // Initializes player and NPCs
    void Update(float deltaTime, Keyboard* keyboard); // Update accepts a keyboard parameter // Updates player and NPC logic
    void Render(); // Renders all game entities

    // Serialization
    void Serialize(std::ostream& _stream) override;
    void Deserialize(std::istream& _stream) override;

private:
    // Members
    SpriteSheet* m_player;
    std::vector<SpriteSheet*> m_npcs;
    int m_npcTaggedCount;
    float m_npcSpeed;

    // Helper methods
    void CreatePlayer();
    void CreateNPCs();
};

#endif // LEVEL_H

