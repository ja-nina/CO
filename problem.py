from numpy.core.fromnumeric import argmax
from classes.car import Car
from classes.street import Street
from classes.light import Light
from copy import deepcopy
import random

import numpy as np
import  heapq
THERESHOLD = 4
class Problem:
    def __init__(self, t, lights, streets, cars, premia):
        self.T = t
        self.lights = lights
        self.streets = streets
        self.premia = premia
        self.cars = cars
        self.used_streets = set()
        self.clogged = dict()
        self.arrival_at_end = dict()

        self.Streets = []
        self.Lights = []
        self.Cars = []

        self.privi = [0]*self.cars
        self.name_to_id = dict()
        self.id_to_name = dict()
        self.id_to_len = dict()

    #_toers
    def add_Street(self, street):
        self.Streets.append(street)

    def add_Car(self, car):
        self.Cars.append(car)

    def add_Lights(self):
        for id in range(self.lights):
            self.Lights.append(Light(id, self.T))

    def create_dictionary(self):
        for street in self.Streets:
            self.name_to_id[street.name] = street.id
            self.id_to_name[street.id] = street.name
            self.id_to_len[street.id] = street.length

    ###
    def _situate_cars(self):
        for i, car in enumerate(self.Cars):
            first_road = car.road[0]
            self.Cars[i].number = self.Streets[first_road].carEnter()
            self.Cars[i].current_location = first_road

    def _is_first(self, id_car, id_street):
        return self.Cars[id_car].number == self.Streets[id_street].passed

    def car_crosses(self, car_id, street_id, light_id, time):
        self.Streets[street_id].carLeave()
        self.Lights[light_id].used[time] = True
        next_street = self.Cars[car_id].cross(time)
        if not self.Cars[car_id].ostatnia_prosta(next_street):
            self.Cars[car_id].number = self.Streets[next_street].carEnter()
        return next_street

    #dosyć wolny, ale i tak jest koncepcyjny ...

    #defines used/unused streets
    def preporcessor(self):
        
        for car in self.Cars:
            car.LB = sum([self.Streets[id].length for id in car.road])
            if car.LB <= self.T:
                for street in car.road:
                    self.used_streets.add(street)
            
        #print("USED STR: ", [self.id_to_name[str] for str in self.used_streets])

        # for street in self.used_streets:
        #     id_light = self.Streets[street].end
        #     self.Lights[id_light].add_street(street)
        #     self.Lights[id_light].add_to_schedule(street, 1)

        for street in self.Streets:
            id_light = street.end
            self.Lights[id_light].add_street(street.id)
            self.Lights[id_light].add_to_schedule(street.id, 1)
        #*JUST FOR TESTS*





