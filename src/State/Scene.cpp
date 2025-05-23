#include "State/Scene.hpp"
#include "Util/Logger.hpp"
#include "Utility/Time.hpp"

void Scene::UpdateTorch(App* app) {
    for (auto torch : m_Torches) {
        if (torch && torch->loot && !torch->loot->IfCollected() && !m_Character->GetLevelUpWhipFlag()) {
            torch->loot->Fall(m_Blocks);
            if (torch->loot->IsCollected(m_Character))
                app->m_Root.RemoveChild(torch);
        }
        else if (torch && torch->loot && torch->loot->IfCollected() && !torch->loot->IfEnded())
            torch->loot->Result(app, m_Character, app->m_Menu);
        if (torch && torch->loot && torch->loot->IfEnded()) {
            if (torch->loot->GetType() == LootType::Whip)
                whipDropped = false;
            app->m_Root.RemoveChild(torch->loot);
            m_Torches.erase(std::remove(m_Torches.begin(), m_Torches.end(), torch), m_Torches.end());
        }
        if (torch && torch->CollideDetection(m_Character) && !torch->loot)
            torch->Destroy(app, m_Character, whipDropped);
    }
}

void Scene::UpdateScroll(int mapWidth, float offset) {
    glm::vec2 pos = m_Character->GetPosition();
    glm::vec2 lastPos = m_Character->GetLastPosition();
    float dx = (offset != 0) ? offset : pos.x - lastPos.x;
    if (((pos.x >= 4.5) && (dx > 0) && (offsetX < mapWidth - screenWidth)) ||
        ((pos.x <= -4.5) && (dx < 0) && (offsetX > 0)) || offset != 0) {
        offsetX += dx;
        m_Background->SetPosition(backgroundPos - glm::vec2(offsetX, 0.0f));
        if (offset == 0)
            m_Character->SetPosition(m_Character->GetPosition() - glm::vec2(dx, 0.0f));
        if (m_Blink)
            m_Blink->SetPosition(m_Background->GetPosition());
        for (auto& block : m_Blocks) {
            block->SetPosition(block->GetPosition() - glm::vec2(dx, 0.0f));
        }
        for (auto& hitableBlock : m_HitableBlocks) {
            hitableBlock->SetPosition(hitableBlock->GetPosition() - glm::vec2(dx, 0.0f));
            if (hitableBlock->loot) {
                hitableBlock->loot->SetPosition(hitableBlock->loot->GetPosition() - glm::vec2(dx, 0.0f));
                if (hitableBlock->loot->score)
                    hitableBlock->loot->score->SetPosition(hitableBlock->loot->score->GetPosition() - glm::vec2(dx, 0.0f));
            }
            if (!hitableBlock->particles.empty()) 
                for (auto& particle : hitableBlock->particles) 
                    particle->SetPosition(particle->GetPosition() - glm::vec2(dx, 0.0f));
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
            for (auto &&loot : m_EnemiesManager->m_Loots){
                if (loot)
                    loot->SetPosition(loot->GetPosition() - glm::vec2(dx, 0.0f));
            }
            for (auto &item : m_EnemiesManager->rendererVec) {
                if (item)
                    item->m_Transform.translation = item->m_Transform.translation - glm::vec2(dx, 0.0f);
            }
        }
        if (m_Boss)
            m_Boss->SetPosition(m_Boss->GetPosition() - glm::vec2(dx, 0.0f));
        if (asLoot)
            asLoot->SetPosition(asLoot->GetPosition() - glm::vec2(dx, 0.0f));
    }
}

void Scene::UpdateSubWeapon(App* app) {
    asLoot = std::dynamic_pointer_cast<Loot>(m_Character->m_SubWeapon);
    if (m_Character->GetUseWeaponFlag() && !m_Character->GetLevelUpWhipFlag()) {
        if (!m_Character->m_SubWeapon) {
            SetSubweapon(app);
            int cost = m_Character->m_SubWeapon->GetCost();
            if (m_Character->GetAmmo() < cost) {
                m_Character->m_SubWeapon = nullptr;
                app->m_Root.RemoveChild(asLoot);
                m_Character->SetUseWeaponFlag(false);
                return;
            }
            app->m_Menu->modifyNumber(app->m_Menu->formatTwoDigits(m_Character->GetAmmo()-cost), 3);
            m_Character->SetAmmo(m_Character->GetAmmo()-cost);
        } else {
            m_Character->m_SubWeapon->Use(m_Blocks);
            if (m_Character->m_SubWeapon->IsDestroyed() ||
                asLoot->GetPosition().x < -screenWidth / 2 ||
                asLoot->GetPosition().x > screenWidth / 2) {
                    m_Character->m_SubWeapon = nullptr;
                    app->m_Root.RemoveChild(asLoot);
                    m_Character->SetUseWeaponFlag(false);
                }
        }
    }
}

void Scene::SetSubweapon(App* app) {
    glm::vec2 pos = m_Character->GetPosition() + glm::vec2(0, 10);
    WeaponType type = m_Character->GetSubWeaponType();
    switch (type) {
        case WeaponType::Axe:
            m_Character->m_SubWeapon = std::make_shared<Subweapon::Axe>(pos, "subweapon");
            break;
        case WeaponType::Dagger:
            m_Character->m_SubWeapon = std::make_shared<Subweapon::Dagger>(pos, "subweapon");
            break;
        case WeaponType::HolyWater:
            m_Character->m_SubWeapon = std::make_shared<Subweapon::HolyWater>(pos, "subweapon");
            break;
        case WeaponType::Stopwatch:
            m_Character->m_SubWeapon = std::make_shared<Subweapon::Stopwatch>(pos, "subweapon");
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

std::shared_ptr<EnemiesManager> Scene::GetEnemiesManager() {
    return m_EnemiesManager;
}

void Scene::Blink() {
    if (blinkStartTime == 0) {
        blinkStartTime = SDL_GetPerformanceCounter();
        m_Blink->SetVisible(true);
        m_Blink->SetLooping(true);
        m_Blink->SetPlaying();
    }
    if (Time::GetRunTimeMs(blinkStartTime) > 800.0f) {
        m_Blink->SetVisible(false);
        m_Blink->SetLooping(false);
        m_Blink->SetPaused();
        blinkStartTime = 0;
    }
}

void Scene::UpdateHitableBlock(App* app) {
    for (auto hitableBlock : m_HitableBlocks) {
        if (hitableBlock && hitableBlock->loot && !hitableBlock->loot->IfCollected() && !m_Character->GetLevelUpWhipFlag()) {
            hitableBlock->loot->Fall(m_Blocks);
            if (hitableBlock->loot->IsCollected(m_Character))
                app->m_Root.RemoveChild(hitableBlock);
        }
        else if (hitableBlock && hitableBlock->loot && hitableBlock->loot->IfCollected() && !hitableBlock->loot->IfEnded())
            hitableBlock->loot->Result(app, m_Character, app->m_Menu);
        if (hitableBlock && hitableBlock->loot && hitableBlock->loot->IfEnded()) {
            app->m_Root.RemoveChild(hitableBlock->loot);
        }
        if (hitableBlock && hitableBlock->CollideDetection(m_Character))
            hitableBlock->RemoveBlock(app, m_Blocks, m_All);
        hitableBlock->ScatterParticles(app);
    }
}