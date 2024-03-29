#ifndef HITPOINTS_H
#define HITPOINTS_H

#include <algorithm>

#include <Fission/Core/Component.h>

class HitPoints : public fsn::Component
{
    FISSION_COMPONENT

    friend class HPInvulnerabilitySystem;

    public:
        HitPoints(int HP = 100) : mHP(HP) {}

        void serialize(fsn::Packet& packet)
        {
            packet << *this;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> *this;
        }

        bool canDamage(const fsn::EntityRef& damager)
        {
            for (auto& dmg : mDamagers)
            {
                if (dmg.entity == damager)
                    return false;
            }

            return true;
        }

        void damage(int damage, std::size_t damageCoolDown = 0, const fsn::EntityRef& damager = fsn::EntityRef())
        {
            // Break out if there's an invulnerability protection
            if (damager.exists() && !canDamage(damager))
                return;

            mHP -= damage;
            if (damageCoolDown > 0 && damager.exists())
                addInvulnerability(damageCoolDown, damager);
        }

        void addInvulnerability(std::size_t damageCoolDown, const fsn::EntityRef& damager)
        {
            mDamagers.push_back(Damager{damager, damageCoolDown});
        }

        int getHP() const {return mHP;}

    private:
        struct Damager
        {
            fsn::EntityRef entity;
            std::size_t invulLeft; // Ticks left of invulnerability
        };

        int mHP;
        std::vector<Damager> mDamagers;
};

#endif // HITPOINTS_H
