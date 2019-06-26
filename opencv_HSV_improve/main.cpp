#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/opencv_modules.hpp>

cv::Mat makeHistogram (cv::Mat& image);
cv::Mat brightnessEnch (cv::Mat& greyScale);

int main() {

    cv::Mat image = cv::imread("Picture1.png");
    if (!image.data){
        std::cerr << "< Ошибка чтения! >" << std::endl;
        return 1;
    }
    cv::imshow("Test", image);
    cv::waitKey(0);

    cv::Mat greyScale;
    cv::cvtColor(image,greyScale, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(greyScale, greyScale, cv::Size(5,5), 3);
    cv::imshow("GreyScale", greyScale);
    cv::waitKey(0);

    cv::Mat histImage = makeHistogram(greyScale);
    cv::imshow("Histogram", histImage);
    cv::waitKey(0);

    cv::Mat newGreyScale = brightnessEnch(greyScale);
    histImage = makeHistogram(newGreyScale);
    cv::imshow("New", newGreyScale);
    cv::imshow("New Histogram", histImage);
    cv::waitKey(0);

    return 0;
}

cv::Mat makeHistogram (cv::Mat& image){
    int histSize = 256;
    float range[] = {0, 255};
    const float*pRange = range;
    int channels[] = {0};
    cv::Mat hist;

    cv::calcHist(&image, 1, (int*)channels, cv::Mat(), hist, 1, &histSize, &(pRange));
    int hist_h = 1000, hist_w = 1000;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0,0,0));
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < histSize; i++) {
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
                 cv::Scalar(255, 255, 255), 2, 8, 0);
    }
    return histImage;
}

cv::Mat brightnessEnch (cv::Mat& greyScale){
    double minValue, maxValue;
    cv::minMaxLoc(greyScale, &minValue, &maxValue);
    double deltaStep = 255 / (maxValue - minValue), value = 0;
    cv::Mat lookUpTable(1, 256, CV_8U);
    cv::Mat newGreyScale;
    for (int i = 0; i < 255; i++){
        if (i < minValue)
            lookUpTable.at<unsigned char>(i) = minValue;
        if (i >= minValue && i <= maxValue) {
            value += deltaStep;
            lookUpTable.at<unsigned char>(i) = (unsigned char) value;
        }
        if (i > maxValue)
            lookUpTable.at<unsigned char>(i) = maxValue;
    }
    cv::LUT(greyScale, lookUpTable, newGreyScale);
    cv::GaussianBlur(newGreyScale, newGreyScale, cv::Size(5,5), 3);
    return newGreyScale;
}