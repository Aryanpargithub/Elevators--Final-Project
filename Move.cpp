
/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */
 
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "Move.h"

using namespace std;

Move::Move(string commandString) : Move() {
    stringstream ss(commandString);
    char junk;
    char moveType;
    
    isPickup = false;
    isPass = false;
    isSave = false;
    isQuit = false;
    
    if (commandString == "") {
        isPass = true;
    } else if (commandString == "S") {
        isSave = true;
    } else if (commandString == "Q") {
        isQuit = true;
    } else {
        ss >> junk >> elevatorId >> moveType;
        if (moveType == 'f') {
            ss >> targetFloor;
        } else if (moveType == 'p') {
            isPickup = true;
        }
    }
}

//check
bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
    if (isPass == true || isQuit == true || isSave == true) {
        return true;
    }
    else if (isPickup == true) {
        if (elevatorId >= 0 && elevatorId < NUM_ELEVATORS && elevators[elevatorId].isServicing() == false) {
            return true;
        } else {
            return false;
        }
    } else if (targetFloor >= 0 && targetFloor < NUM_FLOORS && targetFloor != elevators[elevatorId].getCurrentFloor()) {
        if (elevatorId >= 0 && elevatorId < NUM_ELEVATORS && elevators[elevatorId].isServicing() == false) {
                return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, const Floor& pickupFloor) {
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
    int extremeFloor = currentFloor;

    for (char c : pickupList) {
        int pIndex = c - '0';
        peopleToPickup[numPeopleToPickup++] = pIndex;

        Person pickedPerson = pickupFloor.getPersonByIndex(pIndex);

        totalSatisfaction += pickedPerson.getAngerLevel();

        int pTargetFloor = pickedPerson.getTargetFloor();

        if (pTargetFloor > extremeFloor) {
            extremeFloor = pTargetFloor;
        } else if (pTargetFloor < extremeFloor) {
            extremeFloor = pTargetFloor;
        }
    }

    setTargetFloor(extremeFloor);
}






//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
    elevatorId = -1;
    targetFloor = -1;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
    isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
}

bool Move::isPickupMove() const {
    return isPickup;
}

bool Move::isPassMove() const {
    return isPass;
}

bool Move::isSaveMove() const {
    return isSave;
}

bool Move::isQuitMove() const {
    return isQuit;
}

int Move::getElevatorId() const {
    return elevatorId;
}

int Move::getTargetFloor() const {
    return targetFloor;
}

int Move::getNumPeopleToPickup() const {
    return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
    return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
    targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
    for (int i = 0; i < numPeopleToPickup; ++i) {
        newList[i] = peopleToPickup[i];
    }
}
