#pragma once

#include "Entity.h"
#include "Structs.h"
#include <vector>
#include <memory>
#include <random>

namespace Ecosystem {
    namespace Core {
        class Ecosystem {
        private:
            // Internal state
            std::vector<std::unique_ptr<Entity>> mEntities;
            std::vector<Food> mFoodSources;
            float mWorldWidth;
            float mWorldHeight;
            int mMaxEntities;
            int mDayCycle;

            // Random Generator
            mutable std::mt19937 mRandomGenerator;

            // Ecosystem Statistics
            struct Statistics {
                int totalHerbivores;
                int totalCarnivores;
                int totalPlants;
                int totalFood;
                int deathsToday;
                int birthsToday;
            } mStats;

        public:
            // Constructor/Destructor
            Ecosystem(float width, float height, int maxEntities = 500);
            ~Ecosystem();

            // Public Methods
            void Initialize(int initialHerbivores, int initialCarnivores, int initialPlants);
            void Update(float deltaTime);
            void SpawnFood(int count);
            void RemoveDeadEntities();
            void HandleReproduction();
            void HandleEating();

            // Getters
            int GetEntityCount() const { return mEntities.size(); }
            int GetFoodCount() const { return mFoodSources.size(); }
            Statistics GetStatistics() const { return mStats; }
            float GetWorldHeight() const { return mWorldHeight; }
            float GetWorldWidth() const { return mWorldWidth; }

            // Handling Methods
            void AddEntity(std::unique_ptr<Entity> entity);
            void AddFood(Vector2D position, float energy = 25.0f);

            // Renderer
            void Render(SDL_Renderer* renderer) const;

        private:
            // Private Methods
            void UpdateStatistics();
            void SpawnRandomEntity(EntityType type);
            Vector2D GetRandomPosition() const;
            void HandlePlantGrowth(float deltaTime);
        };

    } // namespace Core
} // namespace Ecosystem