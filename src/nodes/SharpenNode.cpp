#include "SharpenNode.h"
#include <imgui.h>
#include <imnodes.h>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <GL/gl.h>

SharpenNode::SharpenNode() {
    name = "Sharpen";
    position = ImVec2(100, 100); // Optional positioning
    textureID = 0;
    textureValid = false;
    sharpenAmount = 0; // Default Sharpen Amount

    id = Node::generateUniqueId();
    inputs.push_back({generateUniqueId(), "In"});
    outputs.push_back({generateUniqueId(), "Out"});
}

SharpenNode::~SharpenNode() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void SharpenNode::setInputImage(const cv::Mat &img) {
    inputImage = img.clone();
    textureValid = false;  // Mark texture as invalid
}

cv::Mat SharpenNode::getImage() const {
    return outputImage;
}

void SharpenNode::resetInput() {
    inputImage.release();
    textureValid = false;
}

std::string SharpenNode::getName() const {
    return name;
}

void SharpenNode::process() {
    updateTexture();
}

void SharpenNode::renderUI() {
    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(name.c_str());
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

    ImGui::SliderInt("Sharpen Amount", &sharpenAmount, 0, 100);

    if (ImGui::Button("Process")) {
        if (inputImage.empty()) {
            std::cerr << "[SharpenNode] No input image.\n";
            return;
        }
        cv::Mat tempImage;
        cv::GaussianBlur(inputImage, tempImage, cv::Size(0, 0), sharpenAmount);
        cv::addWeighted(inputImage, 1.5, tempImage, -0.5, 0, outputImage);
        if (outputImage.empty()) {
            std::cerr << "[SharpenNode] Output image is empty after processing.\n";
            textureValid = false;
            return;
        }
        textureValid = true; // Mark texture as valid after processing
    }

    if (textureValid && textureID != 0) {
        ImGui::Text("Preview:");
        ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(128, 128));
    }

    ImNodes::EndNode();
}

void SharpenNode::updateTexture() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    if (outputImage.empty()) return;

    cv::Mat displayImage;
    if (outputImage.channels() == 1)
        cv::cvtColor(outputImage, displayImage, cv::COLOR_GRAY2RGBA);
    else if (outputImage.channels() == 3)
        cv::cvtColor(outputImage, displayImage, cv::COLOR_BGR2RGBA);
    else if (outputImage.channels() == 4)
        displayImage = outputImage.clone();
    else
        return; // Unsupported channel count

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, displayImage.cols, displayImage.rows,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, displayImage.data);

    textureValid = true; // Mark texture as valid after updating
}