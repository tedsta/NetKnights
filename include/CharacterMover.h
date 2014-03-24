#ifndef CHARACTERMOVER_H
#define CHARACTERMOVER_H

#include <cstddef>

#include <SFML/System/Vector2.hpp>

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

enum MoveState
{
    Idle,
    Walking,
    Guarding,
    Attacking,
    DashingAttacking
};

struct CharacterState
{
    CharacterState() : direction(Right), moveState(Idle), dashDirection(Right),
        attackCoolDown(0), attackTimeLeft(15), sequence(0)
    {
    }

    // Character's position
    sf::Vector2f position;

    // Facing direction
    Direction direction;

    // The character's movement state
    MoveState moveState;

    // Direction dashing in
    Direction dashDirection;

    // Time in ticks before next attack possible
    std::size_t attackCoolDown;

    // Time in ticks left for attack to complete
    std::size_t attackTimeLeft;

    // Sequence number of the input to obtain this state
    std::size_t sequence;
};

struct CharacterInput
{
    CharacterInput() : up(false), down(false), left(false), right(false), guard(false), attack(false), sequence(0)
    {
    }

    bool sameAs(const CharacterInput& other)
    {
        if (up == other.up && down == other.down && left == other.left && right == other.right && guard == other.guard && attack == other.attack)
        {
            return true;
        }

        return false;
    }

    // Controls
    bool up;
    bool down;
    bool left;
    bool right;
    bool guard;
    bool attack;

    std::size_t sequence;            // Sequence number of this input for the character
    std::size_t ticksSinceLastInput; // Number of game ticks since last input
};

struct CharacterAction
{
    CharacterAction() : up(false), down(false), left(false), right(false), guard(false),
        attack(false), dashAttack(false), sequence(0)
    {
    }

    bool up;
    bool down;
    bool left;
    bool right;
    bool guard;
    bool attack;
    bool dashAttack;

    std::size_t sequence; // Sequence number of this input for the character
};

class CharacterMover
{
    public:
        static CharacterState step(const CharacterState& initial, const CharacterAction& action);
};

#endif // CHARACTERMOVER_H
