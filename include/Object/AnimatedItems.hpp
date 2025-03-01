#ifndef ANIMATED_ITEMS_HPP
#define ANIMATED_ITEMS_HPP
#include <vector>
#include <string>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class AnimatedItems : public Util::GameObject {
public:
    explicit AnimatedItems(const std::vector<std::string>& AnimationPaths);

    [[nodiscard]] bool IsLooping() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
    }

    [[nodiscard]] bool IsPlaying() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
    }

    void SetLooping(bool looping) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetLooping(looping);
    }
    void SetPlaying() {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->Play();
    }
    void SetPaused() {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->Pause();
    }
    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
    const glm::vec2& GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] bool IfAnimationEnds() const;

};



#endif