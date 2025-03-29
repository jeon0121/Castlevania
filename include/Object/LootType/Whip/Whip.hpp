#ifndef LOOT_WHIP_HPP
#define LOOT_WHIP_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class Whip : public Loot {
public:
   Whip(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/whip.png"}, 0) {}

   void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      (void) app;
      (void) menu;
      if (levelUpImage.empty()) {
         for (int i = 0; i < 4; ++i) {
            levelUpImage.emplace_back(GA_RESOURCE_DIR"/main character/color/color-" + std::to_string(i + 1) + ".png");
         }
         character->m_Behavior->SetPaused();
         character->LevelUpWhip();
         character->m_Behavior->SetAnimationFrames(levelUpImage, 30);
         character->m_Behavior->SetPlaying();
         character->m_Behavior->SetLooping(true);
      }
      if (character->m_Behavior->IfPlayingTime(0.9)) {
         character->ChangeBehavior(2);
         character->SetLevelUpWhip(false);
         is_endResult = true;
      }
   }
private:
   std::vector<std::string> levelUpImage;
};
}
#endif
