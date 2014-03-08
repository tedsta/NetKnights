#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <set>
#include <memory>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace gui
{
    class Element
    {
        friend class GUIManager;

        public:
            typedef std::unique_ptr<Element> Ptr;

            Element(int width, int height);
            virtual ~Element();

            /// \brief Update this GUI element
            /// \param dt Change in time in seconds
            virtual void update(const float dt){}

            /// \brief Draw this GUI element
            virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states){}

            /// \brief Add a child to this element
            void addChild(Element* element);

            /// \brief Move this element by a vector
            void move(const sf::Vector2i& delta);

            /// \brief Move this element by a vector
            void move(int x, int y);

            /// \brief Resize this element
            void resize(int w, int h);

            // ************************************************************************************
            // Callbacks

            /// \brief Called when a key is pressed
            virtual bool onKeyPressed(sf::Keyboard::Key key){return false;}

            /// \brief Called when a key is released
            virtual bool onKeyReleased(sf::Keyboard::Key key){return false;}

            /// \brief Called when a mouse button is pressed
            /// \param button The mouse button that was pressed
            /// \param posX The x position of the mouse relative to this element
            /// \param posY The y position of the mouse relative to this element
            virtual bool onMouseButtonPressed(sf::Mouse::Button button){return false;}

            /// \brief Called when a mouse button is released
            /// \param button The mouse button that was released
            /// \param posX The x position of the mouse relative to this element
            /// \param posY The y position of the mouse relative to this element
            virtual bool onMouseButtonReleased(sf::Mouse::Button button){return false;}

            /// \brief Called when the mouse has moved
            /// \param posX The x position of the mouse relative to this element
            /// \param posY The y position of the mouse relative to this element
            /// \param moveX How much the mouse moved on the x axis
            /// \param moveY How much the mouse moved on the y axis
            virtual bool onMouseMoved(int posX, int posY, int moveX, int moveY){return false;}

            /// \brief Called when the mouse has entered this element
            virtual void onMouseEntered(){}

            /// \brief Called when the mouse has left this element
            virtual void onMouseExited(){}

            /// \brief Called when a child is added to this element
            virtual void onChildAdded(Element* child){}

            /// \brief Called when this element is resized
            virtual void onResized(int newW, int newH){}

            // ************************************************************************************
            // Setters

            void setPosition(const sf::Vector2i& position);

            // Getters
            // ************************************************************************************

            const sf::Vector2i& getPosition() const {return mPosition;}
            const sf::Vector2i& getGlobalPosition();
            const sf::IntRect& getLocalBounds() const {return mLocalBounds;}
            const sf::IntRect& getGlobalBounds();
            bool mouseIsInside() const {return mMouseIsInside;}
            const sf::Vector2i& getMousePosition() const {return mMousePosition;}

        private:
            // These functions are used internally to feed input into the elements
            bool keyPressed(sf::Keyboard::Key key);
            bool keyReleased(sf::Keyboard::Key key);
            bool mouseButtonPressed(sf::Mouse::Button button);
            bool mouseButtonReleased(sf::Mouse::Button button);
            bool mouseMoved(int posX, int posY, int moveX, int moveY);

            void recalculateGlobalPosition();
            void makePositionDirty(); // Recursively makes all childrens' positions dirty
            void moveToTop(Element* child); // Moves a child to the top
            void removeChild(Element* child);
            void drawAll(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default); // Used internally to draw this element and all of its children

            Element* mParent;
            std::vector<Element*> mChildren;

            sf::Vector2i mPosition; // Local position
            sf::Vector2i mGlobalPosition; // Global position
            bool mPositionIsDirty;

            sf::IntRect mLocalBounds; // The local bounds
            sf::IntRect mGlobalBounds;

            bool mMouseIsInside; // Whether or not the mouse is currently inside this element
            sf::Vector2i mMousePosition;
    };
}

#endif // GUI_ELEMENT_H
