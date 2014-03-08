#include "GUI/Widgets/Window.h"

#include <SFML/Graphics/Color.hpp>

namespace gui
{
    Window::Window(int width, int height) : Element(width, height), mVertices(sf::Quads, 4)
    {
        mVertices[0].position = sf::Vector2f(0, 0);
        mVertices[1].position = sf::Vector2f(width, 0);
        mVertices[2].position = sf::Vector2f(width, height);
        mVertices[3].position = sf::Vector2f(0, height);

        mVertices[0].color = sf::Color(50, 50, 120, 255);
        mVertices[1].color = sf::Color(50, 50, 120, 255);
        mVertices[2].color = sf::Color(50, 50, 120, 255);
        mVertices[3].color = sf::Color(50, 50, 120, 255);
    }

    void Window::draw(sf::RenderTarget& target, const sf::RenderStates& states)
    {
        target.draw(mVertices, states);
    }

    bool Window::onMouseButtonPressed(sf::Mouse::Button button)
    {
        return true; // If you click inside the window, capture the event
    }

    bool Window::onMouseButtonReleased(sf::Mouse::Button button)
    {
        return false;
    }

    bool Window::onMouseMoved(int posX, int posY, int moveX, int moveY)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            move(moveX, moveY);
            return true;
        }
        return false;
    }

    void Window::onMouseEntered()
    {
    }

    void Window::onMouseExited()
    {
    }
}

