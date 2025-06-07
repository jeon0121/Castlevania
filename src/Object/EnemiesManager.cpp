#include "Object/EnemiesManager.hpp"
#include "Utility/Time.hpp"

EnemiesManager::EnemiesManager(const std::vector<PossibleLootData>& possibleLoots) : m_PossibleLoots(possibleLoots) {
    stopWatchSound = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/26.wav");
    stopWatchSound->SetVolume(60);
}

void EnemiesManager::RemoveAllChild(App *app) {
    for (auto &child : rendererVec)
        app->m_Root.RemoveChild(child);
    for (auto &loot : m_Loots)
        app->m_Root.RemoveChild(loot);
    m_Enemies.clear();
    m_Loots.clear();
    rendererVec.clear();
    m_Bats.clear();
    m_Fishmans.clear();
    fires.clear();
}

void EnemiesManager::AddAllChild(App *app) {
    for (auto &child : rendererVec)
        app->m_Root.AddChild(child);
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
        rendererVec.push_back(zombie);
    }
}

void EnemiesManager::AddLeopard(glm::vec2 positions, std::string direction, App *app) {
    std::shared_ptr<Leopard> leopard = std::make_shared<Leopard>(positions, direction);
    leopard->SetZIndex(7);
    m_Leopards.push_back(leopard);
    m_Enemies.push_back(leopard);
    rendererVec.push_back(leopard);
}

void EnemiesManager::AddBat(glm::vec2 positions, std::string direction, App *app) {
    std::shared_ptr<Bat> bat = std::make_shared<Bat>(positions, direction);
    bat->SetZIndex(7);
    m_Bats.push_back(bat);
    m_Enemies.push_back(bat);
    rendererVec.push_back(bat);
}

void EnemiesManager::AddFishman(glm::vec2 positions, std::string direction, App *app) {
    std::string bubbleImage = GA_RESOURCE_DIR"/enemies/fishman/bubble.png";
    std::vector<std::shared_ptr<ImageItems>> bubbles;
    for (int i = 0; i < 3; i++) {
        bubbles.push_back(std::make_shared<ImageItems>(bubbleImage, glm::vec2(0.8, 0.8)));
        bubbles[i]->SetVisible(false);
        bubbles[i]->SetZIndex(7.5);
        rendererVec.push_back(bubbles[i]);
    }

    std::shared_ptr<Fishman> fishman = std::make_shared<Fishman>(positions, direction, app, bubbles);
    std::shared_ptr<ImageItems> fire = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/enemies/fishman/fire.png", glm::vec2(0.8, 0.8));
    fire->SetVisible(false);
    fire->SetZIndex(6.5);
    fires.push_back(fire);
    isFire = {false, false};
    rendererVec.push_back(fire);


    fishman->SetZIndex(7);
    m_Fishmans.push_back(fishman);
    m_Enemies.push_back(fishman);
    rendererVec.push_back(fishman);
}

