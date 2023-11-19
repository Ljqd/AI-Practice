#include <iostream>
#include <string>
#include <filesystem>
#include <opencv2/opencv.hpp>

#include "sources/ImageLoader.h"

int main()
{
    ImageLoader imageLoader = ImageLoader();
    imageLoader.setResolution(900, 900);
    imageLoader.setResizeType(ImageLoader::FIXED_RATIO);
    imageLoader.setDesiredRatio(16.0 / 9.0);

    cv::Mat image = imageLoader.getImage();

    cv::imshow("Your image.", image);
    cv::waitKey();
}

