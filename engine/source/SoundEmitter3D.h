#pragma once

#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include "Node3D.h"
#include "ResourceManager.h"
#include "SoundController.h"



class SoundEmitter3D : public Node3D {
    private:
        ALuint source;
        std::string defaultSound = "";
    public:
        bool autoPlay = false;
        inline static std::string className = "SoundEmitter3D";
        SoundEmitter3D(glm::vec3 position = { 0, 0, 0 }, std::string sound_name = "", bool autoplay = false) : defaultSound(sound_name), autoPlay(autoplay) {
            name = "SoundEmitter3D";
            transform.translate(position);
            OPENALCALL(alGenSources((ALuint)1, &source));
            OPENALCALL(alSourcef(source, AL_PITCH, 1));
            OPENALCALL(alSourcef(source, AL_GAIN, 1));
            OPENALCALL(alSourcef(source, AL_MAX_DISTANCE, 3));
            OPENALCALL(alSourcef(source, AL_REFERENCE_DISTANCE, 1));
            OPENALCALL(alSource3f(source, AL_POSITION, position.x, position.y, position.z));
            OPENALCALL(alSource3f(source, AL_VELOCITY, 0, 0, 0));
            OPENALCALL(alSourcei(source, AL_LOOPING, AL_FALSE));
        }
        ~SoundEmitter3D() {
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
        void start() override {
            if (autoPlay) play();
        }
        void _render(glm::mat4 _transform) override {
            render(_transform);
            for (Node* child : children) {
                child->_render(_transform * transform.getTransform());
            }
        }
        void render(glm::mat4 _transform) override {
            glm::vec3 position = glm::vec3(_transform[3]) + transform.position;
            OPENALCALL(alSource3f(source, AL_POSITION, position.x, position.z, position.y));
            /*std::cout << "\r(" << position.x << ", " << position.z << ", " << position.y << "); ";
            SoundController::printPos();*/
        }
        bool isPlaying() {
            int state;
            alGetSourcei(source, AL_SOURCE_STATE, &state);
            if (state == AL_PLAYING) return true;
            return false;
        }
        std::string getClassName() override {
            return className;
        }
        json serialize() override {
            json j = Node3D::serialize();
            j["defaultSound"] = defaultSound;
            j["autoPlay"] = autoPlay;
            return j;
        }
        void deserialize(json& j) override {
            Node3D::deserialize(j);
            setSound(j["defaultSound"]);
            autoPlay = j["autoPlay"];
        }
};