#ifndef CHARACTERANIMATION_H
#define CHARACTERANIMATION_H

#include <array>
#include <map>
#include <string>

#include <Fission/Core/Component.h>

struct FrameLoop
{
    FrameLoop(int start = 0, int stop = 0) : start(start), stop(stop) {}

    int start;
    int stop;
};

class CharacterAnimation : public fsn::Component
{
    FISSION_COMPONENT

    public:
        CharacterAnimation() : curAnim(nullptr), loop(true) {}

        void serialize(fsn::Packet& packet)
        {
            packet << animations;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> animations;
        }

        void addAnimation(const std::string& name, const FrameLoop& frameLoop)
        {
            animations[name] = frameLoop;
        }

        void setAnimation(const std::string& name, bool _loop)
        {
            curAnim = &animations[name];
            loop = _loop;
        }

        FrameLoop& getCurrentAnimation()
        {
            return *curAnim;
        }

        bool isLooping()
        {
            return loop;
        }

    private:
        FrameLoop* curAnim;
        bool loop;

        std::map<std::string, FrameLoop> animations;
};

#endif // CHARACTERANIMATION_H
