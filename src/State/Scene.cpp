#include "State/Scene.hpp"
#include "Util/Logger.hpp"

void Scene::UpdateTorch(App* app) {
    for (auto torch : m_Torches) 
        torch->Update(app, m_Character, m_Menu, m_Blocks, torch);
}

void Scene::UpdateScroll(int mapWidth) {
    glm::vec2 pos = m_Character->GetPosition();
    glm::vec2 lastPos = m_Character->GetLastPosition();
    float dx = pos.x - lastPos.x;
    if (((pos.x >= 4.5) && (dx > 0) && (offsetX < mapWidth - screenWidth)) ||
        ((pos.x <= -4.5) && (dx < 0) && (offsetX > 0))) {
        offsetX += dx;
        m_Background->SetPosition(glm::vec2(1050, -60) - glm::vec2(offsetX, 0.0f));
        m_Character->SetPosition(m_Character->GetPosition() - glm::vec2(dx, 0.0f));
        for (auto& block : m_Blocks) {
            block->SetPosition(block->GetPosition() - glm::vec2(dx, 0.0f));
        }
        for (auto& torch : m_Torches) {
            torch->SetPosition(torch->GetPosition() - glm::vec2(dx, 0.0f));
            if (torch->loot)
                torch->loot->SetPosition(torch->loot->GetPosition() - glm::vec2(dx, 0.0f));
        }
    }
}

void Scene::UpdateSubWeapon(App* app) {
    if (m_Character->GetUseWeaponFlag() && m_Character->GetSubWeaponType() != WeaponType::None) {
        if (!m_SubWeapon) {
            SetSubweapon(app);
        }
        m_SubWeapon->Use(m_Character->GetDirection());
    }
}

void Scene::SetSubweapon(App* app) {
    glm::vec2 pos = m_Character->GetPosition();
    WeaponType type = m_Character->GetSubWeaponType();
    switch (type) {
        case WeaponType::Axe:
            m_SubWeapon = std::make_shared<LootItem::Axe>(pos);
            break;
        case WeaponType::Dagger:
            m_SubWeapon = std::make_shared<LootItem::Dagger>(pos);
            break;
        case WeaponType::HolyWater:
            m_SubWeapon = std::make_shared<LootItem::HolyWater>(pos);
            break;
        case WeaponType::Stopwatch:
            m_SubWeapon = std::make_shared<LootItem::Stopwatch>(pos);
            break;
        default:
            m_SubWeapon = nullptr;
    }
    if (m_SubWeapon) {
        std::string dir = m_Character->GetDirection();
        asLoot = std::dynamic_pointer_cast<Loot>(m_SubWeapon);
        asLoot->m_Transform.scale = (dir == "right") ? glm::vec2(-1, 1) : glm::vec2(1, 1);
        app->m_Root.AddChild(asLoot);
    }
}