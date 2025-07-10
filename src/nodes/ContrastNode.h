#pragma once
#include "Node.h"
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <string>

class ContrastNode : public Node {
public:
    ContrastNode();
    ~ContrastNode();
    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& input);
    void setContrastFactor(double factor);
    double getContrastFactor() const;
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
    double contrastFactor; // Factor to adjust contrast, typically in the range [0.0    , 3.0]
};