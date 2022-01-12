from reader import Reader
from reader import SolutionReader
from classes.solution import CopyLights
from copy import deepcopy 
from datetime import datetime
#---------------------------------------------------------
#TABU SEARCH





#SolutionReader.read("solutions/a.txt", my_problem)

#my_problem.get_best_fit()
#my_problem.preporcessor()
#print(my_problem.Lights[499].cycle, sorted(list(my_problem.Lights[499].starting.items()), key = lambda x: x[1]))
# print(my_problem.simulate())
# my_problem.removeUnused()
# my_problem.get_best_fit()
# my_problem.MoreTimeForClogged()
# my_problem.get_best_fit()
# my_problem.reset()
#print(my_problem.Lights[557].cycle, sorted(list(my_problem.Lights[499].starting.items()), key = lambda x: x[1]))


###
#perform loop with augementation- lengthening
#PLAN:
#symulacja działa wolno - można ją usprawnić zamiast obiektów używając dict, no ale na teraz tak jest wygodniej,
#o usprawnianiu takim fest raczej później się pomyśli
# frequented streets: discart non-frequented ones
# zbyt wolne auta: jeśli passują jako jedyne jakieś ulice to można je tam zablokować, żeby już nie blokowały nic
# szczególnie jeśli ta dana ulica jest na początku jakoś

# pomysły (preprocessing)
# z początku sprawdzić najbardziej blocked ulice i zwiększyć ich green time o jeden w okresie
#past polutions 1002, 4.5m , 1.3m, 3.9m, 0.9m, 1.7m

#quite a big problem with d  - max 1.7m

# first you write down the current solution
my_problem = Reader.read("data/b.txt")
my_problem.preporcessor()
start_time = datetime.now()
score = my_problem.simulate()
#my_problem.get_best_fit()
#my_problem.reset()
#score = my_problem.simulate()
my_problem.get_best_fit()
my_problem.reset()
score = my_problem.simulate()
end_time = datetime.now()
time_diff = (end_time - start_time)
print("score: ", score, " execution time: ", time_diff.total_seconds()*1000000, " (micros)")
# for light in my_problem.Lights:
#     print(light.id, " : schedule  ", light.schedule, light.starting)

# prev_score = my_problem.simulate()
# CopyL= CopyLights(my_problem.lights)
# for light in my_problem.Lights:
#         CopyL.add_light(light)

# i = 0
# while(True):
#     if i%7== 6:
#         print("BOOM!")
#         my_problem.MoreTimeForClogged() #gotta find a way to add them back
#     # tabu search for cars, fitness would be wiating time far all cars
#     j = 0
#     my_problem.get_best_fit()
#     my_problem.reset()
#     #priviledged cars should aviod clogged streets and have low bounds
#     score = my_problem.simulate()
#     print(score)
#     #for light in my_problem.Lights:
#         #print(light.id, " : schedule  ", light.schedule, light.starting)
#     i+=1
#     #if i%10 == 9:
#     #    for light in my_problem.Lights:
#     #        print(light.id, " : schedule  ", light.schedule, light.starting)




#---------------------------------------------------------------
#tabu
