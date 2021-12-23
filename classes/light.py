class Light:
    def __init__(self, id, time): #later improve
        self.id = id
        self.streets = set()
        self.usedStreets = set()
        self.schedule = dict() #how long a street has green light
        self.starting = dict() #when green starts
        self.cycle = 0
        self.t = time
        self.used = [False for _ in range(self.t + 1)]  #usedarray
        self.differenceInCycle = 0

    def add_street(self, id_street):
        self.streets.add(id_street)
        self.cycle = max(sum(self.schedule.values()), 0)

    def is_green(self, street_id, time):
        #print(self.streets)
        if street_id not in self.streets: return False
        if self.starting[street_id] is None: return False
        return (self.starting[street_id] <= time % self.cycle) and (time % self.cycle < (self.starting[street_id] + self.schedule[street_id]))

    def can_cross(self, time):
        return not self.used[time]
    
    def add_to_schedule(self, street_id, time):
        self.schedule[street_id] = time
        self.starting[street_id] = self.cycle
        self.cycle = self.cycle + time

    def reset(self):
        self.used = [False for _ in range(self.t + 1)]
        self.usedStreets = set()
        
