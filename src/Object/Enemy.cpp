#include "Object/Enemy.hpp"
#include "Object/Loot.hpp"

Enemy::Enemy(glm::vec2 position, std::string direction, std::vector<std::string> animationPath, int interval, int score)
    : AnimatedItems(animationPath, interval), score(score) {
    this->direction = direction;
    SetPosition(position);
    SetZIndex(7);
    for (int i = 0; i < 4; ++i) {
        deathImages.emplace_back(GA_RESOURCE_DIR"/items/fire/torch/torchDeath-" + std::to_string(i + 1) + ".png");
    }
}

void Enemy::Death(App* app, std::vector<std::shared_ptr<Loot>> &m_Loots, std::vector<PossibleLootData> &m_PossibleLoots) {
    if (IfAnimationStart())
        SetAnimationFrames(deathImages, 120);
    SetPlaying();
    if (IfAnimationEnds()) {
        SetPaused();
        SetVisible(false);
        for (auto& lootData : m_PossibleLoots) {
            if (lootData.spawn < lootData.maxSpawn || lootData.maxSpawn == -1) {
                if (static_cast<float>(std::rand()) / RAND_MAX <= lootData.chance) {
                    lootType = lootData.loot;
                    lootData.spawn++;
                    break;
                }
            }
        }
        std::shared_ptr<Loot> loot = Loot::CreateLoot(lootType, GetPosition());
        if (loot) {
            m_Loots.push_back(loot);
            app->m_Root.AddChild(loot);
        }
        lootType = LootType::None;
    }
}

void Enemy::Flip() {
    glm::vec2 scale = m_Transform.scale;
    m_Transform.scale = glm::vec2(-1 * scale.x, scale.y);
}

void Enemy::UpdatePosition() {
    glm::vec2 enemyPos = GetPosition();
    glm::vec2 enemySize = glm::abs(GetScaledSize());
    enemyTop = enemyPos.y + enemySize.y * 0.5f;
    enemyBottom = enemyPos.y - enemySize.y * 0.5f;
    enemyLeft = enemyPos.x - enemySize.x * 0.5f;
    enemyRight = enemyPos.x + enemySize.x * 0.5f;
}

bool Enemy::CollideDetection(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
    int frameIndex = character->m_Behavior->GetCurrentFrameIndex();
    int whipLevel = character->GetWhipLevel();
    UpdatePosition();
    glm::vec2 charPos = character->GetPosition();
    glm::vec2 charSize = character->GetSize();
    float charLeft = charPos.x - charSize.x * 0.5f;
    float charRight = charPos.x + charSize.x * 0.5f;

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
                menu->modifyNumber(menu->formatScore(menu->m_value.score + score), 0, (menu->m_value.score + score));
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
            character->m_SubWeapon->SetIsCollide(true);
        }
    } else {
        if (!character->GetLevelUpWhipFlag() && !character->GetHurtFlag() && !is_dead && !is_hidden) {
            bool overlapX = (charLeft > enemyLeft && charLeft < enemyRight) ||
                                (charRight > enemyLeft && charRight < enemyRight) ||
                                (charLeft < enemyLeft && charRight > enemyRight);
            bool overlapY = enemyTop > charPos.y && enemyBottom < charPos.y;
            if (overlapX && overlapY) {
                bool ifNeedSlip = false;
                if (direction == character->GetDirection() &&
                    ((direction == "right" && character->GetPosition().x > GetPosition().x) ||
                     (direction == "left" && character->GetPosition().x < GetPosition().x))) {
                    ifNeedSlip = true;
                }
                character->SetHurtFlag(true, ifNeedSlip);
                character->SetHeart(character->GetHeart() - countHurt);
                menu->modifyHealth(character->GetHeart(), "player");
            }
        }
    }
    return is_dead;
}

void Enemy::InWindowDetection(int screenWidth) {
    if (!is_dead) {
        glm::vec2 pos = GetPosition();
        bool outOfWindow = (pos.x < screenWidth * -0.5 || pos.x > screenWidth * 0.5);
        if (is_hidden) {
            SetVisible(false);
            return;
        }
        if (outOfWindow) {
            if (hasEnteredWindow) {
                is_hidden = true;
                SetVisible(false);
            } else {
                SetVisible(true);
            }
        } else {
            hasEnteredWindow = true;
            SetVisible(true);
        }
    }
}

void Enemy::SetHidden(bool hidden) {
    is_hidden = hidden;
    SetVisible(!hidden);
}

void Enemy::SetDead(bool dead) {
    is_dead = dead;
}

bool Enemy::CheckReset() {
    return (is_hidden || is_dead);
}

bool Enemy::IsDead() {
    return is_dead;
}