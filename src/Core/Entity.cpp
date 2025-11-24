#include "Core/Entity.h"
#include <cmath>
#include <iostream>
#include <algorithm>

namespace Ecosystem {
namespace Core {
    // CONSTRUCTOR
    Entity::Entity(EntityType type, Vector2D pos, std::string entityName)
        : mType(type), position(pos), name(entityName), 
        mRandomGenerator(std::random_device{} ()) 
    {
        // Initializing w.r.t Type
        switch (mType)
        {
        case EntityType::HERBIVORE:
            mEnergy = 80.0f;
            mMaxEnergy = 150.0f;
            mMaxAge = 200;
            color = Color::Blue();
            size = 8.0f;
            break;

        case EntityType::CARNIVORE:
            mEnergy = 100.0f;
            mMaxEnergy = 200.0f;
            mMaxAge = 150;
            color = Color::Red();
            size = 12.0f;
            break;

        case EntityType::PLANT:
            mEnergy = 50.0f;
            mMaxEnergy = 100.0f;
            mMaxAge = 300;
            color = Color::Green();
            size = 6.0f;
            break;
        }

        mAge = 0;
        mIsAlive = true;
        mVelocity = GenerateRandomDirection();

        std::cout << "Entite creee avec succes" << name << "a (" << position.x << ", " << position.y << std::endl;    
    }

    // Copy Constructor
    Entity::Entity(const Entity& other) 
    : mType(other.mType), position(other.position), name(other.name + "_copy"),
    mEnergy(other.mEnergy * 0.7f), // Child gets less energy
    mMaxEnergy(other.mMaxEnergy),
    mAge(0), // Child age set to 0
    mMaxAge(other.mMaxAge),
    mIsAlive(true),
    mVelocity(other.mVelocity),
    color(other.color),
    size(other.size * 0.8f), //Child smaller than Parent
    mRandomGenerator(std::random_device{}())
    {
        std::cout << "Copie d'entite creee: " << name << std::endl;
    }

    // DESTRUCTOR
    Entity::~Entity() {
        std::cout << "Entite detruite: " << name << "(Age: " << mAge << ")" << std::endl;
    }
    
    // MAIN UPDATE
    void Entity::Update(float deltaTime) {
        if (!mIsAlive) return;

        // LIFE CYCLE
        ConsumeEnergy(deltaTime);
        Age(deltaTime);
        Move(deltaTime);
        CheckVitality();
    }

    // MOBILITY
    void Entity::Move(float deltaTime) {
        if (mType == EntityType::PLANT) return; // Plants are static

        // Occasional random behaviours
        std::uniform_real_distribution<float> chance(0.0f, 1.0f);
        if (chance(mRandomGenerator) < 0.02f) {
            mVelocity = GenerateRandomDirection();
        }

        // Movement application
        position = position + mVelocity * deltaTime * 20.0f;

        // Energy consumption due to movement
        mEnergy -= mVelocity.Distance(Vector2D(0, 0)) * deltaTime * 0.1f;
    }

    // NUTRITION
    void Entity::Eat(float energy) {
        mEnergy += energy;
        if(mEnergy > mMaxEnergy) {
            mEnergy = mMaxEnergy;
        }
        std::cout << "  " << name << " mange et gagne " << energy << " energie" << std::endl;
    }

    // ENERGY CONSUMPTION
    void Entity::ConsumeEnergy(float deltaTime) {
        float baseConsumption = 0.0f;

        switch (mType) {
            case EntityType::HERBIVORE:
                baseConsumption = 1.5f;
                break;
            case EntityType::CARNIVORE:
                baseConsumption = 2.0f;
                break;
            case EntityType::PLANT:
                baseConsumption = -0.5f; // plants are energy sources
                break;
        }
        mEnergy -= baseConsumption * deltaTime;
    }

    // AGING
    void Entity::Age(float deltaTime) {
        mAge += static_cast<int>(deltaTime * 10.0f);
    }

    // VITALITY CHECK
    void Entity::CheckVitality() {
        if(mEnergy <= 0.0f || mAge >= mMaxAge) {
            mIsAlive = false;
            std::cout << name << "meurt - ";
            if (mEnergy <= 0) std::cout << "Faim";
            else std::cout << "Viellesse";
            std::cout << std::endl;
        }
    }

    // REPRODUCTION
    bool Entity::CanReproduce() const {
        return mIsAlive && mEnergy > mMaxEnergy * 0.8f && mAge > 20;
    }
    std::unique_ptr<Entity> Entity::Reproduce() {
        if(!CanReproduce()) return nullptr;

        // Reproduction chances
        std::uniform_real_distribution<float> chance(0.0f, 1.0f);
        if (chance(mRandomGenerator) < 0.3f) {
            mEnergy *= 0.6f; // energy consumption due to reproduction
            return std::make_unique<Entity>(*this); // use cpy ctor 
        }

        return nullptr;
    }

    // RANDOM DIRECTION GENERATOR
    Vector2D Entity::GenerateRandomDirection() {
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
        return Vector2D(dist(mRandomGenerator), dist(mRandomGenerator));
    }

    // COLOR BASED ON STATE
    Color Entity::CalculateColorBasedOnState() const {
        float energyRatio = GetEnergyPercentage();

        Color baseColor = color;

        // Red if low energy
        if (energyRatio < 0.3f) { 
            baseColor.red = 255; 
            baseColor.green = static_cast<uint8_t>(baseColor.green * energyRatio); 
            baseColor.blue = static_cast<uint8_t>(baseColor.blue * energyRatio); 
        }
        return baseColor; 
    }

    // GRAPHICS RENDERER
    void Entity::Render(SDL_Renderer* renderer) const {
        if (!mIsAlive) return;

        Color renderColor = CalculateColorBasedOnState();

        SDL_FRect rect = {
            position.x - size / 2.0f,
            position.y - size / 2.0f,
            size,
            size
        };

        SDL_SetRenderDrawColor(renderer, renderColor.red, renderColor.green, renderColor.blue, renderColor.alpha);
        SDL_RenderFillRect(renderer, &rect);

        // Energy Indicator (Life Bars) 
        if (mType != EntityType::PLANT) { 
            float energyBarWidth = size * GetEnergyPercentage(); 
            SDL_FRect energyBar = { 
                position.x - size / 2.0f, 
                position.y - size / 2.0f - 3.0f, 
                energyBarWidth,
                2.0f
            };
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
            SDL_RenderFillRect(renderer, &energyBar); 
        }
    }
} // namespace Core    
} // namespace Ecosystem