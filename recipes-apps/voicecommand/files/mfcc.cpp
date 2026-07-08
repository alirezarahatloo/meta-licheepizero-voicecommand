#include "mfcc.h"

#include <cmath>
#include <iostream>
#include <algorithm>

bool MFCC::Process(const std::vector<float>& samples,
                   int sampleRate)
{
    if (sampleRate != 16000)
    {
        std::cout << "Model expects 16000 Hz audio." << std::endl;
        return false;
    }

    std::vector<float> data = samples;
    PreEmphasis(data);
    CreateFrames(data);
    ApplyHamming();
    BuildMelFilterbank(sampleRate);

    mfcc.clear();

    for (const auto& frame : frames)
    {
        std::vector<float> real(FFT_SIZE, 0.0f);
        std::vector<float> imag(FFT_SIZE, 0.0f);

        std::copy(frame.begin(), frame.end(), real.begin());
        ComputeFFT(frame, real, imag);

        std::vector<float> powerSpectrum(FFT_SIZE / 2 + 1);
        for (int i = 0; i <= FFT_SIZE / 2; i++)
        {
            powerSpectrum[i] = real[i] * real[i] + imag[i] * imag[i];
        }

        std::vector<float> melEnergy(MEL_FILTERS, 0.0f);
        ApplyMelFilterbank(powerSpectrum, melEnergy);

        std::vector<float> logEnergy(MEL_FILTERS);
        for (int i = 0; i < MEL_FILTERS; i++)
        {
            logEnergy[i] = log10f(std::max(melEnergy[i], 1e-10f));
        }

        std::vector<float> coeffs(NUM_COEFFS, 0.0f);
        ComputeDCT(logEnergy, coeffs);

        mfcc.push_back(coeffs);
    }

    return true;
}

void MFCC::PreEmphasis(std::vector<float>& samples)
{
    for (size_t i = samples.size() - 1; i >= 1; i--)
    {
        samples[i] = samples[i] - PRE_EMPHASIS * samples[i - 1];
    }
}

void MFCC::CreateFrames(const std::vector<float>& samples)
{
    frames.clear();

    for (size_t start = 0;
         start + FRAME_SIZE <= samples.size();
         start += FRAME_STEP)
    {
        frames.emplace_back(samples.begin() + start,
                            samples.begin() + start + FRAME_SIZE);
    }
}

void MFCC::ApplyHamming()
{
    for (auto& frame : frames)
    {
        for (int i = 0; i < FRAME_SIZE; i++)
        {
            float w = 0.54f - 0.46f * cosf((2.0f * M_PI * i) / (FRAME_SIZE - 1));
            frame[i] *= w;
        }
    }
}

void MFCC::ComputeFFT(const std::vector<float>& frame,
                      std::vector<float>& real,
                      std::vector<float>& imag)
{
    int n = FFT_SIZE;
    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
        {
            std::swap(real[i], real[j]);
            std::swap(imag[i], imag[j]);
        }
    }

    for (int len = 2; len <= n; len <<= 1)
    {
        float ang = 2.0f * M_PI / len;
        float wreal = cosf(ang);
        float wimag = -sinf(ang);

        for (int i = 0; i < n; i += len)
        {
            float wr = 1.0f, wi = 0.0f;

            for (int j = 0; j < len / 2; j++)
            {
                float tr = wr * real[i + j + len / 2] - wi * imag[i + j + len / 2];
                float ti = wr * imag[i + j + len / 2] + wi * real[i + j + len / 2];

                real[i + j + len / 2] = real[i + j] - tr;
                imag[i + j + len / 2] = imag[i + j] - ti;
                real[i + j] += tr;
                imag[i + j] += ti;

                float nwr = wr * wreal - wi * wimag;
                wi = wr * wimag + wi * wreal;
                wr = nwr;
            }
        }
    }
}

float MFCC::HzToMel(float hz) const
{
    return 2595.0f * log10f(1.0f + hz / 700.0f);
}

float MFCC::MelToHz(float mel) const
{
    return 700.0f * (powf(10.0f, mel / 2595.0f) - 1.0f);
}

void MFCC::BuildMelFilterbank(int sampleRate)
{
    float melMin = HzToMel(MEL_MIN_HZ);
    float melMax = HzToMel(MEL_MAX_HZ);
    float melSpacing = (melMax - melMin) / (MEL_FILTERS + 1);

    std::vector<float> melCenters(MEL_FILTERS + 2);
    for (int i = 0; i < MEL_FILTERS + 2; i++)
    {
        melCenters[i] = MelToHz(melMin + i * melSpacing);
    }

    int numFftBins = FFT_SIZE / 2 + 1;
    std::vector<int> fftBins(MEL_FILTERS + 2);
    for (int i = 0; i < MEL_FILTERS + 2; i++)
    {
        fftBins[i] = static_cast<int>(
            floor((FFT_SIZE + 1) * melCenters[i] / sampleRate));
    }

    for (int m = 0; m < MEL_FILTERS; m++)
    {
        melFilterbank[m].resize(numFftBins, 0.0f);

        for (int k = fftBins[m]; k <= fftBins[m + 1]; k++)
        {
            melFilterbank[m][k] =
                (k - fftBins[m]) / (float)(fftBins[m + 1] - fftBins[m]);
        }

        for (int k = fftBins[m + 1]; k <= fftBins[m + 2]; k++)
        {
            melFilterbank[m][k] =
                (fftBins[m + 2] - k) / (float)(fftBins[m + 2] - fftBins[m + 1]);
        }
    }
}

void MFCC::ApplyMelFilterbank(
    const std::vector<float>& powerSpectrum,
    std::vector<float>& melEnergy)
{
    for (int m = 0; m < MEL_FILTERS; m++)
    {
        melEnergy[m] = 0.0f;
        for (size_t k = 0; k < powerSpectrum.size(); k++)
        {
            melEnergy[m] += powerSpectrum[k] * melFilterbank[m][k];
        }
    }
}

void MFCC::ComputeDCT(const std::vector<float>& melEnergy,
                      std::vector<float>& coeffs)
{
    int M = melEnergy.size();

    for (int i = 0; i < NUM_COEFFS; i++)
    {
        coeffs[i] = 0.0f;
        for (int j = 0; j < M; j++)
        {
            coeffs[i] += melEnergy[j] *
                         cosf((float)M_PI * i / M * (j + 0.5f));
        }
    }
}
