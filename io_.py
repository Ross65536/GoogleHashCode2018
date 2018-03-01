import numpy as np
from ride import *
from vehicle import *


def read_input(filename):
    lines = open(filename).readlines()
    R, C, F, N, B, T = [int(val) for val in lines[0].split()]

    temp_rides = []
    temp_vehicles = []
    i = 0

    for j in range(1, F+1):
        temp_vehicles.append(Vehicle(j))

    for row in lines[1:]:
        a, b, x, y, s, f = [int(val) for val in row.split()]
        temp_rides.append(Ride(a, b, x, y, s, f, i))
        i += 1

    rides = np.array(temp_rides)
    vehicles = np.array(temp_vehicles)

    return R, C, F, N, B, T, rides, vehicles


def output(vehicles, name):
    with open(name, 'w') as f:
        for vehicle in vehicles:
            out = str(len(vehicle.rides))
            for ride in vehicle.rides:
                out += " " + str(ride)
            f.write(out)
            f.write('\n')




