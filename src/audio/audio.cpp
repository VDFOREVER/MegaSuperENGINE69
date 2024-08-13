#include <audio/audio.hpp>

Audio::Audio() {
    gSoloud.init();
}

Audio::~Audio() {
    gSoloud.deinit();
}

SoLoud::handle Audio::play(std::string path) {
    gWave.load(path.c_str()); 
    return gSoloud.play(gWave);
}

void Audio::stop(SoLoud::handle handle) {
    gSoloud.stop(handle);
}

void Audio::stopAll() {
    gSoloud.stopAll();
}