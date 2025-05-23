#include "Object/Torch.hpp"

Torch::Torch(glm::vec2 position, glm::vec2 scale, LootType itemType, int type)
    : AnimatedItems((type == 1) ? std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-1.png", GA_RESOURCE_DIR "/items/fire/fire-2.png"}
                                : std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-3.png", GA_RESOURCE_DIR "/items/fire/fire-4.png"},
    120) {
    SetPosition(position);
    m_Transform.scale = scale;
    SetPlaying();
    SetLooping(true);
    SetZIndex(6);
    this->itemType = itemType;

    for (int i = 0; i < 4; ++i) {
        torchDeath.emplace_back(GA_RESOURCE_DIR"/items/fire/torch/torchDeath-" + std::to_string(i + 1) + ".png");
    }
    UpdatePosition();
}

void Torch::UpdatePosition() {
    glm::vec2 torchPos = GetPosition();
    glm::vec2 torchSize = glm::abs(GetScaledSize());
    torchTop = torchPos.y + torchSize.y * 0.5f;
    torchBottom = torchPos.y - torchSize.y * 0.5f;
    torchLeft = torchPos.x - torchSize.x * 0.5f;
    torchRight = torchPos.x + torchSize.x * 0.5f;
}

bool Torch::CollideDetection(std::shared_ptr<Character> &character) {
    int frameIndex = character->m_Behavior->GetCurrentFrameIndex();
    int whipLevel = character->GetWhipLevel();
    UpdatePosition();
    if (character->IfWhip() && !is_destroyed) {
        bool isNormalWhip = (whipLevel != 3 && (frameIndex == 2 || frameIndex == 3));
        bool isLv3Whip = (whipLevel == 3 && frameIndex >= 8 && frameIndex <= 15);
        if (isNormalWhip || isLv3Whip) {
            float whipWidth = (whipLevel == 3) ? abs(character->OffsetValues("whipWidth_lv3")) : abs(character->OffsetValues("whipWidth"));
            glm::vec2 charPos = character->GetPosition();
            glm::vec2 charSize = character->GetSize();
            float charLeft = charPos.x - charSize.x * 0.5f;
            float charRight = charPos.x + charSize.x * 0.5f;
            float whipLeft = (character->GetDirection() == "left") ? charLeft - whipWidth : charRight;
            float whipRight = (character->GetDirection() == "left") ? charLeft : charRight + whipWidth;

            bool overlapX = (whipLeft > torchLeft && whipLeft < torchRight) ||
                            (whipRight > torchLeft && whipRight < torchRight) ||
                            (whipLeft < torchLeft && whipRight > torchRight);
            bool overlapY = torchTop + 1.5f > charPos.y && torchBottom - 1.5f < charPos.y;

            if (overlapX && overlapY) {
                SetPaused();
                is_destroyed = true;
            }
        }
    }
    else if (character->m_SubWeapon != nullptr && character->GetUseWeaponFlag() && !is_destroyed) {
        std::shared_ptr<Loot> asLoot = std::dynamic_pointer_cast<Loot>(character->m_SubWeapon);
        glm::vec2 weaponPos = asLoot->GetPosition();
        glm::vec2 weaponSize = glm::abs(asLoot->GetScaledSize());
        float weaponLeft = weaponPos.x - weaponSize.x * 0.5f;
        float weaponRight = weaponPos.x + weaponSize.x * 0.5f;
        
        bool overlapX = (weaponLeft > torchLeft && weaponLeft < torchRight) ||
                        (weaponRight > torchLeft && weaponRight < torchRight);
        bool overlapY = torchTop > weaponPos.y && torchBottom < weaponPos.y;
        if (overlapX && overlapY && character->GetSubWeaponType() != WeaponType::Stopwatch) {
            SetPaused();
            is_destroyed = true;
            character->m_SubWeapon->SetDestroyed(true);
        }
    }
    return is_destroyed;
}

void Torch::Destroy(App* app, std::shared_ptr<Character> character, bool &whipDropped) {
    if (IfAnimationStart())
        SetAnimationFrames(torchDeath, 120);
    SetPlaying();
    if (IfPlayingTime(0.5)) {
        SetPaused();
        SetVisible(false);
        if (character->GetWhipLevel() != 3 && itemType == LootType::HeartSmall && !whipDropped) {
            itemType = LootType::Whip;
            whipDropped = true;
        }
        loot = Loot::CreateLoot(itemType, GetPosition());
        app->m_Root.AddChild(loot);
    }
}