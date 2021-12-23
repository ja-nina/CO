class Car:
    def __init__(self, id, length):
        self.id = id
        self.length = length
        self.road = []
        self.intersections = []  #wsm niepotrzebne idk
        
        ### for the simulation
        self.current_location = -1  # will return road
        self.seq_road = 0
        self.last_intersection = -1
        self.in_transit = False
        self.number = None
        self.LB = 0

    def add_to_road(self, street_id, end_intersection_id):
        self.road.append(street_id)
        if len(self.road) != self.length:
            self.intersections.append(end_intersection_id)

    def print(self, dic):
        print("Car: ", self.id, [dic[road] for road in self.road])

    def update_transit(self, time, current_len):
        self.in_transit = (time - self.last_intersection) < (current_len)
        self.waiting = (time - self.last_intersection) - (current_len)
        if self.last_intersection == -1:
            self.in_transit = False

    def cross(self, time):
        self.seq_road += 1
        self.last_intersection = time
        self.current_location = self.road[self.seq_road]
        return self.current_location

    def is_end(self, time, length):
        return self.length - 1 == self.seq_road and (time - self.last_intersection >= length)

    def ostatnia_prosta(self, id_street):
        return self.road[-1] == id_street

    def reset(self):
        self.current_location = -1  # will return road
        #self.current_len = 0
        self.seq_road = 0
        self.last_intersection = -1
        self.in_transit = False
        self.number = None