#include "GammaTransformation.h"

void updateImage(int, void* userData);

const std::string GammaTransformation::GAMMA = "Gamma";

const int GammaTransformation::GAMMA_TRACKBAR_COUNT = 100;

const double GammaTransformation::GAMMA_MAX_VALUE = 10.0;
const double GammaTransformation::GAMMA_MIN_VALUE = 0.1;
const double GammaTransformation::GAMMA_DEFAULT_VALUE = 2.2;

const int GammaTransformation::GAMMA_DEFAULT_COUNT = static_cast<int>(
    ((GAMMA_DEFAULT_VALUE - GAMMA_MIN_VALUE) / (GAMMA_MAX_VALUE - GAMMA_MIN_VALUE)) * GAMMA_TRACKBAR_COUNT
    );

void GammaTransformation::createTrackbar(const std::string& windowName, UserData* dataPtr)
{
    cv::createTrackbar(GAMMA, windowName, NULL, 100, updateImage, dataPtr);
}

void GammaTransformation::setTrackbarPos(const std::string& windowName, UserData* dataPtr)
{
    cv::setTrackbarPos(GAMMA, windowName, GAMMA_DEFAULT_COUNT);
}

double GammaTransformation::getGammaValue(int gammaTrackbarValue)
{
    return (GAMMA_MAX_VALUE - GAMMA_MIN_VALUE) * ((double)gammaTrackbarValue / GAMMA_TRACKBAR_COUNT) + GAMMA_MIN_VALUE;
}

void GammaTransformation::apply(const std::string& windowName, cv::Mat& image)
{
    double gammaValue = getGammaValue(cv::getTrackbarPos(GAMMA, windowName));
    float invGamma = 1 / gammaValue;

    cv::Mat table(1, 256, CV_8U);
    uchar* p = table.ptr();
    for (int i = 0; i < 256; ++i)
    {
        p[i] = (uchar)(pow(i / 255.0, invGamma) * 255);
    }
    LUT(image, table, image);
}