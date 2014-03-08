#include "GUI/GUIManager.h"

#include "GUI/Element.h"
#include "GUI/Widgets/Button.h"

namespace gui
{
    GUIManager::GUIManager(int width, int height) : mRoot(width, height)
    {
    }

    void GUIManager::draw(sf::RenderTarget& target)
    {
        mRoot.drawAll(target);
    }

    bool GUIManager::onMouseButtonPressed(sf::Mouse::Button button)
    {
        return mRoot.mouseButtonPressed(button);
    }

    bool GUIManager::onMouseButtonReleased(sf::Mouse::Button button)
    {
        return mRoot.mouseButtonReleased(button);
    }

    bool GUIManager::onMouseMoved(int posX, int posY, int moveX, int moveY)
    {
        return mRoot.mouseMoved(posX, posY, moveX, moveY);
    }
}

