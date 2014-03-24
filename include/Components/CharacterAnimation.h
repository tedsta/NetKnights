#ifndef CHARACTERANIMATION_H
#define CHARACTERANIMATION_H

#include <array>
#include <unordered_map>
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
        CharacterAnimation() : mLoop(true) {}

        void serialize(fsn::Packet& packet)
        {
            packet << mCurrentAnimation;
            packet << mLoop;

            packet << static_cast<std::size_t>(mAnimations.size());
            for (auto& pair : mAnimations)
            {
                packet << pair.first;
                packet << pair.second;
            }
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> mCurrentAnimation;
            packet >> mLoop;

            std::size_t animCount;
            packet >> animCount;

            for (std::size_t i = 0; i < animCount; i++)
            {
                std::size_t anim;
                FrameLoop frameLoop;

                packet >> anim;
                packet >> frameLoop;

                mAnimations[anim] = frameLoop;
            }
        }

        void addAnimation(std::size_t anim, const FrameLoop& frameLoop)
        {
            // If there are no animations, make the current animation this first animation
            if (mAnimations.size() == 0)
                mCurrentAnimation = anim;

            mAnimations[anim] = frameLoop;
        }

        void setAnimation(std::size_t anim, bool loop = true)
        {
            mCurrentAnimation = anim;
            mLoop = loop;
        }

        std::size_t getCurrentAnimation() const
        {
            return mCurrentAnimation;
        }

        const FrameLoop& getCurrentFrameLoop()
        {
            return mAnimations[mCurrentAnimation];
        }

        bool isLooping()
        {
            return mLoop;
        }

    private:
        std::size_t mCurrentAnimation;
        bool mLoop;

        std::unordered_map<std::size_t, FrameLoop> mAnimations;
};

#endif // CHARACTERANIMATION_H
