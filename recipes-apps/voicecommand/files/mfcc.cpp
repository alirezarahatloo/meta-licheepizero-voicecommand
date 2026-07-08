#include "mfcc.h"

#include <cmath>
#include <iostream>

bool MFCC::Process(const std::vector<float>& samples,
                   int sampleRate)
{
    if(sampleRate != 16000)
    {
        std::cout << "Model expects 16000 Hz audio." << std::endl;
        return false;
    }

    CreateFrames(samples);

    ApplyHammingWindow();

    std::cout << "Frame count : "
              << frames.size()
              << std::endl;

    return true;
}

void MFCC::CreateFrames(const std::vector<float>& samples)
{
    frames.clear();

    for(size_t start = 0;
        start + FRAME_SIZE <= samples.size();
        start += FRAME_STEP)
    {
        frames.emplace_back(samples.begin() + start,
                            samples.begin() + start + FRAME_SIZE);
    }
}

void MFCC::ApplyHammingWindow()
{
    for(auto &frame : frames)
    {
        for(int i = 0; i < FRAME_SIZE; i++)
        {
            float w =
                0.54f -
                0.46f *
                cosf((2.0f * M_PI * i) /
                     (FRAME_SIZE - 1));

            frame[i] *= w;
        }
    }
}