from classes.car import Car
from classes.street import Street
from classes.light import Light
from copy import deepcopy
class Problem:
    def __init__(self, t, lights, streets, cars, premia):
        self.T = t
        self.lights = lights
        self.streets = streets
        self.premia = premia
        self.cars = cars

        self.Streets = []
        self.Lights = []
        self.Cars = []

        self.name_to_id = dict()
        self.id_to_name = dict()
        self.id_to_len = dict()

    #adders
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
    def simulate(self):
        self._situate_cars()
        profit = 0
        ended = set()

        for second in range(self.T):
            for car in self.Cars:

                if car.id in ended: continue

                current_street = car.current_location
                next_light = self.Streets[current_street].end
                is_green = self.Lights[next_light].is_green(current_street, second)

                if self.Cars[car.id].ostatnia_prosta(current_street):
                    bonus_car = self.T - self.Streets[current_street].length - car.last_intersection
                    if bonus_car >= 0:
                        profit += self.premia + bonus_car
                    ended.add(car.id)
                    continue

                if not self.Lights[next_light].can_cross(second):
                    continue

                is_first = self._is_first(car.id, current_street)
                self.Cars[car.id].update_transit(second, self.Streets[current_street].length)
                is_in_transit = self.Cars[car.id].in_transit
                if is_green and is_first and not is_in_transit:
                    self.car_crosses(car.id, current_street, next_light, second)
        return profit
