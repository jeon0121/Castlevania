#ifndef APP_HPP
#define APP_HPP
#include "Util/Renderer.hpp"
#include "pch.hpp" // IWYU pragma: export
#include "State/Menu.hpp"

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
        STAGE2A,
        STAGE2B,
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
    GameState m_GameState = GameState::STAGE2A;
    std::shared_ptr<Scene> m_Scene;
    std::shared_ptr<Scene> m_SceneA;
    std::shared_ptr<Scene> m_SceneB;
    std::shared_ptr<Menu> m_Menu;
    std::shared_ptr<Character> m_Character;
    std::vector<int> stairNum = {0, 0};
};

#endif
