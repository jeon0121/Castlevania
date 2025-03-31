#ifndef LOOT_WHIP_HPP
#define LOOT_WHIP_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
   class Whip : public Loot {
   public:
      Whip(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;
   private:
      std::vector<std::string> levelUpImage;
   };
}
#endif
