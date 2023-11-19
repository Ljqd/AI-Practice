#include "ImageLoader.h"

ImageLoader::ImageLoader(
    const std::string& imagesPath, size_t width, size_t height, float ratio, ResizeType resizeType
) : imagesPath(imagesPath), width(width), height(height), desiredRatio(ratio), resizeType(resizeType)
{}

cv::Mat ImageLoader::getImage()
{
    cv::Mat image = getImageMat();
    return updateResolution(image);
}

cv::Mat ImageLoader::getImageMat()
{
    // Print images filenames
    size_t imageIndex = 0;
    std::cout << "\tChose your image (enter number).\n";
    for (const auto& entry : fs::directory_iterator(imagesPath))
    {
        imageIndex += 1;
        std::cout << imageIndex << ". " << entry.path().filename() << "\n";
    }
    std::cout << "\tid: ";

    // Pick image
    size_t pickedImageIndex;
    std::cin >> pickedImageIndex;

    fs::directory_iterator iter(imagesPath);
    while (pickedImageIndex - 1 != 0)   // directory_iterator is an input iterator, not a random access.
    {
        pickedImageIndex--;
        iter++;
    }

    // Return image
    std::cout << std::endl;
    return cv::imread(iter->path().string());
}

cv::Mat ImageLoader::updateResolution(cv::Mat& image)
{
    cv::Mat outputImage;
    int newWidth, newHeight;

    switch (resizeType)
    {
    case BY_WIDTH:
        newWidth = width;
        newHeight = image.rows * width / image.cols;
        cv::resize(image, outputImage, cv::Size(newWidth, newHeight));
        break;

    case BY_HEIGHT:
        newHeight = height;
        newWidth = image.cols * height / image.rows;
        cv::resize(image, outputImage, cv::Size(newWidth, newHeight));
        break;

    case HARD:
        cv::resize(image, outputImage, cv::Size(width, height));
        break;

    case FIXED_RATIO:
        float desiredRatioWidth = 16.0; // Example ratio: 16/9
        float desiredRatioHeight = 9.0;
        if ((float)image.cols / image.rows > desiredRatioWidth / desiredRatioHeight)
        {
            newWidth = width;
            newHeight = newWidth * desiredRatioHeight / desiredRatioWidth;
        }
        else
        {
            newHeight = height;
            newWidth = newHeight * desiredRatioWidth / desiredRatioHeight;
        }
        cv::resize(image, outputImage, cv::Size(newWidth, newHeight));
        break;
    }

    return outputImage;
}


void ImageLoader::setResolution(size_t newWidth, size_t newHeight)
{
    width = newWidth; height = newHeight;
}

void ImageLoader::setDesiredRatio(float newRatio)
{
    desiredRatio = newRatio;
}


void ImageLoader::setResizeType(ResizeType newResizeType)
{
    resizeType = newResizeType;
}
