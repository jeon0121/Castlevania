#include "State/Scene.hpp"
#include "Util/Logger.hpp"

void Scene::UpdateTorch(App* app) {
    for (auto torch : m_Torches) {
        if (torch->loot && !torch->loot->IfCollected()) {
            torch->loot->Fall(m_Blocks);
            if (torch->loot->IsCollected(m_Character))
                app->m_Root.RemoveChild(torch);
        }
        else if (torch->loot && torch->loot->IfCollected() && !torch->loot->IfEnded())
            torch->loot->Result(app, m_Character, app->m_Menu);
        if (torch->loot && torch->loot->IfEnded()) {
            if (torch->loot->GetType() == LootType::Whip)
                whipDropped = false;
            app->m_Root.RemoveChild(torch->loot);
        }
        if (torch->CollideDetection(m_Character) && !torch->loot)
            torch->Destroy(app, m_Character, whipDropped);
    }
}

void Scene::UpdateScroll(int mapWidth) {
    glm::vec2 pos = m_Character->GetPosition();
    glm::vec2 lastPos = m_Character->GetLastPosition();
    float dx = pos.x - lastPos.x;
    if (((pos.x >= 4.5) && (dx > 0) && (offsetX < mapWidth - screenWidth)) ||
        ((pos.x <= -4.5) && (dx < 0) && (offsetX > 0))) {
        offsetX += dx;
        m_Background->SetPosition(backgroundPos - glm::vec2(offsetX, 0.0f));
        m_Character->SetPosition(m_Character->GetPosition() - glm::vec2(dx, 0.0f));
        for (auto& block : m_Blocks) {
            block->SetPosition(block->GetPosition() - glm::vec2(dx, 0.0f));
        }
        for (auto& stair : m_Stairs) {
            stair->SetPosition(stair->GetPosition() - glm::vec2(dx, 0.0f));
        }
        for (auto& torch : m_Torches) {
            torch->SetPosition(torch->GetPosition() - glm::vec2(dx, 0.0f));
            if (torch->loot) {
                torch->loot->SetPosition(torch->loot->GetPosition() - glm::vec2(dx, 0.0f));
                if (torch->loot->score)
                    torch->loot->score->SetPosition(torch->loot->score->GetPosition() - glm::vec2(dx, 0.0f));
            }
        }
        if (m_EnemiesManager) {
            for (auto& enemy : m_EnemiesManager->m_Enemies) {
                if (enemy)
                    enemy->SetPosition(enemy->GetPosition() - glm::vec2(dx, 0.0f));
            }
            for (auto &&loot : m_EnemiesManager->m_Loots){
                if (loot)
                    loot->SetPosition(loot->GetPosition() - glm::vec2(dx, 0.0f));
            }
            
        }
        if (asLoot)
            asLoot->SetPosition(asLoot->GetPosition() - glm::vec2(dx, 0.0f));
    }
}

void Scene::UpdateSubWeapon(App* app) {
    if (m_Character->GetUseWeaponFlag() && m_Character->GetAmmo() > 0) {
        if (!m_Character->m_SubWeapon) {
            SetSubweapon(app);
            app->m_Menu->modifyNumber(app->m_Menu->formatTwoDigits(m_Character->GetAmmo()-1), 3);
        }else {
            m_Character->m_SubWeapon->Use();
            asLoot = std::dynamic_pointer_cast<Loot>(m_Character->m_SubWeapon);
            if (m_Character->m_SubWeapon->IsDestroyed() ||
                asLoot->GetPosition().x < -screenWidth / 2 ||
                asLoot->GetPosition().x > screenWidth / 2) {
                    m_Character->m_SubWeapon = nullptr;
                    app->m_Root.RemoveChild(asLoot);
                    m_Character->SetUseWeaponFlag(false);
                    m_Character->SetAmmo(m_Character->GetAmmo()-1);
                }
        }
    }
}

void Scene::SetSubweapon(App* app) {
    glm::vec2 pos = m_Character->GetPosition() + glm::vec2(0, 10);
    WeaponType type = m_Character->GetSubWeaponType();
    switch (type) {
        case WeaponType::Axe:
            m_Character->m_SubWeapon = std::make_shared<Subweapon::Axe>(pos);
            break;
        case WeaponType::Dagger:
            m_Character->m_SubWeapon = std::make_shared<Subweapon::Dagger>(pos);
            break;
        case WeaponType::HolyWater:
            m_Character->m_SubWeapon = std::make_shared<Subweapon::HolyWater>(pos);
            break;
        case WeaponType::Stopwatch:
            m_Character->m_SubWeapon = std::make_shared<Subweapon::Stopwatch>(pos);
            break;
        default:
            m_Character->m_SubWeapon = nullptr;
            break;
    }
    if (m_Character->m_SubWeapon) {
        std::string dir = m_Character->GetDirection();
        m_Character->m_SubWeapon->SetDirection(dir);
        asLoot = std::dynamic_pointer_cast<Loot>(m_Character->m_SubWeapon);
        asLoot->m_Transform.scale = (dir == "right") ? glm::vec2(-1, 1) : glm::vec2(1, 1);
        app->m_Root.AddChild(asLoot);
    }
}