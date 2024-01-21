#include <iostream>
using namespace std;

#include "opencv2/opencv.hpp"
using namespace cv;

#include <vector>
#include <utility>


string name_pictograms[14] = {"accident", "bomb", "car", "casualty", "electricity", "fire", "firebrigade", "flood", "gas", "injury", "paramedic", "person", "police", "roadblock"};


vector<string> detect_pictograms(Mat& gray_image) {
    vector<pair<string, Point>> all_locations; // Vector containing all the locations of the templates (associated with the name of the template)

    size_t nb_total = 0;  // To verify there is 7 pictograms in the image
    string folder_path = "../templates_pictogram/"; // Path of the folder containing the templates

    Mat blurred_gray_img;

    // Add noise reduction to enhance pictogram recognition
    GaussianBlur(gray_image, blurred_gray_img, cv::Size(5, 5), 0);

    // Search each template
    for (string& name_pictogram : name_pictograms) {
        // cout << "looking for : " << name_pictogram << endl;
        Mat image_template = imread(folder_path + name_pictogram + ".png");

        cvtColor(image_template, image_template, COLOR_BGR2GRAY);

        GaussianBlur(image_template, image_template, cv::Size(5, 5), 0);


        // Find the template in the image
        Mat result;
        matchTemplate(blurred_gray_img, image_template, result, TM_CCOEFF_NORMED);

        Mat mask = result > 0.65;
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

        // Add the locations to the vector containing all the locations with the name of the template
        for (const Point_<int>& location : locations) {
            all_locations.emplace_back(name_pictogram, location);
        }

    }

    // If we don't find 7 pictograms, there is an error
    if (nb_total != 7) {
        cout << "Error: " << nb_total << " sizes found in the image instead of 7" << endl;
        return {};
    }

    else {
        // Sort all the pairs by the location y in the pair
        sort(all_locations.begin(), all_locations.end(), [](const pair<string, Point> &left, const pair<string, Point> &right) {
            return left.second.y < right.second.y;
        });

        vector<string> return_vector = {"", "", "", "", "", "", ""}; // The vector the function will return
        for (int i = 0; i < 7; i++) {
            return_vector[i] = all_locations[i].first;
        }

        return return_vector;
    }
}


