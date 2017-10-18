// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

using namespace std;

Tour::Tour()
{
    cout << "List is being constructed" << endl;
}
/*
Tour::Tour(Point a, Point b, Point c, Point d)
{
    cout << "List is being constructed" << endl;

    endNode = new Node(d);
    startNode = new Node(a, new Node(b, new Node(c, endNode)));
    endNode->next = startNode;
}
*/
Tour::~Tour()
{
    cout << "List is being deconstructed" << endl;
}

void Tour::show() const
{
    if(startNode != nullptr)
    {
        int counter = 2;
        Node* tempNode = startNode->next;
        cout << "Point 1: " << startNode->point.toString() << endl;
        while(tempNode != startNode)
        {
            cout << "Point " << counter << ": " << tempNode->point.toString() << endl;
            tempNode = tempNode->next;
            counter++;
        }
    }
}

void Tour::draw(QGraphicsScene *scene) const
{
    if(startNode != nullptr)
    {
        startNode->point.draw(scene);
        Node* tempNode = startNode->next;
        startNode->point.drawTo(tempNode->point,scene);
        while(tempNode != startNode)
        {
            tempNode->point.draw(scene);
            tempNode->point.drawTo(tempNode->next->point, scene);
            tempNode = tempNode->next;
        }
    }
}


int Tour::size() const
{
    int counter = 0;
    if(startNode != nullptr)
    {
        counter = 1;
        Node* tempNode = startNode->next;
        while(tempNode != startNode)
        {
            tempNode = tempNode->next;
            counter++;
        }
    }
    return counter;
}

double Tour::distance() const
{
    double distance = 0;
    if(startNode != nullptr)
    {
        Node* tempNode = startNode->next;
        distance += startNode->point.distanceTo(tempNode->point);
        while(tempNode != startNode)
        {
            distance += tempNode->point.distanceTo(tempNode->next->point);
            tempNode = tempNode->next;
        }
        return distance;
    }
}

void Tour::insertNearest(Point p)
{
    if(startNode != nullptr)
    {
        Node* tempNode = startNode;
        Node* nearestNode;
        do
        {
            tempNode = tempNode->next;
            if (p.distanceTo(tempNode->point) < p.distanceTo(nearestNode->point))
            {
                nearestNode = tempNode;
            }
        }
        while(tempNode != startNode);
        nearestNode->next = new Node(p, nearestNode->next);
    }
    else
    {
        startNode = new Node(p);
        startNode->next = startNode;
    }
}

void Tour::insertSmallest(Point p)
{
    if(startNode != nullptr)
    {
        Node* tempNode = startNode;
        Node* bestNode;
        double currentLength;
        double bestLength = NULL;
        do
        {
            tempNode = tempNode->next;
            currentLength = tempNode->point.distanceTo(p) + p.distanceTo(tempNode->next->point) - tempNode->point.distanceTo(tempNode->next->point);
            if(currentLength < bestLength || bestLength == NULL)
            {
                bestLength = currentLength;
                bestNode = tempNode;
            }
        }
        while(tempNode != startNode);
        bestNode->next = new Node(p, bestNode->next);
    }

    else
    {
        startNode = new Node(p);
        startNode->next = startNode;
    }






/*    if(startNode != nullptr)
    {
        Node* tempNode = startNode;
        Node* bestNode;
        double resultDistance = -1;
        do
        {
            tempNode = tempNode->next;

            double pToTemp = p.distanceTo(tempNode->point);
            double pToNext = p.distanceTo(tempNode->next->point);
            double tempToNext = tempNode->point.distanceTo(tempNode->next->point);
            double totalDistance = distance() + pToTemp + pToNext - tempToNext;

            if(totalDistance < resultDistance || resultDistance == -1)
            {
                resultDistance = totalDistance;
                bestNode = tempNode;
            }
        }
        while(tempNode != startNode);
        bestNode->next = new Node(p,bestNode->next);
    }
    else
    {
        startNode = new Node(p);
        startNode->next = startNode;
    }
*/
}
