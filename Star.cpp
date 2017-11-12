#include "Star.hpp"
#include <cstdlib>
#include <cmath>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//#define DEBUG
//#define DEBUGB
volatile unsigned int mark = 0;


Star::Star(const cv::Point &center, const cv::Size &imgsize, int radius)
{
    //cv::Size imgsize(760, 480);
    //cv::Mat bg(imgsize, CV_8UC3, cv::Scalar(0, 0, 0));
    this->background = cv::Mat(imgsize, CV_8UC3, cv::Scalar(0, 0, 0));
    this->showpic = cv::Mat(imgsize, CV_8UC3, cv::Scalar(0, 0, 0));
    this->center = center;
    this->radius = radius;
    if (center.x < radius || center.y < radius)
    {
        cout << "Initial parameters is wrong, the point of star may out of boarder of the background" << endl;
        cout << "Press any key to continue!" << endl;
        std::getchar();
        exit(0);
    }
}

Star::Star()
{
    cv::Size imgsize(760, 480);
    //cv::Mat bg(imgsize, CV_8UC3, cv::Scalar(0, 0, 0));
    this->background = cv::Mat(imgsize, CV_8UC3, cv::Scalar(0, 0, 0));
    this->showpic = cv::Mat(imgsize, CV_8UC3, cv::Scalar(0, 0, 0));
    this->center = cv::Point(0, 0);
    this->radius = 0;
}
void Star::release()
{
    this->background.release();
    this->showpic.release();
}

vector<cv::Point> Star::GenerateFivePoint(const cv::Point &center, int radius)
{
    vector<cv::Point> result(5);
    int x_left = center.x - radius;
    // x_right = center.x + radius;
    //int y_top = center.y - radius, y_bottom = center.y + radius;
    //generate the first point random
    cv::Point p1;
    std::srand((unsigned)time(0));
    p1.x = int((double(rand()) / RAND_MAX) * 2 * radius) + x_left; //随机产生正确范围内的点
    p1.y = int(std::sqrt(radius * radius - (p1.x - center.x) * (p1.x - center.x))) + center.y;
    result[0] = p1;
    //generate the other four points
    cv::Point point1 = p1, point2;
    for (int i = 1; i < 5; i++)
    {
        point2 = RotatedToGeneratePoint(point1, center);
        result[i] = point2;
        point1 = point2;
    }
    this->fivepoints = result;
    return result;
}

void Star::DrawStar(cv::Mat &bg, const vector<cv::Point> &fivepoints)
{
    cv::Scalar color;
    if (bg.channels() == 1)
    {
        color = cv::Scalar(255);
    }
    else if (bg.channels() == 3)
    {
        //color = cv::Scalar(255, 255, 255);
        color = cv::Scalar(double(rand()) / RAND_MAX * 255, double(rand()) / RAND_MAX * 255, double(rand()) / RAND_MAX * 255);
    }
    else
    {
        cout << "The background has wrong channels only support 1 or 3 channels" << endl;
        return;
    }
    cv::line(bg, fivepoints[0], fivepoints[2], color, 3);
    cv::line(bg, fivepoints[0], fivepoints[3], color, 3);
    cv::line(bg, fivepoints[1], fivepoints[3], color, 3);
    cv::line(bg, fivepoints[1], fivepoints[4], color, 3);
    cv::line(bg, fivepoints[2], fivepoints[4], color, 3);


}

void Star::FillColorForStar(cv::Mat &imgStar)
{
#ifdef DEBUG
    cv::namedWindow("print color");
#endif
    if (imgStar.channels() != 3)
    {
        cout << "image channel can not be permited to print" << endl;
        cout << "press any key to continue" << endl;
        exit(0);
    }
    cv::Mat grayImg, binaryImg;
    imgStar.copyTo(grayImg);
    cv::cvtColor(grayImg, grayImg, CV_BGR2GRAY);
    cv::threshold(grayImg, binaryImg, 10, 255, cv::THRESH_OTSU);
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    findContours(binaryImg, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE );
    for (int i = 0; i < int(contours.size()); i++)
    {
        cv::Rect rect = cv::boundingRect(contours[i]);
        cv::Point pt(rect.x + rect.width / 2, rect.y + rect.height /2);
        cv::Scalar color(double(rand()) / RAND_MAX * 255, double(rand()) / RAND_MAX * 255, double(rand()) / RAND_MAX * 255);
        cv::floodFill(imgStar, pt, color);
#ifdef DEBUG
        cv::imshow("print color", imgStar);
        cv::waitKey(100);
#endif

    }
    grayImg.release();
    binaryImg.release();

}
cv::Point Star::RotatedToGeneratePoint(const cv::Point &point1, const cv::Point &center)
{
    cv::Point result;
    result.x = int((point1.x - center.x) * std::cos(ROTATEDANGLE) - (point1.y - center.y) * std::sin(ROTATEDANGLE));
    result.y = int((point1.y - center.y) * std::cos(ROTATEDANGLE) + (point1.x - center.x) * std::sin(ROTATEDANGLE));
    return result = result + center;
}

void Star::ImgProducer()
{    
    while(1)
    {        
        //cv::imshow("bg", bg);
        if (mark % 2 == 0)
        {

            cv::Mat bg;
            this->background.copyTo(bg);
#ifdef DEBUGB
            *(bg.ptr(240) + 380 * 3) = 255;
            *(bg.ptr(240) + 380 * 3 + 1) = 255;
            *(bg.ptr(240) + 380 * 3 + 2) = 255;
            cv::imshow("background", this->background);
            cv::waitKey(100);
            //std::getchar();
#endif
            vector<cv::Point> fivepoints;
            fivepoints = this->GenerateFivePoint(this->center, this->radius);
            this->DrawStar(bg, fivepoints);
            this->FillColorForStar(bg);
            //this->showpic = bg;
            bg.copyTo(this->showpic);
            mark++;
            bg.release();
        }
    }
}
void Star::ShowPicture()
{
    while(1)
    {  
        if (mark % 2 == 1)
        { cv::namedWindow("Show rotated star with color");
            cv::imshow("Show rotated star with color", this->showpic);
            cv::waitKey(30);
            mark++;
        } 
    }    
}