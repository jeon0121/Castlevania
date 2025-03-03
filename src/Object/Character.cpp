#include "Object/Character.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"

Character::Character(const glm::vec2 &position) : pos(position){
}

void Character::LoadBehavior() {
    std::vector<std::string> walkImages, deathImages;
    std::string duck, hurt, jump, idle, intro;

    //ImageBehavior
    idle = GA_RESOURCE_DIR"/main character/idle/idle.png";
    duck = GA_RESOURCE_DIR"/main character/duck/duck.png";
    hurt = GA_RESOURCE_DIR"/main character/hurt/hurt.png";
    jump = GA_RESOURCE_DIR"/main character/jump/jump.png";
    intro = GA_RESOURCE_DIR"/main character/intro/intro.png";

    m_Image = std::make_shared<ImageItems>(intro);
    m_Image->SetVisible(false);
    m_Image->SetZIndex(7);

    //AnimatedBehavior
    for (int i = 0; i < 4; ++i) {
        walkImages.emplace_back(GA_RESOURCE_DIR"/main character/walk/walk-" + std::to_string(i + 1) + ".png");
    }
    m_Behavior = std::make_shared<AnimatedItems>(walkImages, 100);
    m_Behavior->SetVisible(false);
    m_Behavior->SetPosition(pos);
    m_Behavior->SetZIndex(7);

    for (int i = 0; i < 2; ++i) {
        deathImages.emplace_back(GA_RESOURCE_DIR"/main character/death/death-" + std::to_string(i + 1) + ".png");
    }
}

void Character::UpdatePosition() {
    m_Behavior->SetPosition(pos);
    m_Image->SetPosition(pos);
}

void Character::SetPosition(const glm::vec2& Position) {
    pos = Position;
}

const glm::vec2& Character::GetPosition() const { return pos; }