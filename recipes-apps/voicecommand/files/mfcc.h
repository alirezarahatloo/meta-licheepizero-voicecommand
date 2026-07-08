#ifndef MFCC_H
#define MFCC_H

#include <vector>

class MFCC
{
public:
    bool Process(const std::vector<float>& samples,
                 int sampleRate);

    const std::vector<std::vector<float>>& GetCoeffs() const
    {
        return mfcc;
    }

private:
    static constexpr int FRAME_SIZE = 400;
    static constexpr int FRAME_STEP = 160;
    static constexpr int FFT_SIZE = 512;
    static constexpr int MEL_FILTERS = 26;
    static constexpr int NUM_COEFFS = 13;
    static constexpr float PRE_EMPHASIS = 0.97f;
    static constexpr float MEL_MIN_HZ = 0.0f;
    static constexpr float MEL_MAX_HZ = 8000.0f;

    std::vector<std::vector<float>> frames;
    std::vector<std::vector<float>> mfcc;

    std::vector<float> melFilterbank[26];

    void PreEmphasis(std::vector<float>& samples);
    void CreateFrames(const std::vector<float>& samples);
    void ApplyHamming();
    void ComputeFFT(const std::vector<float>& frame,
                     std::vector<float>& real,
                     std::vector<float>& imag);
    void BuildMelFilterbank(int sampleRate);
    void ApplyMelFilterbank(const std::vector<float>& powerSpectrum,
                            std::vector<float>& melEnergy);
    void ComputeDCT(const std::vector<float>& melEnergy,
                    std::vector<float>& coeffs);
    float MelToHz(float mel) const;
    float HzToMel(float hz) const;
};

#endif