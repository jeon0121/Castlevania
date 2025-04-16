#include "Object/Character.hpp"
#include "Utility/Position.hpp"
#include "Utility/Time.hpp"

Character::Character(const CharacterValue& value) :
    m_direction(value.direction), currentBeIndex(value.beIndex) {
    //Image string
    std::vector<std::string> walk, death, duck, hurt, jump, idle, intro, sub_as, sub_de, sub_du, sub_st, ascending, descending, asc_idle, desc_idle;

    //ImageBehavior
    idle.emplace_back(GA_RESOURCE_DIR"/main character/idle/idle.png");
    duck.emplace_back(GA_RESOURCE_DIR"/main character/duck/duck.png");
    hurt.emplace_back(GA_RESOURCE_DIR"/main character/hurt/hurt.png");
    jump.emplace_back(GA_RESOURCE_DIR"/main character/jump/jump.png");
    intro.emplace_back(GA_RESOURCE_DIR"/main character/intro/intro.png");
    asc_idle.emplace_back(GA_RESOURCE_DIR"/main character/stair/ascend/ascend-1.png");
    desc_idle.emplace_back(GA_RESOURCE_DIR"/main character/stair/descend/descend-1.png");

    //AnimatedBehavior
    for (int i = 0; i < 2; ++i){
        ascending.emplace_back(GA_RESOURCE_DIR"/main character/stair/ascend/ascend-" + std::to_string(i + 1) + ".png");
        descending.emplace_back(GA_RESOURCE_DIR"/main character/stair/descend/descend-" + std::to_string(i + 1) + ".png");
        death.emplace_back(GA_RESOURCE_DIR"/main character/death/death-" + std::to_string(i + 1) + ".png");
    }
    for (int i = 0; i < 4; ++i) {
        walk.emplace_back(GA_RESOURCE_DIR"/main character/walk/walk-" + std::to_string(i + 1) + ".png");
        sub_as.emplace_back(GA_RESOURCE_DIR"/main character/use_subweapon/ascending/ascending-" + std::to_string(i + 1) + ".png");
        sub_de.emplace_back(GA_RESOURCE_DIR"/main character/use_subweapon/descending/descending-" + std::to_string(i + 1) + ".png");
        sub_du.emplace_back(GA_RESOURCE_DIR"/main character/use_subweapon/ducking/ducking-" + std::to_string(i + 1) + ".png");
        sub_st.emplace_back(GA_RESOURCE_DIR"/main character/use_subweapon/standing/standing-" + std::to_string(i + 1) + ".png");
    }
    behaviorVector = {walk, death, idle, duck, hurt, jump, intro, sub_as, sub_de, sub_du, sub_st, ascending, descending, asc_idle, desc_idle};

    for (int i = 0; i < 3; i++) {
        std::vector<std::string> ascending, descending, ducking, standing;
        std::string basePath = GA_RESOURCE_DIR "/main character/whip/level-" + std::to_string(i + 1) + "/";
        for (int j = 0; j < ((i == 2) ? 20 : 5); ++j) {
            ascending.emplace_back(basePath + "ascending/ascending-" + std::to_string(j + 1) + ".png");
            descending.emplace_back(basePath + "descending/descending-" + std::to_string(j + 1) + ".png");
            ducking.emplace_back(basePath + "ducking/ducking-" + std::to_string(j + 1) + ".png");
            standing.emplace_back(basePath + "standing/standing-" + std::to_string(j + 1) + ".png");
        }
        std::vector<std::vector<std::string>> wVector = {ascending, descending, ducking, standing};
        whipVector.push_back(wVector);
    }
    
    m_Behavior = std::make_shared<AnimatedItems>(behaviorVector[value.beIndex], 100, glm::vec2(value.scale, value.scale));
    SetPosition(value.position);
    m_Behavior->SetZIndex(7);
    m_size = glm::abs(m_Behavior->GetScaledSize());
    jumph = landh = value.position.y;
    landPosition = prevLandPosition = value.position.y - m_size.y * 0.5;
    if (value.direction == "right")
        Flip();
}

