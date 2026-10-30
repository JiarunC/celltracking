/*
 * main.cpp
 * For testing only
 */

#include <random>
#include <filesystem>
#include <iostream>
#include "vis.h"
#include "seg.h"

static Mat generate_syn_image() {
    Mat image = Mat::zeros(1000,1000,CV_8UC3);
    return image;
}

static pair<vector<Point>, vector<pair<int, int>>> generate_syn_polylines() {
    //also generates vertices
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,1000);
    vector<Point> vertices;
    vector<pair<int,int>> lines;
    vertices.reserve(40);
    for (int i=0; i<40;i++) {
        vertices.emplace_back(dis(gen), dis(gen));
        if (i % 2) {
            lines.emplace_back(i-1,i);
        }
    }
    return {vertices,lines};
}

static pair<vector<vector<int>>, vector<vector<int>>> generate_groups_and_families() {
    int j = -1;
    vector<vector<int>> groups;
    groups.reserve(10);
    vector<vector<int>> families;
    families.reserve(5);
    for (int i=0; i<20;i++) {
        if (i % 2) {
            groups.push_back({i-1, i});
            j++;
        }
        if (i % 4 == 3) {
            families.push_back({j-1,j});
        }
    }
    return {groups,families};
}


int main() {
    std::filesystem::path dir = "assets/GOWT-Training/01/";
    constexpr char ow[] = "Original Images";
    constexpr char nw[] = "New Images";
    vector<string> image_paths;
    vector<Mat> images;
    for (auto const& dir_entry : filesystem::directory_iterator(dir)) {
        image_paths.push_back(dir_entry.path().string());
    }
    sort(image_paths.begin(), image_paths.end());
    for (auto const& image_path : image_paths) {
        Mat image;
        cout << image_path << endl;
        imread(image_path, image, IMREAD_GRAYSCALE);
        images.push_back(image);
        auto original = image.clone();
        Mat sure_bg, sure_fg, tbd;

        seg::apply_blur(image);
        seg::apply_threshold(image);
        seg::apply_morph(image);
        seg::prepare_segmentation(image, sure_bg, sure_fg, tbd);
        seg::segment(image, sure_bg, sure_fg, tbd);
        imshow(ow, original);
        imshow(nw, image);
        moveWindow("Original Images", 0, 0);
        moveWindow("New Images", 600, 0);
        waitKey(100);
    }
    return 0;
}