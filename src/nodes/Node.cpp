// Node.cpp
#include "Node.h"

void Node::updateTexture(const cv::Mat& image) {
    if (image.empty()) return;

    if (textureID == 0) {
        glGenTextures(1, &textureID);
    }

    cv::Mat rgba;
    if (image.channels() == 1)
        cv::cvtColor(image, rgba, cv::COLOR_GRAY2RGBA);
    else
        cv::cvtColor(image, rgba, cv::COLOR_BGR2RGBA);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba.cols, rgba.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba.ptr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
