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
            torch->Destroy(app, m_Character, whipDropped, m_Loots);
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
        if (specialBag)
            specialBag->score->SetPosition(specialBag->score->GetPosition() - glm::vec2(dx, 0.0f));
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
        m_All.push_back(asLoot);
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

void Scene::EndAnimation(App* app, std::string imagePath, glm::vec2 scale, std::shared_ptr<AnimatedItems> door_1, std::shared_ptr<AnimatedItems> door_2, std::shared_ptr<Util::SFX> doorSound) {
    if (m_Background->m_Transform.scale.x > 1.0f) {
        m_Character->ChangeBehavior(2);
        m_Background->SetDrawable(std::make_unique<Util::Image>(imagePath));
        m_Background->m_Transform.scale = scale;
        m_Background->SetPosition({522.5, -36.2});
        m_Background->SetZIndex(8);
    }else if (m_Background->GetPosition().x > 0) {
        m_Background->SetPosition({m_Background->GetPosition().x - 4.0f, m_Background->GetPosition().y});
        m_Character->SetPosition({m_Character->GetPosition().x - 4.0f, m_Character->GetPosition().y });
    }else if (m_Background->GetPosition().x < 0 && !door_2->IsLooping()) {
        if (!door_1->IsPlaying() && !m_Character->m_Behavior->IsLooping()) {
            door_1->SetVisible(true);
            door_1->SetPlaying();
        }
        if (door_1->IfAnimationEnds() && !m_Character->m_Behavior->IsLooping()) {
            m_Character->ChangeBehavior(0);
            m_Character->m_Behavior->SetPlaying();
            m_Character->m_Behavior->SetLooping(true);
            doorSound->Play();
        }
        if (m_Character->GetPosition().x < 200 && m_Character->m_Behavior->IsLooping())
            m_Character->SetPosition({m_Character->GetPosition().x + 4.0f, m_Character->GetPosition().y });
        else if (m_Character->GetPosition().x > 200) {
            m_Character->ChangeBehavior(2);
            door_1->SetVisible(false);
            door_1->SetPaused();
            if (!door_2->IsPlaying()) {
                door_2->SetVisible(true);
                door_2->SetPlaying();
                doorSound->Play();
            }
            if (door_2->IfAnimationEnds()) {
                door_2->SetLooping(true);
            }
        }
    } else {
        if (door_1->IsPlaying() && m_Background->GetPosition().x > -516) {
            m_Background->SetPosition({m_Background->GetPosition().x - 4.0f, m_Background->GetPosition().y});
            m_Character->SetPosition({m_Character->GetPosition().x - 4.0f, m_Character->GetPosition().y });
        } else if (!door_1->IsPlaying() && !door_2->IsPlaying()) {
            door_2->SetVisible(false);
            door_1->SetPlaying();
        } else if (m_Background->GetPosition().x < -516) {
            app->m_Menu->SetMenuVisibility(false);
            m_EnemiesManager->RemoveAllChild(app);
            app->RemoveAllChildren(m_All, m_Loots);
            app->m_AppState = App::AppState::START;
        }
    }
}

void Scene::SceneReset(App* app) {
    app->m_Menu->SetMenuVisibility(false);
    if (app->m_Menu->m_value.playerLife == 0 && app->GetModeState() == 0)
        app->m_GameState = App::GameState::GG;
    else if (app->GetModeState() == 0){
        app->m_Menu->modifyNumber(app->m_Menu->formatTwoDigits(5), 3);
        app->m_Menu->modifyNumber(app->m_Menu->formatTwoDigits(--(app->m_Menu->m_value.playerLife)), 4);
        app->m_Menu->SetTime((app->m_Menu->GetTime() / 100 + 1) * 100);
        app->m_Menu->modifyNumber(app->m_Menu->formatTime(app->m_Menu->GetTime()), 1);
    }
    app->m_Menu->modifyWeapon(WeaponType::None);
    app->RemoveAllChildren(m_All, m_Loots);
    app->m_AppState = App::AppState::START;
}

bool Scene::GetSwitchStageFlag() const {
    return switchStage;
}

void Scene::SetSwitchStageFlag(bool ifSwitch) {
    switchStage = ifSwitch;
}