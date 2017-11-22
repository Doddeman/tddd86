/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"
#include "iostream"

using namespace std;

GameState::GameState(){}


GameState::GameState(int numberOfRobots) {
    for (int i = 0; i < numberOfRobots; i++) {
        Robot * robot;
        while(true){
            robot = new Robot();                //assign random coordinates to robot
            if(!isEmpty (*robot)){              //delete robot if coordinates aren't free
                delete robot;
            }else{
                break;
            }
        }
        robots.push_back(robot);
    }
    teleportHero();
}

GameState::~GameState(){
    while(!robots.empty()){
        delete robots.back();
        robots.pop_back();
    }
}

//Copy constructor
GameState::GameState(const GameState &other) {
    hero = other.hero;
    for (auto robot : other.robots) {
        if(robot->isJunk()){
            robots.push_back(new Junk(*robot));
        }
        else{
            robots.push_back(new Robot(*robot));
        }
    }
}

GameState& GameState::operator=(const GameState& other) {
    if (this != &other) {
        GameState temp(other);

        swap(hero, temp.hero);
        swap(robots, temp.robots);
    }
    return *this;
}

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
    for(unsigned int i = 0; i < robots.size(); i++) {
        bool collision = (countRobotsAt (*robots[i]) > 1);
        if (collision) {
            if(!(*robots[i]).isJunk()){//this if needs since junk will collide with each other
                numberDestroyed++;
            }
            Robot * newJunk = new Junk(*robots[i]);
            delete robots[i];
            robots[i] = newJunk;
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
