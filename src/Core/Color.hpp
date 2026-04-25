#pragma once
#include <random>
#include <SFML/Graphics.hpp>

namespace al3d {
	namespace Core {
        sf::Color getRandomColor() {
            static std::random_device rd;
            static std::mt19937 gen(rd());

            std::uniform_int_distribution<int> dist(0, 255);

            return sf::Color(
                static_cast<uint8_t>(dist(gen)),
                static_cast<uint8_t>(dist(gen)),
                static_cast<uint8_t>(dist(gen)),
                255
            );
        }
	}
}