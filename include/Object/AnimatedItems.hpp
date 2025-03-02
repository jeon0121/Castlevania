#ifndef ANIMATED_ITEMS_HPP
#define ANIMATED_ITEMS_HPP
#include <vector>
#include <string>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class AnimatedItems : public Util::GameObject {
public:
    explicit AnimatedItems(const std::vector<std::string>& AnimationPaths, int interval);

    [[nodiscard]] bool IsLooping() const;

    [[nodiscard]] bool IsPlaying() const;

    void SetLooping(bool looping);

    void SetPlaying();

    void SetPaused();

    int GetCurrentFrameIndex();

    void SetCurrentFrame(std::size_t index);

    void SetPosition(const glm::vec2& Position);

    const glm::vec2& GetPosition() const;

    bool IfPlayingTime(float time);

    [[nodiscard]] bool IfAnimationEnds() const;
private:
    unsigned long startTime = 0;
};

#endif