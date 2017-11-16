/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"
#include "iostream"


GameState::GameState(){}

GameState::GameState(int numberOfRobots) {
    for (int i = 0; i < numberOfRobots; i++) {  //for each robot
        Robot * robot;                          //create robot object (to the stack)
        while(true){
            robot = new Robot();                //assign random coordinates to robot (in the heap)
            if(!isEmpty (*robot)){              //are coordinates free?
                delete[] robot;                 //no: delete robot (from heap), free memory
            }else{
                break;                          //yes: break loop
            }
        }
        robots.push_back(robot);                //add to vector
    }
    teleportHero();
}

GameState::~GameState(){
    for(auto robot : robots){
        //delete[] robot;
    }
}

/*
GameState::GameState(const GameState &other) {
    copyOther(other);
}

GameState& GameState::operator=(const GameState& other) {
    if (this != &other) {

        for (auto robot : robots) {
            delete robot; // Free memory
        }
        robots.clear();
        copyOther(other);
    }
    return *this;
}

void GameState::copyOther(const GameState &other) {
    hero = other.hero;
    for (auto robot : other.robots) {
        robots.push_back(robot->clone());
    }
}
*/

void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();
    for (size_t i = 0; i < robots.size(); ++i){
        (*robots[i]).draw(scene);
    }
    hero.draw(scene);
}

void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}

void GameState::moveRobots() {
    for (unsigned int i = 0; i < robots.size(); i++)
        (*robots[i]).moveTowards (hero);
}

int GameState::countCollisions() {
    int numberDestroyed = 0;
    for(unsigned int i = 0; i < robots.size(); i++) {       //for each robot
        bool collision = (countRobotsAt (*robots[i]) > 1);  //check if collision
        if (collision) {                                    //if collision
            if(!(*robots[i]).isJunk()){                     //if this robot isn't junk
                numberDestroyed++;                          //one more robot destroyed
            }                                               //This is because junk will collide with each other
            delete[] robots[i];                             //release memory (from heap)
            robots[i] = new Junk(*robots[i]);               //turn into junk (assign memory in heap)

        }
    }
    return numberDestroyed;
}

bool GameState::anyRobotsLeft() const {
    for(unsigned int i = 0; i < robots.size(); i++){
        if(!(*robots[i]).isJunk()){
            return true;
        }
    }
    return false;
}

bool GameState::heroDead() const {
    return !isEmpty(hero);
}

bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); i++)
        if ((*robots[i]).attacks(unit) || countRobotsAt(unit) > 0){
            return false;
        }
    return true;
}

void GameState::moveHeroTowards(const Unit& dir) {
    hero.moveTowards(dir);
}

Hero GameState::getHero() const {return hero;}

/*
 * Free of robots and junk only
 */
bool GameState::isEmpty(const Unit& unit) const {
    return (countRobotsAt(unit) == 0);
}

/*
 * How many robots or junk are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (size_t i = 0; i < robots.size(); ++i) {
        if ((*robots[i]).at(unit))
            count++;
    }
    return count;
}