void Character::SetPosition(const glm::vec2& position) {
    m_pos = position;
    if (!is_whip && !is_subweapon)
        m_Behavior->SetPosition(position);
}

void Character::SetDirection(const std::string& Direction) {
    m_direction = Direction;
}

void Character::SetSubWeaponType(WeaponType type) {
    m_subweapon = type;
}

void Character::SetUseWeaponFlag(bool ifuse) {
    is_useweapon = ifuse;
}

void Character::SetHurtFlag(bool ifhurt, bool ifNeedSlip) {
    is_hurt = ifhurt;
    this->ifNeedSlip = ifNeedSlip;
}

const WeaponType& Character::GetSubWeaponType() const {
    return m_subweapon;
}

const bool& Character::GetUseWeaponFlag() const {
    return is_useweapon;
}

bool Character::GetHurtFlag() {
    return (startHurtTime > 0);
}

const glm::vec2& Character::GetPosition() const {
    return m_pos;
}

const glm::vec2& Character::GetLastPosition() const {
    return lastPos;
}

const glm::vec2& Character::GetSize() const {
    return m_size;
}

std::string Character::GetDirection() const {
    return m_direction;
}

void Character::Hurt() {
    if (is_jump && is_hurt && !is_onStair) {
        Fall();
        x_vel = ((m_direction == "right") ? -4.5f : 4.5f);
    }
    if (startHurtTime == 0) {
        if (ifNeedSlip) {
            m_direction = (m_direction == "right") ? "left" : "right";
            Flip();
        }
        if (!is_onStair) 
            Jump();
        startHurtTime = SDL_GetPerformanceCounter();
    } else {
        if (Time::GetRunTimeMs(startHurtTime) > 2000.0f) 
            startHurtTime = 0;
        else {
            int timeCount = static_cast<int>(Time::GetRunTimeMs(startHurtTime)) / 10;
            m_Behavior->SetVisible(timeCount % 4 != 0);
        }
    }
    if (is_collide.y)
        startDuckTime = SDL_GetPerformanceCounter();
    if (startDuckTime) {
        if (is_onStair) {
            if (Time::GetRunTimeMs(startHurtTime) > 2000.0f) 
                is_hurt = false;
        }
        else
            HandleFallDuck(m_direction);
    }
}

bool Character::IfWhip() const {
    return is_whip;
}

int Character::GetWhipLevel() const {
    return m_whip_level;
}

void Character::SetAmmo(int ammo) {
    m_ammo = ammo;
}

int Character::GetAmmo() const {
    return m_ammo;
}

void Character::SetHeart(int heart) {
    m_life = heart;
}

int Character::GetHeart() const {
    return m_life;
}

void Character::SetLevelUpWhip(bool ifLevelUp) {
    is_levelUpWhip = ifLevelUp;
}

void Character::LevelUpWhip(){
    m_whip_level += 1;
    is_levelUpWhip = true;
}

