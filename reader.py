from problem import Problem
from classes.street import Street
from classes.car import Car
class Reader:
    @staticmethod
    def read(path):
        with open(path, 'r') as file:
            line0 = file.readline().strip().split()

            [t, i, s, c, p] = [int(element) for element in line0]
            problem = Problem(t, i, s, c, p)

            ##adding streets
            id = 0
            while id < s:
                [start, end, name, length] = file.readline().strip().split()
                problem.add_Street(Street(int(start), int(end), name, int(length), id))
                id+=1

            #adding cars
            id = 0
            problem.create_dictionary()
            while id < c:
                road = file.readline().strip().split()
                len_road = int(road[0])
                problem.add_Car(Car(id, len_road))
                for street_name in road[1:]:
                    street_id = problem.name_to_id[street_name]
                    problem.Cars[id].add_to_road(street_id, problem.Streets[street_id].end)
                    problem.Cars[id].LB += problem.Streets[street_id].length
            
                id+=1
        problem.add_Lights()
        return problem

class SolutionReader:
    @staticmethod
    def read(path, problem):
        with open(path, 'r') as file:
            no_lights = int(file.readline().strip())
            i = 0
            while i < no_lights:
                id_light = int(file.readline().strip())
                no_streets = int(file.readline().strip())
                s = 0
                start_green = 0
                while s < no_streets:
                    [st_name, len_gth] = file.readline().strip().split()
                    st_id = problem.name_to_id[st_name]
                    problem.Lights[id_light].starting[st_id] = start_green
                    problem.Lights[id_light].schedule[st_id] = int(len_gth)
                    problem.Lights[id_light].add_street(st_id)
                    start_green += int(len_gth)
                    s += 1
                problem.Lights[id_light].cycle = start_green
                i += 1
