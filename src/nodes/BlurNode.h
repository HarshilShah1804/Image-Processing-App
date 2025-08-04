#pragma once
#include "Node.h"
#include <opencv2/core.hpp>
#include <string>
#include <GL/gl.h>

class BlurNode : public Node {
public:
    BlurNode();
    ~BlurNode();

    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;

    void loadImage(const std::string& path);
    void setInputImage(const cv::Mat& img) override;
    void resetInput() override;

    // Node-specific properties
    int blurAmount = 0; // Blur intensity level
    void setBlurAmount(int value) { blurAmount = value; }

private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
    void updateTexture();
};
