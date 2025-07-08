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

    // Called by the graph to pass data into this node
    void setInputImage(const cv::Mat &img);
    void loadImage();
    void saveImage(const std::string &path);

private:
    std::string filepath;
    cv::Mat image;
    GLuint textureID;
    bool textureValid;
    char inputBuffer[512];
    std::string saveStatus;

};
