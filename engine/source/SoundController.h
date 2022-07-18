#pragma once

#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include <glm/glm.hpp>
#include "ResourceManager.h"

#define OPENALCALL(function)\
    function;\
    {\
        ALenum error = alGetError();\
        assert(error != AL_NO_ERROR, "OpenAL Error");\
    }


class SoundController {
    private:
        inline static ALCdevice* device;
        inline static ALCcontext* context;
        inline static glm::vec3 listenerPosition = { 0, 0, 0 };
        inline static ALfloat listenerOrientation[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
        SoundController() {};
    public:
	    static int initialize() {
            const char* devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
            device = OPENALCALL(alcOpenDevice(devicename));
            if (device == nullptr) return -1;

            ALboolean enumeration = OPENALCALL(alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"));
            if (enumeration == AL_FALSE) {
                std::cout << "enumeration not supported" << std::endl;
            } else {
                std::cout << "enumeration supported" << std::endl;
            }

            context = OPENALCALL(alcCreateContext(device, NULL));
            if (!alcMakeContextCurrent(context)) return -1;

            std::cout << "OpenAL context OK" << std::endl;

            //---------------------------------------------
            OPENALCALL(alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED));
            updateListener({ 0, 0, 0 }, 0);

            return 0;
	    }
        static void updateListener(glm::vec3 position, float theta) {
            listenerPosition = position;
            OPENALCALL(alListener3f(AL_POSITION, position.x, position.z, position.y));
            OPENALCALL(alListener3f(AL_VELOCITY, 0, 0, 0));
            listenerOrientation[0] = glm::sin(theta);
            listenerOrientation[2] = -glm::cos(theta);
            OPENALCALL(alListenerfv(AL_ORIENTATION, listenerOrientation));
        }
        static void printPos() {
            std::cout << "(" << listenerPosition.x << ", " << listenerPosition.z << ", " << listenerPosition.y << "); (" << listenerPosition.x + listenerOrientation[0] << ", " << listenerPosition.z << ", " << listenerPosition.y + listenerOrientation[2] << ")";// << std::endl;
        }
       /* static void playSound(std::string name) {
            OPENALCALL(alSourcei(source, AL_BUFFER, ResourceManager::getSound(name)));
            OPENALCALL(alSourcePlay(source));
        }
        static void playSound2D(std::string name) {
            OPENALCALL(alSourcei(source, AL_BUFFER, ResourceManager::getSound(name)));
            OPENALCALL(alSourcePlay(source));
        }
        static void playSound3D(std::string name) {
            OPENALCALL(alSourcei(source, AL_BUFFER, ResourceManager::getSound(name)));
            OPENALCALL(alSourcePlay(source));
        }*/
        static void clear() {

        }
};