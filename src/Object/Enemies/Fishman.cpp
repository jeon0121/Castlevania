#include "Object/Enemies/Fishman.hpp"

#include <utility>
#include "Utility/Time.hpp"
//walk towards char about 4.5 block, then attack, <4.5 drop when they hit border
//about 2~3s reborn.

Fishman::Fishman(glm::vec2 position, std::string direction, App* app, std::vector<std::shared_ptr<ImageItems>> bubbles): Enemy(position, direction, {}, 250, 100) {
    idleImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-1.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-1.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-2.png");
    shootImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-3.png");
    shootImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-4.png");
    this->bubbles = bubbles;

    SetAnimationFrames(idleImages, 0);
    m_Transform.scale = glm::vec2(1, 0.88f);
    if (direction == "right")
        Flip();
    SetPlaying();
    SetLooping(true);
    soundEft = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/14.wav");
    countHurt = 2;
}

void Fishman::MoveBehav(std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks) {
    isFire = false;
    SetPlaying();
    SetLooping(true);
    glm::vec2 pos = GetPosition();
    SetPosition({pos.x, pos.y + y_vel});
    if (!isOnBlock) {
        if (!isBubble && pos.y < -240 && !is_hidden)
            isBubble = true;
        SetAnimationFrames(idleImages, 400);
    }

    if (state != "Attack" && isOnBlock) {
        Walk(character);
    }else if (state == "Spawn") {
        Spawn(character);
    }else if (state == "Attack"){
        if (IfAnimationEnds())
            state = "Walk";
    }

    if (isBubble)
        Bubble(pos);

    if (y_vel < 0.0f)
        CollideBlock(blocks);
}

void Fishman::SetReset() {
    is_dead = false;
    is_hidden = false;
    isFire = false;
    isBubble = false;
    isOnBlock = false;
    hasEnteredWindow = false;
    startWalkTime = 0;
    y_vel = 29.5f;
    state = "Spawn";
    SetAnimationFrames(idleImages, 0);
    SetPlaying();
    SetLooping(true);
}

bool Fishman::CheckReset() {
    glm::vec2 pos = GetPosition();
    if (pos.y < -380 || pos.x > 480 || pos.x < -500)
        is_hidden = true;
    return !isBubble && (is_hidden || is_dead);
}

void Fishman::Spawn(std::shared_ptr<Character> &character) {
    glm::vec2 pos = GetPosition();
    if (GetPosition().y == -250) {
        soundEft->LoadMedia(GA_RESOURCE_DIR "/Sound Effects/14.wav");
        soundEft->Play();
        soundEft->SetVolume(30);
        if (character->GetPosition().x > pos.x)
            SetDirection("right");
        else
            SetDirection("left");
    }
    y_vel = std::max(y_vel - ((-2.0f <= y_vel && y_vel <= 2.0f) ? 0.3f : 1.0f), -29.5f);
}

void Fishman::Bubble(glm::vec2 fishpos) {
    if (bbl_vel[0] == 0) {
        for (int i = 0; i < 3; i++) {
            bubbles[i]->SetPosition({fishpos.x, -220});
            bubbles[i]->SetVisible(true);
        }
        soundEft->LoadMedia(GA_RESOURCE_DIR "/Sound Effects/15.wav");
        soundEft->Play();
        soundEft->SetVolume(30);
    }
    bool check = false;
    for (int i = 0; i < 3; i++) {
        glm::vec2 pos = bubbles[i]->GetPosition();
        float x = (i == 0) ? -1.8 :((i == 1) ? 1.0 : 1.8);
        bbl_vel[i] = std::max(bbl_vel[i] - ((bbl_vel[i] >= 0) ? 1.2f : 1.8f), -5.0f);
        bubbles[i]->SetPosition({pos.x + x, pos.y + bbl_vel[i]});
        if (pos.y > -410)
            check = true;
    }
    if (!check) {
        bbl_vel = {0, 12.0f, 0};
        isBubble = false;
    }
}

void Fishman::Walk(std::shared_ptr<Character> &character) {
    glm::vec2 pos = GetPosition();
    if (startWalkTime == 0) {
        if (character->GetPosition().x > pos.x)
            SetDirection("right");
        else
            SetDirection("left");
        SetAnimationFrames(walkImages, 400);
        startWalkTime = SDL_GetPerformanceCounter();
    }
    SetPosition({pos.x + (direction == "right" ? 3.0f : -3.0f), pos.y});
    if (Time::GetRunTimeMs(startWalkTime) > 1800.0f) {
        startWalkTime = 0;
        state = "Attack";
        SetAnimationFrames(shootImages, 400);
        isFire = true;
    }
}

bool Fishman::GetFireFlag() const {
    return isFire;
}

bool Fishman::GetBubbleFlag() const {
    return isBubble;
}

void Fishman::CollideBlock(std::vector<std::shared_ptr<Block>> &blocks) {
    bool check = false;
    glm::vec2 pos = GetPosition();
    glm::vec2 size = glm::abs(GetScaledSize());
    float fishTop = pos.y + size.y * 0.5f;
    float fishBottom = pos.y - size.y * 0.5f;
    float fishLeft = pos.x - size.x * 0.5f;
    float fishRight = pos.x + size.x * 0.5f;
    for (auto block : blocks) {
        glm::vec2 blockPos = block->GetPosition();
        glm::vec2 blockSize = glm::abs(block->GetScaledSize());
        float blockTop = blockPos.y + blockSize.y * 0.5f - 8.5f;
        float blockBottom = blockPos.y - blockSize.y * 0.5f;
        float blockLeft = blockPos.x - blockSize.x * 0.5f;
        float blockRight = blockPos.x + blockSize.x * 0.5f;

        float overlapTop = fishTop - blockBottom;
        float overlapBottom = blockTop - fishBottom;
        float overlapLeft = fishRight - blockLeft;
        float overlapRight = blockRight - fishLeft;

        if ((fishRight > blockLeft && fishLeft < blockRight) &&  //overlap x
            (fishTop > blockBottom && fishBottom < blockTop)) {  //overlap y
            float minOverlap = std::min({abs(overlapTop), abs(overlapBottom), abs(overlapLeft), abs(overlapRight)});
            if (minOverlap == overlapBottom) {
                SetPosition({pos.x, blockTop + size.y * 0.5f});
                check = true;
                isOnBlock = true;
            }
        }else if (!check)
            isOnBlock = false;
    }
}