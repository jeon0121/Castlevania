#ifndef LOOT_HPP
#define LOOT_HPP

#include "Object/AnimatedItems.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"
#include "State/Menu.hpp"

enum class LootType {
   // Sub weapon
   Axe,
   HolyWater,
   Dagger,
   Stopwatch,
   
   // Whip
   Whip,

   // Relic
   Pot,        // invisibility
   Rosary,     // kill all enemies
   Badge,      // badge

   // Heals
   Crystal,    // recover full life (when boss defeated)
   Chicken,    // recover 6 life (hit blocks at stage 2)

   // Bags
   RedBag,     // +100
   WhiteBag,   // +700
   PurpleBag,  // +400
   SpecialBag, // +1000

   // Hearts
   HeartSmall, // +1
   HeartBig    // +5
};

class Loot : public AnimatedItems {
public:
   Loot(glm::vec2 position, std::vector<std::string> animationPath, int interval);

   LootType GetType();

   glm::vec2 UpdatePosition();

   virtual void Result(std::shared_ptr<Menu>& menu) = 0;

   void Fall(const std::vector<std::shared_ptr<Block>>& m_Blocks);

   void IsCollected(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu);

private:
   LootType type;
   float x_vel = 0;
   float y_vel = 0;
   float itemBottom;
   float itemTop;
   float itemLeft;
   float itemRight;
   bool is_landed = false;
};

#endif
