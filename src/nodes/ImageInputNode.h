#pragma once
#include "Node.h"
#include <opencv2/opencv.hpp>
#include <GL/gl.h>
#include <string>

class ImageInputNode : public Node {
public:
    ImageInputNode();
    ~ImageInputNode();

    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void loadImage(const std::string& path);

private:
    std::string filepath;
    cv::Mat image;
    GLuint textureID;
    bool textureValid;
    char inputBuffer[512];
};
