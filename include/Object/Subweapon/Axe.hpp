#ifndef SUBWEAPON_AXE_HPP
#define SUBWEAPON_AXE_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include "IUseSubweapon.hpp"

namespace Subweapon {
   class Axe : public Loot, public IUseSubweapon {
   public:
      Axe(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

      void Use() override;
   };
}
#endif
