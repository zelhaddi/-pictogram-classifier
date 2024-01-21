#include <iostream>
using namespace std;

#include "opencv2/imgproc.hpp"
using namespace cv;

#include "number_page.hpp" // The linked header file
#include "tools.hpp"  // To show the image


int find_number_page(Mat& gray_image, bool show_image) {
    // Apply thresholding
    Mat thresholded;
    threshold(gray_image, thresholded, 150, 255, THRESH_BINARY);

    // Find contours
    vector<vector<Point>> contours;     // Represent the contours of the image
    vector<Vec4i> hierarchy;            // Represent the relationship between contours
    findContours(thresholded, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    Mat contours_img;
    if (show_image) {
        contours_img = Mat(thresholded.size(), CV_8UC3, Scalar(255, 255, 255));
        // Draw the contours on a white image
        drawContours(contours_img, contours, -1, Scalar(0, 0, 0), 2);
    }

    Point points[15]; // The center of the 15 squares that define the number page in binary
    int* tab_binary = new int[15]; // The binary number page
    // Initialize the binary number page
    for (int i = 0; i < 15; i++) {
        tab_binary[i] = 0;
    }

    // Iterate through contours to find the rectangle of approximate size : 20*270 pixels
    for (const vector<Point_<int>>& contour : contours) {
        Rect rect = boundingRect(contour);
        int area = rect.width * rect.height;

        // Find the big rectangle
        if (area > 20000 && area < 100000 && rect.height < 150) {
            if (show_image) {
                // Draw the rectangle in red on the image
                rectangle(contours_img, rect, Scalar(0, 0, 255), 5);
            }

            // Find the coordinates of the center of each square in the rectangle
            int size_square = rect.width / 15;
            int y_coord = rect.y + size_square / 2;
            int x_coord = rect.x + size_square / 2;
            for (int i = 0; i < 15; i++) {
                points[i] = Point(x_coord + i * size_square, y_coord);

                if (show_image) {
                    // Draw the points in blue on the image
                    circle(contours_img, points[i], 5, Scalar(255, 0, 0), 5);
                }
            }
        }

        // Find the small rectangles
        else if (area > 1800 && area < 5000 && abs(rect.width - rect.height) < 10 && rect.x > 1000 && rect.y > 500 && rect.y < 720) {
            if (show_image) {
                // Draw the rectangle in green on the image
                rectangle(contours_img, rect, Scalar(0, 255, 0), 5);
            }

            // Find the coordinates of the center of each square in the rectangle
            Point center = Point(rect.x + rect.width / 2, rect.y + rect.height / 2);

            // Find the closest point in points
            double min_dist = 100000.0;
            int min_index = 0;
            for (int i = 0; i < 15; i++) {
                double dist = norm(points[i] - center);
                if (dist < min_dist) {
                    min_dist = dist;
                    min_index = i;
                }
            }
            tab_binary[min_index] = 1;
        }
    }

    // Print the number page
    int number_page = 0;
    for (int i = 0; i < 15; i++) {
        number_page += tab_binary[i] << (14 - i);
    }

    if (show_image) {
        // Show the image
        show_reduced_img(contours_img, "contours", 4);
    }

    return number_page;
}
