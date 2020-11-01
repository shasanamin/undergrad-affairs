import random
from Robot import Robot
from utils import *
from copy import deepcopy

# 8x8 grid as environment including wall
# 1 represents wall, 0, the absence of it

random.seed(675)  # 675 for 158 generations; 431 for 2500 generations, 41 for 1520 gen at 16 population size
prob_mutation = 0.01

# include something in arena to test fitness or hard code?
test_arena = [[1, 1, 1, 1, 1, 1, 1, 1],
              [1, 0, 0, 0, 0, 0, 0, 1],
              [1, 0, 0, 0, 0, 0, 0, 1],
              [1, 0, 0, 0, 0, 0, 1, 1],
              [1, 0, 0, 0, 0, 0, 1, 1],
              [1, 0, 0, 0, 0, 0, 0, 1],
              [1, 0, 0, 0, 0, 0, 0, 1],
              [1, 1, 1, 1, 1, 1, 1, 1]]
# train_arena includes markers (say 2) for fitness calculation
train_arena = [[1, 1, 1, 1, 1, 1, 1, 1],
               [1, 2, 2, 2, 2, 2, 2, 1],
               [1, 2, 0, 0, 0, 0, 2, 1],
               [1, 2, 0, 0, 0, 2, 1, 1],
               [1, 2, 0, 0, 0, 2, 1, 1],
               [1, 2, 0, 0, 0, 0, 2, 1],
               [1, 2, 2, 2, 2, 2, 2, 1],
               [1, 1, 1, 1, 1, 1, 1, 1]]

# create and maintain population of say 30 robots
static_pop_size = 30

Robots = []
# create static_pop_size number of robots with random responses
for i in range(static_pop_size):
    robo = Robot()
    for j in range(32):
        robo.response.append(random.randint(0, 3))
    Robots.append(robo)

CurrentGen = Robots
Num_Generations = 10000  # can alternatively continue looping till desired fitness achieved
avg_fitness_over_gens = []
for generation in range(Num_Generations):
    NextGen = []
    random.shuffle(CurrentGen)
    for parents in range(0, len(CurrentGen), 2):
        recombination(CurrentGen[parents], CurrentGen[parents+1])
        mutation(CurrentGen[parents], prob_mutation)
        mutation(CurrentGen[parents+1], prob_mutation)
        Off1_fitness = fitness(CurrentGen[parents], 28, deepcopy(train_arena))
        Off2_fitness = fitness(CurrentGen[parents+1], 28, deepcopy(train_arena))
        for copies in range(Off1_fitness*Off1_fitness):  # can select even higher number of fit individuals but that would be unrealistic/ not simulating how evolution really takes place
            NextGen.append(CurrentGen[parents])
        for copies in range(Off2_fitness*Off2_fitness):
            NextGen.append(CurrentGen[parents+1])

    # len(NextGen) corresponds to total generation fitness
    CurrentGen = random.sample(NextGen, static_pop_size)
    Pop_Fitness = 0
    for num in range(len(CurrentGen)):
        Pop_Fitness += fitness(CurrentGen[num], 28, deepcopy(train_arena))

    Avg_Fitness = Pop_Fitness/len(CurrentGen)  # max_fitness = 20 because 20 markers
    avg_fitness_over_gens.append(Avg_Fitness)
    print("Generation", generation, "'s average fitness:", Pop_Fitness / len(CurrentGen))
    if Avg_Fitness > 17:
        print('An elite population of Wall-Following Robots has evolved after', generation, 'generations')
        break


print("One random robot, represented as '++', would now demonstrate abilities in following arena:")
for i in range(len(test_arena)):
    print(test_arena[i])
print()
print()
Showbaz = CurrentGen[5]
num_moves = 50  # capable of looping and not just traversing once
for something in range(num_moves):
    Showbaz.run()
    arena_disp = test_arena
    temp_ar = arena_disp[Showbaz.coordinates[0]][Showbaz.coordinates[1]]
    arena_disp[Showbaz.coordinates[0]][Showbaz.coordinates[1]] = '++'
    for xyz in range(len(arena_disp)):
        print(arena_disp[xyz])
    print()
    print()
    arena_disp[Showbaz.coordinates[0]][Showbaz.coordinates[1]] = temp_ar
