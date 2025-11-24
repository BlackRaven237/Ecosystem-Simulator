#pragma once

#include "../Graphics/Window.h"
#include "Ecosystem.h"
#include <chrono>

namespace Ecosystem {
namespace Core {

    class GameEngine {
    private:
        // GameEngine State
        Graphics::Window mWindow;
        Ecosystem mEcosystem;
        bool mIsRunning;
        bool mIsPaused;
        float mTimeScale;

        // Timer
        std::chrono::high_resolution_clock::time_point mLastUpdateTime;
        float mAccumulatedTime;
    
    public:
        // Constructor
        GameEngine(const std::string& title, float width, float height);

        // Public Methods
        bool Initialize();
        void Run();
        void Shutdown();

        // Events Handling
        void HandleEvents();
        void HandleInput(SDL_Keycode key);

    private:
        // Private Methods
        void Update(float deltaTime);
        void Render();
        void RenderUI();
    };
    
} // namespace Core
} // namespace Ecosystem