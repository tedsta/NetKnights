#ifndef VELOCITY_H
#define VELOCITY_H

#include <Fission/Core/Component.h>

class Velocity : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Velocity(float x = 0.f, float y = 0.f) : x(x), y(y)
        {
        }

        float x;
        float y;
};

#endif // VELOCITY_H
