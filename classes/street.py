class Street:
    def __init__(self,start, end, name, length, id):
        self.start = start
        self.end = end
        self.name = name
        self.id = id
        self.length = length

        ###end for debugging purposes
        self.passed = 0
        self.entered = 0



    def carEnter(self):
        self.entered += 1
        #self.queue.append(car_id)
        return self.entered - 1

    def carLeave(self):
        self.passed += 1
        return self.passed

    def is_empty(self):
        return self.passed == self.entered

    def get_first(self, time):
        bool_waits = time >= self.queue[0][1] + self.length
        return self.queue[0], bool_waits
