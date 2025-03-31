#ifndef SUBWEAPON_STOPWATCH_HPP
#define SUBWEAPON_STOPWATCH_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include "IUseSubweapon.hpp"

namespace Subweapon {
   class Stopwatch : public Loot, public IUseSubweapon {
   public:
      Stopwatch(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

      void Use() override;
   };
}
#endif
