#pragma once

#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include "Node.h"
#include "ResourceManager.h"
#include "SoundController.h"



class SoundEmitter : public Node {
    private:
        ALuint source;
        std::string defaultSound = "";
    public:
        bool autoPlay = false;
        inline static std::string className = "SoundEmitter";
        SoundEmitter(std::string sound_name="", bool autoplay = false) : defaultSound(sound_name), autoPlay(autoplay) {
            OPENALCALL(alGenSources((ALuint)1, &source));
            OPENALCALL(alSourcef(source, AL_PITCH, 1));
            OPENALCALL(alSourcef(source, AL_GAIN, 1));
            OPENALCALL(alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE));
            OPENALCALL(alSource3f(source, AL_POSITION, 0, 0, 0));
            OPENALCALL(alSource3f(source, AL_VELOCITY, 0, 0, 0));
            OPENALCALL(alSourcei(source, AL_LOOPING, AL_TRUE));
        }
        ~SoundEmitter() {
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
        std::string getClassName() override {
            return className;
        }
        json serialize() override {
            json j = Node::serialize();
            j["defaultSound"] = defaultSound;
            j["autoPlay"] = autoPlay;
            return j;
        }
        void deserialize(json& j) override {
            Node::deserialize(j);
            setSound(j["defaultSound"]);
            autoPlay = j["autoPlay"];
        }
};