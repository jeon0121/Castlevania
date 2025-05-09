#include "Object/LootType/Item/Rosary.hpp"
#include "State/Scene.hpp"

namespace LootItem {
   Rosary::Rosary(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/rosary.png"}, 0, LootType::Rosary) {}

   void Rosary::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
      (void) character;
      (void) menu;
      auto enemyManager = app->m_Scene->GetEnemiesManager();
      if (enemyManager) {
         for (auto& enemy : enemyManager->m_Enemies) {
            if (enemy) {
               enemy->SetPaused();
               enemy->SetDead(true);
            }
         }
      }
      app->m_Scene->Blink();
      is_endResult = true;
   }
}