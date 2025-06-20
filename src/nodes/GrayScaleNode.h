#pragma once
#include "Node.h"
#include <GL/gl.h>

class GrayScaleNode : public Node {
public:
    GrayScaleNode();
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
