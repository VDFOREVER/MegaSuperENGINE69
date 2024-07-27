#include <audio/soundDevice.hpp>
#include <log.hpp>
#include <vector>

static SoundDevice* _instance = nullptr;

SoundDevice::SoundDevice() {
    p_Device = alcOpenDevice(nullptr);
    if (!p_Device)
        throw std::runtime_error("failed to get sound device");

    p_Context = alcCreateContext(p_Device, nullptr);
    if (!p_Context)
        throw std::runtime_error("failed to get sound context");

    if(!alcMakeContextCurrent(p_Context))
        throw std::runtime_error("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_Device, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_Device, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_Device) != AL_NO_ERROR)
		name = alcGetString(p_Device, ALC_DEVICE_SPECIFIER);

    LOG_INFO("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice() {
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_Context);
	alcCloseDevice(p_Device);
}

void SoundDevice::Init() {
	if (_instance == nullptr)
		_instance = new SoundDevice();
}

SoundDevice* SoundDevice::Get(){
	Init();
	return _instance;
}

void SoundDevice::SetOrientation(const float& atx, const float& aty, const float& atz, const float& upx, const float& upy, const float& upz) {
	std::vector<float> ori;
	ori.push_back(atx);
	ori.push_back(aty);
	ori.push_back(atz);
	ori.push_back(upx);
	ori.push_back(upy);
	ori.push_back(upz);
	alListenerfv(AL_ORIENTATION, ori.data());
}

void SoundDevice::SetLocation(const float& x, const float& y, const float& z) {
	alListener3f(AL_POSITION, x, y, z);
}

void SoundDevice::SetAttunation(int key) {
	if (key < 0xD001 || key > 0xD006)
		throw("bad attunation key");

	alListeneri(AL_DISTANCE_MODEL, key);
}

float SoundDevice::GetGain() {
	float curr_gain;
	alGetListenerf(AL_GAIN, &curr_gain);
	return curr_gain;
}

void SoundDevice::GetOrientation(float& ori) {
	alGetListenerfv(AL_ORIENTATION, &ori);
}

void SoundDevice::GetLocation(float& x, float& y, float& z) {
	alGetListener3f(AL_POSITION, &x, &y, &z);
}