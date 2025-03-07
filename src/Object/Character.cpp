#include "Object/Character.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"

Character::Character(const glm::vec2 &position) : pos(position){
}

void Character::LoadBehavior(int imIndex, int beIndex) {
    //Image string
    std::string duck, hurt, jump, idle, intro;
    std::vector<std::string> imageVector;

    //Animated string
    std::vector<std::string> walkImages, deathImages;
    std::vector<std::vector<std::string>> behaviorVector;

    //ImageBehavior
    idle = GA_RESOURCE_DIR"/main character/idle/idle.png";
    duck = GA_RESOURCE_DIR"/main character/duck/duck.png";
    hurt = GA_RESOURCE_DIR"/main character/hurt/hurt.png";
    jump = GA_RESOURCE_DIR"/main character/jump/jump.png";
    intro = GA_RESOURCE_DIR"/main character/intro/intro.png";
    imageVector = {idle, duck, hurt, jump, intro};

    m_Image = std::make_shared<ImageItems>(imageVector[imIndex], glm::vec2(0.8, 0.8));
    m_Image->SetVisible(false);
    m_Image->SetZIndex(7);

    //AnimatedBehavior
    for (int i = 0; i < 4; ++i) {
        walkImages.emplace_back(GA_RESOURCE_DIR"/main character/walk/walk-" + std::to_string(i + 1) + ".png");
    }
    for (int i = 0; i < 2; ++i) {
        deathImages.emplace_back(GA_RESOURCE_DIR"/main character/death/death-" + std::to_string(i + 1) + ".png");
    }
    behaviorVector = {walkImages, deathImages};

    m_Behavior = std::make_shared<AnimatedItems>(behaviorVector[beIndex], 100, glm::vec2(0.8, 0.8));
    m_Behavior->SetVisible(false);
    m_Behavior->SetPosition(pos);
    m_Behavior->SetZIndex(7);

}

void Character::UpdatePosition() {
    m_Behavior->SetPosition(pos);
    m_Image->SetPosition(pos);
}

void Character::SetPosition(const glm::vec2& Position) {
    pos = Position;
}

const glm::vec2& Character::GetPosition() const { return pos; }