void EnemiesManager::Update(float offsetX, int screenWidth, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, App* app) {
    if (character->GetLevelUpWhipFlag() ||
       (character->GetSubWeaponType() == WeaponType::Stopwatch && character->GetUseWeaponFlag())) {
        if (!character->GetLevelUpWhipFlag() && !isEnemyPause) {
            stopWatchSound->SetVolume(60);
            stopWatchSound->Play(1, 5000);
            app->BGM->Pause();
        }
        PauseAllEnemy();
        isEnemyPause = true;
    }
    else if (isEnemyPause) {
        stopWatchSound->SetVolume(0);
        app->BGM->Resume();
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
        ManageBat(offsetX, character, screenWidth);
        ManageFishman(app, character, blocks, screenWidth);
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
            if (!zombie->CheckReset() && !zombie->IsDead()) {
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
        if (!leopard->CheckReset() && !leopard->IsDead()) {
            leopard->MoveBehav(character, blocks);
        }
        else if (initialPosX < -screenWidth || screenWidth < initialPosX) {
            leopard->SetReset();
            leopard->SetDirection((leopard->GetInitialPos().x - offsetX > 0) ? "left" : "right");
            leopard->SetPosition(leopard->GetInitialPos() - glm::vec2(offsetX, 0));
        }
    }
}

void EnemiesManager::ManageBat(float offsetX, std::shared_ptr<Character> &character, int screenWidth) {
    constexpr float delay = 2000.0f;
    for (auto &bat : m_Bats) {
        bool reset = true;
        if (!bat->CheckReset() && !bat->IsDead()) {
            bat->MoveBehav();
            reset = false;
        }
        if (reset && character->GetPosition().y >= -266) {
            if (resetStartTime == 0)
                resetStartTime = SDL_GetPerformanceCounter();
            if (Time::GetRunTimeMs(resetStartTime) > delay) {
                for (auto &bat : m_Bats) {
                    if (character->GetPosition().x > 10.0f)
                        bat->SetPosition({-screenWidth * 0.5, character->GetPosition().y + 10.0f});
                    else if (character->GetPosition().x < -10.0f)
                        bat->SetPosition({screenWidth * 0.5, character->GetPosition().y + 10.0f});
                    else {
                        int sign = (std::rand() % 2 == 0) ? 1 : -1;
                        float baseX = screenWidth * 0.5;
                        bat->SetPosition({sign * baseX, character->GetPosition().y + 10.0f});
                    }
                    bat->SetReset();
                    bat->SetDirection((bat->GetPosition().x > 0) ? "left" : "right");
                }
                resetStartTime = 0;
            }
        }
    }
}

void EnemiesManager::ManageFishman(App *app, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, int screenWidth) {
    constexpr float delay = 2000.0f;
    FireAttack(character, screenWidth, app->m_Menu);
    for (auto &fishman : m_Fishmans) {
        bool reset = true;
        if (!fishman->CheckReset() && !fishman->IsDead()) {
            fishman->MoveBehav(character, blocks);
            auto it = std::find(m_Fishmans.begin(), m_Fishmans.end(), fishman);
            auto index = std::distance(m_Fishmans.begin(), it);
            if (fishman->GetFireFlag()) {
                isFire[index] = true;
                if (fireDirections[index] != fishman->GetDirection()) {
                    glm::vec2 scale = fires[index]->m_Transform.scale;
                    fires[index]->m_Transform.scale = glm::vec2(-1 * scale.x, scale.y);
                    fireDirections[index] = fishman->GetDirection();
                }
                glm::vec2 pos = fishman->GetPosition();
                fires[index]->SetPosition({pos.x, pos.y + 25.0f});
                fires[index]->SetVisible(true);
            }
            reset = false;
        }
        if (reset) {
            if (resetStartTime == 0)
                resetStartTime = SDL_GetPerformanceCounter();
            if (Time::GetRunTimeMs(resetStartTime) > delay) {
                int rd = std::rand() % (460 - 150 + 1) + 150;
                float posX = character->GetPosition().x;
                fishman->SetPosition({(rd % 2 == 0 ? posX + rd : posX - rd), -250});
                if (fishman->GetPosition().x < -460.0f)
                    fishman->SetPosition({-460.0f, -250});
                else if (fishman->GetPosition().x > 245.0f)
                    fishman->SetPosition({245.0f, -250});

                if (character->GetPosition().x > fishman->GetPosition().x)
                    fishman->SetDirection("right");
                else
                    fishman->SetDirection("left");
                fishman->SetReset();
                resetStartTime = 0;
            }
        }
    }
}

void EnemiesManager::FireAttack(std::shared_ptr<Character> &character, int screenWidth, std::shared_ptr<Menu> &menu) {
    for (int i = 0; i < 2; i++) {
        if (isFire[i]) {
            glm::vec2 firePos = fires[i]->GetPosition();
            fires[i]->SetPosition({firePos.x + (fireDirections[i] == "right" ? 6.0f : -6.0f), firePos.y});
            if (fires[i]->GetPosition().x > screenWidth * 2.0f || fires[i]->GetPosition().x < screenWidth * -2.0f)
                isFire[i] = false;

            if (!character->GetLevelUpWhipFlag() && !character->GetHurtFlag()) {
                glm::vec2 charPos = character->GetPosition();
                glm::vec2 charSize = character->GetSize();
                float charLeft = charPos.x - charSize.x * 0.5f;
                float charRight = charPos.x + charSize.x * 0.5f;
                float charTop = charPos.y + charSize.y * 0.5f;
                float charBottom = charPos.y - charSize.y * 0.5f;
                glm::vec2 pos = fires[i]->GetPosition();
                glm::vec2 size = glm::abs(fires[i]->GetScaledSize());
                float fireLeft = pos.x - size.x * 0.5f;
                float fireRight = pos.x + size.x * 0.5f;
                bool overlapX = (charRight > fireLeft && charRight < fireRight) ||
                                (charLeft < fireRight && charLeft > fireLeft);
                bool overlapY = charTop > firePos.y && charBottom < firePos.y;
                if (overlapX && overlapY) {
                    bool ifNeedSlip = false;
                    if (fireDirections[i] == character->GetDirection() &&
                        ((fireDirections[i] == "right" && character->GetPosition().x > fires[i]->GetPosition().x) ||
                         (fireDirections[i] == "left" && character->GetPosition().x < fires[i]->GetPosition().x))) {
                        ifNeedSlip = true;
                    }
                    character->SetHurtFlag(true, ifNeedSlip);
                    character->SetHeart(character->GetHeart() - 2);
                    menu->modifyHealth(character->GetHeart(), "player");
                }
            }
        }
    }
}