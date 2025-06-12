#include "Object/LootType/Item/Heart.hpp"
#include "Utility/Time.hpp"

namespace LootItem {
Heart::Heart(glm::vec2 position, LootType type) : Loot(position, {GA_RESOURCE_DIR "/Sound Effects/22.wav"}, {}, 0, type) {
   SetPosition({position.x, position.y + 20.0f});
   this->type = type;
   switch(type) {
      case LootType::HeartSmall:
         SetAnimationFrames({GA_RESOURCE_DIR"/items/heart/heart-1.png"}, 0);
         addAmmo = 1;
         y_vel = -0.7f;
         break;
      case LootType::HeartBig:
         SetAnimationFrames({GA_RESOURCE_DIR"/items/heart/heart-2.png"}, 0);
         addAmmo = 5;
         break;
      default:
         break;
   }
}

void Heart::Fall(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
   glm::vec2 itemPos = UpdatePosition();
   glm::vec2 itemSize = glm::abs(GetScaledSize());
   if (type == LootType::HeartBig){
      SetPosition(itemPos + glm::vec2(x_vel, y_vel));
      y_vel = std::max(y_vel - ((y_vel >= -2.0f) ? 0.5f : 1.3f), -15.0f);
   }
   if (!is_landed){
      if (type == LootType::HeartSmall){
         itemPos.y += y_vel;
         itemPos.x -= 3 * sin(itemPos.y / 28);
         SetPosition(itemPos + glm::vec2(x_vel, y_vel));
      }
      for (auto &&block : m_Blocks) {
         glm::vec2 blockPos = block->GetPosition();
         glm::vec2 blockSize = glm::abs(block->GetScaledSize());
         float blockTop = blockPos.y + blockSize.y * 0.5f;
         float blockLeft = blockPos.x - blockSize.x * 0.5f;
         float blockRight = blockPos.x + blockSize.x * 0.5f;
         bool overlapX = itemRight > blockLeft && itemLeft < blockRight;
         bool isLanding = itemBottom <= blockTop && itemPos.y > blockTop;
         if (overlapX && isLanding) {
            is_landed = true;
            SetPosition({itemPos.x, blockTop + itemSize.y * 0.2f});
            break;
         }
      }
   }
   if (is_landed) {
      y_vel = 0.0f;
      x_vel = 0.0f;
      if (startLandTime == 0)
         startLandTime = SDL_GetPerformanceCounter();
      else if (Time::GetRunTimeMs(startLandTime) > 1600.0f)
         is_endResult = true;
   }
}

void Heart::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
   (void) app;
   menu->modifyNumber(menu->formatTwoDigits(character->GetAmmo() + addAmmo), 3);
   character->SetAmmo(character->GetAmmo() + addAmmo);
   is_endResult = true;
}
}