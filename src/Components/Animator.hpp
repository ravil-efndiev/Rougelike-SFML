#pragma once
#include <include.hpp>

struct AnimationProps {
    f32 frameDuration;
    i32 startX, startY, endX; 
    i32 subTextureSize;
};

class Animation {
public:
    Animation(const AnimationProps& props);
    
    void play();
    sf::IntRect getSubTexture() const;
    bool isFinished() const;

    void setTrigger(const std::function<void(i32)>& trigger);

    const AnimationProps Props;

private:
    i32 mCurrentX, mCurrentFrame = 0;
    f32 mTimer = 0;
    bool mFinished;
    sf::IntRect mCurrentSubTexture;
    std::function<void(i32)> mTrigger;
};

struct Animator {
    Animator() = default;

    Ref<Animation> currentAnimation;
    std::unordered_map<std::string, Ref<Animation>> animations;
    std::unordered_map<std::string, std::vector<Ref<Animation>>> animCategories;

    void play(const std::string& animName);
    void addAnimation(
        const std::string& animName, 
        const AnimationProps& props, 
        const std::string& category = "");

    void setAnimationsTrigger(
        const std::vector<Ref<Animation>>& animations,
        const std::function<void(i32)>& trigger);
};
