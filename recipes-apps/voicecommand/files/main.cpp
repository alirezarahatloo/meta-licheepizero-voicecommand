#include "wav_reader.h"
#include "mfcc.h"

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: voicecommand <wav file>" << std::endl;
        return 1;
    }

    WavReader reader;

    if (!reader.Open(argv[1]))
    {
        std::cerr << "Failed to open WAV file." << std::endl;
        return 1;
    }

    std::cout << "Sample Rate : "
              << reader.GetSampleRate() << std::endl;

    std::cout << "Channels    : "
              << reader.GetChannels() << std::endl;

    std::cout << "Samples     : "
              << reader.GetNumSamples() << std::endl;

    std::cout << "Duration    : "
              << (float)reader.GetNumSamples() /
                     reader.GetSampleRate() /
                     reader.GetChannels()
              << " sec" << std::endl;

              MFCC mfcc;

    if(!mfcc.Process(reader.GetSamples(),
                 reader.GetSampleRate()))
    {
        return 1;
    }

    return 0;
}
