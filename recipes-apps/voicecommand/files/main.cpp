#include "wav_reader.h"
#include "mfcc.h"
#include "inference.h"

#include <iostream>
#include <iomanip>

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        std::cout << "Usage: voicecommand <wav file> [--print-mfcc]" << std::endl;
        return 1;
    }

    bool printMfcc = (argc == 3 && std::string(argv[2]) == "--print-mfcc");

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

    if (!mfcc.Process(reader.GetSamples(),
                      reader.GetSampleRate()))
    {
        return 1;
    }

    const auto& coeffs = mfcc.GetCoeffs();

    std::cout << "Frame count : "
              << coeffs.size()
              << std::endl;

    if (printMfcc)
    {
        std::cout << std::fixed << std::setprecision(2);

        for (size_t f = 0; f < coeffs.size(); f++)
        {
            std::cout << "MFCC[" << f << "]:";
            for (int c = 0; c < 13; c++)
            {
                std::cout << " " << std::setw(7) << coeffs[f][c];
            }
            std::cout << std::endl;
        }
    }

    // Flatten MFCC coefficients to 1D vector and classify
    std::vector<float> mfccFlat;
    mfccFlat.reserve(coeffs.size() * 13);
    for (const auto& frame : coeffs)
    {
        for (float val : frame)
        {
            mfccFlat.push_back(val);
        }
    }

    VoiceCommandClassifier classifier;
    classifier.LoadWeights();
    auto result = classifier.Classify(mfccFlat);
    std::cout << "Predicted   : "
              << VoiceCommandClassifier::LabelName(result.first)
              << " (" << std::fixed << std::setprecision(2) << result.second << ")"
              << std::endl;

    return 0;
}