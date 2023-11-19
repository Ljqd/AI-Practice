#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>

#include "src/UserData.h"
#include "src/GammaTransformation.h"
#include "src/LinearTransformation.h"


static const std::string WINDOW_NAME = "Color balance";

void updateImage(int, void* userData);

int main(int argc, char** argv)
{
    // Remove all INFO logs
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    // our image 
    std::string path = cv::samples::findFile("starry_night.jpg");
    cv::Mat originalImage = cv::imread(path, -1);
    if (originalImage.empty()) return -1;

    std::cout << "Gamma correction: before|after|without (1, 2, 0): ";
    int gammaFlag;
    std::cin >> gammaFlag;

    UserData* data = new UserData;
    data->originalImage = originalImage;
    data->currentModifiedImage = originalImage.clone();
    data->gammaFlag = (UserData::GammaFlag)gammaFlag;

    // output
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);

    LinearTransformation::createTrackbar(WINDOW_NAME, data);
    GammaTransformation::createTrackbar(WINDOW_NAME, data);

    LinearTransformation::setTrackbarPos(WINDOW_NAME, data);
    GammaTransformation::setTrackbarPos(WINDOW_NAME, data);
    // cv::createButton("button", callbackButton, NULL, cv::QT_CHECKBOX, 0);

    cv::imshow(WINDOW_NAME, data->currentModifiedImage);
    cv::waitKey(0);
    cv::destroyWindow(WINDOW_NAME);
    return 0;
}


void updateImage(int, void* userData)
{
    UserData* data = reinterpret_cast<UserData*>(userData);

    data->currentModifiedImage = data->originalImage.clone();

    if (data->gammaFlag == UserData::BEFORE)
    {
        GammaTransformation::apply(WINDOW_NAME, data->currentModifiedImage);
        LinearTransformation::apply(WINDOW_NAME, data->currentModifiedImage);
    }
    else if (data->gammaFlag == UserData::AFTER)
    {
        LinearTransformation::apply(WINDOW_NAME, data->currentModifiedImage);
        GammaTransformation::apply(WINDOW_NAME, data->currentModifiedImage);
    }
    else
    {
        LinearTransformation::apply(WINDOW_NAME, data->currentModifiedImage);
    }
    cv::imshow(WINDOW_NAME, data->currentModifiedImage);
}