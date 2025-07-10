#pragma once
#include "Node.h"
#include <GL/gl.h>

class BrightnessNode : public Node {
public:
    brightnessNode();
    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;

    void setInputImage(const cv::Mat& input);

private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
};
