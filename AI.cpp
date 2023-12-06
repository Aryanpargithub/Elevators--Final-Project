/*
 * Copyright 2023 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */

#include "AI.h"
#include <cassert>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core

string getAIMoveString(const BuildingState& buildingState) {
    // determining when the elevator should pass
    bool passCondition = false;
    bool floorCondition = false;
    bool pickUpCondition = false;
    int sumAngerLevel = 0;
    int floorAnger[NUM_FLOORS] = {0};
    bool floorFilled[NUM_FLOORS] = {0};
    int floorWanted = 0;
    string moveAI = "";

    // Calculating total anger levels on each floor
    for (int a = 0; a < NUM_FLOORS; a++) {
        for (int i = 0; i < buildingState.floors[a].numPeople; i++) {
            sumAngerLevel += buildingState.floors[a].people[i].angerLevel;
        }
        floorAnger[a] = sumAngerLevel;
        sumAngerLevel = 0;
    }

    int highestAnger = 0;
    int temp1 = 0;

    // Finding the floor with the highest total anger level
    for (int b = 0; b < NUM_FLOORS; b++) {
        temp1 = floorAnger[b];
        if (highestAnger < temp1) {
            highestAnger = temp1;
        }
    }

    // Checking if elevators are already servicing a floor
    for (int e = 0; e < NUM_FLOORS; e++) {
        for (int i = 0; i < NUM_ELEVATORS; i++) {
            if (buildingState.elevators[i].targetFloor == e) {
                floorFilled[e] = true;
            }
        }
    }

    // Identifying the floor with the highest anger and not being serviced
    for (int c = 0; c < NUM_FLOORS; c++) {
        if (floorAnger[c] == highestAnger) {
            floorWanted = c;
        } else if (floorAnger[c] == highestAnger && floorFilled[c]) {
            floorAnger[c] = 0;
            for (int d = 0; d < NUM_FLOORS; d++) {
                temp1 = floorAnger[d];
                if (highestAnger < temp1) {
                    highestAnger = temp1;
                }
            }
            for (int e = 0; e < NUM_FLOORS; e++) {
                if (floorAnger[e] == highestAnger && !floorFilled[e]) {
                    floorWanted = e;
                }
            }
        }
    }

    // Finding the closest elevator
    int minDistance = 100;
    int diff = 0;
    int elevatorNumber = 0;

    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        if (!buildingState.elevators[i].isServicing) {
            diff = abs(floorWanted - buildingState.elevators[i].currentFloor);

            if (minDistance > diff) {
                minDistance = diff;
                elevatorNumber = i;
            }
        }
    }

    int counter = 0;

    // Checking conditions for different scenarios
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        if (!buildingState.elevators[i].isServicing) {
            counter++;
        }
    }

    if (counter > 0 && buildingState.elevators[elevatorNumber].currentFloor != floorWanted && !floorFilled[floorWanted]) {
        floorCondition = true;
    } else if (counter > 0 && buildingState.floors[buildingState.elevators[elevatorNumber].currentFloor].numPeople != 0) {
        pickUpCondition = true;
    } else {
        passCondition = true;
    }

    if (floorCondition) {
        moveAI = "e" + to_string(elevatorNumber) + "f" + to_string(floorWanted);
    } else if (pickUpCondition) {
        moveAI = "e" + to_string(elevatorNumber) + "p";
    }

    return moveAI;
}










