#include "GUI/Widgets/RenderArea.h"

namespace gui
{
    RenderArea::RenderArea(int width, int height, bool depthBuffer) : Element(width, height)
    {
        mTarget.create(width, height, depthBuffer);
        mSprite.setTexture(mTarget.getTexture());
    }

    RenderArea::~RenderArea()
    {
        //dtor
    }

    void RenderArea::draw(sf::RenderTarget& target, const sf::RenderStates& states)
    {
        mTarget.display(); // Update the contents of the texture before displaying it

        target.draw(mSprite, states);
    }

    void RenderArea::drawIn(const sf::Drawable& drawable, const sf::RenderStates& states)
    {
        mTarget.draw(drawable, states);
    }

    void RenderArea::clear(const sf::Color& color)
    {
        mTarget.clear(color);
    }

    void RenderArea::display()
    {
        mTarget.display();
    }
}

