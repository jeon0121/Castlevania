#include "Object/Boss/PhantomBat.hpp"
#include "Utility/Time.hpp"

PhantomBat::PhantomBat(const glm::vec2& position) : Enemy(position, "", {}, 300, 200) {
    idleImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/boss-1.png");
    flyImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/boss-2.png");
    flyImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/boss-3.png");
    for (int i = 0; i < 6; i++)
        deathImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/death-" + std::to_string(i + 1) + ".png");
    SetAnimationFrames(idleImages, 0);
    auto hurtImages = std::vector<std::string>{GA_RESOURCE_DIR"/enemies/boss/hurt.png", GA_RESOURCE_DIR"/enemies/boss/hurt.png"};
    hurtEffect = std::make_shared<AnimatedItems>(hurtImages, 700, glm::vec2(1.0f, 1.0f));
    hurtEffect->SetZIndex(10);
    hurtEffect->SetPaused();
    hurtEffect->SetLooping(false);
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    m_state = "idle";
}

void PhantomBat::MoveBehav(std::shared_ptr<Character> &character, int screenHeight, int screenWidth) {
    static int testCount = 0;
    testCount++;
    if (idleTime == 0 && m_state == "idle")
        idleTime = SDL_GetPerformanceCounter();
    if (Time::GetRunTimeMs(idleTime) > 1000.0f && m_state == "idle") {
        m_state = "fly";
        pos = GetPosition();
        SetAnimationFrames(flyImages, 100);
        idleTime = 0;
        flyTime = SDL_GetPerformanceCounter();
        flyPos.x = character->GetPosition().x + (rand() % 100 - 50);
        flyPos.y = rand() % 30;
        glm::vec2 distance = flyPos - GetPosition();
        vel.x = (pos.x < flyPos.x) ? 2.0f : -2.0f;
        vel.y = std::abs(distance.y) / (std::abs(distance.x) / std::abs(vel.x));
        vel.y *= (pos.y < flyPos.y) ? 1.0f : -1.0f;
    }
    SetLooping(true);
    SetPlaying();
    if (m_state == "fly")
        Fly(character, screenHeight, screenWidth);
    else if (m_state == "setdive")
        SetDivePosition(character);
    else if (m_state == "dive")
        Dive(screenWidth);
}

void PhantomBat::Fly(std::shared_ptr<Character> &character, int screenHeight, int screenWidth) {
    glm::vec2 charPos = character->GetPosition();
    if (flyTime == 0) {
        flyTime = SDL_GetPerformanceCounter();
        glm::vec2 targetPos1 = {charPos.x + 300 + (rand() % 100 - 50), charPos.y + 300 + (rand() % 100 - 50)};
        glm::vec2 targetPos2 = {charPos.x - 300 + (rand() % 100 - 50), charPos.y + 300 + (rand() % 100 - 50)};
        while (targetPos1.y > screenHeight * 0.2f || targetPos2.y > screenHeight * 0.2f) {
            targetPos1.y -= rand() % 20;
            targetPos2.y -= rand() % 20;
        }
        if (screenWidth * 0.5f < targetPos1.x)
            flyPos = targetPos2;
        else if (targetPos2.x < -screenWidth * 0.5f)
            flyPos = targetPos1;
        else
            flyPos = (glm::length(targetPos1 - GetPosition()) < glm::length(targetPos2 - GetPosition())) ? targetPos1 : targetPos2;
        glm::vec2 distance = flyPos - GetPosition();
        vel.x = (pos.x < flyPos.x) ? 2.0f : -2.0f;
        vel.y = std::abs(distance.y) / (std::abs(distance.x) / std::abs(vel.x));
        vel.y *= (pos.y < flyPos.y) ? 1.0f : -1.0f;
    }
    pos.x += vel.x;
    pos.y += vel.y;
    SetPosition(pos);
    if (std::abs(pos.x - flyPos.x) < 4.0f) {
        vel = {0.0f, 0.0f};
        if (idleTime == 0) 
            idleTime = SDL_GetPerformanceCounter();
        if (Time::GetRunTimeMs(idleTime) > 1000.0f) {
            idleTime = 0;
            flyTime = 0;
            if (pos.y < charPos.y + 300 && idleTime == 0) {
                m_state = "setdive";
                idleTime = 0;
            }
        }
        
    }
}

