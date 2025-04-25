#include "Object/EnemiesManager.hpp"
#include "Utility/Time.hpp"

EnemiesManager::EnemiesManager(const std::vector<PossibleLootData>& possibleLoots) : m_PossibleLoots(possibleLoots) {}

void EnemiesManager::RemoveAllEnemies(App *app) {
    for (auto &enemy : m_Enemies)
        app->m_Root.RemoveChild(enemy);
}

void EnemiesManager::AddZombie(glm::vec2 range, int numZombie, glm::vec2 pos, std::string direction, App *app) {
    std::shared_ptr<ZombieHorde> zombieHorde = std::make_shared<ZombieHorde>(range, numZombie, pos, direction);
    m_Zombies.push_back(zombieHorde);
    if (zombieHorde->GetSpawnRange().x > app->m_Character->GetPosition().x || app->m_Character->GetPosition().x > zombieHorde->GetSpawnRange().y) {
        for (auto &zombie : zombieHorde->zombies) 
            zombie->SetHidden(true);
    }
    for (auto &zombie : zombieHorde->zombies) {
        zombie->SetZIndex(7);
        m_Enemies.push_back(zombie);
        app->m_Root.AddChild(zombie);
    }
}

void EnemiesManager::AddLeopard(glm::vec2 positions, std::string direction, App *app) {
    std::shared_ptr<Leopard> leopard = std::make_shared<Leopard>(positions, direction);
    leopard->SetZIndex(7);
    m_Leopards.push_back(leopard);
    m_Enemies.push_back(leopard);
    app->m_Root.AddChild(leopard);
}

void EnemiesManager::Update(float offsetX, int screenWidth, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, App* app) {
    if (character->GetLevelUpWhipFlag() ||
       (character->GetSubWeaponType() == WeaponType::Stopwatch && character->GetUseWeaponFlag())) {
        PauseAllEnemy();
        isEnemyPause = true;
    }
    else if (isEnemyPause) {
        PlayAllEnemy();
        isEnemyPause = false;
    }
    for (auto &enemy : m_Enemies) {
        enemy->InWindowDetection(screenWidth);
        if (enemy->CollideDetection(character, app->m_Menu)) {
            enemy->Death(app, m_Loots, m_PossibleLoots);
        }
    }
    for (auto &&loot : m_Loots) {
        if (loot && !loot->IfCollected() && !character->GetLevelUpWhipFlag()) {
            loot->Fall(blocks);
            loot->IsCollected(character);
        }
        else if (loot && loot->IfCollected() && !loot->IfEnded()) 
            loot->Result(app, character, app->m_Menu);
        if (loot && loot->IfEnded()) {
            app->m_Root.RemoveChild(loot);
            m_Loots.erase(std::remove(m_Loots.begin(), m_Loots.end(), loot), m_Loots.end());
        }
    }
    if (!isEnemyPause) {
        ManageZombies(offsetX, character, screenWidth);
        ManageLeopard(offsetX, character, blocks, screenWidth);
    }
}

void EnemiesManager::PauseAllEnemy() {
    for (auto &&enemy : m_Enemies) 
        if (!enemy->IsDead())
            enemy->SetPaused();
}

void EnemiesManager::PlayAllEnemy() {
    for (auto &&enemy : m_Enemies) 
        enemy->SetPlaying();
}

void EnemiesManager::ManageZombies(float offsetX, std::shared_ptr<Character> &character, int screenWidth) {
    constexpr float delay = 2000.0f;
    for (auto &zombieHorde : m_Zombies) {
        bool reset = true;
        for (auto &zombie : zombieHorde->zombies) {
            if (!zombie->CheckReset()) {
                zombie->MoveBehav();
                reset = false;
            }
        }
        if (reset && (zombieHorde->GetSpawnRange().x < offsetX && offsetX < zombieHorde->GetSpawnRange().y)) {
            if (resetStartTime == 0)
                resetStartTime = SDL_GetPerformanceCounter();
            if (Time::GetRunTimeMs(resetStartTime) > delay) {
                if (character->GetPosition().x > 10.0f) {
                    for (int i = 0; i < zombieHorde->GetNumZombie(); i++) {
                        float spawnX = (std::rand() % 20 + 120) * i;
                        zombieHorde->zombies[i]->SetPosition({-screenWidth * 0.5 + spawnX, -265.35f});
                    }
                } else if (character->GetPosition().x < -10.0f) {
                    for (int i = 0; i < zombieHorde->GetNumZombie(); i++) {
                        float spawnX = (std::rand() % 20 + 120) * i;
                        zombieHorde->zombies[i]->SetPosition({screenWidth * 0.5 + spawnX, -265.35f});
                    }
                } else {
                    for (int i = 0; i < zombieHorde->GetNumZombie(); i++) {
                        int sign = (std::rand() % 2 == 0) ? 1 : -1;
                        float spawnX = (std::rand() % 20 + 120) * i;
                        float baseX = screenWidth * 0.5 + spawnX;
                        zombieHorde->zombies[i]->SetPosition({sign * baseX, -265.35f});
                    }
                }
                for (auto &zombie : zombieHorde->zombies) {
                    zombie->SetReset();
                    zombie->SetDirection((zombie->GetPosition().x > 0) ? "left" : "right");
                }
                resetStartTime = 0;
            }
        }
    }
}

void EnemiesManager::ManageLeopard(float offsetX, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, int screenWidth) {
    for (auto &leopard : m_Leopards) {
        int initialPosX = leopard->GetInitialPos().x - offsetX;
        if (!leopard->CheckReset()) {
            leopard->MoveBehav(character, blocks);
        }
        else if (initialPosX < -screenWidth || screenWidth < initialPosX) {
            leopard->SetReset();
            leopard->SetDirection((leopard->GetInitialPos().x - offsetX > 0) ? "left" : "right");
            leopard->SetPosition(leopard->GetInitialPos() - glm::vec2(offsetX, 0));
        }
    }
}