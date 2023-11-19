#include "LinearTransformation.h"

void updateImage(int, void* userData);

const std::string LinearTransformation::CONTRAST = "Contrast";
const std::string LinearTransformation::BRIGHTNESS = "Brightness";

const int LinearTransformation::CONTRAST_TRACKBAR_COUNT = 100;
const int LinearTransformation::BRIGHTNESS_TRACKBAR_COUNT = 100;

const double LinearTransformation::CONTRAST_MAX_FACTOR = 3.0;
const double LinearTransformation::BRIGHTNESS_MAX_VALUE = 100.0;
const double LinearTransformation::BRIGHTNESS_MIN_VALUE = -100.0;

const double LinearTransformation::CONTRAST_DEFAULT_FACTOR = 1.0;
const double LinearTransformation::BRIGHTNESS_DEFAULT_VALUE = 0.0;

const int LinearTransformation::CONTRAST_DEFAULT_COUNT = static_cast<int>(
    (CONTRAST_DEFAULT_FACTOR / CONTRAST_MAX_FACTOR) * CONTRAST_TRACKBAR_COUNT
    );
const int LinearTransformation::BRIGHTNESS_DEFAULT_COUNT = static_cast<int>(
    ((BRIGHTNESS_DEFAULT_VALUE - BRIGHTNESS_MIN_VALUE) / (BRIGHTNESS_MAX_VALUE - BRIGHTNESS_MIN_VALUE)) * BRIGHTNESS_TRACKBAR_COUNT
    );

void LinearTransformation::createTrackbar(const std::string& windowName, UserData* dataPtr)
{
    for (int i = 0; i < 3; i++)
    {
        std::string contrastLabel = getTrackbarString(CONTRAST, i);
        std::string brightnessLabel = getTrackbarString(BRIGHTNESS, i);

        cv::createTrackbar(contrastLabel, windowName, NULL, CONTRAST_TRACKBAR_COUNT, updateImage, dataPtr);
        cv::createTrackbar(brightnessLabel, windowName, NULL, BRIGHTNESS_TRACKBAR_COUNT, updateImage, dataPtr);
    }    
}

void LinearTransformation::setTrackbarPos(const std::string& windowName, UserData* dataPtr)
{
    for (int i = 0; i < 3; i++)
    {
        std::string contrastLabel = getTrackbarString(CONTRAST, i);
        std::string brightnessLabel = getTrackbarString(BRIGHTNESS, i);

        cv::setTrackbarPos(contrastLabel, windowName, CONTRAST_DEFAULT_COUNT);
        cv::setTrackbarPos(brightnessLabel, windowName, BRIGHTNESS_DEFAULT_COUNT);
    }
}

std::string LinearTransformation::getTrackbarString(const std::string& prefix, int i)
{
    char color = i == 0 ? 'B' : i == 1 ? 'G' : 'R';
    return prefix + " " + color;
}

double LinearTransformation::getContrastFactor(int contrastTrackbarValue)
{
    return CONTRAST_MAX_FACTOR * ((double)contrastTrackbarValue / CONTRAST_TRACKBAR_COUNT);
}

double LinearTransformation::getBrightnessValue(int brightnessTrackbarValue)
{
    return (BRIGHTNESS_MAX_VALUE - BRIGHTNESS_MIN_VALUE) * ((double)brightnessTrackbarValue / BRIGHTNESS_TRACKBAR_COUNT) + BRIGHTNESS_MIN_VALUE;
}

void LinearTransformation::apply(const std::string& windowName, cv::Mat& image)
{
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    for (int i = 0; i < 3; i++)
    {
        std::string contrastLabel = getTrackbarString(CONTRAST, i);
        std::string brightnessLabel = getTrackbarString(BRIGHTNESS, i);

        double contrastFactor = getContrastFactor(cv::getTrackbarPos(contrastLabel, windowName));
        double brightnessValue = getBrightnessValue(cv::getTrackbarPos(brightnessLabel, windowName));

        channels[i].convertTo(channels[i], -1, contrastFactor, brightnessValue);
    }
    cv::merge(channels, image);
}