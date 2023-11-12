/*
 * Copyright 2023 University of Michigan EECS183
 *
 * test.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Final Project - Elevators
 */
 
////////////////////////////////////////////////
// You will submit test.cpp to the autograder //
// FOR LAB 8, but NOT the final project Core  //
////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include "AI.h"
#include "Building.h"
#include "BuildingState.h"
#include "Elevator.h"
#include "Floor.h"
#include "Game.h"
#include "Move.h"
#include "Person.h"
#include "SatisfactionIndex.h"
#include "Utility.h"

using namespace std;

// declare your test functions here
void person_tests();

void start_tests() {
    // call your test functions here
    person_tests();
    return;
}

// write test functions here
void person_tests() {
    //initialize a person with targetFloor 5, anger 5
    Person p("0f0t5a5");

    cout << p.getTargetFloor() << " " << p.getAngerLevel() << " Expected 5 5" << endl;

    //simulate time being a multiple of TICKS_PER_ANGER_INCREASE
    bool exploded = p.tick(TICKS_PER_ANGER_INCREASE);

    cout << exploded << " Expected 0" << endl;

    cout << p.getTargetFloor() << " " << p.getAngerLevel() << " Expected 5 6" << endl;

    //simulate time not being a multiple
    p.tick(TICKS_PER_ANGER_INCREASE - 1);

    //no change
    cout << p.getTargetFloor() << " " << p.getAngerLevel() << " Expected 5 6" << endl;

    p.tick(TICKS_PER_ANGER_INCREASE); //7 after

    p.tick(TICKS_PER_ANGER_INCREASE); //8 after

    p.tick(TICKS_PER_ANGER_INCREASE); //9 after

    exploded = p.tick(TICKS_PER_ANGER_INCREASE);

    cout << exploded << " Expected 1" << endl;
}
