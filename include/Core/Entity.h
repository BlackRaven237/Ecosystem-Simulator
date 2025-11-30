#pragma once
 
#include "Structs.h" 
#include <SDL3/SDL.h> 
#include <memory> 
#include <random> 
#include <vector> 

namespace Ecosystem {
    namespace Core {
        // Enumeration of Entity Types 
        enum class EntityType { 
            HERBIVORE, 
            CARNIVORE, 
            PLANT 
        }; 

        class Entity { 
        private: 
            // Private Data - Protected Internal State 
            float mEnergy; 
            float mMaxEnergy; 
            int mAge; 
            int mMaxAge; 
            bool mIsAlive; 
            Vector2D mVelocity; 
            EntityType mType; 

            // Random Generator 
            mutable std::mt19937 mRandomGenerator; 

        public: 
            // Public Data - Secured Direct Access 
            Vector2D position; 
            Color color; 
            float size; 
            std::string name; 

            // Constructors
            Entity(EntityType type, Vector2D pos, std::string entityName = "Unnamed"); 
            Entity(const Entity& other);  // copy constructor 

            // Destructor
            ~Entity(); 

            // Public Methods
            void Update(float deltaTime); 
            void Move(float deltaTime); 
            void Eat(float energy); 
            bool CanReproduce() const; 
            std::unique_ptr<Entity> Reproduce(); 
            void ApplyForce(Vector2D force); 

            // GETTERS - Controlled Access To Private Data
            float GetEnergy() const { return mEnergy; } 
            float GetEnergyPercentage() const { return mEnergy / mMaxEnergy; } 
            int GetAge() const { return mAge; } 
            bool IsAlive() const { return mIsAlive; } 
            EntityType GetType() const { return mType; } 
            Vector2D GetVelocity() const { return mVelocity; } 

            // Behaviour Methods
            Vector2D SeekFood(const std::vector<Food>& foodSources) const; 
            Vector2D AvoidPredators(const std::vector<Entity>& predators) const; 
            Vector2D StayInBounds(float worldWidth, float worldHeight) const; 

            // Renderer Method
            void Render(SDL_Renderer* renderer) const;
             
        private: 
            // Private Methods - Internal Logic
            void ConsumeEnergy(float deltaTime); 
            void Age(float deltaTime); 
            void CheckVitality(); 
            Vector2D GenerateRandomDirection() const; 
            Color CalculateColorBasedOnState() const; 
        };

    } // namespace Core 
} // namespace Ecosystem 