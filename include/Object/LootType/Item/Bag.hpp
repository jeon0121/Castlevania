#ifndef LOOT_BAG_HPP
#define LOOT_BAG_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
   class Bag : public Loot {
   public:
      Bag(glm::vec2 position, LootType type);

      std::vector<std::string> GetImage();

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

   private:
      int addScore;
   };
};
#endif
