# Knapsack Problem Solver
C++ Project assignment for semester credit. Program uses genetic algorithm to solve knapsack problem.

Task description: 

Write a program that is designed to solve the knapsack problem using a genetic algorithm. Program should take
input data from an input file that hols a set of items with their names, masses and values. Program should find a
subset of items with maximal value and total weight that does not exceed capacity of the knapsack provided by
user.<br/>
This program can be used from the command line by passing the following switches: <br/>
-i name.txt : input file with a set of items<br/>
-o name.txt : output file with the best solutions in all generations<br/>
-c value : knapsack capacity<br/>
-g value : number of generations<br/>
-n value : number of individuals in a generation<br/><br/><br/>


Example usage (using g++ compiler): ./{nameOfFile.exe} -i example_items.txt -o example_solution.txt -c 5.0 -g 1000 -n 100<br/><br/>

Result of a solution is provided in example_solution.txt in repository.
