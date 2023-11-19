#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

// we need this structure, because otherwise we will allocate new channels in each update()
struct UserData
{
    enum GammaFlag { BEFORE = 1, AFTER = 2, WITHOUT = 0 };

    cv::Mat originalImage;
    cv::Mat currentModifiedImage;
    GammaFlag gammaFlag;
};