#pragma once
#include "Node.h"
#include <opencv2/core.hpp>
#include <GL/gl.h>

class GrayScaleNode : public Node {
public:
    GrayScaleNode();
    void process() override;
    void renderUI() override;
    void loadImage();
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& input) override;
    void resetInput() override;

private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
};