float Character::OffsetValues(std::string typeName) {
    if (typeName == "whipOffset") {
        return (currentFrame == 0) ? -32
             : (currentFrame == 1) ? -32
             : (currentFrame == 2) ? 56
             : (currentFrame == 3) ? 56
             : (currentFrame == 4) ? 0
             : 0;
    }
    if (typeName == "whipHeightAscending") {
        return (currentFrame == 0) ? -4 : 0;
    }
    if (typeName == "whipHeightDescending") {
        return (currentFrame <= 4 && currentFrame >= 2) ? 2 : 0;
    }
    if (typeName == "whipHeightAscending_lv3") {
        return (currentFrame <= 3) ? -4 : 0;
    }
    if (typeName == "whipHeightDescending_lv3") {
        return 0;
    }
    if (typeName == "whipOffset_lv3") {
        return (currentFrame <= 3) ? -32
             : (currentFrame <= 7 && currentFrame >= 4) ? -32
             : (currentFrame <= 11 && currentFrame >= 8) ? 88
             : (currentFrame <= 15 && currentFrame >= 12) ? 88
             : (currentFrame <= 19 && currentFrame >= 16) ? 0
             : 0;
    }
    if (typeName == "whipHeight_lv3") {
        return (currentFrame <= 7 && currentFrame >= 4) ? 2
             : (currentFrame <= 15 && currentFrame >= 12) ? 2
             : 0;
    }
    if (typeName == "whipWidth") {
        return (currentFrame == 0) ? 35
             : (currentFrame == 1) ? 60
             : (currentFrame == 2) ? 100
             : (currentFrame == 3) ? 110
             : 0;
    }
    if (typeName == "whipWidth_lv3") {
        return (currentFrame <= 3) ? 61
             : (currentFrame <= 7 && currentFrame >= 4) ? 104
             : (currentFrame <= 11 && currentFrame >= 8) ? 165
             : (currentFrame <= 15 && currentFrame >= 12) ? 175
             : 0;
    }
    if (typeName == "subWeaponOffset") {
        return (currentFrame == 0) ? -16
             : (currentFrame == 2 || currentFrame == 3) ? 16
             : 0;
    }
    if (typeName == "duck") {
        return 50.0f;//32
    }
    return 0;
}

void Character::ChangeBehavior(int BehaviorIndex, bool if_whip) {
    auto& targetVector = if_whip ? whipVector[m_whip_level - 1] : behaviorVector;
    if (currentBeIndex != BehaviorIndex || lastVecPos != (if_whip ? "whip" : "be")  || is_levelUpWhip) {
        m_Behavior->SetAnimationFrames(targetVector[BehaviorIndex], if_whip && m_whip_level == 3 ? 25 : 100);
        currentBeIndex = BehaviorIndex;
        lastVecPos = if_whip ? "whip" : "be";
    }
}

void Character::UpdatePosition() {
    glm::vec2 pos = m_Behavior->GetPosition();
    if (is_jump)
        jumph = pos.y;
    height = jumph - landh;

    if (is_whip) {
        this->currentFrame = m_Behavior->GetCurrentFrameIndex();
        int offset = (m_whip_level == 3) ? OffsetValues("whipOffset_lv3") : OffsetValues("whipOffset");
        int offsetHeight = 0;
        if (is_onStair) {
            if (prevStairState == 0)
                offsetHeight = (m_whip_level == 3) ? OffsetValues("whipHeightAscending_lv3") : OffsetValues("whipHeightAscending");
            else if (prevStairState == 1)
                offsetHeight = (m_whip_level == 3) ? OffsetValues("whipHeightDescending_lv3") : OffsetValues("whipHeightDescending");
        }
        else 
            offsetHeight = (m_whip_level == 3 && !is_duck) ? OffsetValues("whipHeight_lv3") : 0;
        m_Behavior->SetPosition({m_pos.x + (m_direction == "right" ? offset : -offset), m_pos.y + offsetHeight});
        if (this->currentFrame == 4 * (m_whip_level == 3 ? 4 : 1))
            is_whip = false;
    }
    lastPos = m_pos;
    if (is_subweapon) {
        this->currentFrame = m_Behavior->GetCurrentFrameIndex();
        float offset = OffsetValues("subWeaponOffset");
        m_Behavior->SetPosition({m_pos.x + (m_direction == "right" ? offset : -offset), m_pos.y});
        if (this->currentFrame == 3) {
            is_subweapon = false;
            if (m_subweapon != WeaponType::None) 
                is_useweapon = true;
        }
    }
    m_pos += glm::vec2(x_vel, y_vel);
    if (!is_ascending && !is_descending && !is_onStair) {
        x_vel = 0;
        y_vel = std::max(y_vel - ((-2.0f <= y_vel && y_vel <= 2.0f) ? 0.3f : 1.0f), -14.5f);
    }
    if (m_pos.y - m_size.y * 0.5f < landPosition && !is_jump && !is_onStair)
        m_pos.y = landPosition + m_size.y * 0.5f;
    is_collide = {false, false};
    SetPosition(m_pos);
}

