#pragma once

#include <AL/al.h>
#include <vector>

#define SE_LOAD SoundEffectsLibrary::Get()->Load
#define SE_UNLOAD SoundEffectsLibrary::Get()->UnLoad

class SoundEffectsLibrary{
    public:
        static SoundEffectsLibrary* Get();

        ALuint Load(const char* filename);
        bool UnLoad(const ALuint& bufferId);

    private:
        SoundEffectsLibrary();
        ~SoundEffectsLibrary();

        std::vector<ALuint> p_SoundEffectBuffers;
};
