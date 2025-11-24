#pragma once

#include <cstdint>
#include <string> 
#include <cmath>

namespace Ecosystem {
    namespace Core
    {
        // STRUCTS FOR SIMPLE DATATYPES
        struct Vector2D {
            float x;
            float y;
            
            // Params or Default constructor
            Vector2D(float xValue = 0.0f, float yValue = 0.0f) : x(xValue), y(yValue) {}

            // Struct methods
            float Distance(const Vector2D& other) const {
                float dx = x - other.x;
                float dy = y - other.y;
                return std::sqrt(dx * dx + dy * dy);
            }

            Vector2D operator+ (const Vector2D& other) const {
                return Vector2D(x + other.x, y + other.y); // operator overload
            }

            Vector2D operator* (float scalar) const {
                return Vector2D(x * scalar, y * scalar);
            }
        };
        
        struct Color {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t alpha;

            // Default constructor
            Color() : red(255), green(255), blue(255), alpha(255) {} // Set white by default
            // Params constructor
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) 
                : red(r), green(g), blue(b), alpha(a) {}
            
            // Pre-defined colors
            static Color Red() {return Color(255, 0, 0); }
            static Color Green() {return Color(0, 255, 0); }
            static Color Blue() {return Color(0, 0, 255); }
            static Color Yellow() {return Color(255, 255, 0); }
        };

        struct Food {
            Vector2D position;
            float energyValue;
            Color color;

            // Params constructor
            Food(Vector2D pos, float energy = 25.0f) 
                : position(pos), energyValue(energy), color(Color::Green()) {}
        };

    } // namespace Core
} // namespace Ecosystem