void Character::Keys(const std::vector<std::shared_ptr<Block>>& m_Blocks, const std::vector<std::shared_ptr<Stair>>& m_Stairs) {
    if (is_hurt || startHurtTime != 0) {
        Hurt();
    }
    if (!is_levelUpWhip) {
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

        std::shared_ptr<Stair> stair = CollideStair(m_Stairs);

        /* priority order :
         * - subweapon
         * - ascending and descending
         * - whip
         * - duck
         * - jump
         * - left and right
         * - idle
        */

        // whip on stair
        if ((Util::Input::IsKeyDown(A) || is_whip) && (is_onStair && (!is_ascending && !is_descending))) {
            Whip();
        }
        // ascending
        else if ((Util::Input::IsKeyPressed(UP) && ((stair != nullptr && stair->GetDirection() == "down") || is_onStair)) || is_ascending) {
            Ascending(stair);
        }
        // descending
        else if ((Util::Input::IsKeyPressed(DOWN) && ((stair != nullptr && stair->GetDirection() == "up") || is_onStair)) || is_descending) {
            Descending(stair);
        }
        else if (!is_hurt) {
            // duck subweapon
            if (((Util::Input::IsKeyPressed(UP) && Util::Input::IsKeyPressed(DOWN) && Util::Input::IsKeyDown(A)) || (is_subweapon && is_duck)) && !is_jump) {
                Duck("");
                SubWeapon();
            }
            // subweapon
            else if (((Util::Input::IsKeyPressed(UP) && Util::Input::IsKeyPressed(A) && !is_jump) || is_subweapon) && m_subweapon != WeaponType::None && !is_useweapon && m_ammo > 0) {
                SubWeapon();
            }
            // duck whip
            else if (((Util::Input::IsKeyPressed(DOWN) && Util::Input::IsKeyDown(A)) || (is_whip && is_duck)) && !is_jump){
                Duck("");
                Whip();
            }
            // whip
            else if (Util::Input::IsKeyDown(A) || is_whip)
                Whip();

            // duck
            else if (Util::Input::IsKeyPressed(DOWN) && !is_jump)
                Duck(Util::Input::IsKeyPressed(LEFT) ? "left" : (Util::Input::IsKeyPressed(RIGHT) ? "right" : ""));

            // jump
            else if (Util::Input::IsKeyDown(B) && !is_jump && !change_land && !is_onStair) {
                Jump();
                jumptype = (Util::Input::IsKeyPressed(LEFT)) ? 1 :
                           (Util::Input::IsKeyPressed(RIGHT)) ? 2 : 0;
            }
            // fall
            else if (is_jump)
                Fall();

            // when pressing both key, character will idle
            else if (Util::Input::IsKeyPressed(LEFT) && Util::Input::IsKeyPressed(RIGHT))
                Idle();

            // left
            else if (Util::Input::IsKeyPressed(LEFT) && !is_duck && !is_onStair) {
                HandleFallDuck("left");
                Idle();
            }
            // right
            else if (Util::Input::IsKeyPressed(RIGHT) && !is_duck && !is_onStair) {
                HandleFallDuck("right");
                Idle();
            }
            // idle
            else if (!is_jump) {
                if (is_onStair) {
                    if (currentStair->GetDirection() == "down") 
                        ChangeBehavior(13);
                    else if (currentStair->GetDirection() == "up")
                        ChangeBehavior(14);
                }
                else {
                    ChangeBehavior(2);
                    Idle();
                }
            }
            // std::cout << m_pos.x << ", "
            //           << m_pos.y << ", "
            //           << m_size.x << ", "
            //           << m_size.y << ", "
            //           << std::endl;
        }
    }
    UpdatePosition();
    if (!is_onStair)
        CollideBoundary(m_Blocks);
}

