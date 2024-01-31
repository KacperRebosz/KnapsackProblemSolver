#pragma once

#ifndef STRUCTURES_H
#define STRUCTURES_H
/**
* @file structures.h
*
* @brief File that contains the necessary structure definitions for the program to work.
*
*/
#include <vector>
#include <string>

/**
* @struct Item
* @brief Struct representing an item in knapsack problem
* 
*/
struct Item {
    std::string name;/**< The name of the item */
    double weight;/**< The weight of the item */
    int value;/**< The value of the item */
};

const double mutationRate = 0.05; /**< The mutation rate */

#endif