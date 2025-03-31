#ifndef SUBWEAPON_HOLYWATER_HPP
#define SUBWEAPON_HOLYWATER_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include "IUseSubweapon.hpp"

namespace Subweapon {
   class HolyWater : public Loot, public IUseSubweapon {
   public:
      HolyWater(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

      void Use() override;
   };
}
#endif
