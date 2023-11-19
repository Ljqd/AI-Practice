#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

#include "src/utils.h"

void updateImage(int, void* userData);
void fixSRGB(cv::Mat& image);

int main()
{
    // Remove all INFO logs
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    // our image 
    std::string path = "./resources/castle.jpg";
    cv::Mat originalImage = cv::imread(path, -1);
    if (originalImage.empty()) return -1;

    UserData* data = new UserData;
    data->originalImage = originalImage;
    data->fixedImageSRGB = originalImage.clone();
    
    fixSRGB(data->fixedImageSRGB);

    // output
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);

    createTaskbar(data);

    cv::Mat combined;
    cv::hconcat(data->originalImage, data->fixedImageSRGB, combined);
    cv::imshow(WINDOW_NAME, combined);
    cv::waitKey(0);
    cv::destroyWindow(WINDOW_NAME);
    return 0;
}

void fixSRGB(cv::Mat& image)
{
    // fix sRGB
    image.convertTo(image, CV_32F);
    image /= 255.0f;

    auto sRGB2Linear = [](cv::Vec3f& pixel, const int* position) -> void {
        for (int i = 0; i < 3; i++) {
            pixel[i] = (pixel[i] <= 0.04045f) ? (pixel[i] / 12.92f) : powf((pixel[i] + 0.055f) / 1.055f, 2.4f);
        }
    };

    image.forEach<cv::Vec3f>(sRGB2Linear);

    // convert the image back to 8-bit format
    image *= 255.0f;
    image.convertTo(image, CV_8UC3);
}


void updateImage(int, void* userData)
{
    UserData* data = reinterpret_cast<UserData*>(userData);

    double gammaValue = getGammaValue(cv::getTrackbarPos(GAMMA, WINDOW_NAME));
    float invGamma = 1 / gammaValue;

    cv::Mat table(1, 256, CV_8U);
    uchar* p = table.ptr();
    for (int i = 0; i < 256; ++i) 
    {
        p[i] = (uchar)(pow(i / 255.0, invGamma) * 255);
    }
    cv::Mat newImage1, newImage2;
    LUT(data->originalImage, table, newImage1);
    LUT(data->fixedImageSRGB, table, newImage2);

    cv::Mat combined;
    cv::hconcat(newImage1, newImage2, combined);
    cv::imshow(WINDOW_NAME, combined);
}