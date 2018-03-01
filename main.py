from io_ import *


def simulation():
    global rides, vehicles, T
    for step in range(T):
        for vehicle in vehicles:
            if vehicle.active:
                vehicle.move()
            else:
                rides_to_remove = []
                best_ratio = -1
                best_ride = -1
                for ride in rides:
                    steps_needed = vehicle.steps_to(ride.start_pos) + ride.distance
                    how_good_it_is = ride.how_good_it_is(steps_needed, step)
                    quotient = how_good_it_is/steps_needed
                    if quotient > best_ratio and how_good_it_is > 0:
                        best_ratio = steps_needed
                        best_ride = ride
                    if ride.end_step < step:
                        rides_to_remove.append(ride)
                if best_ride != -1:
                    vehicle.rides.append(best_ride.id)
                    rides_to_remove.append(best_ride)

                for rtr in rides_to_remove:
                    rides = rides[rides != rtr]


files = ["a_example", "b_should_be_easy", "c_no_hurry", "d_metropolis", "e_high_bonus"]

for filename in files:
    R, C, F, N, B, T, rides, vehicles = read_input("files/" + filename + ".in")
    simulation()
    output(vehicles, "files/" + filename + ".out")
