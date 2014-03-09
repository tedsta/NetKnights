#ifndef CHARACTERMOVER_H
#define CHARACTERMOVER_H

#include <cstddef>

#include <SFML/System/Vector2.hpp>

struct CharacterState
{
    sf::Vector2f position;

    bool up;
    bool down;
    bool left;
    bool right;
    bool firing;

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

    std::size_t sequence;            // Sequence number of this input for the character
    std::size_t ticksSinceLastInput; // Number of game ticks since last input
};

class CharacterMover
{
    public:
        static CharacterState step(const CharacterState& initial, const CharacterInput& input);
};

#endif // CHARACTERMOVER_H
