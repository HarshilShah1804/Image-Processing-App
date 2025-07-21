#include "GrayScaleNode.h"
#include <opencv2/imgproc.hpp>
#include <GL/gl.h>
#include <iostream>
#include <imgui.h>
#include <imnodes.h>

GrayScaleNode::GrayScaleNode() {
    name = "GrayScale";
    position = ImVec2(300, 100); // Optional positioning
    textureID = 0;
    textureValid = false;

    id = Node::generateUniqueId();
    inputs.push_back({generateUniqueId(), "In"});
    outputs.push_back({generateUniqueId(), "Out"});
}

void GrayScaleNode::renderUI() {
    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("Grayscale");
    ImNodes::EndNodeTitleBar();

    if (!inputs.empty()) {
        ImNodes::BeginInputAttribute(inputs[0].id);
        ImGui::Text("In");
        ImNodes::EndInputAttribute();
    }

    if (!outputs.empty()) {
        ImNodes::BeginOutputAttribute(outputs[0].id);
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();
    }

    if (ImGui::Button("Process")) {
        if (inputImage.empty()) {
            std::cerr << "[GrayScaleNode] No input image.\n";
            return;
        }
        loadImage();
    }

    if (textureValid && textureID != 0) {
        ImGui::Text("Preview:");
        ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(128, 128));
    }

    ImNodes::EndNode();
}

void GrayScaleNode::loadImage() {
    if (inputImage.empty()) {
        std::cerr << "[GrayScaleNode] No input image.\n";
        textureValid = false;
        return;
    }

    if (textureID != 0) {
        std::cout << textureID << " deleted.\n";
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    cv::cvtColor(inputImage, outputImage, cv::COLOR_BGR2GRAY);

    cv::Mat displayImage;
    if (inputImage.channels() == 1)
        cv::cvtColor(inputImage, displayImage, cv::COLOR_GRAY2RGBA);
    else if (inputImage.channels() == 3)
        cv::cvtColor(inputImage, displayImage, cv::COLOR_BGR2RGBA);
    else if (inputImage.channels() == 4)
        displayImage = inputImage.clone();
    else
        return;
    cv::flip(displayImage, displayImage, 0);

    // Upload as OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, displayImage.cols, displayImage.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, displayImage.ptr());
    glBindTexture(GL_TEXTURE_2D, 0);

    textureValid = true;
}

std::string GrayScaleNode::getName() const {
    return name;
}

cv::Mat GrayScaleNode::getImage() const {
    return outputImage;
}

void GrayScaleNode::setInputImage(const cv::Mat& input) {
    inputImage = input.clone();
}

void GrayScaleNode::resetInput() {
    inputImage.release();
    textureValid = false;
}

void GrayScaleNode::process() {
    
}
