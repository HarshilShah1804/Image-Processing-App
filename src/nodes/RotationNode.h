#pragma once
#include "Node.h"
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <string>

class RotationNode : public Node {
public:
    RotationNode();
    ~RotationNode();
    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& input);
    void setRotationAngle(double angle);
    double getRotationAngle() const;
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
    double rotationAngle; // Angle in degrees
};