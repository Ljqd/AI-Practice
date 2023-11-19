#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "userData.h"

class GammaTransformation
{
    static const std::string GAMMA;

    static const int GAMMA_TRACKBAR_COUNT;

    static const double GAMMA_MAX_VALUE;
    static const double GAMMA_MIN_VALUE;
    static const double GAMMA_DEFAULT_VALUE;

    static const int GAMMA_DEFAULT_COUNT;

public:
    static void createTrackbar(const std::string& windowName, UserData* dataPtr);
    static void setTrackbarPos(const std::string& windowName, UserData* dataPtr);

    static double getGammaValue(int gammaTrackbarValue);

    static void apply(const std::string& windowName, cv::Mat& image);
};