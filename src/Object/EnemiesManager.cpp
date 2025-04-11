#include "Object/EnemiesManager.hpp"
#include "Utility/Time.hpp"

EnemiesManager::EnemiesManager(App *app) {
    std::shared_ptr<Zombie> zombie_1 = std::make_shared<Zombie>(glm::vec2(520, -265.35), "left");
    std::shared_ptr<Zombie> zombie_2 = std::make_shared<Zombie>(glm::vec2(640, -265.35), "left");
    std::shared_ptr<Zombie> zombie_3 = std::make_shared<Zombie>(glm::vec2(760, -265.35), "left");
    m_Zombies.push_back(zombie_1);
    m_Zombies.push_back(zombie_2);
    m_Zombies.push_back(zombie_3);
    for (auto &zombie : m_Zombies) {
        m_Enemies.push_back(zombie);
        app->m_Root.AddChild(zombie);
    }

}

void EnemiesManager::Update(int screenWidth, std::shared_ptr<Character> &character) {
    for (auto &enemy : m_Enemies) {
        enemy->InWindowDetection(screenWidth);
        if (enemy->CollideDetection(character)) {
            enemy->Death();
        }
    }
    ManageZombies();
}

void EnemiesManager::ManageZombies() {
    bool reset = true;
    constexpr float delay = 2000.0f;
    for (auto &zombie : m_Zombies) {
        zombie->MoveBehav();
        if (!zombie->CheckReset())
            reset = false;
    }
    if (reset) {
        if (resetStartTime == 0) {
            resetStartTime = SDL_GetPerformanceCounter();
        }
        if (Time::GetRunTimeMs(resetStartTime) > delay) {
            int r = std::rand() % 3;
            m_Zombies[0]->SetPosition({520, -265.35});
            if (r == 0) {
                m_Zombies[1]->SetPosition({640, -265.35});
                m_Zombies[2]->SetPosition({760, -265.35});
            }else if (r == 1) {
                m_Zombies[1]->SetPosition({640, -265.35});
                m_Zombies[2]->SetPosition({-520, -265.35});
            }else {
                m_Zombies[1]->SetPosition({-520, -265.35});
                m_Zombies[2]->SetPosition({-640, -265.35});
            }
            for (auto &zombie : m_Zombies) {
                zombie->SetReset();
                zombie->SetDirection((zombie->GetPosition().x > 0) ? "left" : "right");
            }
            resetStartTime = 0;
        }
    }
}
