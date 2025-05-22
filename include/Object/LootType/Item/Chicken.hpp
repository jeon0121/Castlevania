#ifndef LOOT_CHICKEN_HPP
#define LOOT_CHICKEN_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
   class Chicken : public Loot {
   public:
      Chicken(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;
   };
};
#endif
