class Ride:
    def __init__(self, a, b, x, y, s, f, i):
        self.start_pos = [a, b]
        self.end_pos = [x, y]
        self.start_step = s
        self.end_step = f
        self.id = i
        self.distance = abs(a - x) + abs(b - y)

    def steps_to(self, new_position):
        return abs(self.current_position[0] - new_position[0]) + abs(self.current_position[1] - new_position[1])

    def how_good_it_is(self, steps_needed, current_step):
        return self.end_step - (current_step+steps_needed)

