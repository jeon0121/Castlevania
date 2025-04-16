#include "Object/EnemiesManager.hpp"
#include "Utility/Time.hpp"

EnemiesManager::EnemiesManager(App *app) {
    std::shared_ptr<Zombie> zombie_1 = std::make_shared<Zombie>(glm::vec2(520, -265.35), "left");
    std::shared_ptr<Zombie> zombie_2 = std::make_shared<Zombie>(glm::vec2(640, -265.35), "left");
    std::shared_ptr<Zombie> zombie_3 = std::make_shared<Zombie>(glm::vec2(760, -265.35), "left");
    std::shared_ptr<Leopard> leopard_1 = std::make_shared<Leopard>(glm::vec2(2355, -69), "left");
    std::shared_ptr<Leopard> leopard_2 = std::make_shared<Leopard>(glm::vec2(3150, 50), "left");
    std::shared_ptr<Leopard> leopard_3 = std::make_shared<Leopard>(glm::vec2(3450, -69), "left");
    m_Zombies.push_back(zombie_1);
    m_Zombies.push_back(zombie_2);
    m_Zombies.push_back(zombie_3);
    m_Leopards.push_back(leopard_1);
    m_Leopards.push_back(leopard_2);
    m_Leopards.push_back(leopard_3);
    for (auto &zombie : m_Zombies) {
        m_Enemies.push_back(zombie);
        app->m_Root.AddChild(zombie);
    }
    for (auto &leopard : m_Leopards) {
        m_Enemies.push_back(leopard);
        app->m_Root.AddChild(leopard);
    }
    for (auto &enemy : m_Enemies) 
        enemy->SetZIndex(7);
}

void EnemiesManager::Update(float offsetX, int screenWidth, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, std::shared_ptr<Menu> &menu) {
    for (auto &enemy : m_Enemies) {
        enemy->InWindowDetection(screenWidth);
        if (enemy->CollideDetection(character, menu)) {
            enemy->Death();
        }
    }
    ManageZombies(offsetX, character);
    ManageLeopard(offsetX, character, blocks, screenWidth);
}

void EnemiesManager::ManageZombies(float offsetX, std::shared_ptr<Character> &character) {
    bool reset = true;
    constexpr float delay = 2000.0f;
    for (auto &zombie : m_Zombies) {
        if (!zombie->CheckReset()) {
            zombie->MoveBehav();
            reset = false;
        }
    }
    if (reset && (offsetX < 1215 || offsetX > 4030)) {
        if (resetStartTime == 0) {
            resetStartTime = SDL_GetPerformanceCounter();
        }
        if (Time::GetRunTimeMs(resetStartTime) > delay) {
            if (character->GetPosition().x > 0) {
                m_Zombies[0]->SetPosition({-520, -265.35});
                m_Zombies[1]->SetPosition({-640, -265.35});
                m_Zombies[2]->SetPosition({-760, -265.35});
            }
            else if (character->GetPosition().x < 0) {
                m_Zombies[0]->SetPosition({520, -265.35});
                m_Zombies[1]->SetPosition({640, -265.35});
                m_Zombies[2]->SetPosition({760, -265.35});
            }
            else {
                int r = std::rand() % 3;
                m_Zombies[0]->SetPosition({520, -265.35});
                if (r == 0) {
                    m_Zombies[1]->SetPosition({640, -265.35});
                    m_Zombies[2]->SetPosition({760, -265.35});
                } else if (r == 1) {
                    m_Zombies[1]->SetPosition({640, -265.35});
                    m_Zombies[2]->SetPosition({-520, -265.35});
                } else {
                    m_Zombies[1]->SetPosition({-520, -265.35});
                    m_Zombies[2]->SetPosition({-640, -265.35});
                }
            }
            for (auto &zombie : m_Zombies) {
                zombie->SetReset();
                zombie->SetDirection((zombie->GetPosition().x > 0) ? "left" : "right");
            }
            resetStartTime = 0;
        }
    }
}

void EnemiesManager::ManageLeopard(float offsetX, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, int screenWidth) {
    for (auto &leopard : m_Leopards) {
        if (!leopard->CheckReset()) {
            leopard->MoveBehav(character, blocks);
        }
        // else if (0 < leopard->GetInitialPos().x - offsetX && leopard->GetInitialPos().x - offsetX < screenWidth) {
        //     leopard->SetReset();
        //     leopard->SetDirection((leopard->GetPosition().x > 0) ? "left" : "right");
        // }
    }
}