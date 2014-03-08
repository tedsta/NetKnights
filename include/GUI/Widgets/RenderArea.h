#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "GUI/Element.h"

namespace gui
{
    class RenderArea : public Element
    {
        public:
            RenderArea(int width, int height, bool depthBuffer = false);
            ~RenderArea();

            void draw(sf::RenderTarget& target, const sf::RenderStates& states);

            /// \brief Draw a drawable in this render area
            void drawIn(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

            /// \brief Clear the contents of the render area
            void clear(const sf::Color& color = sf::Color::Black);

            /// \brief Update the contents of the render texture to be displayed.
            void display();

        private:
            sf::RenderTexture mTarget;
            sf::Sprite mSprite;
    };
}


#endif // RENDERAREA_H