#purpose of this one is defeated - integrated in get best fit.
    def removeUnused(self):
        for street in self.arrival_at_end.keys():
            if not self.arrival_at_end[street]:
                if street in self.Lights[self.Streets[street].end].streets and random.random() > 0.7:
                    length = self.Lights[self.Streets[street].end].schedule.pop(street, None)
                    self.Lights[self.Streets[street].end].streets.remove(street)
                    self.Lights[self.Streets[street].end].cycle -=length


    def reset(self):
        for street in self.Streets:
            street.reset()
        for light in self.Lights:
            light.reset()
        for car in self.Cars:
            car.reset()

    def simulate(self):

        self._situate_cars()
        profit = 0
        ended = set()

        for street in self.used_streets:
            self.clogged[street] = set([])
            self.arrival_at_end[street] = []
        #this approach definitely requires proprocessing sincle the used streets set has to be initialised
        
        #przydałoby się robić listę kiedy arrive 

        #top 5% zapchane 

        for second in range(self.T):

            for car in self.Cars:
                
               
                if car.id in ended: continue
                if self.privi[car.id] == 1:
                    weight = THERESHOLD
                else:
                    weight = 1

                current_street = car.current_location
                next_light = self.Streets[current_street].end
                is_green = self.Lights[next_light].is_green(current_street, second)

                if self.Cars[car.id].ostatnia_prosta(current_street):
                    bonus_car = self.T - self.Streets[current_street].length - car.last_intersection
                    if bonus_car >= 0:
                        profit += self.premia + bonus_car
                    ended.add(car.id)
                    self.privi[car.id] = 1
                    continue

                if not self.Lights[next_light].can_cross(second):
                    continue

                is_first = self._is_first(car.id, current_street)
                self.Cars[car.id].update_transit(second, self.Streets[current_street].length)
                is_in_transit = self.Cars[car.id].in_transit
                if is_green and is_first and not is_in_transit:
                    self.arrival_at_end[current_street].append((second, weight)) # time, weight
                    self.Lights[next_light].usedStreets.add(current_street)
                    self.car_crosses(car.id, current_street, next_light, second)
                    
                    

                if not is_in_transit and not is_green:
                    self.Lights[next_light].usedStreets.add(current_street) # wait what?, i think if should only be done so if the car is privileged, but here no privileges so ok
                    if current_street in self.used_streets: self.clogged[current_street].add(weight)
                    if current_street in self.arrival_at_end: self.arrival_at_end[current_street].append((second, weight)) # time, weight


        ##get most clogged streets

        return profit



    #increase time for top 2% clogged streets
    def MoreTimeForClogged(self):
        k = round(1 + 0.02*self.streets)
        augment_group = heapq.nlargest(k, self.clogged, key = lambda x: len(self.clogged[x])/max(self.Lights[self.Streets[x].end].schedule[x], 1))
        #print(augment_group)
        for street in augment_group:
            if random.random() > 0.7 and street in self.used_streets: #metaparametr
                self.Lights[self.Streets[street].end].schedule[street] += 1
                self.Lights[self.Streets[street].end].cycle +=1

    def get_best_fit(self, thereshold = THERESHOLD):
        #based on the times of arrival at an intersection 
        #we see how the density function looks, then on the basis of the density functions we choose then to activate what green light
        for light in self.Lights:
            light.cycle = max(light.cycle + light.differenceInCycle,1)
            light.differenceInCycle = 0
            plots = np.array(np.zeros(light.cycle))
            ids = []
            to_be_deleted = []
            to_be_added = []
            #print("LIGHT: ", light.id, " SCHEDULE: ", light.schedule, light.starting, light.cycle)
            for street_id, length in light.schedule.items():

                my_plot = np.zeros(light.cycle)
                #da sie uproscic ale juz japierdole jezu
                if street_id in light.usedStreets:
                    for (measurement, weight) in self.arrival_at_end[street_id]:
                        my_plot[measurement%light.cycle] += weight


                suma = sum(my_plot)
                #print("suma: ", suma)
                if suma >= thereshold and street_id in self.used_streets:
                    if light.schedule[street_id] > 0:
                        ids.append(street_id)
                        plots = np.vstack([plots, self.sum_over(length, plot = my_plot, cycle = light.cycle)])
                    else:
                        to_be_added.append((street_id, suma))
                else:
                    to_be_deleted.append((street_id, length))

            #print(light.id, to_be_deleted)
            #there shuld be a mechanism to add those unreached streets somehow
            for street, sum_of_plot in to_be_added:
                if sum_of_plot > thereshold:
                    light.schedule[street] = 1
                    light.starting[street] = None
                    light.differenceInCycle += 1
            if light.cycle > 1:
                for unreached_street, length in to_be_deleted:
                    light.schedule[unreached_street] = 0
                    light.starting[unreached_street] = None
                    light.cycle -= length

                next_row = 0
                sec = 0
                #print("size of plots: ", plots.size, ids)
                sums_of_rows = plots.sum(axis= 0)

                while(len(ids) > 0):
                    plots = np.delete(plots, (next_row), axis = 0)
                    #print("IDS: ", ids, "sec: ", sec, plots, light.cycle)
                    next_row = argmax(plots, axis = 0)[sec]         #########HERE DELETE STUFF THO
                    light.starting[ids[next_row]] = sec
                    sec = sec + light.schedule[ids[next_row]]
                    ids.pop(next_row)
            #print("LIGHT: ", light.id, " SCHEDULE: ", light.schedule, light.starting, light.cycle)
                #get a way where you yiu reduce number of streetswhere there is nothing really !

            #print("SCHEDULE FOR LIGHT ", light.id, light.starting, light.schedule)
            #not only arrival time is weighed but also each war waiting

    def sum_over(self, length, plot, cycle):
        #print("basically the plot: ", np.remainder(np.array(plot), cycle))
        if len(plot)>0:
            convolution = np.convolve(np.remainder(np.array(plot), cycle), np.ones(length, dtype=np.int), mode='valid')
            maximum = np.amax(convolution)
            pom = np.zeros(cycle)
            pom[0:cycle - length + 1] = convolution/max(maximum, 1)
            return pom
        else:
            return np.zeros(cycle)