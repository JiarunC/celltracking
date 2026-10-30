#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/geometry/2d.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
using namespace cv;
using namespace std;

class seg {
public:
    static void apply_blur(Mat& src, const int distance = 11, const double sigma_color = 15, const double sigma_space = 15) {
        const Mat dst = src.clone();
        bilateralFilter(dst, src, distance, sigma_color, sigma_space);
    }

    static void apply_threshold(Mat& src) {
        auto th = threshold(src, src, 0, 255, THRESH_BINARY + THRESH_TRIANGLE + THRESH_DRYRUN);
        threshold(src, src, th * .8, 255, THRESH_BINARY);
    }

    static void apply_morph(Mat& src) {
        auto kernel_close = getStructuringElement(MORPH_ELLIPSE, Size(7,7));
        auto kernel_open = getStructuringElement(MORPH_ELLIPSE, Size(5,5));
        morphologyEx(src, src, MORPH_OPEN, kernel_open, Point(-1,-1), 1);
        morphologyEx(src, src, MORPH_CLOSE, kernel_close, Point(-1,-1), 3);
        morphologyEx(src, src, MORPH_OPEN, kernel_open, Point(-1,-1), 3);
    }

    static void prepare_segmentation(Mat& src, Mat& sure_bg, Mat& sure_fg, Mat& tbd) {
        // L1 is |x1-x2| + |y1-y2|
        sure_bg= src.clone();
        sure_fg= src.clone();
        auto kernel = getStructuringElement(MORPH_ELLIPSE, Size(5,5));
        morphologyEx(src, sure_bg, MORPH_DILATE, kernel);
        // bitwise_not(sure_bg, sure_bg);
        distanceTransform(src, sure_fg, DIST_L1, 3, CV_8UC1);

        double max_d, min_d;
        minMaxLoc(sure_fg, &min_d, &max_d);
        threshold(sure_fg, sure_fg, .35 * max_d, 255, THRESH_BINARY);
        assert(sure_fg.type() == sure_bg.type());
        subtract(sure_bg, sure_fg, tbd);
    }
    static void segment(Mat& src, Mat& sure_bg, Mat& sure_fg, Mat& tbd) {
        Mat markers;
        auto labels = connectedComponents(sure_fg, markers, 8, CV_16U);

        markers.convertTo(markers, CV_8UC1);
        markers = markers + 1;

        assert(markers.type() == tbd.type());
        Mat tmp1;
        // bitwise_not(tbd, tbd);
        subtract(markers, tbd, markers);

        cvtColor(src, src, cv::COLOR_GRAY2BGR);
        assert(src.type() == CV_8UC3);
        markers.convertTo(markers, CV_32SC1);
        watershed(src, markers);
        src = markers.clone();
        src = Mat(markers.rows, markers.cols, CV_32UC1, markers.data);
        src.convertTo(src, CV_8UC1);
    }

    static void post_processing(Mat& src) {

    }
};