#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/opencv_modules.hpp>

int main() {

    cv::Mat image = cv::imread("Picture1.png");
    if (!image.data){
        std::cerr << "< Ошибка чтения! >" << std::endl;
        return 1;
    }
    cv::imshow("Test", image);
    cv::waitKey(0);

    std::vector<cv::Mat> planes;
    cv::split(image, planes);

    for (int i = 0; i < planes.size(); i++){
        std::string name;
        switch (i){
            case 2:
                name = "red";
                break;
            case 1:
                name = "green";
                break;
            case 0:
                name = "blue";
                break;
        }
        cv::imshow(name, planes[i]);
    }
    cv::waitKey(0);

    int histSize = 256;
    float range[] = {0, 255};
    const float*pRange = range;
    int channels[] = {0};

    cv::Mat rHist, bHist, gHist;
    cv::calcHist(&planes[0], 1, (int*)channels, cv::Mat(), bHist, 1, &histSize, &(pRange));
    cv::calcHist(&planes[1], 1, (int*)channels, cv::Mat(), gHist, 1, &histSize, &(pRange));
    cv::calcHist(&planes[2], 1, (int*)channels, cv::Mat(), rHist, 1, &histSize, &(pRange));

    int hist_h = 1000, hist_w = 1000;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0,0,0));
    cv::normalize(bHist, bHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(gHist, gHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(rHist, rHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < histSize; i++){
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(bHist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(bHist.at<float>(i))),
                 cv::Scalar(255, 0, 0), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(gHist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(gHist.at<float>(i))),
                 cv::Scalar(0, 255, 0), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(rHist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(rHist.at<float>(i))),
                 cv::Scalar(0, 0, 255), 2, 8, 0);
    }

    cv::imshow("Hist", histImage);
    cv::waitKey(0);
    return 0;
}