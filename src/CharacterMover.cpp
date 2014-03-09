#include "CharacterMover.h"
CharacterState CharacterMover::step(const CharacterState& initial, const CharacterInput& input)
{
    CharacterState after = initial;
    after.sequence = input.sequence;

    if (input.up)
    {
        after.position.y -= 1.f;
        after.up = true;
        after.down = false;
    }
    else if (input.down)
    {
        after.position.y += 1.f;
        after.down = true;
        after.up = false;
    }
    else if (input.left || input.right)
    {
        after.up = false;
        after.down = false;
    }

    if (input.left)
    {
        after.position.x -= 1.f;
        after.left = true;
        after.right = false;
    }
    else if (input.right)
    {
        after.position.x += 1.f;
        after.right = true;
        after.left = false;
    }
    else if (input.up || input.down)
    {
        after.left = false;
        after.right = false;
    }

    if (input.fire)
        after.firing = true;
    else
        after.firing = false;

    return after;
}
