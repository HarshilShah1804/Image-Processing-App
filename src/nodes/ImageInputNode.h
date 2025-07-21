#pragma once
#include "Node.h"
#include <opencv2/core.hpp>
#include <string>
#include <GL/gl.h>

class ImageInputNode : public Node {
public:
    ImageInputNode();
    ~ImageInputNode();

    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;

    void loadImage(const std::string& path);
    void setInputImage(const cv::Mat& img) override;
    void resetInput() override;

private:
    std::string filepath;
    char inputBuffer[256];
    cv::Mat image;
    GLuint textureID;
    bool textureValid;
};
