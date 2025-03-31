#ifndef SUBWEAPON_DAGGER_HPP
#define SUBWEAPON_DAGGER_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include "IUseSubweapon.hpp"

namespace Subweapon {
   class Dagger : public Loot, public IUseSubweapon {
   public:
      Dagger(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

      void Use() override;
   };
}
#endif
