#ifndef STAMINA_H
#define STAMINA_H

#include <Fission/Core/Component.h>

class Stamina : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Stamina(std::size_t stamina = 100) : mStamina(stamina)
        {
        }

        void serialize(fsn::Packet& packet)
        {
            packet << *this;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> *this;
        }

        void addStamina(std::size_t stamina)
        {
            mStamina += stamina;
        }

        bool takeStamina(std::size_t stamina)
        {
            if (mStamina >= stamina)
            {
                mStamina -= stamina;
                return true;
            }

            return false;
        }

        std::size_t getStamina() const
        {
            return mStamina;
        }

        private:
            std::size_t mStamina;
};

#endif // STAMINA_H
