/*
 * TDDD86 Pattern Recognition
 * This program computes and plots all line segments involving 4 points
 * in a file using Qt.
 */

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Point.h"
#include <set>

// constants
static const int SCENE_WIDTH = 512;
static const int SCENE_HEIGHT = 512;

void render_points(QGraphicsScene* scene, const vector<Point>& points) {
    for(const auto& point : points) {
        point.draw(scene);
    }
}

void render_line(QGraphicsScene* scene, const Point& p1, const Point& p2) {
    p1.lineTo(scene, p2);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // open file
    string filename = "input100.txt";
    ifstream input;
    input.open(filename);

    // the vector of points
    vector<Point> points;

    // read points from file
    int N;
    int x;
    int y;

    input >> N;

    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        points.push_back(Point(x, y));
    }
    input.close();

    // setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    view->setScene(scene);
    // draw points to screen all at once
    render_points(scene, points);
    view->scale(1, -1); //screen y-axis is inverted
    view->resize(view->sizeHint());
    view->setWindowTitle("Fast Force Pattern Recognition");
    view->show();

    // sort points by natural order
    // makes finding endpoints of line segments easy
    sort(points.begin(), points.end());
    auto begin = chrono::high_resolution_clock::now();

    //Create final map for all the lines
    //A set is a line containing ordered points
    //A vector contains all lines with the same slope
    map<double, vector<set<Point>>> finalMap;
    double slope;
    double currentSlope;
    for(int p = 0; p < N; p++){
        //cout << "point: " << points[p] << endl;
        //sort all lines from p in pMap
        map<double, set<Point>> pMap;
        for(int q = 0; q < N; q++){
            slope = points[p].slopeTo(points[q]);
            pMap[slope].insert(points[q]);
        }
        //Iterate all line from p
        for (auto it = pMap.begin(); it != pMap.end(); it++){
            set<Point> currentLine = it->second;
            if(currentLine.size() >= 3){
                currentSlope = it->first;
                //get vector of all lines with current slope
                vector<set<Point>> existingVector = finalMap[currentSlope];
                bool foundLine = false;
                for(unsigned int i = 0; i < existingVector.size(); i++){
                    set<Point> existingLine = existingVector[i];
                    for(Point currentPoint: currentLine) {
                        //if existing line doesn't contain current point
                        if(existingLine.find(currentPoint) != existingLine.end()){
                            //Add entire current line to existing line
                            finalMap[currentSlope][i].insert(currentLine.begin(), currentLine.end());
                            foundLine = true;
                            break;
                        }
                    }
                }
                if(!foundLine){
                    //add set to vector
                    finalMap[currentSlope].push_back(currentLine);
                }
            }
        }
    }
    //Draw
    for (auto it = finalMap.begin(); it != finalMap.end(); it++){
        vector<set<Point>> currentVector = it->second;
        for(unsigned int i = 0; i < currentVector.size(); i++){
            set<Point> currentLine = currentVector[i];
            //make vector for output
            vector<Point> line(currentLine.begin(), currentLine.end());
            /*for(Point print : line){
                cout << print << " - ";
            }
            cout << endl;*/
            render_line(scene, line.front(), line.back());
            a.processEvents(); //show rendered line
        }
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return a.exec(); // start Qt event loop
}
