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
    int totalFloorAngerLevel = 0;
    int angerByFloor[NUM_FLOORS] = {0};
    bool floorServiced[NUM_FLOORS] = {0};
    int floorPriority = 0;
    string moveAI = "";
    
    //putting total anger levels of everyone on each floor into array of angerByFloor (position 0 is floor 0)
    for (int a = 0; a < NUM_FLOORS; a++) {
        for (int i = 0; i < buildingState.floors[a].numPeople; i++) {
            totalFloorAngerLevel += buildingState.floors[a].people[i].angerLevel;
        }
        angerByFloor[a] = totalFloorAngerLevel;
        totalFloorAngerLevel = 0;
    }
    
    //creating array of whether a floor is already being serviced
    for (int b = 0; b < NUM_FLOORS; b++) {
        for (int i = 0; i < NUM_ELEVATORS; i++) {
            if (buildingState.elevators[i].targetFloor == b) {
                floorServiced[b] = true;
            }
        }
    }
    
    //finding highest anger level among unserviced floors
    int highestAnger = 0;
    for (int c = 0; c < NUM_FLOORS; c++) {
        if (highestAnger < angerByFloor[c] && floorServiced[c] == false) {
            highestAnger = angerByFloor[c];
        }
    }
    
    //Set floor priority based on highest anger
    for (int d = 0; d < 10; d++) {
        if (angerByFloor[d] == highestAnger) {
            floorPriority = d;
        }
    }
        
    //Finding closestElevator
    int smallestDistance = 10;
    int distanceEToF = 0;
    int closestElevator = 0;
    for (int e = 0; e < NUM_ELEVATORS; e++) {
        if (!buildingState.elevators[e].isServicing) {
            distanceEToF = abs(floorPriority - buildingState.elevators[e].currentFloor);
            if (distanceEToF < smallestDistance) {
                smallestDistance = distanceEToF;
                closestElevator = e;
            }
        }
    }
    
    if (!buildingState.elevators[closestElevator].isServicing) {
        moveAI = 'e' + to_string(closestElevator) + 'f' + to_string(floorPriority);
    } else {
        moveAI = "";
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

