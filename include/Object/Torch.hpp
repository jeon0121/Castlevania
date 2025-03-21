#ifndef TORCH_HPP
#define TORCH_HPP

#include "Object/AnimatedItems.hpp"
#include "Object/Character.hpp"
#include "Object/Loot.hpp"
#include "Object/Loot/Subweapon/Axe.hpp"

class Torch : public AnimatedItems {
public:
   Torch(glm::vec2 position, glm::vec2 scale, LootType itemType, int type=1);

   void CollideDetection(std::shared_ptr<Character> &character);

   void Destroy();

   std::shared_ptr<Loot> CreateLoot(LootType itemType, glm::vec2 position);

   bool is_destroyed;

   bool IfCollected();

   void SetCollected();

   std::shared_ptr<Loot> loot = nullptr;
private:
   int type;
   float torchTop;
   float torchBottom;
   float torchLeft;
   float torchRight;
   bool is_collected = false;
   LootType itemType;
   std::vector<std::string> torchDeath;
};

#endif
