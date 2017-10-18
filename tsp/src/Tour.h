// This is the .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header

#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:

    Tour();
    //Tour(Point a, Point b, Point c, Point d); // create an empty tour
    ~Tour();                                  // free all memory used by list nodes
    void show() const;                        // print the tour to standard output
    void draw(QGraphicsScene *scene) const;   // draw the tour on scene
    int size() const;                         // number of points on tour
    double distance() const;                  // return the total distance of the tour
    void insertNearest(Point p);              // insert p using nearest neighbor heuristic
    void insertSmallest(Point p);             // insert p using smallest increase heuristic

private:

    Node* startNode = nullptr;
    Node* endNode = nullptr;

};

#endif // TOUR_H
