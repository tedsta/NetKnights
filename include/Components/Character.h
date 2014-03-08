#ifndef CHARACTER_H
#define CHARACTER_H

#include <Fission/Core/Component.h>

class Character : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Character(std::size_t characterID = 0) {}

        std::size_t characterID; // Character ID
};

#endif // CHARACTER_H