string getAIPickupList(const Move& move, const BuildingState& buildingState, 
                       const Floor& floorToPickup) 
                       {

    string pickupList = "";
    int upRequests = 0;
    int downRequests = 0;
    int angerLevelUp = 0;
    int angerLevelDown = 0;
    bool hasUpRequest = false;
    bool hasDownRequest = false;

    

    int currentNumPeople = floorToPickup.getNumPeople();
    
    //keeping record of up requests and downrequests
    for (int p = 0; p < currentNumPeople; p++) {
        Person currentPerson = floorToPickup.getPersonByIndex(p);
        int targetFloor = currentPerson.getTargetFloor();
        int currentFloor = currentPerson.getCurrentFloor();
        if (currentFloor < targetFloor) {
            upRequests++;
        } else {
            downRequests++;
        }
    }

    // checking if current person with priority anger level is up request or down request
    for (int a = 0; a < currentNumPeople; a++) {
        Person currentPerson = floorToPickup.getPersonByIndex(a);
        int targetFloor = currentPerson.getTargetFloor();
        int currentFloor = currentPerson.getCurrentFloor();
        
        if (currentPerson.getAngerLevel() >= 8) {
            if (currentFloor < targetFloor) {
                hasUpRequest = true;
            } else {
                hasDownRequest = true;
            }
        }
    }
    
    //depending on uprequest or downrequest of priority pick up the rest of the ppl
    if (hasUpRequest == true && hasDownRequest == false) {
        for (int b = 0; b < currentNumPeople; b++) {
            Person currentPerson = floorToPickup.getPersonByIndex(b);
            int targetFloor = currentPerson.getTargetFloor();
            int currentFloor = currentPerson.getCurrentFloor();
            if (currentFloor < targetFloor) {
                pickupList += to_string(b);
            }
        }
    } else if (hasUpRequest == false && hasDownRequest == true) {
        for (int d = 0; d < currentNumPeople; d++) {
            Person currentPerson = floorToPickup.getPersonByIndex(d);
            int targetFloor = currentPerson.getTargetFloor();
            int currentFloor = currentPerson.getCurrentFloor();
            if (currentFloor > targetFloor) {
                pickupList += to_string(d);
            }
        }
    }
    
        
    //comparing angerlevels of up and down requests and adding to pickup list

    //if up requests and down requests only differ by one person consider anger levels
    if (abs(upRequests-downRequests) <= 1) {
        for (int i = 0; i < currentNumPeople; i++) {
            Person currentPerson = floorToPickup.getPersonByIndex(i);
            int targetFloor = currentPerson.getTargetFloor();
            int currentFloor = currentPerson.getCurrentFloor();
            if (currentFloor < targetFloor) {
                angerLevelUp += currentPerson.getAngerLevel();
            } else {
                angerLevelDown += currentPerson.getAngerLevel();
            }
        }
        if (angerLevelUp > angerLevelDown) {
            for (int j = 0; j < currentNumPeople; j++) {
                Person currentPerson = floorToPickup.getPersonByIndex(j);
                int targetFloor = currentPerson.getTargetFloor();
                int currentFloor = currentPerson.getCurrentFloor();
                if (currentFloor < targetFloor) {
                    pickupList += to_string(j);
                }
            }
        } else {
            for (int k = 0; k < currentNumPeople; k++) {
                Person currentPerson = floorToPickup.getPersonByIndex(k);
                int targetFloor = currentPerson.getTargetFloor();
                int currentFloor = currentPerson.getCurrentFloor();
                if (currentFloor > targetFloor) {
                    pickupList += to_string(k);
                }
            }
        }
        
    //if uprequests and downrequests differ by 2 or more consider which type of request has more
    } else if (abs(upRequests-downRequests) >= 2) {
        if (upRequests > downRequests) {
            for (int p = 0; p < currentNumPeople; p++) {
                Person currentPerson = floorToPickup.getPersonByIndex(p);
                int targetFloor = currentPerson.getTargetFloor();
                int currentFloor = currentPerson.getCurrentFloor();
                if (currentFloor < targetFloor) {
                    pickupList += to_string(p);
                }
            }
        } else {
            for (int c = 0; c < currentNumPeople; c++) {
                Person currentPerson = floorToPickup.getPersonByIndex(c);
                int targetFloor = currentPerson.getTargetFloor();
                int currentFloor = currentPerson.getCurrentFloor();
                if (currentFloor > targetFloor) {
                    pickupList += to_string(c);
                }
            }
        }
    }
    return pickupList;
}

