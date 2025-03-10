#include "Object/Character.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"

Character::Character(const CharacterValue& value) :
    m_direction(value.direction) {
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
        for (int j = 0; j < ((i == 3) ? 16 : 4); ++j) {
            ascending.emplace_back(basePath + "ascending/ascending-" + std::to_string(j + 1) + ".png");
            descending.emplace_back(basePath + "descending/descending-" + std::to_string(j + 1) + ".png");
            ducking.emplace_back(basePath + "ducking/ducking-" + std::to_string(j + 1) + ".png");
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
    if_whip ? m_Behavior->SetAnimationFrames(whipVector[m_whip_level][BehaviorIndex], m_whip_level == 3 ? 25 : 100)
            : m_Behavior->SetAnimationFrames(behaviorVector[BehaviorIndex], 100);
}

void Character::SetPosition(const glm::vec2& Position) {
    m_Behavior->SetPosition(Position);
}

const glm::vec2& Character::GetPosition() const {
    return m_Behavior->GetPosition();
}

void Character::Keys() {
    m_Behavior->SetPlaying();
    m_Behavior->SetLooping(true);
    /* priority order :
     * - whip
     * - duck
     * - jump
     * - left and right
     * - idle
    */

    // change behavior must called once, otherwise character will not animate since we change the m_Drawable
    if (Util::Input::IsKeyDown(Util::Keycode::J)) {
        is_whip = true;
        ChangeBehavior(3, true);
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::A) ||
             Util::Input::IsKeyDown(Util::Keycode::D) ||
            // to detect when releasing one key after pressing both key, it will change behavior into walk
            (Util::Input::IsKeyUp(Util::Keycode::A) && Util::Input::IsKeyPressed(Util::Keycode::D))  ||
            (Util::Input::IsKeyUp(Util::Keycode::D) && Util::Input::IsKeyPressed(Util::Keycode::A))) ChangeBehavior(0);


    if (is_whip) {
        Whip();
    }
    // when pressing both key, character will idle
    else if (Util::Input::IsKeyPressed(Util::Keycode::A) && Util::Input::IsKeyPressed(Util::Keycode::D)) ChangeBehavior(2);
    // left
    else if (Util::Input::IsKeyPressed(Util::Keycode::A)) Move("left");
    // right
    else if (Util::Input::IsKeyPressed(Util::Keycode::D)) Move("right");
    // jump
    else if (Util::Input::IsKeyDown(Util::Keycode::K)) Jump();
    // idle
    else ChangeBehavior(2); // since idle dont have animation, its okay to be called more than once
    
    glm::vec2 pos = GetPosition();

    // if (pos.y < ) 
    pos.x += x_vel;
    pos.y += y_vel;
    x_vel = 0;
    SetPosition(pos);
}

void Character::Whip(){
    if (m_Behavior->IfAnimationEnds()) is_whip = false;
}

void Character::Jump(){
    
}

void Character::Move(std::string direction){
    if (direction == "left") x_vel = -4.5f;
    if (direction == "right") x_vel = 4.5f;
    if (direction != m_direction){
        m_direction = direction;
        Flip();
    }
}

void Character::Flip() {
    glm::vec2 scale = m_Behavior->m_Transform.scale;
    m_Behavior->m_Transform.scale=glm::vec2(-1 * scale.x, scale.y);
}