void Character::HandleFallDuck(const std::string& direction) {
    if (!startDuckTime && !(change_land && prevLandPosition > landPosition)) {
        Move(direction);
        return;
    }
    m_pos.y -= OffsetValues("duck") * 0.5;
    if (is_hurt && startDuckTime && !is_onStair) {
        Duck(m_direction);
        if (Time::GetRunTimeMs(startDuckTime) > 150.0f) {
            is_hurt = false;
            startDuckTime = 0;
        }
    }else {
        if (!startDuckTime)
            startDuckTime = SDL_GetPerformanceCounter();
        if (prevLandPosition - landPosition > 150.0f)
            Duck(m_direction);
        const float limit = (prevLandPosition - landPosition > 150.0f) ? 450.0f : 150.0f;
        if (Time::GetRunTimeMs(startDuckTime) > limit) {
            change_land = false;
            startDuckTime = 0;
        }
    }
}

void Character::Ascending(std::shared_ptr<Stair>& stair) {
    if (!is_onStair && stair && stair->GetDirection() == "down") {
        if (std::abs(m_pos.x - stair->GetPosition().x) > 4.0f) {
            if (m_pos.x < stair->GetPosition().x)
                Move("right");
            else
                Move("left");
            return;
        }
        is_onStair = true;
        currentStair = stair;

        float xOffset = (stair->GetPosition().x < stair->GetConnected()->GetPosition().x) ? 30.0f : -30.0f;
        nextStairPos = m_pos + glm::vec2(xOffset, 30.0f);
        std::string desiredDir = (xOffset > 0) ? "right" : "left";
        if (m_direction != desiredDir) {
            m_direction = desiredDir;
            Flip();
        }
    }
    if (is_onStair && !is_ascending) {
        if (prevStairState == 1) {
            m_direction = (m_direction == "right") ? "left" : "right";
            currentStair = currentStair->GetConnected();
            Flip();
        }
        prevStairState = 0;
        ChangeBehavior(11);
        is_ascending = true;
        float xOffset = (currentStair->GetPosition().x < currentStair->GetConnected()->GetPosition().x) ? 30.0f : -30.0f;
        nextStairPos = m_pos + glm::vec2(xOffset, 30.0f);
        std::string desiredDir = (xOffset > 0) ? "right" : "left";
        if (m_direction != desiredDir) {
            m_direction = desiredDir;
            Flip();
        }
    }
    if (is_ascending) {
        if (m_pos.y < nextStairPos.y) {
            y_vel = 1.5f;
            x_vel = (nextStairPos.x > m_pos.x) ? 1.7f : -1.7f;
        }
        else {
            m_pos.y = nextStairPos.y - 1.0f;
            x_vel = y_vel = 0.0f;
            is_ascending = false;
        }
    }
    if (currentStair == nullptr) return;
    float targetY = currentStair->GetConnected()->GetPosition().y - currentStair->GetConnected()->GetScaledSize().y * 0.5f;
    float charBottom = m_pos.y - m_size.y * 0.5f;
    if (charBottom >= targetY) {
        is_ascending = false;
        is_onStair = false;
        currentStair = nullptr;
        x_vel = y_vel = 0.0f;
        prevStairState = -1;
        landPosition = m_pos.y - m_size.y * 0.5f;
    }
}

