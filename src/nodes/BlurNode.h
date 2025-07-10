#pragma once
#include "Node.h"
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <string>

class BlurNode : public Node {
public:
    BlurNode();
    ~BlurNode();
    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& input);
    void setBlurSize(int size);
    int getBlurSize() const;
    void setBlurType(int type);
    int getBlurType() const;
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
    int blurSize;
    int blurType; // 0 for Gaussian, 1 for Median, 2 for
    // Bilateral
};