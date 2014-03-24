#include "CharacterMover.h"

#include <iostream>

CharacterState CharacterMover::step(const CharacterState& initial, const CharacterAction& action)
{
    // Create the next state and set it to the current state
    CharacterState after = initial;

    // Set the new state's input sequence number
    after.sequence = action.sequence;

    // If there's an attack cooldown, decrement it
    if (after.attackCoolDown > 0)
        after.attackCoolDown--;

    switch (initial.moveState)
    {
        case MoveState::Idle:
        case MoveState::Walking:
        {
            if (!action.up && !action.down && !action.left && !action.right && !action.guard && !action.attack && !action.dashAttack)
            {
                after.moveState = MoveState::Idle;
                break;
            }

            // Character wants to guard
            if (action.guard)
            {
                after.moveState = MoveState::Guarding;
                break;
            }

            // Character is not moving and just wants to attack in place. Attack.
            if (action.attack)
            {
                after.moveState = MoveState::Attacking;
                break;
            }

            if (action.up)
            {
                after.position.y -= 0.75f;
                after.moveState = MoveState::Walking;
            }
            else if (action.down)
            {
                after.position.y += 0.75f;
                after.moveState = MoveState::Walking;
            }

            if (action.left)
            {
                after.direction = Direction::Left;

                if (action.dashAttack)
                {
                    after.moveState = MoveState::DashingAttacking;
                    after.dashDirection = Direction::Left;
                }
                else
                {
                    after.position.x -= 1.f;
                    after.moveState = MoveState::Walking;
                }
            }
            else if (action.right)
            {
                after.direction = Direction::Right;

                if (action.dashAttack)
                {
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
            if (!action.guard)
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
                after.attackCoolDown = 30;
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
                after.attackCoolDown = 45;
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
