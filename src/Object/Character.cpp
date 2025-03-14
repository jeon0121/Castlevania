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
    SetPosition(value.position);
    m_Behavior->SetPosition(value.position);
    m_Behavior->SetZIndex(7);
    m_size = glm::abs(m_Behavior->GetScaledSize());
    jumph = landh = value.position.y;
    if (value.direction == "right") Flip();
}

void Character::ChangeBehavior(int BehaviorIndex, bool if_whip) {
    if (if_whip){
        if ((currentBeIndex != BehaviorIndex) || (currentBeIndex == BehaviorIndex && lastVecPos == "be")) {
            m_Behavior->SetAnimationFrames(whipVector[m_whip_level - 1][BehaviorIndex], m_whip_level == 3 ? 25 : 100);
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

void Character::SetPosition(const glm::vec2& position) {
    m_pos = position;
    m_Behavior->SetPosition(position);
    if (is_whip) {
        this->currentFrame = m_Behavior->GetCurrentFrameIndex();
        glm::vec2 pos = position;
        int offset = OffsetValues("whipOffset");
        pos.x += (m_direction == "right") ? offset : -offset;
        if (currentFrame == 4 * (m_whip_level == 3 ? 4 : 1)) is_whip = false;
        m_Behavior->SetPosition(pos);
    }
}

const glm::vec2& Character::GetPosition() const {
    return m_Behavior->GetPosition();
}

void Character::Keys() {
    // Position::PrintCursorCoordinate();

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
    if (Util::Input::IsKeyDown(A) || is_whip) Whip();
    // duck
    else if (Util::Input::IsKeyPressed(DOWN) && !is_jump) Duck();
    // left jump
    else if (Util::Input::IsKeyPressed(LEFT) && Util::Input::IsKeyDown(B) && !is_jump) {
        Jump();
        jumptype = 1;
    }
    // right jump
    else if (Util::Input::IsKeyPressed(RIGHT) && Util::Input::IsKeyDown(B) && !is_jump) {
        Jump();
        jumptype = 2;
    }
    // normal jump
    else if (Util::Input::IsKeyDown(B) && !is_jump) {
        Jump();
        jumptype = 0;
    }
    // fall
    else if (is_jump) {
        Fall();
    }
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
    else if (!is_jump) Idle();
    
    glm::vec2 pos = GetPosition();
    if (is_jump) jumph = pos.y;
    height = jumph - landh;

    m_pos.x += x_vel;
    m_pos.y += y_vel;
    x_vel = 0;
    y_vel = (y_vel > -17.0f) ?
            ((-2.0f <= y_vel && y_vel <= 2.0f) ? y_vel - 0.3f : y_vel - 1.0f) 
            : -17.0f;
    is_collide.y = false;
    SetPosition(m_pos);
}

float Character::OffsetValues(std::string typeName) {
    if (typeName == "whipOffset") {
        float whipOffset = (currentFrame == 0) ? -32
                         : (currentFrame == 1 * (m_whip_level == 3 ? 4 : 1)) ? -32
                         : (currentFrame == 2 * (m_whip_level == 3 ? 4 : 1)) ? 56
                         : (currentFrame == 3 * (m_whip_level == 3 ? 4 : 1)) ? 56
                         : (currentFrame == 4 * (m_whip_level == 3 ? 4 : 1)) ? 0
                         : 0;
        return whipOffset;
    }
    if (typeName == "whipWidth") {
        float whipWidth = (currentFrame == 0) ? 35
                        : (currentFrame == 1 * (m_whip_level == 3 ? 4 : 1)) ? 60
                        : (currentFrame == 2 * (m_whip_level == 3 ? 4 : 1)) ? 130
                        : (currentFrame == 3 * (m_whip_level == 3 ? 4 : 1)) ? 140
                        : 0;
        return whipWidth;
    }
    if (typeName == "duck"){
        float duck = 50.0f;
        return duck;
    }
    return 0;
}

void Character::Whip(){
    is_whip = true;
    if (is_jump) Fall();
    ChangeBehavior(3, true);

    // m_size = glm::abs(m_Behavior->GetScaledSize());
    // float whipWidth = OffsetValues("whipWidth");
    // m_size.x -= whipWidth;
    // m_pos.x += (m_direction == "right") ? (-1 * whipWidth * 0.5f) : (whipWidth * 0.5f);
}

void Character::Duck(){
    ChangeBehavior(3);
    glm::vec2 pos = GetPosition();
    SetPosition({pos.x, pos.y - 20});
    is_duck = true;
    if (!is_duck) m_size.y /= 2;
}

void Character::Jump(){
    if (!is_jump) y_vel = 17.0f;
    is_jump = true;
}

void Character::Fall(){
    if (!is_whip) ChangeBehavior(height > 80.0f ? 3 : 2);
    if (y_vel <= -17.0f) is_jump = false;
    if (is_collide.y) {
        y_vel = 0;
        is_jump = false;
    }
    else {
        if (y_vel >= -16.0f && !is_collide.y) {
            if (jumptype == 1) Move("left");
            else if (jumptype == 2) Move("right");
        }
        is_jump = true;
    }
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
    m_size = glm::abs(m_Behavior->GetScaledSize());
    glm::vec2 pos = GetPosition();
    jumph = landh = pos.y;
}

void Character::Flip() {
    glm::vec2 scale = m_Behavior->m_Transform.scale;
    m_Behavior->m_Transform.scale = glm::vec2(-1 * scale.x, scale.y);
}

void Character::CollideBoundary(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
    float charTop = m_pos.y + m_size.y * 0.5f;
    float charBottom = m_pos.y - m_size.y * 0.5f;
    float charLeft = m_pos.x - m_size.x * 0.5f;
    float charRight = m_pos.x + m_size.x * 0.5f;

    for (auto &block : m_Blocks) {
        glm::vec2 blockPos = block->GetPosition();
        glm::vec2 blockSize = glm::abs(block->GetScaledSize());
        float blockTop = blockPos.y + blockSize.y * 0.5f - 8.5;
        float blockBottom = blockPos.y - blockSize.y * 0.5f;
        float blockLeft = blockPos.x - blockSize.x * 0.5f;
        float blockRight = blockPos.x + blockSize.x * 0.5f;

        if ((charRight > blockLeft && charLeft < blockRight) &&  //overlap x
            (charTop > blockBottom && charBottom < blockTop)) {  //overlap y
            float overlapTop = abs(charTop - blockBottom);
            float overlapBottom = abs(blockTop - charBottom);
            float overlapLeft = abs(charRight - blockLeft);
            float overlapRight = abs(blockRight - charLeft);

            //determine collision base on the smallest
            float minOverlap = std::min({overlapTop, overlapBottom, overlapLeft, overlapRight});

            //above (char on the block)
            if (minOverlap == overlapTop && !is_whip) {
                SetPosition({m_pos.x, blockBottom - m_size.y * 0.5f});
                is_collide.y = true;
            //below (char hit head)
            } else if (minOverlap == overlapBottom) {
                SetPosition({m_pos.x, blockTop + m_size.y * 0.5f});
                is_collide.y = true;
                y_vel = 0;
            //left
            } else if (minOverlap == overlapLeft) {
                SetPosition({blockLeft - m_size.x * 0.5f, m_pos.y});
                is_collide.x = true;
            //right
            } else if (minOverlap == overlapRight) {
                SetPosition({blockRight + m_size.x * 0.5f, m_pos.y});
                is_collide.x = true;
            }
        }
    }
}