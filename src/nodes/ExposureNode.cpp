#include "ExposureNode.h"
#include <imgui.h>
#include <imnodes.h>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <GL/gl.h>

ExposureNode::ExposureNode() {
    name = "Exposure";
    position = ImVec2(100, 100); // Optional positioning
    textureID = 0;
    textureValid = false;
    exposureValue = 0; // Default Exposure Value

    id = Node::generateUniqueId();
    inputs.push_back({generateUniqueId(), "In"});
    outputs.push_back({generateUniqueId(), "Out"});
}

ExposureNode::~ExposureNode() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void ExposureNode::setInputImage(const cv::Mat &img) {
    inputImage = img.clone();
    textureValid = false;  // Mark texture as invalid
}

cv::Mat ExposureNode::getImage() const {
    return outputImage;
}

void ExposureNode::resetInput() {
    inputImage.release();
    textureValid = false;
}

std::string ExposureNode::getName() const {
    return name;
}

void ExposureNode::process() {
    updateTexture();
}

void ExposureNode::renderUI() {
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

    ImGui::SliderFloat("Exposure Value", &exposureValue, -100.0f, 100.0f);

    if (ImGui::Button("Process")) {
        if (inputImage.empty()) {
            std::cerr << "[ExposureNode] No input image.\n";
            return;
        }
        cv::Mat tempImage;
        inputImage.convertTo(tempImage, CV_32F, 1.0 / 255.0); // Convert to float
        tempImage += cv::Scalar(exposureValue, exposureValue, exposureValue); // Apply exposure
        cv::normalize(tempImage, outputImage, 0, 255, cv::NORM_MINMAX);
        outputImage.convertTo(outputImage, CV_8U); // Convert back to 8-bit
        if (outputImage.empty()) {
            std::cerr << "[ExposureNode] Output image is empty after processing.\n";
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

void ExposureNode::updateTexture() {
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