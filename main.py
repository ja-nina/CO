from reader import Reader
from reader import SolutionReader

my_problem = Reader.read("data/f.txt")
SolutionReader.read("solutions/f.txt", my_problem)

print(my_problem.simulate())


#PLAN:
#symulacja działa wolno - można ją usprawnić zamiast obiektów używając dict, no ale na teraz tak jest wygodniej,
#o usprawnianiu takim fest raczej później się pomyśli
# frequented streets: discart non-frequented ones
# zbyt wolne auta: jeśli passują jako jedyne jakieś ulice to można je tam zablokować, żeby już nie blokowały nic
# szczególnie jeśli ta dana ulica jest na początku jakoś

# pomysły(preprocessing)
# z początku sprawdzić najbardziej blocked ulice i zwiększyć ich green time o jeden w okresie
#