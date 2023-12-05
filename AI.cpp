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
    return "";
}

string getAIPickupList(const Move& move, const BuildingState& buildingState, 
                       const Floor& floorToPickup) {
    string pickupList = "";
    int upRequests = 0;
    int downRequests = 0;
    int angerLevelUp = 0;
    int angerLevelDown = 0;

    
    //comparing angerlevels of up and down requests and adding to pickup list
    int currentNumPeople = floorToPickup.getNumPeople();
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
                    pickupList += j;
                }
            }
        } else {
            for (int k = 0; k < currentNumPeople; k++) {
                Person currentPerson = floorToPickup.getPersonByIndex(k);
                int targetFloor = currentPerson.getTargetFloor();
                int currentFloor = currentPerson.getCurrentFloor();
                if (currentFloor > targetFloor) {
                    pickupList += k;
                }
            }
        }
    } else if (abs(upRequests-downRequests) >= 2) {
        if (upRequests > downRequests) {
            for (int p = 0; p < currentNumPeople; p++) {
                Person currentPerson = floorToPickup.getPersonByIndex(p);
                int targetFloor = currentPerson.getTargetFloor();
                int currentFloor = currentPerson.getCurrentFloor();
                if (currentFloor < targetFloor) {
                    pickupList += p;
                }
            }
        } else {
            for (int c = 0; c < currentNumPeople; c++) {
                Person currentPerson = floorToPickup.getPersonByIndex(c);
                int targetFloor = currentPerson.getTargetFloor();
                int currentFloor = currentPerson.getCurrentFloor();
                if (currentFloor > targetFloor) {
                    pickupList += c;
                }
            }
        }
    }
    return pickupList;
}
