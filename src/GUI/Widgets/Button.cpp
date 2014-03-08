#include "GUI/Widgets/Button.h"

#include <Fission/Core/ResourceManager.h>

namespace gui
{
    Button::Button() : Element(96, 32), mState(Up)
    {
        mSprite.setTexture(*fsn::ResourceManager::get()->getTexture("Content/Textures/GUI/Button.png"));
    }

    Button::~Button()
    {
        //dtor
    }

    void Button::update(const float dt)
    {
    }

    void Button::draw(sf::RenderTarget& target, const sf::RenderStates& states)
    {
        mSprite.setTextureRect(sf::IntRect(static_cast<int>(mState)*getLocalBounds().width, 0,
                                           getLocalBounds().width, getLocalBounds().height));
        target.draw(mSprite, states);
    }

    bool Button::onMouseButtonPressed(sf::Mouse::Button button)
    {
        mState = Down;
        return true;
    }

    bool Button::onMouseButtonReleased(sf::Mouse::Button button)
    {
        if (mouseIsInside())
            mState = Hover;
        else
            mState = Up;
        return true;
    }

    void Button::onMouseEntered()
    {
        if (mState == Up)
            mState = Hover;
    }

    void Button::onMouseExited()
    {
        if (mState == Hover)
            mState = Up;
    }
}
