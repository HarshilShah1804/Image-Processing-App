#pragma once
#include "Node.h"
#include <opencv2/opencv.hpp>

class ImageInputNode : public Node {
public:
    ImageInputNode();
    void renderUI() override;
    void process() override;
    std::string getName() const override { return "Image Input"; }
    const cv::Mat& getImage() const { return image; }
// In ImageInputNode.h
private:
    char imagePathBuffer[256] = "";  // <-- char buffer
    cv::Mat image;
    bool loaded = false;

};
