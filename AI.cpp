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
    // if the building is empty, return empty string 
    bool peeps = false; 
    int goFloor = 0;
    int anger;
    int newAnger;
    int Angersum;
    int Count;
    int MostPeep;
    string move = "e";
    for(int target = 0; target < NUM_ELEVATORS; target++)
    {
            if(!buildingState.elevators[target].isServicing)
            {
                //move += to_string(target);
                for (int floors = 0; floors < NUM_FLOORS; ++floors) 
                {
                    Angersum = 0;
                    Count = 0;
                        for (int person = 0; person < buildingState.floors[floors].numPeople; ++person) 
                        {
                            Count++;
                            newAnger = buildingState.floors[floors].people[person].angerLevel;
                            Angersum += newAnger;
                           if(newAnger > anger)
                           {
                                anger = newAnger;
                                goFloor = buildingState.floors[floors].floorNum;
                           }
                        }
                        if(anger < 8)
                        {
                            if(Count > 5 && Angersum > 15)
                            {
                                goFloor = buildingState.floors[floors].floorNum;
                            }
                        }
                }
                
                
               
                
                    if(abs(buildingState.elevators[target].currentFloor - goFloor) > 6)
                    {
                        if(target + 1 < NUM_ELEVATORS)
                        {
                            if(!buildingState.elevators[target +1].isServicing)
                            {
                                if(!(abs(buildingState.elevators[target + 1].currentFloor - goFloor) > 6))
                                {
                                    move += to_string(target + 1);
                                    move += "f";
                                    move += to_string(goFloor);
                                    return move;
                                }
                                else if((target + 2 < NUM_ELEVATORS) && !(abs(buildingState.elevators[target + 2].currentFloor - goFloor) > 6))
                                {
                                    move += to_string(target + 2);
                                    move += "f";
                                    move += to_string(goFloor);
                                    return move;
                                }
                                else
                                {
                                    move += to_string(target);
                                    move += "f";
                                    move += to_string(goFloor);
                                    return move;
                                }
                            }
                        }
                        
                    }
                    else
                    {
                        move += to_string(target);
                        move += "f";
                        move += to_string(goFloor);
                        return move;
                    }
                
                
            }
    }
}

string getAIPickupList(const Move& move, const BuildingState& buildingState, 
                       const Floor& floorToPickup) 
                       {
    string pickupList = "";
    int upRequests = 0;
    int downRequests = 0;
    int angerLevelUp = 0;
    int angerLevelDown = 0;


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

/* incomplete
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
                pickupList += b;
            }
        }
    } else if (hasUpRequest == false && hasDownRequest == true) {
        for (int d = 0; d < currentNumPeople; d++) {
            Person currentPerson = floorToPickup.getPersonByIndex(d);
            int targetFloor = currentPerson.getTargetFloor();
            int currentFloor = currentPerson.getCurrentFloor();
            if (currentFloor > targetFloor) {
                pickupList += d;
            }
        }
    } 
    
*/


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
    //if uprequests and downrequests differ by 2 or more consider which type of request has more
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

}
