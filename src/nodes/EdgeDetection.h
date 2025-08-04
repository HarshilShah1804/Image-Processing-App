#pragma once
#include "Node.h"
#include <opencv2/core.hpp>
#include <string>
#include <GL/gl.h>

class EdgeDetectionNode : public Node {
public:
    EdgeDetectionNode();
    ~EdgeDetectionNode();

    void renderUI() override;
    void process() override;
    std::string getName() const override;
    cv::Mat getImage() const override;

    void loadImage(const std::string& path);
    void setInputImage(const cv::Mat& img) override;
    void resetInput() override;

    int edgeType; // Edge detection type (0: "Canny", 1: "Sobel", 2: "Laplacian")
    void setEdgeType(int type) { edgeType = type; }
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    GLuint textureID;
    bool textureValid;
    void updateTexture();
};
