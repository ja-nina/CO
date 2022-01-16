﻿# Combinatorial Optimization Project 21/22
## Basic Information
Authors:
 - Janina Żukowska, 148278
 - Mateusz Kądziela, 148271

Subject : Combinatorial optimization
Student group : SI3
# Algorithms used
## Structure of data
Everything is stored as an object of a class. There are classes for streets, cars, intersections, solutions. There is also a class Solver with the algorithms. Schedules are stored as a vector of pointers to intersections.
Evaluation of a solution works by simulating 

## Simple approach
For a simple program, we decided to use preprocessing of the data.  It works by


## Complex approach
For a complex program, we decided to use genetic algorithm. It uses preprocessing to generate initial pop and improves on its scores.  
We create a starting population, by mutating  the solution obtained from preprocessing, until we get 30 different solutions. Then we run a loop that generates new population. It has 2 stopping conditions: no improvement of best score in 50 iterations or reaching the time limit. To ensure that the best solution is in the next population, we move 2 of the best solutions to the new population. Then we perform tournament selection 14 times to get 2 parents. We then use crossover on them, and each child may be mutated with the probability of 0.25. 

Unfortunately, genetic approach is limited if the instance problem is huge. It is due to the fact that simulating such big schedules takes quite some time. We tried to fix it, by creating an approximate evaluation, but none of our approaches to it worked.
### Mutation
Creates a new solution by changing the previous one.
### Crossover
A simple crossover that iterates over all intersections of both parents and randomly assigns one intersection to one child and the other to the second child.
## How to run
We used C++ 14 standard.
To compile the program, run a command 
To run the program with simple solution, run a command`./program.exe 1 [instance]`
To run the program with complex solution, run a command`./program.exe 2 [instance]`
## Bibliography
We mostly relied on our knowledge from the previous year of studies and searching for some stuff on the internet. CPP documentation and stackoverflow were both pretty helpful.
Two, useful pieces of code from stackoverflow were:

 - https://stackoverflow.com/questions/9345087/choose-m-elements-randomly-from-a-vector-containing-n-elements
 - https://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair


