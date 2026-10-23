/*
 * main.cpp
 * For testing only
 */

#include <random>
#include <iostream>
#include <iomanip>
#include <map>
#include "vis.h"

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
    auto pr = generate_syn_polylines();
    auto vertices = pr.first;
    auto lines = pr.second;
    auto gf = generate_groups_and_families();
    auto groups = gf.first;
    auto families = gf.second;
    auto v = vis();
    v.set_image(generate_syn_image());
    cout << "image in class" << endl;
    v.visualize();
    v.set_vertices(&vertices);
    v.visualize();
    v.set_polylines(&lines);
    v.visualize();
    v.set_groups(&groups);
    v.visualize();
    v.set_families(&families);
    cout << "full class" << endl;
    v.visualize();

    cout << "static image" << endl;
    vis::visualize(generate_syn_image());
    cout << "static polyline" << endl;
    vis::visualize(generate_syn_image(), vertices, lines);
    cout << "static families" << endl;
    vis::visualize(generate_syn_image(), vertices, lines, groups, families);

    return 0;
}