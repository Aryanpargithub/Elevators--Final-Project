/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Floor.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */


#include "Floor.h"

using namespace std;

int Floor::tick(int currentTime) {
    int explodedPeople = 0;
    int explodedIndices[MAX_PEOPLE_PER_FLOOR] = {};
    for (int i = 0; i < numPeople; i++) {
        if (people[i].tick(currentTime)) {
            explodedIndices[explodedPeople] = i;
            explodedPeople++;
        }
    }
    for (int i = explodedPeople; i < numPeople - 1; i++) {
        people[i] = people[i + 1];
    }
    numPeople--;

    return explodedPeople;
}



void Floor::addPerson(Person newPerson, int request) {
    if (numPeople < MAX_PEOPLE_PER_FLOOR) {
        people[numPeople] = newPerson;
        numPeople++;
    }
    if (request > 0) {
        hasUpRequest = true;
    } else {
        hasDownRequest = true;
    }
}

void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR], int numPeopleToRemove) {
    //creating copy of constant indicesToRemove
    int targets[MAX_PEOPLE_PER_FLOOR] = {};
    for (int i = 0; i < MAX_PEOPLE_PER_FLOOR; i++) {
        targets[i] = indicesToRemove[i];
    }
    //sorting targets to remove from least to greatest
    sort(targets, targets + numPeopleToRemove);
    
    for (int j = 0; j < numPeopleToRemove; j++) {
        for (int k = 0; k < numPeople; k++) {
            people[targets[j] + k] = people[targets[j] + k + 1]; // removing person
        }
        numPeople -= 1;
    }
    
    resetRequests();
}

void Floor::resetRequests() {
    for (int i = 0; i < numPeople; i++) {
        int floorDifference = people[i].getTargetFloor() - people[i].getCurrentFloor();
        if (floorDifference > 0) {
            hasUpRequest = true;
        } else {
            hasDownRequest = true;
        }
    }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
    hasDownRequest = false;
    hasUpRequest = false;
    numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream& outs) const {
    string up = "U";
    outs << (hasUpRequest ? up : " ") << " ";
    for (int i = 0; i < numPeople; ++i){
        outs << people[i].getAngerLevel();
        outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
    }
    outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream& outs) const {
    string down = "D";
    outs << (hasDownRequest ? down : " ") << " ";
    for (int i = 0; i < numPeople; ++i) {
        outs << "o   ";
    }
    outs << endl;
}

void Floor::printFloorPickupMenu(ostream& outs) const {
    cout << endl;
    outs << "Select People to Load by Index" << endl;
    outs << "All people must be going in same direction!";
    /*  O   O
    // -|- -|-
    //  |   |
    // / \ / \  */
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " O   ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << "-|-  ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " |   ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << "/ \\  ";
    }
    outs << endl << "INDEX:        ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << i << "   ";
    }
    outs << endl << "ANGER:        ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << people[i].getAngerLevel() << "   ";
    }
    outs << endl << "TARGET FLOOR: ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << people[i].getTargetFloor() << "   ";
    }
}

void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
    return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
    return hasDownRequest;
}

int Floor::getNumPeople() const {
    return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
    return people[index];
}