void Character::Descending(std::shared_ptr<Stair>& stair) {
    if (!is_onStair && stair && stair->GetDirection() == "up") {
        if (std::abs(m_pos.x - stair->GetPosition().x) > 4.0f) {
            if (m_pos.x < stair->GetPosition().x)
                Move("right");
            else
                Move("left");
            return;
        }
        is_onStair = true;
        currentStair = stair;

        float xOffset = (stair->GetPosition().x < stair->GetConnected()->GetPosition().x) ? 30.0f : -30.0f;
        nextStairPos = m_pos + glm::vec2(xOffset, -30.0f);
        std::string desiredDir = (xOffset > 0) ? "right" : "left";
        if (m_direction != desiredDir) {
            m_direction = desiredDir;
            Flip();
        }
    }
    if (is_onStair && !is_descending) {
        if (prevStairState == 0) {
            m_direction = (m_direction == "right") ? "left" : "right";
            currentStair = currentStair->GetConnected();
            Flip();
        }
        prevStairState = 1;
        ChangeBehavior(12);
        is_descending = true;
        float xOffset = (currentStair->GetPosition().x < currentStair->GetConnected()->GetPosition().x) ? 30.0f : -30.0f;
        nextStairPos = m_pos + glm::vec2(xOffset, -30.0f);
        std::string desiredDir = (xOffset > 0) ? "right" : "left";
        if (m_direction != desiredDir) {
            m_direction = desiredDir;
            Flip();
        }
    }
    if (is_descending) {
        if (m_pos.y > nextStairPos.y) {
            y_vel = -1.5f;
            x_vel = (nextStairPos.x > m_pos.x) ? 1.7f : -1.7f;
        }
        else {
            m_pos.y = nextStairPos.y + 1.0f;
            x_vel = y_vel = 0.0f;
            is_descending = false;
        }
    }
    if (currentStair == nullptr) return;
    float targetY = currentStair->GetConnected()->GetPosition().y - currentStair->GetConnected()->GetScaledSize().y * 0.5f;
    float charBottom = m_pos.y - m_size.y * 0.5f;
    if (charBottom <= targetY) {
        is_descending = false;
        is_onStair = false;
        currentStair = nullptr;
        x_vel = y_vel = 0.0f;
        prevStairState = -1;
        landPosition = m_pos.y - m_size.y * 0.5f;
    }
}

void Character::SubWeapon() {
    is_subweapon = true;
    if (is_jump)
        Fall();
    if (is_duck)
        ChangeBehavior(9);
    else
        ChangeBehavior(10);
}

void Character::Whip(){
    m_size.x = 64;
    is_whip = true;
    if (is_jump)
        Fall();
    if (is_onStair) 
        ChangeBehavior(prevStairState, true);
    else if (is_duck)
        ChangeBehavior(2, true);
    else
        ChangeBehavior(3, true);
}

void Character::Duck(std::string direction){
    if (!is_whip && !is_subweapon)
        ChangeBehavior(3);
    if (direction != m_direction && direction != ""){
        m_direction = direction;
        Flip();
    }
    is_duck = true;
    m_size = glm::abs(m_Behavior->GetScaledSize());
    m_size.y -= OffsetValues("duck");
}

void Character::Jump(){
    if (!is_jump)
        y_vel = 14.5f;
    if (is_hurt && is_jump)
        y_vel = 12.5f;
    is_jump = true;
}

void Character::Fall(){
    if(is_hurt)
        ChangeBehavior(4);
    else if (!is_whip)
        ChangeBehavior(height > 80.0f ? 3 : 2);
    is_jump = false;
    if (is_collide.y)
        y_vel = 0;
    else {
        if (jumptype == 1)
            x_vel = -4.5f;
        else if (jumptype == 2)
            x_vel = 4.5f;
        is_jump = true;
    }
}

void Character::Move(std::string direction){
    ChangeBehavior(0);
    x_vel = direction == "left" ? -4.5f
          : direction == "right" ? 4.5f : 0.0f;
    if (direction != m_direction){
        m_direction = direction;
        Flip();
    }
}

void Character::Idle() {
    if ((change_land && prevLandPosition > landPosition) || startDuckTime) {
        m_pos.y -= OffsetValues("duck") * 0.5;
        if (prevLandPosition - landPosition > 150.0f)
            Duck(m_direction);
        if (!startDuckTime)
            startDuckTime = SDL_GetPerformanceCounter();
        const float limit = (prevLandPosition - landPosition > 150.0f) ? 450.0f : 150.0f;
        //limit = 150 prevent char from being able to jump while still in air; limit = 450 means fall duck
        if (Time::GetRunTimeMs(startDuckTime) > limit) {
                change_land = false;
                startDuckTime = 0;
        }
    }
    else {
        startDuckTime = 0;
        change_land = is_whip = is_subweapon = is_duck = is_jump = false;
        m_size = glm::abs(m_Behavior->GetScaledSize());
        jumph = landh = m_Behavior->GetPosition().y;
    }
}

