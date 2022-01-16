# Combinatorial Optimization Project 21/22
## Basic Information
Authors:
 - Nina Żukowska, 148278
 - Mateusz Kądziela, 148271

Subject : Combinatorial optimization
Student group : SI3
# Algorithms used
## Structure of data
Everything is stored as an object of a class. There are classes for streets, cars, intersections, solutions. There is also a class Solver with the algorithms. Schedules are stored as a vector of pointers to intersections.
Evaluation of a solution works by simulating 

## Simple approach
For a simple program, we decided to use preprocessing of the data.  It is also used in complex aproach, so it will be explained there. We decided to go with it, because it produced the best results for a simple and quick algorithm.

## Complex approach
For a complex program, we decided to use genetic algorithm. We thought that searching the solution space would be a great approach to the problem.
It uses preprocessing to generate initial pop and improves on its scores. 
### How it works
We create a starting population, by mutating the solution obtained from preprocessing, until we get 30 different solutions. Then we run a loop that generates new population. It has 2 stopping conditions: no improvement of best score in 50 iterations or reaching the time limit. To ensure that the best solution is in the next population, we move 2 of the best solutions to the new population. Then we perform tournament selection 14 times to get 2 parents. We then use crossover on them, and each child may be mutated with the probability of 0.25. 

Unfortunately, genetic approach is limited if the instance problem is huge. It is due to the fact that simulating such big schedules takes quite some time. We tried to fix it, by creating an approximate evaluation, but none of our approaches to it worked.

### Preprocessing
Since the most basic type of solution would be to only assign 1 to each street in its corresponding light schedule, we modified this approach making it a bit smarter.
For each car we compute its lower bound for completion time, which is the sum of lengths of streets on its road( excluding the street it starts on). 
Then we check whether a given street hosts any car which chas a chance to arrive before the deadline(LB <= T), only if this is the case, the streets gets some "green time" at its given intersection. The perks or this approach are twofold:
firstly - the cycles or intersections are smaller allowing for other streets on that intersection to have more "green time", secondly, cars that wouldn't finish either way get stuck on that street, which prohibits them from potentially clogging other vital streets down the road.
### Mutation

A solution can be "mutated" in a variety of ways but we focused on 3 main approaches:
1. Changing the order of streets in the schedule
2. Changing the streets which appear in the schedule 
3. Changing the length of "green time" a street gets in its given schedule

- Heuristic 1. 
In the first approach, we iterate over Problematic Lights*, for each light, we perform a maximum of r swaps( r < 0.3* number of used streets), which means that at most 60% of the order gets modified, but in practice, this number is much lower. Lights are iterated through on the basis of their problematicness, and we iterate over maximum 25% to 75% of the most problematic lights, but this number too is much smaller, since additional randomness is introduced.

- Heuristic 2.
For the deletion we check a random 1% of the streets in the simulation, if a given street did not host any cars in the schedule, it is removed from the schedule.

- Heuristic 3. 
For the change of "green time", we iterate over all intersections, if there exists such a street in the schedule, for which the quotient of time cars on this street spend waiting to the "green time" of that street is two times bigger than summed value of this quotient for each street divided by the number of the streets on that schedule, we increase the green time of that street by 1.

- Hashing - before a mutated solution is admitted we check if it was already considered by checking the hash of that solutiuon, the hash function is the sum of hash functions of lights multiplied by the light id. The hash function for the light is the sum of a prime numbers corresponding to the incomming streets times the prime numbers corresponding to the green time (and order) of those streets.

### Crossover
A simple crossover that iterates over all intersections of both parents and randomly assigns one intersection to one child and the other to the second child.
## How to run
We used C++ 14 standard.
To compile the program, run a command .
To run the program with simple solution, run a command`./program.exe 1`.
To run the program with complex solution, run a command`./program.exe 2`.
Next, you put the problem instance into stdin.
## Bibliography
We mostly relied on our knowledge from the previous year of studies and searching for some stuff on the internet. CPP documentation and stackoverflow were both pretty helpful.
Two, useful pieces of code from stackoverflow were:

 - https://stackoverflow.com/questions/9345087/choose-m-elements-randomly-from-a-vector-containing-n-elements
 - https://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair


