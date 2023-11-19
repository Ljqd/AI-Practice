#pragma once

#include <filesystem>
#include <string>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

// The idea of this class is to provide a way of effortless image loading
// Builder pattern: set prerocessing, colorscheme, etc and then call your getImage()
class ImageLoader
{
public:
    enum ResizeType
    {
        BY_WIDTH,   // (newWidth, newHeight) = (width, imageHeight * width / imageWidth)
        BY_HEIGHT,
        HARD,       // (newWidth, newHeight) = (width, height)
        FIXED_RATIO // (newWidth, newHeight); IF (imageWidth / imageHeight > desiredRatio) => (width, width * desiredRatio)
    };

    ImageLoader(
        const std::string& imagesPath = "./resources",
        size_t width = 900, size_t height = 900, float ratio = 16.0 / 9.0,
        ResizeType resizeType = ResizeType::FIXED_RATIO
    );

    cv::Mat getImage();
    
    void setResolution(size_t newWidth, size_t newHeight);    

    void setResizeType(ResizeType newResizeType);
    void setDesiredRatio(float newRatio);

private:
    cv::Mat getImageMat();
    cv::Mat updateResolution(cv::Mat& image);

    fs::path imagesPath;

    size_t width, height;
    float desiredRatio;
    ResizeType resizeType;
};