void Character::Flip() {
    glm::vec2 scale = m_Behavior->m_Transform.scale;
    m_Behavior->m_Transform.scale = glm::vec2(-1 * scale.x, scale.y);
}

void Character::CollideBoundary(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
    float testLanding = -330.0f;
    float charTop = m_pos.y + m_size.y * 0.5f;
    float charBottom = m_pos.y - m_size.y * 0.5f;
    if (currentBeIndex == 3 && is_jump && !is_whip && (jumph - landPosition) >= 25.0f)
        charBottom += 30.0f;
    float charLeft = m_pos.x - m_size.x * 0.5f;
    float charRight = m_pos.x + m_size.x * 0.5f;
    for (auto &block : m_Blocks) {
        glm::vec2 blockPos = block->GetPosition();
        glm::vec2 blockSize = glm::abs(block->GetScaledSize());
        float blockTop = blockPos.y + blockSize.y * 0.5f - 8.5f;
        float blockBottom = blockPos.y - blockSize.y * 0.5f;
        float blockLeft = blockPos.x - blockSize.x * 0.5f;
        float blockRight = blockPos.x + blockSize.x * 0.5f;

        float overlapTop = charTop - blockBottom;
        float overlapBottom = blockTop - charBottom;
        float overlapLeft = charRight - blockLeft;
        float overlapRight = blockRight - charLeft;
        if ((charRight > blockLeft && charLeft < blockRight) &&  //overlap x
            (charTop > blockBottom && charBottom < blockTop)) {  //overlap y
            //determine collision base on the smallest
            float minOverlap = std::min({abs(overlapTop), abs(overlapBottom), abs(overlapLeft), abs(overlapRight)});
            //below (char hit head)
            if (minOverlap == overlapTop && !is_jump) {
                SetPosition({m_pos.x, blockBottom - m_size.y * 0.5f});
                is_collide.y = true;
            //above (char on the block)
            } else if (minOverlap == overlapBottom) {
                SetPosition({m_pos.x, blockTop + m_size.y * 0.5f});
                is_collide.y = true;
            //left
            } else if (minOverlap == overlapLeft && !(is_jump && charRight - 5.0f >= blockLeft)) {
                SetPosition({blockLeft - m_size.x * 0.5f, m_pos.y});
                is_collide.x = true;
            //right
            } else if (minOverlap == overlapRight && !(is_jump && charLeft + 5.0f <= blockRight)) {
                SetPosition({blockRight + m_size.x * 0.5f, m_pos.y});
                is_collide.x = true;
            }
        }
        if ((charRight > blockLeft + 5.0f && charLeft < blockRight - 5.0f) && // Overlap X
            (charTop - 20.0f > blockTop && blockTop > testLanding)) { // character above block & highest block detected
            testLanding = blockTop;
        }
    }
    if (testLanding != landPosition) {
        prevLandPosition = landPosition;
        if (testLanding < landPosition && ((landPosition - testLanding) > 120.0f || is_jump)) 
            change_land = true;
        landPosition = testLanding;
    }
}

std::shared_ptr<Stair> Character::CollideStair(const std::vector<std::shared_ptr<Stair>>& m_Stairs) {
    float charTop = m_pos.y + m_size.y * 0.5f;
    float charBottom = m_pos.y - m_size.y * 0.5f;
    float charLeft = m_pos.x - m_size.x * 0.5f;
    float charRight = m_pos.x + m_size.x * 0.5f;
    for (auto &stair : m_Stairs) {
        glm::vec2 stairPos = stair->GetPosition();
        glm::vec2 stairSize = glm::abs(stair->GetScaledSize());
        float stairTop = stairPos.y + stairSize.y * 0.5f;
        float stairBottom = stairPos.y - stairSize.y * 0.5f;
        float stairLeft = stairPos.x - stairSize.x * 0.5f;
        float stairRight = stairPos.x + stairSize.x * 0.5f;

        if ((charRight > stairLeft && charLeft < stairRight) &&  //overlap x
            (charTop > stairBottom && charBottom < stairTop)) {  //overlap y
            return stair;
        }
    }
    return nullptr;
}