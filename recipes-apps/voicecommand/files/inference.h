#ifndef INFERENCE_H
#define INFERENCE_H

#include <string>
#include <vector>

class VoiceCommandClassifier
{
public:
    VoiceCommandClassifier();
    bool LoadWeights();
    std::pair<int, float> Classify(const std::vector<float>& mfccFlat);
    static std::string LabelName(int label);

private:
    float Relu(float x) const;
    void Softmax(std::vector<float>& logits) const;
    bool weightsLoaded;
};

#endif
