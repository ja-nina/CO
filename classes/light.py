class Light:
    def __init__(self, id, time): #later improve
        self.id = id
        self.streets = set()
        self.schedule = dict()
        self.starting = dict()
        self.cycle = 0

        self.used = [False for _ in range(time + 1)]  #usedarray

    def add_street(self, id_street):
        self.streets.add(id_street)
        self.cycle = max(sum(self.schedule.keys()), 0)

    def is_green(self, street_id, time):
        if street_id not in self.streets: return False
        return (self.starting[street_id] <= time % self.cycle) and (time % self.cycle < (self.starting[street_id] + self.schedule[street_id]))

    def can_cross(self, time):
        return not self.used[time]
