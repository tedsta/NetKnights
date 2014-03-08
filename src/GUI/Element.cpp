#include "GUI/Element.h"

#include <iostream>

// ************************************************************************************************
// This is a wonderful little snippet I found that allows you to use C++11 range based for loops to
// iterate backwards.
// Source: http://goodliffe.blogspot.com/2013/05/c11-iterating-range-based-for-backwards.html

template <typename T>
class iterate_backwards
{
    public:
        explicit iterate_backwards(const T &t) : t(t) {}
        typename T::const_reverse_iterator begin() const { return t.rbegin(); }
        typename T::const_reverse_iterator end()   const { return t.rend(); }
    private:
        const T &t;
};

template <typename T>
iterate_backwards<T> backwards(const T &t)
{
    return iterate_backwards<T>(t);
}

// ************************************************************************************************

namespace gui
{
    Element::Element(int width, int height) : mParent(nullptr), mPositionIsDirty(true),
        mLocalBounds(0, 0, width, height), mGlobalBounds(0, 0, width, height), mMouseIsInside(false)
    {
        //ctor
    }

    Element::~Element()
    {
        if (mParent)
            mParent->removeChild(this);

        for (auto child : mChildren)
            child->mParent = nullptr;
    }

    void Element::addChild(Element* element)
    {
        mChildren.push_back(element); // Add it to the set of children
        element->mParent = this; // WHO'S YOUR DADDY?!?

        // Do the callback
        onChildAdded(element);
    }

    void Element::move(const sf::Vector2i& delta)
    {
        move(delta.x, delta.y);
    }

    void Element::move(int x, int y)
    {
        mPosition.x += x;
        mPosition.y += y;
        makePositionDirty();
    }

    void Element::resize(int w, int h)
    {
        mLocalBounds.width = w;
        mLocalBounds.height = h;
        onResized(w, h);
    }

    void Element::setPosition(const sf::Vector2i& position)
    {
        mPosition = position;
        makePositionDirty(); // Make the position dirty
    }

    const sf::Vector2i& Element::getGlobalPosition()
    {
        // If the position is dirty, it needs to be recalculated
        if (mPositionIsDirty)
            recalculateGlobalPosition();

        return mGlobalPosition;
    }

    const sf::IntRect& Element::getGlobalBounds()
    {
        // If the position is dirty, it needs to be recalculated
        if (mPositionIsDirty)
            recalculateGlobalPosition();

        return mGlobalBounds;
    }

    bool Element::keyPressed(sf::Keyboard::Key key)
    {
        for (auto child : mChildren)
        {
            if (child->keyPressed(key))
                return true;
        }

        return onKeyPressed(key);
    }

    bool Element::keyReleased(sf::Keyboard::Key key)
    {
        for (auto child : mChildren)
        {
            if (child->keyReleased(key))
                return true;
        }

        return onKeyReleased(key);
    }

    bool Element::mouseButtonPressed(sf::Mouse::Button button)
    {
        if (mMouseIsInside)
        {
            if (mParent)
                mParent->moveToTop(this);

            for (auto child : mChildren)
            {
                if (child->mouseButtonPressed(button))
                    return true;
            }

            return onMouseButtonPressed(button);
        }
        return false;
    }

    bool Element::mouseButtonReleased(sf::Mouse::Button button)
    {
        for (auto child : mChildren)
        {
            if (child->mouseButtonReleased(button))
                return true;
        }

        return onMouseButtonReleased(button);
    }

    bool Element::mouseMoved(int posX, int posY, int moveX, int moveY)
    {
        mMousePosition.x = posX;
        mMousePosition.y = posY;

        if (!mMouseIsInside && getGlobalBounds().contains(posX, posY))
        {
            mMouseIsInside = true;
            onMouseEntered();
        }
        else if (mMouseIsInside && !getGlobalBounds().contains(posX, posY))
        {
            mMouseIsInside = false;
            onMouseExited();
        }

        // If mouse is used to be in window before the move, we still process it. This is so you
        // don't lose your hold on a window if you drag the mouse too quickly
        if (mMouseIsInside || getGlobalBounds().contains(posX-moveX, posY-moveY))
        {
            for (auto child : mChildren)
            {
                if (child->mouseMoved(posX, posY, moveX, moveY))
                    return true;
            }

            return onMouseMoved(posX, posY, moveX, moveY);
        }

        return false;
    }

    void Element::recalculateGlobalPosition()
    {
        mGlobalPosition = mPosition;

        // Iterate up the parent chain
        for (auto parent = mParent; parent != nullptr; parent=parent->mParent)
            mGlobalPosition += parent->mPosition;

        mGlobalBounds.left = mGlobalPosition.x;
        mGlobalBounds.top = mGlobalPosition.y;

        mPositionIsDirty = false; // Position is no longer dirty
    }

    void Element::makePositionDirty()
    {
        mPositionIsDirty = true;

        // Make all the children dirty as well
        for (auto child : mChildren)
            child->makePositionDirty();
    }

    void Element::moveToTop(Element* child)
    {
        // Remove it from wherever it was
        mChildren.erase(std::find(mChildren.begin(), mChildren.end(), child));

        // Re-insert it at the top.
        mChildren.insert(mChildren.begin(), child);
    }

    void Element::removeChild(Element* child)
    {
        // Remove it from wherever it was
        mChildren.erase(std::find(mChildren.begin(), mChildren.end(), child));
    }

    void Element::drawAll(sf::RenderTarget& target, sf::RenderStates states)
    {
        states.transform.translate(mPosition.x, mPosition.y);

        draw(target, states); // Draw me

        // Draw all the children, backwards so the first element gets drawn on top.
        for (auto child : backwards(mChildren))
            child->drawAll(target, states);
    }
}
