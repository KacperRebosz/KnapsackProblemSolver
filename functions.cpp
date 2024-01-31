/**
 * @file functions.cpp
 * @brief File containing necessary functions for program to work.
 */
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <vector>
#include <string>
#include "structures.h"
/**
 * @brief Loads a list of items from a file and returns them as a vector.
 *
 * @param fileName Input file name as a string
 * @return Vector of items loaded from the file.
 */
std::vector<Item> loadItems(const std::string& fileName) {
    std::vector<Item> items;
    std::ifstream file(fileName);
    Item item;

    while (file >> item.name >> item.weight >> item.value) {
        items.push_back(item);
    }

    file.close();
    return items;
}
/**
 * @brief Calculates the fitness of a solution using a list of items and a weight limit.
 *
 * @param solution Vector representing the solution to evaluate.
 * @param items Vector of items.
 * @param weightLimit The maximum weight allowed for the solution.
 * @return The fitness value of the solution.
 */
double calculateFitness(const std::vector<int>& solution, const std::vector<Item>& items, double& weightLimit) {
    double weight = 0.0;
    int value = 0;

    for (int i = 0; i < solution.size(); i++) {
        if (solution[i] == 1) {
            weight += items[i].weight;
            value += items[i].value;
        }
    }
    if (weight > weightLimit) {
        value = 0;
    }
    return value;
}
/**
 * @brief Mutates solution vector.
 *
 * @param solution Solution vector to mutate.
 */
void mutate(std::vector<int>& solution) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < solution.size(); i++) {
        if (distribution(generator) < mutationRate) {
            solution[i] = 1 - solution[i];
        }
    }
}
/**
 * @brief Selects two individuals from a population using tournament selection.
 *
 * @param population Vector of vectors representing the population of solutions.
 * @param items Vector of items used to evaluate the solutions.
 * @param weightLimit The maximum weight allowed for the solutions.
 * @return Pair of vectors representing the selected individuals.
 */
std::pair<std::vector<int>, std::vector<int>> selection(const std::vector<std::vector<int>>& population,
    const std::vector<Item>& items, double weightLimit) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, population.size() - 1);

    std::vector<int> parent1 = population[distribution(generator)];
    std::vector<int> parent2 = population[distribution(generator)];

    // Choosing two best solutions
    if (calculateFitness(parent1, items, weightLimit) < calculateFitness(parent2, items, weightLimit)) {
        parent1 = population[distribution(generator)];
    }
    else {
        parent2 = population[distribution(generator)];
    }

    return std::make_pair(parent1, parent2);
}
/**
 * @brief Performs crossover between two parent solutions to produce two offspring.
 *
 * @param parent1 The first parent solution.
 * @param parent2 The second parent solution.
 *
 * @return Pair of offspring solutions produced by performing crossover between the parent solutions.
 */
std::pair<std::vector<int>, std::vector<int>> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    int crossoverPoint = rand() % parent1.size();
    std::vector<int> offspring1(parent1.size());
    std::vector<int> offspring2(parent2.size());
    for (int i = 0; i < crossoverPoint; i++) {
        offspring1[i] = parent1[i];
        offspring2[i] = parent2[i];
    }
    for (int i = crossoverPoint; i < parent1.size(); i++) {
        offspring1[i] = parent2[i];
        offspring2[i] = parent1[i];
    }
    return std::make_pair(offspring1, offspring2);
}