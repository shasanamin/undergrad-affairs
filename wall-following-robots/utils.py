import random

# calculates fitness of "robot" by testing it for "time" number of cycles in "envir"onment/ arena
# max fitness = 20 because 20 markers
def fitness(robot, time, envir):
    fit_lvl = 0
    robot.arena = envir
    for i in range(time):
        if robot.arena[robot.coordinates[0]][robot.coordinates[1]] == 2:
            robot.arena[robot.coordinates[0]][robot.coordinates[1]] = 0
            fit_lvl += 1
        robot.run()
    robot.coordinates = [1, 1]
    return fit_lvl


def recombination(robot1, robot2):
    start = random.randint(0, 31)
    end = random.randint(start, 31)
    for i in range(start, end):
        temp = robot1.response[i]
        robot1.response[i] = robot2.response[i]
        robot2.response[i] = temp
    return


def mutation(robot, mut_prob):  # probability of mutation (float b/w 0 and 1)
    for i in range(32):
        r = random.random()
        # if r < (0.5 + mut_prob/2) and r > (0.5 - mut_prob/2):  # for more equitable distribution
        if r < mut_prob:
            robot.response[i] = random.randint(0, 3)
    return robot
