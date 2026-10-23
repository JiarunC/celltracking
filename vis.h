/*
 * vis.h
 * Contains class vis for visualization
 * Supports display of grayscale image and overlay
 * Contains both member and static visualize functions
 * DC
 */


/*
 * Mat is never a pointer or a reference
 * member vectors are pointers
 * vectors in statics are references
 */

#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
using namespace cv;
using namespace std;

/*
 * Class vis
 * Params:
 * _image: Matrix of a grayscale image
 * _vertices: list of vertices (2D coordinates) each represented by a Point
 * _polylines: list of polylines each represented by indices in _vertices
 * _groups: list of groups of polylines closed and manifold (hopefully);
 * can be used as faces; each represented by indices in _polylines
 * _families: list of families of groups each represented by indices in _groups
 * members of the same family share a color
 */
class vis {
public:
    /* Constructors */
    vis() = default;

    explicit vis(const Mat &image) {
        _image = image;
    }

    explicit vis(const Mat &image, vector<Point> *vertices, vector<pair<int, int> > *polylines,
                 vector<vector<int> > *groups, vector<vector<int> > *families) {
        _image = image;
        _vertices = vertices;
        _polylines = polylines;
        _groups = groups;
        _families = families;
    }

    /* Setters */
    void set_image(const Mat &image) {
        _image = image;
    }

    void set_vertices(vector<Point> *vertices) {
        _vertices = vertices;
    }

    void set_polylines(vector<pair<int, int> > *polylines) {
        _polylines = polylines;
    }

    void set_groups(vector<vector<int> > *groups) {
        _groups = groups;
    }

    void set_families(vector<vector<int> > *families) {
        _families = families;
    }

    // Function visualize
    // Member of vis class
    // Visualizes class member _image, if one of the other member is not established
    // Visualizes _image with overlay, if all members are established
    // Each family shares a color
    // Param: thickness - int thickness of drawn polylines (has default)
    // Return: void
    void visualize(const int thickness = 2) const {
        if (_image.empty()) {
            return;
        }
        // X overwrite image
        auto output = _image.clone();

        if (_polylines != nullptr && _groups != nullptr && _vertices != nullptr && _families != nullptr) {
            for (const auto &family: *_families) {
                // per group assign a color
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, 255);
                // does not matter the order
                auto c1 = dis(gen);
                auto c2 = dis(gen);
                auto c3 = dis(gen);
                // mirror bell curve on quartiles to make contrasty colors
                for (auto &c: {&c1, &c2, &c3}) {
                    if (*c > 255 / 2) {
                        *c = 255 - (*c - 255 / 2);
                    } else {
                        *c = 255 / 2 - *c;
                    }
                }
                auto color = Scalar(c1, c2, c3);

                for (const auto &group_index: family) {
                    for (const auto &polyline_index: _groups->at(group_index)) {
                        const auto polyline = _polylines->at(polyline_index);
                        line(output,
                            _vertices->at(polyline.first),
                            _vertices->at(polyline.second),
                            color,
                            thickness,
                            LINE_AA);
                    }
                }
            }
        }

        constexpr char image_window[] = "Image Window";
        imshow(image_window, output);
        waitKey(0);
        moveWindow(image_window, 0, 200);
    }

    // Static Overload of visualize
    // Takes an image and a thickness (with default)
    static void visualize(const Mat& image, const int thickness = 2) {
        if (image.empty()) {
            return;
        }

        constexpr char image_window[] = "Image Window";
        imshow(image_window, image);
        waitKey(0);
        moveWindow(image_window, 0, 200);
    }

    // Static Overload of visualize
    // Takes an image, list of vertices, list of polylines, and a thickness (with default)
    static void visualize(const Mat& image, const vector<Point>& vertices, const vector<pair<int,int>>& polylines, const int thickness = 2) {
        if (image.empty()) {
            return;
        }
        // X overwrite image
        auto output = image.clone();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        // does not matter the order
        auto c1 = dis(gen);
        auto c2 = dis(gen);
        auto c3 = dis(gen);
        // mirror bell curve on quartiles to make contrasty colors
        for (auto &c: {&c1, &c2, &c3}) {
            if (*c > 255 / 2) {
                *c = 255 - (*c - 255 / 2);
            } else {
                *c = 255 / 2 - *c;
            }
        }
        const auto color = Scalar(c1, c2, c3);

        for (const auto polyline : polylines) {
            line(output,
                vertices.at(polyline.first),
                vertices.at(polyline.second),
                color,
                thickness,
                LINE_AA);
        }

        constexpr char image_window[] = "Image Window";
        imshow(image_window, output);
        waitKey(0);
        moveWindow(image_window, 0, 200);
    }

    // Static Overload of visualize with everything
    // If you simply want a family per group, pass in range(groups) for families
    static void visualize(const Mat& image, const vector<Point>& vertices, const vector<pair<int,int>>& polylines, vector<vector<int> > &groups, vector<vector<int> > &families, const int thickness = 2) {
        if (image.empty()) {
            return;
        }
        // X overwrite image
        auto output = image.clone();


        for (const auto &family: families) {
            // per group assign a color
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);
            // does not matter the order
            auto c1 = dis(gen);
            auto c2 = dis(gen);
            auto c3 = dis(gen);
            // mirror bell curve on quartiles to make contrasty colors
            for (auto &c: {&c1, &c2, &c3}) {
                if (*c > 255 / 2) {
                    *c = 255 - (*c - 255 / 2);
                } else {
                    *c = 255 / 2 - *c;
                }
            }
            auto color = Scalar(c1, c2, c3);

            for (const auto &group_index: family) {
                for (const auto &polyline_index: groups.at(group_index)) {
                    const auto polyline = polylines.at(polyline_index);
                    line(output,
                        vertices.at(polyline.first),
                        vertices.at(polyline.second),
                        color,
                        thickness,
                        LINE_AA);
                }
            }
        }


        constexpr char image_window[] = "Image Window";
        imshow(image_window, output);
        waitKey(0);
        moveWindow(image_window, 0, 200);
    }


private:
    // Already pointer like
    Mat _image;
    vector<Point> *_vertices{};
    vector<pair<int, int> > *_polylines{};
    vector<vector<int> > *_groups{}; // can also be used as faces
    vector<vector<int> > *_families{}; // family of cells (mother-daughter)
};
