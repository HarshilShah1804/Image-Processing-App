// Node.h
#pragma once
#include <opencv2/opencv.hpp>
#include <imgui.h>
#include <GL/gl.h>

class Node {
public:
    virtual void renderUI() = 0;
    virtual void process() = 0;
    virtual std::string getName() const = 0;

    // Optional: use this for future dragging or layout
    ImVec2 position = ImVec2(100, 100);

protected:
    GLuint textureID = 0;
    void updateTexture(const cv::Mat& image);
};
