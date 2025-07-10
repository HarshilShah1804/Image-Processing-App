#pragma once
#include "Node.h"
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <string>

class SharpenNode : public Node {
public:
    SharpenNode();
    ~SharpenNode();
    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& input);
    void setSharpeningKernel(const cv::Mat& kernel);
    cv::Mat getSharpeningKernel() const;

private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    cv::Mat sharpeningKernel;
    GLuint textureID;
    bool textureValid;
};