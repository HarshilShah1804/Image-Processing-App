#pragma once
#include "Node.h"
#include <opencv2/core.hpp>
#include <string>
#include <GL/gl.h>

class ImageOutputNode : public Node {
public:
    ImageOutputNode();
    ~ImageOutputNode();

    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& img) override;
    void resetInput() override;

private:
    void loadImage();
    void saveImage(const std::string& path);

    std::string filepath;
    char inputBuffer[256];
    std::string saveStatus;

    cv::Mat image;
    GLuint textureID;
    bool textureValid;

    void updateTexture();
    bool imageChanged;  // Flag to indicate if the image has changed
};
