// #include <SFML/Graphics.hpp>
#include "AutoLayout.hpp"
#include <json/json.h>
#include <fstream>
#include <iostream>
int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1900, 1080}), "Blueprint Auto Layout", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);
    std::string file_path = "assets";
    AutoLayout a_layout(file_path);
    a_layout.printDataContents();
    a_layout.setAutoLayout(1, 200, 540);
    std::cout << std::endl;
    a_layout.printDataContents();
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }
            if (const auto resized = event->getIf<sf::Event::Resized>())
            {
                window.setView(sf::View(sf::FloatRect({ 0, 0 }, static_cast<sf::Vector2f>(resized->size))));
            }
        }


        window.clear(sf::Color(96, 91, 81));
        window.draw(a_layout);
        window.display();
    }

    // std::string 
    return 0;
}