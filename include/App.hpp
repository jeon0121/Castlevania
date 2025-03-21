#ifndef APP_HPP
#define APP_HPP
#include "Util/Renderer.hpp"
#include "pch.hpp" // IWYU pragma: export

class Scene;

class App {
public:
    enum class AppState {
        START,
        UPDATE,
        END,
    };

    enum class GameState {
        TITLE,
        STAGE0,
        STAGE1,
        STAGE2,
        STAGE3,
    };

    AppState GetAppState() const { return m_AppState; }
    
    GameState GetGameState() const { return m_GameState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void AddAllChildren(std::vector<std::shared_ptr<Util::GameObject>> m_All);

    void RemoveAllChildren(std::vector<std::shared_ptr<Util::GameObject>> m_All);

    Util::Renderer m_Root;
    AppState m_AppState = AppState::START;
    GameState m_GameState = GameState::STAGE0; // start with stage0, so no need to wait title animation
    std::shared_ptr<Scene> m_Scene;
};

#endif
