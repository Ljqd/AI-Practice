#pragma once

#include <string>

#include "UserData.h"

class LinearTransformation
{
    static const std::string CONTRAST;
    static const std::string BRIGHTNESS;
    static const std::string WINDOW_NAME;

    static const int CONTRAST_TRACKBAR_COUNT;
    static const int BRIGHTNESS_TRACKBAR_COUNT;

    static const double CONTRAST_MAX_FACTOR;
    static const double BRIGHTNESS_MAX_VALUE;
    static const double BRIGHTNESS_MIN_VALUE;

    static const double CONTRAST_DEFAULT_FACTOR;
    static const double BRIGHTNESS_DEFAULT_VALUE;

    static const int CONTRAST_DEFAULT_COUNT;
    static const int BRIGHTNESS_DEFAULT_COUNT;
public:
    static void createTrackbar(const std::string& windowName, UserData* dataPtr);
    static void setTrackbarPos(const std::string& windowName, UserData* dataPtr);

    static double getContrastFactor(int contrastTrackbarValue);
    static double getBrightnessValue(int brightnessTrackbarValue);

    static void apply(const std::string& windowName, cv::Mat& image);

private:
    static std::string getTrackbarString(const std::string& prefix, int i);
};