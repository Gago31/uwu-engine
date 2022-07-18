#pragma once

#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include "Node2D.h"
#include "ResourceManager.h"
#include "SoundController.h"



class SoundEmitter2D : public Node2D {
    private:
    ALuint source;
    std::string defaultSound = "";
    public:
    bool autoPlay = false;
    inline static std::string className = "SoundEmitter3D";
    SoundEmitter2D(glm::vec2 position = { 0, 0 }, std::string sound_name = "", bool autoplay = false) : defaultSound(sound_name), autoPlay(autoplay) {
        transform.translate(position);
        OPENALCALL(alGenSources((ALuint)1, &source));
        OPENALCALL(alSourcef(source, AL_PITCH, 1));
        OPENALCALL(alSourcef(source, AL_GAIN, 1));
        OPENALCALL(alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE));
        OPENALCALL(alSource3f(source, AL_POSITION, position.x, position.y, 0));
        OPENALCALL(alSource3f(source, AL_VELOCITY, 0, 0, 0));
        OPENALCALL(alSourcei(source, AL_LOOPING, AL_TRUE));
    }
    ~SoundEmitter2D() {
        OPENALCALL(alDeleteSources(1, &source));
    }
    void play(std::string sound_name = "") {
        if (sound_name.empty() && defaultSound.empty()) return;
        OPENALCALL(alSourcei(source, AL_BUFFER, ResourceManager::getSound(sound_name.empty() ? defaultSound : sound_name)));
        OPENALCALL(alSourcePlay(source));
    }
    void setLooping(bool loop) {
        OPENALCALL(alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE));
    }
    void setPitch(float value) {
        OPENALCALL(alSourcef(source, AL_PITCH, value));
    }
    void setGain(float value) {
        OPENALCALL(alSourcef(source, AL_GAIN, value));
    }
    void setSound(std::string sound_name) {
        defaultSound = sound_name;
    }
    bool isPlaying() {
        int state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        if (state == AL_TRUE) return true;
        return false;
    }
    void start() override {
        if (autoPlay) play();
    }
    void render(glm::mat4 _transform) override {
        glm::vec2 position = glm::vec2(_transform[3]) + transform.position;
        //glm::vec3 position = glm::vec2(_transform[3]) + transform.position;
        OPENALCALL(alSource3f(source, AL_POSITION, position.x, 0, position.y));
    }
    std::string getClassName() override {
        return className;
    }
    json serialize() override {
        json j = Node2D::serialize();
        j["defaultSound"] = defaultSound;
        j["autoPlay"] = autoPlay;
        return j;
    }
    void deserialize(json& j) override {
        Node2D::deserialize(j);
        setSound(j["defaultSound"]);
        autoPlay = j["autoPlay"];
    }
};