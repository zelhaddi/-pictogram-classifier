#include <iostream>
using namespace std;

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

#include <vector>  // To use vectors
#include <utility> // To use pair
#include "tools.hpp"  // To show reduced images


string name_sizes[3] = {"large", "medium", "small"};


vector<string> detect_sizes(Mat& gray_image, bool show_image) {
    vector<pair<string, Point>> all_locations; // Vector containing all the locations of the templates (associated with the name of the template)

    unsigned long nb_total = 0;  // To verify there is 7 sizes in the image
    string folder_path = "../templates_size/"; // Path of the folder containing the templates


    // Search each template
    for (int i = 0; i < 3; i++) {
        // cout << "looking for : " << name_sizes[i] << endl;
        Mat image_template = imread(folder_path + name_sizes[i] + ".png");
        if(image_template.data == nullptr){
            cout << "Template not found: " << folder_path + name_sizes[i] + ".png" << endl;
            exit(EXIT_FAILURE);
        }
        cvtColor(image_template, image_template, COLOR_BGR2GRAY);

        // Find the template in the image
        Mat result;
        matchTemplate(gray_image, image_template, result, TM_CCOEFF_NORMED);

        Mat mask = result > 0.9;
        // Find all locations above the threshold
        vector<Point> locations;
        findNonZero(mask, locations);

        // Loop through each location found from template matching to remove duplicates in locations vector
        for (int j = 0; j<locations.size(); j++) {
            for (int k = j+1 ; k < locations.size(); k++) {
                if (abs(locations[j].x - locations[k].x) < 150 && abs(locations[j].y - locations[k].y) < 150) {
                    locations.erase(locations.begin() + k);
                    k--;
                }
            }
        }

        nb_total += locations.size(); // increment the number of sizes found

        // Draw the rectangles
        if (show_image) {
            for (const Point_<int>& location : locations) {
                // Create a rectangle around the detected template in the original image
                Rect boundingBox(location, Size(image_template.cols, image_template.rows));
                // Draw the rectangle on the image with different colors for each name
                if (i == 0) {
                    rectangle(gray_image, boundingBox, Scalar(255, 0, 0), 5);
                }
                else if (i == 1) {
                    rectangle(gray_image, boundingBox, Scalar(0, 255, 0), 5);
                }
                else {
                    rectangle(gray_image, boundingBox, Scalar(0, 0, 255), 5);
                }
            }
        }

        // Add the locations to the vector containing all the locations with the name of the template
        for (const Point_<int>& location : locations) {
            all_locations.emplace_back(name_sizes[i], location);
        }

    }

    // Show the image
    if (show_image) {
        show_reduced_img(gray_image, "Sizes", 4);
    }

    if(nb_total == 0) {
        // No images found
        return {};
    }

    else

    if (nb_total != 7) {
        cout << "Error: " << nb_total << " sizes found in the image instead of 7" << endl;
        return {};
    }

    else {
        // Sort all the pairs by the location y in the pair
        sort(all_locations.begin(), all_locations.end(), [](const pair<string, Point>& left, const pair<string, Point>& right) {
            return left.second.y < right.second.y;
        });

        vector<string> return_vector = {"", "", "", "", "", "", ""}; // The vector the function will return
        for (int i = 0; i < 7; i++) {
            return_vector[i] = all_locations[i].first;
        }

        return return_vector;
    }
}
