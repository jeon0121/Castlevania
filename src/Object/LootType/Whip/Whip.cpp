#include "Object/LootType/Whip/Whip.hpp"

LootItem::Whip::Whip(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/Sound Effects/24.wav"}, {GA_RESOURCE_DIR "/items/weapon/whip.png"}, 0, LootType::Whip) {}

void LootItem::Whip::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
   (void) app;
   (void) menu;
   if (levelUpImage.empty()) {
      for (int i = 0; i < 4; ++i) {
         levelUpImage.emplace_back(GA_RESOURCE_DIR "/main character/color/color-" + std::to_string(i + 1) + ".png");
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