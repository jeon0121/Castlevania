#include "Object/Loot.hpp"
#include "Object/LootType/Loot.hpp"

Loot::Loot(glm::vec2 position, std::vector<std::string> animationPath, int interval) : AnimatedItems(animationPath, interval) {
   SetPosition(position);
   SetZIndex(7);
}

LootType Loot::GetType() { return type; }

glm::vec2 Loot::UpdatePosition() {
   glm::vec2 itemPos = GetPosition();
   glm::vec2 itemSize = glm::abs(GetScaledSize());
   itemBottom = itemPos.y - itemSize.y * 0.5f;
   itemTop = itemPos.y + itemSize.y * 0.5f;
   itemLeft = itemPos.x - itemSize.x * 0.5f;
   itemRight = itemPos.x + itemSize.x * 0.5f;
   return itemPos;
}

void Loot::Fall(const std::vector<std::shared_ptr<Block>>& m_Blocks){
   glm::vec2 itemPos = UpdatePosition();
   SetPosition(itemPos + glm::vec2(x_vel, y_vel));
   y_vel = std::max(y_vel - ((y_vel >= -2.0f) ? 0.5f : 1.5f), -15.0f);

   if (!is_landed){
      for (auto &&block : m_Blocks) {
         glm::vec2 blockPos = block->GetPosition();
         glm::vec2 blockSize = glm::abs(block->GetScaledSize());
         float blockTop = blockPos.y + blockSize.y * 0.5f + 8.5f;
         float blockLeft = blockPos.x - blockSize.x * 0.5f;
         float blockRight = blockPos.x + blockSize.x * 0.5f;
         bool overlapX = itemRight > blockLeft && itemLeft < blockRight;
         bool isLanding = itemBottom <= blockTop;
         if (overlapX && isLanding) {
            is_landed = true;
            break;
         }
      }
   }
   if (is_landed) 
      y_vel = 0.0f;
}

bool Loot::IsCollected(std::shared_ptr<Character>& character) {
   UpdatePosition();
   glm::vec2 charPos = character->GetPosition();
   glm::vec2 charSize = character->GetSize();
   float charBottom = charPos.y - charSize.y * 0.5f;
   float charTop = charPos.y + charSize.y * 0.5f;
   float charLeft = charPos.x - charSize.x * 0.5f;
   float charRight = charPos.x + charSize.x * 0.5f;

   bool overlapX = itemRight > charLeft && itemLeft < charRight;
   bool overlapY = itemTop > charBottom && itemBottom < charTop;

   if (overlapX && overlapY) {
      SetVisible(false);
      is_collected = true;
      return true;
   }
   return false;
}

bool Loot::IfCollected() const {
   return is_collected;
}

bool Loot::IfEnded() const {
   return is_endResult;
}

std::shared_ptr<Loot> Loot::CreateLoot(LootType itemType, glm::vec2 position) {
   switch (itemType) {
      // Sub weapon
      case LootType::Axe:
         return std::make_shared<Subweapon::Axe>(position);
      case LootType::Dagger:
         return std::make_shared<Subweapon::Dagger>(position);
      case LootType::HolyWater:
         return std::make_shared<Subweapon::HolyWater>(position);
      case LootType::Stopwatch:
         return std::make_shared<Subweapon::Stopwatch>(position);

      // Whip
      case LootType::Whip:
         return std::make_shared<LootItem::Whip>(position);

      // Bags
      case LootType::RedBag:
         return std::make_shared<LootItem::Bag>(position, LootType::RedBag);
      case LootType::WhiteBag:
         return std::make_shared<LootItem::Bag>(position, LootType::WhiteBag);
      case LootType::PurpleBag:
         return std::make_shared<LootItem::Bag>(position, LootType::PurpleBag);
      case LootType::SpecialBag:
         return std::make_shared<LootItem::Bag>(position, LootType::SpecialBag);

      // Items
      case LootType::HeartSmall:
         return std::make_shared<LootItem::Heart>(position, LootType::HeartSmall);
      case LootType::HeartBig:
         return std::make_shared<LootItem::Heart>(position, LootType::HeartBig);
      default:
         break;
   }
   return nullptr;
}