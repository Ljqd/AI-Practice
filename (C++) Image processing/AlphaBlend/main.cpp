#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

static const std::string WINDOW_NAME = "Alpha blend";

struct Data
{
    cv::Mat smallerImage;
    cv::Mat largerImage;
    cv::Mat mask;
    cv::Mat blended;
    cv::Mat blendedRoi;
};

cv::Mat getMask(cv::Mat& image);
void updateImage(int, void* userData);
void createTrackbar(Data* dataPtr);

int main(int argc, char** argv)
{
    // Load two images
    cv::Mat img1 = cv::imread("./resources/image1.png");
    cv::Mat img2 = cv::imread("./resources/image2.jpg");

    if (img1.empty() || img2.empty())
    {
        std::cout << "Could not open or find the images!\n" << std::endl;
        return -1;
    }

    // Determine smaller and larger image
    cv::Mat smallerImage, largerImage;
    if (img1.size().area() <= img2.size().area())
    {
        smallerImage = img1;
        largerImage = img2;
    }
    else 
    {
        smallerImage = img2;
        largerImage = img1;
    }

    // Convert images to float32
    smallerImage.convertTo(smallerImage, CV_32FC3);
    largerImage.convertTo(largerImage, CV_32FC3);

    // Create a binary mask based on smaller image
    cv::Mat mask = getMask(smallerImage);

    cv::Mat blended = largerImage.clone();
    Data data = { smallerImage, largerImage, mask, blended, cv::Mat::zeros(smallerImage.size(), CV_32FC3) };

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    createTrackbar(&data);
    updateImage(0, &data);
    cv::waitKey(0);
    return 0;
}

cv::Mat getMask(cv::Mat& image)
{
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
    cv::Mat grayscale;
    cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
    cv::inRange(grayscale, 1, 255, mask);
    mask.convertTo(mask, CV_32F, 1.0 / 255);
    cv::merge(std::vector<cv::Mat>(3, mask), mask);

    return mask;
}

void createTrackbar(Data* dataPtr)
{
    cv::createTrackbar("Alpha", WINDOW_NAME, NULL, 100, updateImage, dataPtr);
    cv::setTrackbarPos("Alpha", WINDOW_NAME, 0);
}

void updateImage(int, void* dataPtr)
{
    Data* data = reinterpret_cast<Data*>(dataPtr);

    double alpha = cv::getTrackbarPos("Alpha", WINDOW_NAME) / 100.0;

    // Calculate the top-left point for centering the smaller image within the larger image
    cv::Point topLeft((data->largerImage.cols - data->smallerImage.cols) / 2 + 100, 
                      (data->largerImage.rows - data->smallerImage.rows) / 2 + 60);

    // Define ROI in the larger image based on the calculated position
    cv::Rect roi(topLeft, data->smallerImage.size());
    cv::Mat largerImageROI = data->largerImage(roi);

    // Blend the smaller image and the ROI of the larger image
    data->blendedRoi = data->smallerImage.mul(alpha * data->mask) + data->largerImage(roi).mul(cv::Scalar::all(1.0) - alpha * data->mask);

    // Place the blended ROI back into the larger image
    
    data->blendedRoi.copyTo(data->blended(roi));

    // Display the result
    cv::imshow(WINDOW_NAME, data->blended / 255.0);
}