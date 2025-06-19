#include "Object/Enemies/Bat.hpp"

Bat::Bat(glm::vec2 position, std::string direction): Enemy(position, direction, {}, 250, 100) {
   for (int i = 0; i < 3; ++i) {
      batImages.emplace_back(GA_RESOURCE_DIR"/enemies/bat/bat-" + std::to_string(i + 1) + ".png");
   }
   x_initial = position.x;
   SetAnimationFrames(batImages, 250);
   m_Transform.scale = glm::vec2(1, 0.9f);
   if (direction == "right")
      Flip();
   SetPlaying();
   SetLooping(true);
   countHurt = 2;
}

void Bat::MoveBehav() {
   glm::vec2 pos = GetPosition();
   pos.y -= 2 * sin(x_initial / 50.0f);
   pos.x += (direction == "right" ? 3 : -3);
   x_initial += (direction == "right" ? 3 : -3);
   SetPosition(pos);
}

bool Bat::CollideDetection(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu, int modeState) {
   int frameIndex = character->m_Behavior->GetCurrentFrameIndex();
   int whipLevel = character->GetWhipLevel();
   UpdatePosition();
   glm::vec2 charPos = character->GetPosition();
   glm::vec2 charSize = character->GetSize();
   float charLeft = charPos.x - charSize.x * 0.5f;
   float charRight = charPos.x + charSize.x * 0.5f;
   float charTop = charPos.y + charSize.y * 0.5f;
   float charBottom = charPos.y - charSize.y * 0.5f;

   if (character->IfWhip() && !is_dead) {
      bool isNormalWhip = (whipLevel != 3 && (frameIndex == 2 || frameIndex == 3));
      bool isLv3Whip = (whipLevel == 3 && frameIndex >= 8 && frameIndex <= 15);
      if (isNormalWhip || isLv3Whip) {
         float whipWidth = (whipLevel == 3) ? abs(character->OffsetValues("whipWidth_lv3")) : abs(character->OffsetValues("whipWidth"));
         float whipLeft = (character->GetDirection() == "left") ? charLeft - whipWidth : charRight;
         float whipRight = (character->GetDirection() == "left") ? charLeft : charRight + whipWidth;

         bool overlapX = (whipLeft > enemyLeft && whipLeft < enemyRight) ||
                         (whipRight > enemyLeft && whipRight < enemyRight) ||
                         (whipLeft < enemyLeft && whipRight > enemyRight);
         bool overlapY = enemyTop > charPos.y && enemyBottom < charPos.y;

         if (overlapX && overlapY) {
            SetPaused();
            is_dead = true;
            deadSound->Play();
            menu->modifyNumber(menu->formatScore(menu->GetScore() + score), 0, (menu->GetScore() + score));
         }
      }
   } else if (character->m_SubWeapon != nullptr && character->GetUseWeaponFlag() && !is_dead) {
      std::shared_ptr<Loot> asLoot = std::dynamic_pointer_cast<Loot>(character->m_SubWeapon);
      glm::vec2 pos = asLoot->GetPosition();
      glm::vec2 size = glm::abs(asLoot->GetScaledSize());
      float weaponLeft = pos.x - size.x * 0.5f;
      float weaponRight = pos.x + size.x * 0.5f;
      bool overlapX = (weaponLeft > enemyLeft && weaponLeft < enemyRight) ||
                      (weaponRight > enemyLeft && weaponRight < enemyRight);
      bool overlapY = enemyTop > pos.y && enemyBottom < pos.y;
      if (overlapX && overlapY && character->GetSubWeaponType() != WeaponType::Stopwatch) {
         SetPaused();
         is_dead = true;
         if (character->GetSubWeaponType() != WeaponType::HolyWater)
            deadSound->Play();
         character->m_SubWeapon->SetIsCollide(true);
      }
   } else {
      if (!character->GetLevelUpWhipFlag() && !character->GetHurtFlag() && !character->GetInvisibleFlag() && !character->GetDeadFlag() && !is_dead && !is_hidden) {
         bool overlapX = (charLeft > enemyLeft && charLeft < enemyRight)   ||
                         (charRight > enemyLeft && charRight < enemyRight) ||
                         (charLeft < enemyLeft && charRight > enemyRight);
         bool overlapY = (enemyTop > charPos.y && enemyBottom < charPos.y) ||
                         (charTop > enemyBottom && charBottom < enemyTop);
         if (overlapX && overlapY) {
            bool ifNeedSlip = false;
            SetPaused();
            is_dead = true;
            deadSound->Play();
            if (direction == character->GetDirection() &&
               ((direction == "right" && character->GetPosition().x > GetPosition().x) ||
                (direction == "left" && character->GetPosition().x < GetPosition().x))) {
               ifNeedSlip = true;
            }
            if (modeState == 0) {
               character->SetHurtFlag(true, ifNeedSlip);
               character->SetHeart(character->GetHeart() - countHurt);
               menu->modifyHealth(character->GetHeart(), "player");
            }
         }
      }
   }
   return is_dead;
}

void Bat::SetReset() {
   is_dead = false;
   is_hidden = false;
   hasEnteredWindow = false;
   SetAnimationFrames(batImages, 250);
   SetPlaying();
   SetLooping(true);
}