#include "inference.h"
#include "model_weights.h"

#include <cmath>
#include <algorithm>

VoiceCommandClassifier::VoiceCommandClassifier()
    : weightsLoaded(false)
{
}

bool VoiceCommandClassifier::LoadWeights()
{
    weightsLoaded = true;
    return true;
}

float VoiceCommandClassifier::Relu(float x) const
{
    return x > 0.0f ? x : 0.0f;
}

void VoiceCommandClassifier::Softmax(std::vector<float>& logits) const
{
    float maxVal = *std::max_element(logits.begin(), logits.end());
    float sum = 0.0f;
    for (auto& v : logits)
    {
        v = std::exp(v - maxVal);
        sum += v;
    }
    for (auto& v : logits)
    {
        v /= sum;
    }
}

std::pair<int, float> VoiceCommandClassifier::Classify(
    const std::vector<float>& mfccFlat)
{
    if (!weightsLoaded)
    {
        return {-1, 0.0f};
    }

    const float* input = mfccFlat.data();
    int inputDim = mfccFlat.size();

    // Layer 0: Dense(64, ReLU): input_dim=1274, units=64
    std::vector<float> h1(DENSE0_UNITS, 0.0f);
    for (int j = 0; j < DENSE0_UNITS; j++)
    {
        float sum = b0[j];
        for (int i = 0; i < INPUT_DIM && i < inputDim; i++)
        {
            sum += input[i] * W0[i][j];
        }
        h1[j] = Relu(sum);
    }

    // Layer 1: Dense(2, Softmax): input_dim=64, units=2
    std::vector<float> logits(OUTPUT_DIM, 0.0f);
    for (int j = 0; j < OUTPUT_DIM; j++)
    {
        logits[j] = b1[j];
        for (int i = 0; i < DENSE0_UNITS; i++)
        {
            logits[j] += h1[i] * W1[i][j];
        }
    }

    // Softmax
    Softmax(logits);

    // Argmax
    int bestIdx = 0;
    float bestVal = logits[0];
    for (int i = 1; i < OUTPUT_DIM; i++)
    {
        if (logits[i] > bestVal)
        {
            bestVal = logits[i];
            bestIdx = i;
        }
    }

    return {bestIdx, bestVal};
}

std::string VoiceCommandClassifier::LabelName(int label)
{
    if (label >= 0 && label < OUTPUT_DIM)
    {
        return LABEL_NAMES[label];
    }
    return "unknown";
}
