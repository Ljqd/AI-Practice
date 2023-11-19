#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

static const std::string WINDOW_NAME = "Gamma correction";
static const std::string GAMMA = "Gamma";

static const int GAMMA_TRACKBAR_COUNT = 100;

static const double GAMMA_MAX_VALUE = 10.0;
static const double GAMMA_MIN_VALUE = 0.1;
static const double GAMMA_DEFAULT_VALUE = 2.2;

static int GAMMA_DEFAULT_COUNT = static_cast<int>(
    ((GAMMA_DEFAULT_VALUE - GAMMA_MIN_VALUE) / (GAMMA_MAX_VALUE - GAMMA_MIN_VALUE)) * GAMMA_TRACKBAR_COUNT
    );


void updateImage(int, void* userData);

// we need this structure, because otherwise we will allocate new channels in each update()
struct UserData
{
    cv::Mat originalImage;
    cv::Mat fixedImageSRGB;
};

void createTaskbar(UserData* dataPtr)
{
    cv::createTrackbar(GAMMA, WINDOW_NAME, NULL, 100, updateImage, dataPtr);
    cv::setTrackbarPos(GAMMA, WINDOW_NAME, GAMMA_DEFAULT_COUNT);
}

double getGammaValue(int gammaTrackbarValue)
{
    return (GAMMA_MAX_VALUE - GAMMA_MIN_VALUE) * ((double)gammaTrackbarValue / GAMMA_TRACKBAR_COUNT) + GAMMA_MIN_VALUE;
}