void PhantomBat::Dive(int screenWidth) {
    if ((pos.x < -screenWidth * 0.5f + 50.0f || pos.x > screenWidth * 0.5f - 50.0f) && !hitWall) {
        hitWall = true;
        vel.x *= -1.0f;
    } else
        hitWall = false;
    static int diveTime;
    diveTime += 15;
    vel.y = -yDistance * sin(diveTime / tDive * M_PI);
    pos.x += vel.x;
    SetPosition({pos.x, pos.y + vel.y});
    if ((diveTime > tDive) || (pos.y + vel.y > pos.y)) {
        m_state = "fly";
        diveTime = 0;
        flyTime = 0;
        idleTime = 0;
        pos = GetPosition();
        vel = {0.0f, 0.0f};
    }
}

void PhantomBat::SetDivePosition(std::shared_ptr<Character> &character) {
    pos = GetPosition();
    tDive = 1.5 + (rand() % 100) / 100.0;
    tDive *= 1000.0f;
    divePos = {character->GetPosition().x, 290.0f};
    glm::vec2 distance = divePos - pos;
    yDistance = std::abs(distance.y);
    vel.x = (distance.x * 2) / (tDive / 20.0f); 
    vel.y = 0;
    hitWall = false;
    m_state = "dive";
}

bool PhantomBat::CollideDetection(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu, int modeState) {
    int frameIndex = character->m_Behavior->GetCurrentFrameIndex();
    int whipLevel = character->GetWhipLevel();
    UpdatePosition();
    glm::vec2 charPos = character->GetPosition();
    glm::vec2 charSize = character->GetSize();
    float charLeft = charPos.x - charSize.x * 0.5f;
    float charRight = charPos.x + charSize.x * 0.5f;
    float charTop = charPos.y + charSize.y * 0.5f;
    float charBottom = charPos.y - charSize.y * 0.5f;
    static bool is_subweaponHurt = false;
    static bool is_whipHurt = false;
    if (!is_dead) {
        if (!character->IfWhip()) 
            is_whipHurt = false;
        if (!character->m_SubWeapon || !character->GetUseWeaponFlag())
            is_subweaponHurt = false;
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

                if (overlapX && overlapY && !is_whipHurt) {
                    SetPaused();
                    is_whipHurt = true;
                    is_hurt = true;
                    int hurtCount = (whipLevel == 3) ? 2 : 1;
                    health-=hurtCount;
                    menu->modifyHealth(health, "boss");
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
            if (overlapX && overlapY && character->GetSubWeaponType() != WeaponType::Stopwatch && !is_subweaponHurt) {
                SetPaused();
                is_subweaponHurt = true;
                is_hurt = true;
                health-=2;
                menu->modifyHealth(health, "boss");
            }
        } else {
            if (!character->GetLevelUpWhipFlag() && !character->GetHurtFlag() && !is_dead && !is_hidden) {
                bool overlapX = (charLeft > enemyLeft && charLeft < enemyRight)   ||
                                (charRight > enemyLeft && charRight < enemyRight) ||
                                (charLeft < enemyLeft && charRight > enemyRight);
                bool overlapY = (enemyTop > charPos.y && enemyBottom < charPos.y) ||
                                (charTop > enemyBottom && charBottom < enemyTop);
                if (overlapX && overlapY) {
                    bool ifNeedSlip = false;
                    if (direction == character->GetDirection() &&
                        ((direction == "right" && character->GetPosition().x > GetPosition().x) ||
                        (direction == "left" && character->GetPosition().x < GetPosition().x))) {
                        ifNeedSlip = true;
                    }
                    if (modeState == 0) {
                        character->SetHurtFlag(true, ifNeedSlip);
                        character->SetHeart(character->GetHeart() - 2);
                        menu->modifyHealth(character->GetHeart(), "player");
                    }
                }
            }
        }
    } else if (health <= 0) {
        SetAnimationFrames(deathImages, 100);
        SetLooping(true);
        SetPlaying();
        vel = {0.0f, 0.0f};
        if (IfAnimationEnds()) 
            is_dead = true;
        return true;
    }
    
    return is_subweaponHurt || is_whipHurt;
}

void PhantomBat::Hurt(App* app) {
    if (hurtEffect->IfAnimationStart() && is_hurt) {
        app->m_Root.AddChild(hurtEffect);
        hurtEffect->SetPosition(GetPosition());
        hurtEffect->SetPlaying();
    } else if (hurtEffect->IfAnimationEnds()) {
        app->m_Root.RemoveChild(hurtEffect);
        hurtEffect->SetPaused();
        is_hurt = false;
    }
}