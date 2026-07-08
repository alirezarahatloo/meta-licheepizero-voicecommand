#ifndef MFCC_H
#define MFCC_H

#include <vector>

class MFCC
{
public:
    bool Process(const std::vector<float>& samples,
                 int sampleRate);

private:
    static constexpr int FRAME_SIZE = 400;   // 25 ms @16kHz
    static constexpr int FRAME_STEP = 160;   // 10 ms

    std::vector<std::vector<float>> frames;

    void CreateFrames(const std::vector<float>& samples);
    void ApplyHammingWindow();
};

#endif