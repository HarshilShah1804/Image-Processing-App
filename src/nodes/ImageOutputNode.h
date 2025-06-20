#pragma once
#include "Node.h"
#include <opencv2/opencv.hpp>
#include <GL/gl.h>
#include <string>

class ImageOutputNode : public Node {
public:
    ImageOutputNode();
    ~ImageOutputNode();

    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& input);
    void saveImage(const std::string& path);

private:
    std::string filepath;
    cv::Mat image;
    cv::Mat inputImage;
    GLuint textureID;
    bool textureValid;
    bool imageChanged;
    char inputBuffer[512];
    std::string saveStatus;

    void updateTextureFromImage();
};
