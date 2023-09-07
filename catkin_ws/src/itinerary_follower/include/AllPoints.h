#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

    struct Point {
        double x;
        double y;
        double z;
    };


class AllPoints{
private:
    std::string folderPath = "../waypoints";
    std::vector<std::vector<Point>> allPoints;

    //Read points from a File
    std::vector<Point> readPointsFromFile(const std::string &filename) {
        std::vector<Point> points;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                Point p;
                if (sscanf(line.c_str(), "%lf %lf %lf", &p.x, &p.y, &p.z) == 3) {
                    points.push_back(p);
                }
            }
            file.close();
        }
        return points;
    }

public: 
    AllPoints()
    {
        for (const auto &entry : std::filesystem::directory_iterator(folderPath)) 
        {
            if (entry.is_regular_file()) {
                std::vector<Point> points = readPointsFromFile(entry.path().string());
                allPoints.push_back(points);
            }
        }
    }

    Point getPoint(int itinerary, int point) const {
            return allPoints[itinerary][point];
    }

    void ShowAllPoints()
    {
        for (const auto &route : allPoints) 
        {
            for (const Point &p : route) 
            {
                std::cout << "x: " << p.x << ", y: " << p.y << ", z: " << p.z << std::endl;
            }
            std::cout << "-----------" << std::endl;
        }
    }
};
