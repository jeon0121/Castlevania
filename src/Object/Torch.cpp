#include "Object/Torch.hpp"

Torch::Torch(glm::vec2 position, glm::vec2 scale, int type)
   : AnimatedItems((type == 1) ? std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-1.png", GA_RESOURCE_DIR "/items/fire/fire-2.png"}
                               : std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-3.png", GA_RESOURCE_DIR "/items/fire/fire-4.png"},
   120) {
    SetPosition(position);
    m_Transform.scale = scale;
    SetPlaying();
    SetLooping(true);

    for (int i = 0; i < 4; ++i) {
        torchDeath.emplace_back(GA_RESOURCE_DIR"/items/fire/torch/torchDeath-" + std::to_string(i + 1) + ".png");
    }
}

void Torch::IsWhipped() {
    if (IfAnimationStart())
        SetAnimationFrames(torchDeath, 120);
    SetPlaying();
    if (IfPlayingTime(0.5)) {
        SetPaused();
        SetVisible(false);
        is_destroyed = false;
    }
}