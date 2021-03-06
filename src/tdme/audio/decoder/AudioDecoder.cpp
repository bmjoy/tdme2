#include <tdme/audio/decoder/AudioDecoder.h>

using tdme::audio::decoder::AudioDecoder;

constexpr int32_t AudioDecoder::CHANNELS_NONE;

constexpr int32_t AudioDecoder::SAMPLERATE_NONE;

constexpr int32_t AudioDecoder::BITSPERSAMPLES_NONE;

AudioDecoder::AudioDecoder()
{
	channels = AudioDecoder::CHANNELS_NONE;
	sampleRate = AudioDecoder::SAMPLERATE_NONE;
	bitsPerSample = AudioDecoder::BITSPERSAMPLES_NONE;
	samples = 0;
}

AudioDecoder::~AudioDecoder() {
}

int32_t AudioDecoder::getChannels()
{
	return channels;
}

int32_t AudioDecoder::getSampleRate()
{
	return sampleRate;
}

int32_t AudioDecoder::getBitsPerSample()
{
	return bitsPerSample;
}

int32_t AudioDecoder::getSamples()
{
	return samples;
}

