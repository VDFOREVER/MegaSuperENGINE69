#pragma once

#include <soloud/include/soloud.h>
#include <soloud/include/soloud_wav.h>
#include <string>

class Audio {
    public:
        Audio();
        ~Audio();
        SoLoud::handle play(std::string path);
        void stop(SoLoud::handle handle);
        void stopAll();

    private:
        SoLoud::Soloud gSoloud;
        SoLoud::Wav gWave;
};  