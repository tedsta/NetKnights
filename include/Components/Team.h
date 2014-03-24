#ifndef TEAM_H
#define TEAM_H

#include <Fission/Core/Component.h>

class Team : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Team(std::size_t team = 0) : team(team) {}

        void serialize(fsn::Packet& packet)
        {
            packet << *this;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> *this;
        }

        std::size_t team;
};

#endif // TEAM_H
