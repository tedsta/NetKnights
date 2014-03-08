#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics/Sprite.hpp>

#include "GUI/Element.h"

namespace gui
{
    class Button : public Element
    {
        public:
            Button();
            ~Button();

            void update(const float dt);

            void draw(sf::RenderTarget& target, const sf::RenderStates& states);

            bool onMouseButtonPressed(sf::Mouse::Button button);
            bool onMouseButtonReleased(sf::Mouse::Button button);
            void onMouseEntered();
            void onMouseExited();

        private:
            enum State
            {
                Up = 0,
                Hover,
                Down
            };

            sf::Sprite mSprite;
            State mState;
    };
}

#endif // BUTTON_H
