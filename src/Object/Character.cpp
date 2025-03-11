#include "Object/Character.hpp"
#include "Utility/Position.hpp"

Character::Character(const CharacterValue& value) :
    m_direction(value.direction), currentBeIndex(value.beIndex) {
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

    for (int i = 0; i < 3; i++) {
        std::vector<std::string> ascending, descending, ducking, standing;
        std::string basePath = GA_RESOURCE_DIR "/main character/whip/level-" + std::to_string(i + 1) + "/";
        for (int j = 0; j < ((i == 3) ? 20 : 5); ++j) {
            // ascending.emplace_back(basePath + "ascending/ascending-" + std::to_string(j + 1) + ".png");
            // descending.emplace_back(basePath + "descending/descending-" + std::to_string(j + 1) + ".png");
            // ducking.emplace_back(basePath + "ducking/ducking-" + std::to_string(j + 1) + ".png");
            standing.emplace_back(basePath + "standing/standing-" + std::to_string(j + 1) + ".png");
        }
        std::vector<std::vector<std::string>> wVector = {ascending, descending, ducking, standing};
        whipVector.push_back(wVector);
    }
    
    m_Behavior = std::make_shared<AnimatedItems>(behaviorVector[value.beIndex], 100, glm::vec2(value.scale, value.scale));
    m_Behavior->SetPosition(value.position);
    m_Behavior->SetZIndex(7);

    if (value.direction == "right") Flip();
}

void Character::ChangeBehavior(int BehaviorIndex, bool if_whip) {
    if (if_whip){
        if ((currentBeIndex != BehaviorIndex) || (currentBeIndex == BehaviorIndex && lastVecPos == "be")) {
            m_Behavior->SetAnimationFrames(whipVector[m_whip_level][BehaviorIndex], m_whip_level == 3 ? 25 : 100);
            currentBeIndex = BehaviorIndex;
            lastVecPos = "whip";
        }
    }
    else {
        if (currentBeIndex != BehaviorIndex || (currentBeIndex == BehaviorIndex && lastVecPos == "whip")) {
            m_Behavior->SetAnimationFrames(behaviorVector[BehaviorIndex], 100);
            currentBeIndex = BehaviorIndex;
            lastVecPos = "be";
        }
    }
}

void Character::SetPosition(const glm::vec2& Position) {
    m_Behavior->SetPosition(Position);
}

const glm::vec2& Character::GetPosition() const {
    return m_Behavior->GetPosition();
}

void Character::Keys() {
    Position::PrintCursorCoordinate();

    constexpr Util::Keycode A      = Util::Keycode::J;
    constexpr Util::Keycode B      = Util::Keycode::K;
    constexpr Util::Keycode UP     = Util::Keycode::W;
    constexpr Util::Keycode DOWN   = Util::Keycode::S;
    constexpr Util::Keycode LEFT   = Util::Keycode::A;
    constexpr Util::Keycode RIGHT  = Util::Keycode::D;
    constexpr Util::Keycode START  = Util::Keycode::RETURN;
    constexpr Util::Keycode SELECT = Util::Keycode::RSHIFT;

    m_Behavior->SetPlaying();
    m_Behavior->SetLooping(true);

    /* priority order :
     * - whip
     * - duck
     * - jump
     * - left and right
     * - idle
    */
    
    // whip
    if (Util::Input::IsKeyDown(B) || is_whip) Whip();
    // duck
    else if (Util::Input::IsKeyPressed(DOWN) && !is_jump) Duck();
    // left jump
    else if (Util::Input::IsKeyPressed(LEFT) && Util::Input::IsKeyDown(UP) && !is_jump) {
        Jump();
        jumptype = 1;
    }
    // right jump
    else if (Util::Input::IsKeyPressed(RIGHT) && Util::Input::IsKeyDown(UP) && !is_jump) {
        Jump();
        jumptype = 2;
    }
    // normal jump
    else if (Util::Input::IsKeyDown(UP) && !is_jump) {
        Jump();
        jumptype = 0;
    }
    // fall
    else if (is_jump) Fall();
    // when pressing both key, character will idle
    else if (Util::Input::IsKeyPressed(LEFT) && Util::Input::IsKeyPressed(RIGHT)) Idle();
    // left
    else if (Util::Input::IsKeyPressed(LEFT)) {
        ChangeBehavior(0);
        Move("left");
    }
    // right
    else if (Util::Input::IsKeyPressed(RIGHT)) {
        ChangeBehavior(0);
        Move("right");
    }
    // idle
    else if (!is_jump) Idle(); // since idle dont have animation, its okay to be called more than once

    glm::vec2 pos = GetPosition();
    pos.x += x_vel;
    pos.y += y_vel;
    x_vel = 0;
    SetPosition(pos);
}

void Character::Whip(){
    is_whip = true;
    Fall();
    ChangeBehavior(3, true);
    int currentFrame = m_Behavior->GetCurrentFrameIndex();
    glm::vec2 pos = GetPosition();
    if (this->currentFrame != currentFrame) {
        this->currentFrame = currentFrame;
        int offset = (currentFrame == 0) ? -32
                   : (currentFrame == 2) ? 88
                   : (currentFrame == 4) ? -56
                   : 0;
        pos.x += (m_direction == "right") ? offset : -offset;
        if (currentFrame == 4) is_whip = false;
    }
    SetPosition(pos);
}

void Character::Duck(){
    ChangeBehavior(3);
    glm::vec2 pos = GetPosition();
    SetPosition({pos.x, pos.y - 20});
}

void Character::Jump(){
    if (!is_jump) y_vel = 17.0f;
    is_jump = true;
}

void Character::Fall(){
    if (!is_whip) {
        if (y_vel > -5.0f) ChangeBehavior(3);
        else ChangeBehavior(2);
    }
    if (y_vel <= -17.0f) is_jump = false;
    else {
        if (y_vel >= -16.0f) {
            if (jumptype == 1) Move("left");
            else if (jumptype == 2) Move("right");
        }
        is_jump = true;
    }
    y_vel = (y_vel >= -17.0f) ? 
            ((-2.0f <= y_vel && y_vel <= 2.0f) ? y_vel - 0.3f : y_vel - 1.0f) 
            : -17.0f;
}

void Character::Move(std::string direction){
    if (direction == "left") {
        x_vel = -4.5f;
        is_left = true;
    }
    if (direction == "right") {
        x_vel = 4.5f;
        is_right = true;
    }
    if (direction != m_direction){
        m_direction = direction;
        Flip();
    }
}

void Character::Idle(){
    ChangeBehavior(2);
    is_whip = false;
    is_duck = false;
    is_jump = false;
    is_left = false;
    is_right = false;
}

void Character::Flip() {
    glm::vec2 scale = m_Behavior->m_Transform.scale;
    m_Behavior->m_Transform.scale = glm::vec2(-1 * scale.x, scale.y);
}

void Character::CollideBoundary(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
    glm::vec2 charPos = GetPosition();
    glm::vec2 charSize = glm::abs(m_Behavior->GetScaledSize());
    for (auto &block : m_Blocks) {
        int blockType = block->GetType();
        glm::vec2 blockPos = block->GetPosition();
        glm::vec2 blockSize = glm::abs(block->GetScaledSize());
        if (blockType == 0) {
            float blockTop = blockPos.y + blockSize.y * 0.5f - 8.5;
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