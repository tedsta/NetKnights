#include "CharacterMover.h"
CharacterState CharacterMover::step(const CharacterState& initial, const CharacterInput& input)
{
    CharacterState after = initial;
    after.sequence = input.sequence;

    if (input.up)
        after.position.y -= 1.f;
    if (input.down)
        after.position.y += 1.f;
    if (input.left)
        after.position.x -= 1.f;
    if (input.right)
        after.position.x += 1.f;

    return after;
}
