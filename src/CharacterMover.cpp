#include "CharacterMover.h"

#include <iostream>

CharacterState CharacterMover::step(const CharacterState& initial, const CharacterInput& input)
{
    // Create the next state and set it to the current state
    CharacterState after = initial;

    // Set the new state's input sequence number
    after.sequence = input.sequence;

    // If there's an attack cooldown, decrement it
    if (after.attackCoolDown > 0)
        after.attackCoolDown--;

    switch (initial.moveState)
    {
        case MoveState::Idle:
        case MoveState::Walking:
        {
            if (!input.up && !input.down && !input.left && !input.right && !input.guard && !input.attack)
            {
                after.moveState = MoveState::Idle;
                break;
            }

            // Character wants to guard
            if (input.guard)
            {
                after.moveState = MoveState::Guarding;
                break;
            }

            // Character is not moving and just wants to attack in place. Attack.
            if (!input.up && !input.down && !input.left && !input.right && input.attack && initial.attackCoolDown == 0)
            {
                after.moveState = MoveState::Attacking;
                break;
            }

            if (input.up)
            {
                after.position.y -= 0.75f;
                after.moveState = MoveState::Walking;
            }
            else if (input.down)
            {
                after.position.y += 0.75f;
                after.moveState = MoveState::Walking;
            }

            if (input.left)
            {
                after.direction = Direction::Left;

                if (input.attack && initial.attackCoolDown == 0 && initial.stamina >= 25)
                {
                    after.stamina -= 25;
                    after.moveState = MoveState::DashingAttacking;
                    after.dashDirection = Direction::Left;
                }
                else
                {
                    after.position.x -= 1.f;
                    after.moveState = MoveState::Walking;
                }
            }
            else if (input.right)
            {
                after.direction = Direction::Right;

                if (input.attack && initial.attackCoolDown == 0 && initial.stamina >= 25)
                {
                    after.stamina -= 25;
                    after.moveState = MoveState::DashingAttacking;
                    after.dashDirection = Direction::Right;
                }
                else
                {
                    after.position.x += 1.f;
                    after.moveState = MoveState::Walking;
                }
            }

            break;
        }

        case MoveState::Guarding:
        {
            // Character wants to stop guarding
            if (!input.guard)
            {
                after.moveState = MoveState::Idle;
                break;
            }

            break;
        }

        case MoveState::Attacking:
        {
            if (initial.attackTimeLeft == 0)
            {
                after.attackTimeLeft = 15;
                after.attackCoolDown = 45;
                after.moveState = MoveState::Idle;

                break;
            }
            else
            {
                after.attackTimeLeft--;
            }

            break;
        }

        case MoveState::DashingAttacking:
        {
            if (initial.attackTimeLeft == 0)
            {
                // Character is done dashing+attacking. Reset the move state to idle and reset the
                // dashTimeLeft to full.

                after.attackTimeLeft = 15;
                after.attackCoolDown = 60;
                after.moveState = MoveState::Idle;

                break;
            }
            else
            {
                after.attackTimeLeft--;

                switch (after.dashDirection)
                {
                    case Direction::Up:
                    {
                        after.position.y -= 2.f;
                        break;
                    }
                    case Direction::Down:
                    {
                        after.position.y += 2.f;
                        break;
                    }
                    case Direction::Left:
                    {
                        after.position.x -= 2.f;
                        break;
                    }
                    case Direction::Right:
                    {
                        after.position.x += 2.f;
                        break;
                    }
                }
            }

            break;
        }
    }

    return after;
}
