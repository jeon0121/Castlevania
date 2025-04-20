#ifndef OBJECT_DATA_HPP
#define OBJECT_DATA_HPP

#include "Object/Loot.hpp"

struct TorchData {
   glm::vec2 pos;
   glm::vec2 scale;
   LootType loot;
   int type;
};

struct BlockData {
   glm::vec2 pos;
   glm::vec2 scale;
};

struct StairData {
   glm::vec2 pos1;
   glm::vec2 pos2;
};

struct PossibleLootData {
   LootType loot;
   float chance;
   int maxSpawn;
};

struct ZombieData {
   glm::vec2 range;
   int numZombie;
   float yPos;
};

struct LeopardData {
   glm::vec2 pos;
};

#endif