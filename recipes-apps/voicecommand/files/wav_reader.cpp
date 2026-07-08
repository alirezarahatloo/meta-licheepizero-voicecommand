#include "wav_reader.h"

#include <sndfile.h>
#include <iostream>

bool WavReader::Open(const std::string& filename)
{
    SF_INFO info{};
    SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &info);

    if (!file)
    {
        std::cerr << "Cannot open "
                  << filename
                  << std::endl;

        return false;
    }

    sampleRate = info.samplerate;
    channels   = info.channels;

    samples.resize(info.frames * channels);

    sf_read_float(file,
                  samples.data(),
                  samples.size());

    sf_close(file);

    return true;
}

int WavReader::GetSampleRate() const
{
    return sampleRate;
}

int WavReader::GetChannels() const
{
    return channels;
}

int WavReader::GetNumSamples() const
{
    return samples.size();
}

const std::vector<float>&
WavReader::GetSamples() const
{
    return samples;
}