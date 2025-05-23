#include "Object/Boss/PhantomBat.hpp"
#include "Utility/Time.hpp"

PhantomBat::PhantomBat(const glm::vec2& position) : Enemy(position, "", {}, 300, 200) {
    idleImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/boss-1.png");
    flyImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/boss-2.png");
    flyImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/boss-3.png");
    deathImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/death-1.png");
    deathImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/death-2.png");
    deathImages.emplace_back(GA_RESOURCE_DIR"/enemies/boss/death-3.png");
    SetAnimationFrames(idleImages, 0);
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    m_state = "idle";
}

void PhantomBat::MoveBehav(std::shared_ptr<Character> &character) {
    if (idleTime == 0 && m_state == "idle")
        idleTime = SDL_GetPerformanceCounter();
    if (Time::GetRunTimeMs(idleTime) > 2000.0f && m_state == "idle") {
        m_state = "fly";
        SetAnimationFrames(flyImages, 100);
    }
    SetLooping(true);
    SetPlaying();
    if (m_state == "fly") {
        Fly(character);
    }
    else if (m_state == "dive") {
        Dive(character);
    }
    else if (m_state == "setdive") {
        SetDivePosition(character);
    }
}

void PhantomBat::Fly(std::shared_ptr<Character> &character) {

}

void PhantomBat::Dive(std::shared_ptr<Character> &character) {

}

void PhantomBat::SetDivePosition(std::shared_ptr<Character> &character) {

}