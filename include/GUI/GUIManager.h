#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <SFML/Graphics/RenderTarget.hpp>

#include <Fission/Input/IKeyboardListener.h>
#include <Fission/Input/IMouseListener.h>
#include <Fission/Rendering/RenderManager.h>

#include "GUI/Element.h"

namespace gui
{
    class Element;

    class GUIManager : public fsn::RenderOverlay, public fsn::IMouseListener
    {
        public:
            GUIManager(int width, int height);

            void draw(sf::RenderTarget& target);

            bool onMouseButtonPressed(sf::Mouse::Button button);
            bool onMouseButtonReleased(sf::Mouse::Button button);
            bool onMouseMoved(int posX, int posY, int moveX, int moveY);

            // Getters

            Element& getRoot() {return mRoot;}

        private:
            Element mRoot; // The root element
    };
}


#endif // GUIMANAGER_H
