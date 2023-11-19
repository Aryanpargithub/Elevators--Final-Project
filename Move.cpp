
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
    //TODO: Implement non-default constructor
    stringstream ss(commandString);
        char moveType;
        ss >> moveType;
    
        isPickup = false;
        isPass = false;
        isSave = false;
        isQuit = false;
   

        if (moveType == 'p') {
            isPickup = true;
        } else if (moveType == ' ') {
            isPass = true;
        } else if (moveType == 'S') {
            isSave = true;
        } else if (moveType == 'Q') {
            isQuit = true;
        } else if (moveType == 'f') {
            int floorNum;
            ss >> floorNum;
        }
     
    }
    
    





bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
   
    if (isPass == true || isQuit == true || isSave == true) {
        return true;
    }
    
    else if (0 <= elevatorId && elevatorId < NUM_ELEVATORS && !elevators[elevatorId].isServicing()) {
       
        if (isPickup || elevators[elevatorId].getCurrentFloor() != targetFloor || (0 <= targetFloor && targetFloor < NUM_FLOORS) ) {
            return true;
        
        }
        
    }
        
        return false;
    
}

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, const Floor& pickupFloor) {
    
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
    int extremeFloor = currentFloor;

   
    for (int i = 0; i < pickupList.size(); i++) {
           int pIndex = i - '0';
           peopleToPickup[numPeopleToPickup++] = pIndex;
           
           Person pickedPerson = pickupFloor.getPersonByIndex(pIndex);
           
           totalSatisfaction += pickedPerson.getAngerLevel();

         
           int pTargetFloor = pickedPerson.getTargetFloor();
        
           if (pTargetFloor > extremeFloor || pTargetFloor < extremeFloor ) {
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
