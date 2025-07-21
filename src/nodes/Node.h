#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <imgui.h>
#include <vector>

struct Pin {
    int id;
    std::string label;
};

class Node {
public:
    virtual ~Node() = default;

    virtual void renderUI() = 0;
    virtual void process() = 0;
    virtual std::string getName() const = 0;
    virtual cv::Mat getImage() const = 0;

    virtual void setInputImage(const cv::Mat& img) = 0;
    virtual void resetInput() = 0;

    int id = -1;
    ImVec2 position = ImVec2(0, 0);

    std::vector<Pin> inputs;
    std::vector<Pin> outputs;

    const std::vector<Pin>& getInputs() const { return inputs; }
    const std::vector<Pin>& getOutputs() const { return outputs; }

protected:
    std::string name;
    static int generateUniqueId();  // Declaration only
};
