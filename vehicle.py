class Vehicle:
    def __init__(self, i):
        self.id = i
        self.current_position = [0, 0]
        self.rides = []
        self.active = False
        self.steps_missing = 0

    def steps_to(self, new_position):
        return abs(self.current_position[0] - new_position[0]) + abs(self.current_position[1] - new_position[1])

    def move(self):
        if self.active:
            self.steps_missing -= 1
            if self.steps_missing == 0:
                self.active = False

