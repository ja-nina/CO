from classes.light import Light
import copy

class CopyLights:
    def __init__(self, number_of_lights): #later improve
        self.lights = []

    def erase_memo(self):
        self.lights = []

    def add_light(self, light):
        self.lights.append(copy.deepcopy(light))
    
