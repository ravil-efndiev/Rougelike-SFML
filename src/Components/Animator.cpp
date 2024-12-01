#include "Animator.hpp"
#include <Time.hpp>

void Animator::play(const std::string& animName) {
    auto anim = animations.at(animName);
    if (currentAnimation != anim) {
        currentAnimation = anim;
    }
    currentAnimation->play();
}

void Animator::addAnimation(
    const std::string& animName, 
    const AnimationProps& props,
    const std::string& category
) {
    Ref<Animation> newAnim = newRef<Animation>(props);
    animations[animName] = newAnim;
    
    if (!category.empty()) {
        animCategories[category].push_back(newAnim);
    }
}

void Animator::setAnimationsTrigger(
    const std::vector<Ref<Animation>>& animations,
    const std::function<void(i32)>& trigger
) {
    for (const auto& anim : animations) {
        anim->setTrigger(trigger);
    }
}

Animation::Animation(const AnimationProps& props) : Props(props) {
    mCurrentSubTexture = sf::IntRect(
        { Props.startX * Props.subTextureSize, Props.startY * Props.subTextureSize },
        { Props.subTextureSize, Props.subTextureSize }
    );
    mCurrentX = Props.startX * Props.subTextureSize;
}

void Animation::setTrigger(const std::function<void(i32)>& trigger) {
    mTrigger = trigger;
}

void Animation::play() {
    mFinished = false;
    mTimer += 100.f * Time::dt();

    if (mTimer >= Props.frameDuration) {
        mTimer = 0;

        if (mTrigger)
            mTrigger(mCurrentFrame);

        if (mCurrentX >= Props.endX * Props.subTextureSize) {
            mFinished = true;
            mCurrentX = Props.startX * Props.subTextureSize;
            mCurrentFrame = 0;
        }
        else {
            mCurrentX += Props.subTextureSize;
            mCurrentFrame++;
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
