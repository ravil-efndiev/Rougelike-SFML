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

    const AnimationProps Props;

private:
    i32 mCurrentX;
    f32 mTimer = 0;
    bool mFinished;
    sf::IntRect mCurrentSubTexture;
};

struct Animator {
    Animator() = default;

    Animation* currentAnimation;
    std::unordered_map<std::string, Ref<Animation>> animations;

    void play(const std::string& animName);
    void addAnimation(const std::string& animName, const AnimationProps& props);
};

