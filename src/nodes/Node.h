#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <imgui.h>

class Node {
public:
    virtual ~Node();  // Declare, but not define here

    virtual void renderUI() = 0;
    virtual void process() = 0;
    virtual std::string getName() const = 0;
    virtual cv::Mat getImage() const = 0;
    int id = -1;

protected:
    std::string name;
    ImVec2 position = ImVec2(0, 0);
};
