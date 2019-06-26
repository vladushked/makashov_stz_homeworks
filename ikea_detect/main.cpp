#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <cmath>
#include <math.h>

using namespace std;

int low_h = 115;
int high_h = 120;
string window_name = "Object Detection";
cv::Mat image;
cv::Mat hsvImage;
cv::Mat red_Image;
cv::Mat mask;
cv::Mat mask2;

cv::Mat makeHistogram (cv::Mat& image)
{
    int histSize = 256;
    float range []= {0, 255};
    const float* pRange = range;
    int channels [] = {0};
    cv::Mat hist;

    cv::calcHist (&image,1,(int*)channels,cv::Mat(), hist, 1, &histSize, &(pRange));
    int hist_w = 700, hist_h = 700;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar (0,0,0));
    normalize (hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    for (int i = 1; i < histSize; i++)
    {
        cv::line (histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1)) ),
                  cv::Point (bin_w * (i), hist_h - cvRound (hist.at<float>(i)) ),
                  cv::Scalar (255,255,255), 2, 8, 0);
    }
    return histImage;
}

void hue_trackbar(int, void*) {
    cv::inRange (hsvImage, cv::Scalar (low_h, 110, 110), cv::Scalar (high_h, 255, 255), red_Image);
    cv::imshow("Test", red_Image);
}

int main()
{
    image = cv::imread("new.jpg");
    if (!image.data) {
        cout << "Failed to open image!" << endl;
        return -1;
    }
    cv::imshow("Test", image);
    //cv::waitKey();

    std::vector<cv::Mat> planes; // вектор матриц
    cvtColor(image, hsvImage, cv::COLOR_RGB2HSV); //преобразуем в HSV
    cv::split(image, planes); //разделяется изображение на каналы
    cv::Mat value = planes[0];
    cv::imshow("Test", value);
    //cv::waitKey();

    cv::inRange (hsvImage, cv::Scalar (low_h, 110, 110), cv::Scalar (high_h, 255, 255), red_Image);
    cv::createTrackbar ("Low_HUE", "Test", &low_h, 179, hue_trackbar); //создаем TRACKBAR
    cv::createTrackbar ("HIGH_HUE", "Test", &high_h, 179, hue_trackbar);
    hue_trackbar (0,0);

    mask = red_Image.clone();
    mask2 = image.clone();

    cv::medianBlur (red_Image, red_Image, 5);
    std::vector <std::vector<cv::Point>> contours;
    cv::findContours (red_Image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); //находим все контуры на изображении

    double max = 0; //площадь наибольшего контура
    int number_contour = 0; //номер наибольшего контура
    for( int i = 0; i < contours.size(); i++ ){
        double area = cv::contourArea(contours[i]);
        if (area > max){
            max = area;
            number_contour = i;
        }
    }
    std::vector <std::vector<cv::Point>> contour;
    contour.push_back(contours[number_contour]);
    cv::Mat frame1 = cv::Mat::zeros(red_Image.rows, red_Image.cols, CV_8UC1);//нулевое изображение размера frame1 типа CV_8UC1
    cv::drawContours(frame1, contour, -1, cv::Scalar(255), cv::FILLED); //функция рисует заполненный контур
    cv::imshow("Contour", frame1);

    cv::Rect line_rect = cv::boundingRect(contours[number_contour]); //координаты прямоугольника, куда вписан наибольший контур
    cv::Mat croppedImage = mask(line_rect); //обрезаем изображение по наибольшему контуру
    cv::Mat croppedImageRGB = mask2(line_rect); //обрезаем изображение по наибольшему контуру
    //cv::imshow("Cropped", croppedImage);
    //cv::GaussianBlur( croppedImage, croppedImage, cv::Size( 9, 9 ), 0, 0 );
    cv::medianBlur (croppedImage, croppedImage, 5);
    //dilating
    int dilation_size = 3;
    cv::Mat element = getStructuringElement( 0,
                                             cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                             cv::Point( dilation_size, dilation_size ) );
    /// Apply the dilation operation
    dilate( croppedImage, croppedImage, element );
    cv::imshow("Cropped", croppedImage);

    cv::Mat canny_output;
    int thresh = 255;
    /// Detect edges using canny
    Canny(croppedImage, canny_output, thresh, thresh*2, 3 );
    cv::imshow("frame3", canny_output);
    cv::waitKey();

    std::vector <std::vector<cv::Point>> contours_cropped;
    std::vector <std::vector<cv::Point>> contours_cropped_f;
    vector<cv::Point> approx;
    vector< cv::Vec4i > hierarchy;
    cv::findContours (canny_output, contours_cropped, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE); //находим все контуры на изображении
    cv::Mat frame2 = cv::Mat::zeros(croppedImage.rows, croppedImage.cols, CV_8UC1);//нулевое изображение размера frame1 типа CV_8UC1
    for( int i = 0; i< contours_cropped.size(); i++ ) // iterate through each contour.
    {
        if (hierarchy[i][2]<0){
            //cv::drawContours(frame2, contours_cropped_f, contours_cropped_f.size() - 1, cv::Scalar(100), cv::FILLED);
            //cv::imshow("frame2", frame2);
            //cv::waitKey();
            continue;
        } //Check if there is a child contour

        else {
            contours_cropped_f.push_back(contours_cropped[i]);
            //cv::drawContours(frame2, contours_cropped_f, contours_cropped_f.size() - 1, cv::Scalar(255), cv::FILLED);
            //cv::imshow("frame2", frame2);
            //cv::waitKey();
        }
    }

    std::vector <std::vector<cv::Point>> contours_final;

    for( int i = 0; i< contours_cropped_f.size(); i++ ) // iterate through each contour.
    {
        double peri = cv::arcLength(contours_cropped_f[i], true);
        cv::approxPolyDP(contours_cropped_f[i], approx, 10, true);
        if (approx.size() == 4){
            contours_final.push_back(contours_cropped_f[i]);
        } else
            continue;
    }
    cv::RNG rng(12345);
    for( int i = 0; i< contours_final.size(); i++ )
    {
        cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( croppedImageRGB, contours_final, i, color, 2, 8, hierarchy, 0, cv::Point() );
    }
    cv::imshow("final", croppedImageRGB);


    cv::waitKey();
    return 0;
}


