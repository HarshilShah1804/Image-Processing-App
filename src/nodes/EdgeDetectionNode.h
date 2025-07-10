#pragma once
#include "Node.h"
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <string>

class EdgeDetectionNode : public Node {
public:
    EdgeDetectionNode();
    ~EdgeDetectionNode();
    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;
    void setInputImage(const cv::Mat& input);
    void setEdgeDetectionMethod(int method);
    int getEdgeDetectionMethod() const;
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
    int edgeDetectionMethod; // 0 for Canny, 1 for Sobel,
    // 2 for Laplacian
};