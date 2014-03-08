#ifndef NETWORKCOMPONENT_H
#define NETWORKCOMPONENT_H

#include <Fission/Core/Component.h>

class Network : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Network(std::size_t ID = 0) : ID(ID) {}

        std::size_t ID; // Network Entity ID
};

#endif // NETWORKCOMPONENT_H
