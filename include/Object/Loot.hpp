#ifndef LOOT_HPP
#define LOOT_HPP

#include "Object/AnimatedItems.hpp"
#include <string>

enum class LootType {
   // Sub weapon
   Axe,
   HolyWater,
   Dagger,
   Stopwatch,
   
   // Whip
   Whip,

   // Utility
   Pot,        // invisibility
   Rosary,     // kill all enemies

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

   Loot();

   LootType GetType();

   void IsCollected();

private:
   LootType type;
   bool isCollected;
};

#endif
