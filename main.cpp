/**
* 
*   @file main.cpp
*   @brief Main function of the program.
*   @author Kacper Rebosz
*   @date 2023-02-15
*/

/**
*
    @mainpage Task: 
*   Write a program that is designed to solve the knapsack problem using a genetic algorithm. Program should take input data from an input file that hols a set of items with their names, masses and values.
*   Program should find a subset of items with maximal value and total weight that does not exceed capacity of the knapsack provided by user. \n
*   This program can be used from the command line by passing the following switches: \n
*       -i name.txt : input file with a set of items \n
*       -o name.txt : output file with the best solutions in all generations \n
*       -c value : knapsack capacity \n
*       -g value : number of generations \n
*       -n value : number of individuals in a generation \n
*/
#include "structures.h"
#include "functions.cpp"

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << std::endl;
        std::cout << "Switches in program :" << std::endl;
        std::cout << "-i name.txt" << " -- input file with a set of items" << std::endl;
        std::cout << "-o name.txt" << " -- output file with the best solutions in all generations" << std::endl;
        std::cout << "-c value" << " -- knapsack capacity" << std::endl;
        std::cout << "-g value" << " -- number of generations" << std::endl;
        std::cout << "-n value" << " -- number of individuals in a generation" << std::endl;
        return 1;
    }
    bool isEverythingOk = true;
    std::string outputFilename,itemsInput;
    double weightLimit;
    int sizeOfPopulation = 100;
    int numberOfGenerations = 100; // Number of iterations

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-i" && i + 1 < argc) {
            itemsInput = argv[i + 1];
        }else if (arg == "-o" && i + 1 < argc) {
            outputFilename = argv[i + 1];
        }
        else if (arg == "-g" && i + 1 < argc) {
            if (std::stoi(argv[i + 1]) < 1) {
                std::cout << "Number of generations provided is invalid. Provide [int] number higher than 0" << std::endl;
                isEverythingOk = false;
            }
            else {
                numberOfGenerations = std::stoi(argv[i + 1]);
            }
        }
        else if (arg == "-c" && i + 1 < argc) {
            if (std::stod(argv[i + 1]) <1) {
                std::cout<<"Capacity provided is invalid. Provide number higher than 0"<<std::endl;
                isEverythingOk = false;
            }
            else {
                weightLimit = std::stod(argv[i + 1]);
            }
        }
        else if(arg=="-n"&&i+1<argc) {
            if(std::stoi(argv[i+1])<1){
                std::cout << "Number of individuals in a generation provided is invalid. Provide [int] number higher than 0" << std::endl;
                isEverythingOk = false;
            }
            else {
                sizeOfPopulation = std::stoi(argv[i + 1]);
            }
        }
    }
    if (isEverythingOk == false) {
        exit(0);
    }
    // Loading items from file
    std::vector<Item> items = loadItems(itemsInput); 

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 1);

    // Initialization of the population
    std::vector<std::vector<int>> population(sizeOfPopulation, std::vector<int>(items.size()));
    for (int i = 0; i < sizeOfPopulation; i++) {
        for (int j = 0; j < items.size(); j++) {
            population[i][j] = distribution(generator);
        }
    }

    std::ofstream outputFile(outputFilename);
    // Main function loop
    for (int i = 0; i < numberOfGenerations; i++) {
        // Selection of two solutions by using tournament selection
        std::pair<std::vector<int>, std::vector<int>> parents = selection(population, items, weightLimit);

        // Crossover the two solutions to produce a new solution
        std::vector<int> offspring(items.size());
        for (int j = 0; j < items.size(); j++) {
            offspring[j] = (j < items.size() / 2) ? parents.first[j] : parents.second[j];
        }

        // Mutation of new solution
        mutate(offspring);

        // Replacing the worst solution in the population with the new solution
        int worstIndex = 0;
        for (int j = 1; j < sizeOfPopulation; j++) {
            if (calculateFitness(population[j], items, weightLimit) <
                calculateFitness(population[worstIndex], items, weightLimit)) {
                worstIndex = j;
            }
        }
        population[worstIndex] = offspring;

        // Output the best solution for this generation
        double bestFitness = -1.0;
        std::vector<int> bestSolution;
        for (int j = 0; j < sizeOfPopulation; j++) {
            double fitness = calculateFitness(population[j], items, weightLimit);
            if (fitness > bestFitness) {
                bestFitness = fitness;
                bestSolution = population[j];
            }
        }

        outputFile << "Generation " << i+1 << ": ";
        double totalWeight = 0.0;
        double totalValue = 0.0;
        for (int j = 0; j < bestSolution.size(); j++) {
            if (bestSolution[j] == 1) {
                outputFile << items[j].name << " (";
                outputFile << "weight: " << items[j].weight << ", ";
                outputFile << "value: " << items[j].value << ") ";
                totalWeight += items[j].weight;
                totalValue += items[j].value;
            }
        }
        outputFile << "Total weight: " << totalWeight << ", Total value: " << totalValue << std::endl;
    }

    // Finding the best solution in the population
    int bestIndex = 0;
    for (int i = 1; i < sizeOfPopulation; i++) {
        if (calculateFitness(population[i], items, weightLimit) >
            calculateFitness(population[bestIndex], items, weightLimit)) {
            bestIndex = i;
        }
    }
    std::vector<int> bestSolution = population[bestIndex];

    // Printing items in the best solution
    double weight = 0.0;
    int value = 0;
    outputFile << "Best solution: " << std::endl;
    for (int i = 0; i < bestSolution.size(); i++) {
        if (bestSolution[i] == 1) {
            outputFile << "  " << items[i].name << " (weight: " << items[i].weight << ", value: " << items[i].value << ")"
                << std::endl;
            weight += items[i].weight;
            value += items[i].value;
        }
    }
    outputFile << "Total weight: " << weight << std::endl;
    outputFile << "Total value: " << value << std::endl;
    outputFile.close();
}
