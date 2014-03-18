#ifndef CHARACTERMOVER_H
#define CHARACTERMOVER_H

#include <cstddef>

#include <SFML/System/Vector2.hpp>

struct CharacterState
{
    CharacterState() : moving(false), up(false), down(false), left(false), right(true), firing(false), sequence(0) {}

    sf::Vector2f position;

    bool moving;
    bool up;
    bool down;
    bool left;
    bool right;
    bool firing;
    bool dashing;

    std::size_t sequence;
};

struct CharacterInput
{
    CharacterInput() : up(false), down(false), left(false), right(false), fire(false), sequence(0)
    {
    }

    // Controls
    bool up;
    bool down;
    bool left;
    bool right;

    bool fire;
    bool dash;

    std::size_t sequence;            // Sequence number of this input for the character
    std::size_t ticksSinceLastInput; // Number of game ticks since last input
};

class CharacterMover
{
    public:
        static CharacterState step(const CharacterState& initial, const CharacterInput& input);
};

#endif // CHARACTERMOVER_H
