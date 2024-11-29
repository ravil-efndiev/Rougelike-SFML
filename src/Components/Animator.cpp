#include "Animator.hpp"
#include <Time.hpp>

void Animator::play(const std::string& animName) {
    auto anim = animations.at(animName);
    if (currentAnimation != anim) {
        currentAnimation = anim;
    }
    currentAnimation->play();
}

void Animator::addAnimation(const std::string& animName, const AnimationProps& props) {
    animations[animName] = newRef<Animation>(props);
}

Animation::Animation(const AnimationProps& props) : Props(props) {
    mCurrentSubTexture = sf::IntRect(
        { Props.startX * Props.subTextureSize, Props.startY * Props.subTextureSize },
        { Props.subTextureSize, Props.subTextureSize }
    );
    mCurrentX = Props.startX * Props.subTextureSize;
}

void Animation::play() {
    mFinished = false;
    mTimer += 100.f * Time::dt();

    if (mTimer >= Props.frameDuration) {
        mTimer = 0;

        if (mCurrentX >= Props.endX * Props.subTextureSize) {
            mFinished = true;
            mCurrentX = Props.startX * Props.subTextureSize;
        }
        else {
            mCurrentX += Props.subTextureSize;
        }

        mCurrentSubTexture = sf::IntRect(
            { mCurrentX, Props.startY * Props.subTextureSize },
            { Props.subTextureSize, Props.subTextureSize }
        );
    }
}

sf::IntRect Animation::getSubTexture() const {
    return mCurrentSubTexture;
}

bool Animation::isFinished() const {
    return mFinished;
}
