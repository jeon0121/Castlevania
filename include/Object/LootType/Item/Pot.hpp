#ifndef LOOT_POT_HPP
#define LOOT_POT_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
   class Pot : public Loot {
   public:
      Pot(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;
   };
}
#endif
