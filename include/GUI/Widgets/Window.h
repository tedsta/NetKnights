#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics/VertexArray.hpp>

#include "GUI/Element.h"

namespace gui
{
    class Window : public Element
    {
        public:
            Window(int width, int height);

            void draw(sf::RenderTarget& target, const sf::RenderStates& states);

            bool onMouseButtonPressed(sf::Mouse::Button button);
            bool onMouseButtonReleased(sf::Mouse::Button button);
            bool onMouseMoved(int posX, int posY, int moveX, int moveY);
            void onMouseEntered();
            void onMouseExited();

        private:
            sf::VertexArray mVertices;
    };
}

#endif // WINDOW_H
