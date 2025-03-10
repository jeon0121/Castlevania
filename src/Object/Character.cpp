#include "Object/Character.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"

Character::Character(const glm::vec2 &position, int beIndex, float scale){
    //Image string
    std::vector<std::string> walk, death, duck, hurt, jump, idle, intro;

    //ImageBehavior
    idle.emplace_back(GA_RESOURCE_DIR"/main character/idle/idle.png");
    duck.emplace_back(GA_RESOURCE_DIR"/main character/duck/duck.png");
    hurt.emplace_back(GA_RESOURCE_DIR"/main character/hurt/hurt.png");
    jump.emplace_back(GA_RESOURCE_DIR"/main character/jump/jump.png");
    intro.emplace_back(GA_RESOURCE_DIR"/main character/intro/intro.png");

    //AnimatedBehavior
    for (int i = 0; i < 4; ++i) {
        walk.emplace_back(GA_RESOURCE_DIR"/main character/walk/walk-" + std::to_string(i + 1) + ".png");
    }
    for (int i = 0; i < 2; ++i) {
        death.emplace_back(GA_RESOURCE_DIR"/main character/death/death-" + std::to_string(i + 1) + ".png");
    }
    behaviorVector = {walk, death, idle, duck, hurt, jump, intro};

    m_Behavior = std::make_shared<AnimatedItems>(behaviorVector[beIndex], 100, glm::vec2(scale, scale));
    m_Behavior->SetPosition(position);
    m_Behavior->SetZIndex(7);

}

void Character::ChangeBehavior(int BehaviorIndex) {
    m_Behavior->SetAnimationFrames(behaviorVector[BehaviorIndex], 100);
}

void Character::SetPosition(const glm::vec2& Position) {
    m_Behavior->SetPosition(Position);
}

const glm::vec2& Character::GetPosition() const {
    return m_Behavior->GetPosition();
}

void Character::Keys() {
    // change behavior must called once, otherwise character will not animate since we change the m_Drawable
    if (Util::Input::IsKeyDown(Util::Keycode::A) || Util::Input::IsKeyDown(Util::Keycode::D)) ChangeBehavior(0);

    // left
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) Move("left");

    // right
    else if (Util::Input::IsKeyPressed(Util::Keycode::D)) Move("right");

    // idle
    else {
        ChangeBehavior(2); // since idle dont have animation, its okay to be called more than once
        m_Behavior->SetPaused();
        m_Behavior->SetLooping(false);
    }
}

void Character::Move(std::string direction){
    glm::vec2 pos = GetPosition();
    if (direction == "left") pos.x -= 3;
    if (direction == "right") pos.x += 3;
    if (direction != m_direction){
        m_direction = direction;
        Flip();
    }
    SetPosition(pos);
    m_Behavior->SetPlaying();
    m_Behavior->SetLooping(true);
}

void Character::Flip() {
    glm::vec2 scale = m_Behavior->m_Transform.scale;
    m_Behavior->m_Transform.scale=glm::vec2(-1 * scale.x, scale.y);
}

void Character::CollideBoundary(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
    glm::vec2 charPos = GetPosition();
    glm::vec2 charSize = glm::abs(m_Behavior->GetScaledSize());
    for (auto &block : m_Blocks) {
        int blockType = block->GetType();
        glm::vec2 blockPos = block->GetPosition();
        glm::vec2 blockSize = glm::abs(block->GetScaledSize());
        if (blockType == 0) {
            float blockTop = blockPos.y + blockSize.y * 0.5f - 12;
            if (charPos.y - charSize.y * 0.5f < blockTop) {
                m_Behavior->SetPosition({charPos.x, blockTop + charSize.y * 0.5f});
            }
        }else if (blockType == 1) {
            float blockLeft = blockPos.x - blockSize.x * 0.5f;
            if (charPos.x - charSize.x * 0.5f < blockLeft) {
                this->SetPosition({blockLeft + charSize.x * 0.5f, charPos.y});
            }
        }
    }
}