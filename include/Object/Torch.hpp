#ifndef TORCH_HPP
#define TORCH_HPP

#include "Object/AnimatedItems.hpp"
#include "Object/Character.hpp"
#include "Object/Loot.hpp"
#include "Object/LootType/Loot.hpp"
#include <Util/SFX.hpp>
#include "App.hpp"

class Torch : public AnimatedItems {
public:
   Torch(glm::vec2 position, glm::vec2 scale, LootType itemType, int type=1);

   void UpdatePosition();

   bool CollideDetection(std::shared_ptr<Character> &character);

   void Destroy(App* app, std::shared_ptr<Character> character, bool &whipDropped, std::vector<std::shared_ptr<Util::GameObject>> &m_All);

   LootType GetLootType() const;

   std::shared_ptr<Loot> loot = nullptr;

private:
   int type;
   float torchTop;
   float torchBottom;
   float torchLeft;
   float torchRight;
   bool is_destroyed = false;
   LootType itemType;
   std::vector<std::string> torchDeath;
   std::shared_ptr<Util::SFX> destroySound;
};

#endif
