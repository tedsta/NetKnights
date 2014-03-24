#ifndef KNIGHTAI_H
#define KNIGHTAI_H

#include <Fission/Core/Component.h>

#include "CharacterMover.h"

class KnightAI : public fsn::Component
{
    FISSION_COMPONENT

    public:
        KnightAI() : mLastInputTick(0) {}

        void serialize(fsn::Packet& packet)
        {
        }

        void deserialize(fsn::Packet& packet)
        {
        }

        fsn::EntityRef target;

        CharacterInput mInput;
        std::size_t mLastInputTick;
};

#endif // KNIGHTAI_H
