#ifndef TORCH_HPP
#define TORCH_HPP

#include "Object/AnimatedItems.hpp"
#include "Object/Character.hpp"
#include "Object/Loot.hpp"

class Torch : public AnimatedItems {
public:
   Torch(glm::vec2 position, glm::vec2 scale, LootType itemType, int type=1);

   void CollideDetection(std::shared_ptr<Character> &character);

   void IsWhipped();

   bool is_destroyed;

private:
   int type;
   float torchTop;
   float torchBottom;
   float torchLeft;
   float torchRight;
   LootType itemType;
   std::vector<std::string> torchDeath;
};

#endif
