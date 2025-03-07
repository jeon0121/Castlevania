#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;
    context->Update();
    while (!context->GetExit()) {
        context->Update();
        switch (app.GetAppState()) {
            case App::AppState::START:
                app.Start();
                break;

            case App::AppState::UPDATE:
                app.Update();
                break;

            case App::AppState::END:
                app.End();
                context->SetExit(true);
                break;
        }
    }
    return 0;
}
