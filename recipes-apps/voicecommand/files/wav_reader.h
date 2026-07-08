#ifndef WAV_READER_H
#define WAV_READER_H

#include <vector>
#include <string>

class WavReader
{
public:
    bool Open(const std::string& filename);

    int GetSampleRate() const;
    int GetChannels() const;
    int GetNumSamples() const;

    const std::vector<float>& GetSamples() const;

private:
    int sampleRate = 0;
    int channels = 0;

    std::vector<float> samples;
};

#endif