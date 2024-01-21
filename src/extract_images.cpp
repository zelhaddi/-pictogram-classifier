#include <iostream>
using namespace std;

#include "opencv2/imgproc.hpp"
using namespace cv;

#include "tools.hpp"


vector<vector<Mat>> extract_images(Mat &image, Mat &gray_image, bool show_image) {
    int row = 1, column = 1;

    // Apply Canny edge detection to find edges in the image
    Mat canny_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    Canny(gray_image, canny_output, 100, 200, 3);

    // Find contours in the Canny output
    findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<Rect> squares; // Vector to store bounding rectangles of detected squares

    // Iterate through detected contours and filter to select bounding rectangles that represent squares of a specific size and aspect ratio
    for (const vector<Point_<int>>& contour : contours) {
        double area = contourArea(contour);
        Rect rect = boundingRect(contour);

        double aspectRatio = static_cast<double>(rect.width) / rect.height;
        double aspectRatioMin = 0.7;
        double aspectRatioMax = 1.3;
        int areaMin = 15000;
        int areaMax = 100000;

        // Select the bounding rectangles that represent squares of a specific size and aspect ratio to only keep the icons
        if (area > areaMin && area < areaMax && aspectRatio > aspectRatioMin && aspectRatio < aspectRatioMax) {
            int border = 5;
            Rect rectCropped(rect.x + border, rect.y + border, rect.width - 2 * border, rect.height - 2 * border);
            squares.push_back(rectCropped);
        }
    }

    if (show_image) {
        Mat visual = image.clone();
        for (const Rect_<int>& rect : squares) {
            rectangle(visual, rect, Scalar(0, 255, 0), 2);
        }
        show_reduced_img(visual, "Output", 4);
    }

    if (squares.size() != 35) {
        cout << "Error: number of squares detected is : " << squares.size() << ", should be 35" << endl;
        return {};
    }

    // Sort squares by ascending y-coordinate of their top-left corners
    sort(squares.begin(), squares.end(), [](const Rect& a, const Rect& b) {
        return a.y < b.y;
    });


    // Ensure 'squares' size is a multiple of 5 (representing squares in rows)
    const int blockSize = 5;

    // Sort squares within each row by ascending x-coordinate in blocks of 5 (number of squares per row)
    for (size_t i = 0; i < squares.size(); i += blockSize) {
        if (i + blockSize <= squares.size()) {
            std::sort(squares.begin() + i, squares.begin() + i + blockSize, [](const Rect& a, const Rect& b) {
                return a.x < b.x;
            });
        }
    }


    // Create a 2D matrix (vector of vectors) to store the extracted images
    const int rows = 7;
    const int columns = 5;
    vector<vector<Mat>> result(rows, vector<Mat>(columns));

    // Iterate through squares and extract regions, write them to separate files, and draw rectangles around them
    for (const Rect_<int>& rect : squares) {
        Mat drawing = image(rect);

        result[row - 1][column - 1] = drawing;

        // Replace the darker pixel with white pixel
        int threshold = 200;  // Define the threshold to consider
        int border_distance = 5; // Define the distance from the border to consider

        for (int y = 0; y < drawing.rows; y++) {
            for (int x = 0; x < drawing.cols; x++) {
                if (x < border_distance || x >= drawing.cols - border_distance ||
                    y < border_distance || y >= drawing.rows - border_distance) {
                    Vec3b pixel = drawing.at<Vec3b>(y, x);
                    if (pixel[0] < threshold && pixel[1] < threshold && pixel[2] < threshold) {
                        drawing.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
                    }
                }
            }
        }

        // Update column and row count for file naming
        column++;
        if (column == 6) {
            column = 1;
            row++;
        }
    }

    return result;
}