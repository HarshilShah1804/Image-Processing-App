#pragma once
#include "Node.h"
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <string>

class ExposureNode : public Node {
public:
    ExposureNode();
    ~ExposureNode();
    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& input);
    void setExposureFactor(double factor);
    double getExposureFactor() const;
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
    double exposureFactor; // Factor to adjust exposure, typically in the range [0.0, 3.0]
};