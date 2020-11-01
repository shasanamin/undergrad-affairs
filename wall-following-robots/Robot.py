class Robot:
    def __init__(self):
        self.arena = []  # M x M (square) matrix describing environment. 1 = wall. 0 = no wall
        self.state = 0
        # Response is list with ith index containing response (0 = do_nothing, 1 = move_forward
        # 2 = turn_right, 3 = turn_left) of robot if found in state i
        self.response = []   # 5 sensors, so 32 states (element long list) right now
        self.head_ang = 90  # represents angle at which robot's head is facing North = 0, E = 90, S = 180, W = 270
        self.coordinates = [1, 1]  # Top Left Cell (where wall currently) set as reference (0,0)

    # Sensing
    # assumption: not sensing out of arena - valid here because arena is wall 'padded' here
    def sens_fl(self):  # returns reading of Front Left sensor. 1 if wall in that is on front left (NW from head)
        if self.head_ang == 0:
            return self.arena[self.coordinates[0] - 1][self.coordinates[1] - 1]
        elif self.head_ang == 90:
            return self.arena[self.coordinates[0] - 1][self.coordinates[1] + 1]
        elif self.head_ang == 180:
            return self.arena[self.coordinates[0] + 1][self.coordinates[1] + 1]
        else:
            return self.arena[self.coordinates[0] + 1][self.coordinates[1] - 1]

    def sens_f(self):  # returns reading of Front sensor
        if self.head_ang == 0:
            return self.arena[self.coordinates[0] - 1][self.coordinates[1]]
        elif self.head_ang == 90:
            return self.arena[self.coordinates[0]][self.coordinates[1] + 1]
        elif self.head_ang == 180:
            return self.arena[self.coordinates[0] + 1][self.coordinates[1]]
        else:
            return self.arena[self.coordinates[0]][self.coordinates[1] - 1]

    def sens_fr(self):
        if self.head_ang == 0:
            return self.arena[self.coordinates[0] - 1][self.coordinates[1] + 1]
        elif self.head_ang == 90:
            return self.arena[self.coordinates[0] + 1][self.coordinates[1] + 1]
        elif self.head_ang == 180:
            return self.arena[self.coordinates[0] + 1][self.coordinates[1] - 1]
        else:
            return self.arena[self.coordinates[0] - 1][self.coordinates[1] - 1]

    def sens_l(self):
        if self.head_ang == 0:
            return self.arena[self.coordinates[0]][self.coordinates[1] - 1]
        elif self.head_ang == 90:
            return self.arena[self.coordinates[0] - 1][self.coordinates[1]]
        elif self.head_ang == 180:
            return self.arena[self.coordinates[0]][self.coordinates[1] + 1]
        else:
            return self.arena[self.coordinates[0] + 1][self.coordinates[1]]

    def sens_r(self):
        if self.head_ang == 0:
            return self.arena[self.coordinates[0]][self.coordinates[1] + 1]
        elif self.head_ang == 90:
            return self.arena[self.coordinates[0] + 1][self.coordinates[1]]
        elif self.head_ang == 180:
            return self.arena[self.coordinates[0]][self.coordinates[1] - 1]
        else:
            return self.arena[self.coordinates[0] - 1][self.coordinates[1]]

    # have to keep track of robot's front/ head
    def check_state(self):  # check sensors and calculates state
        sens = []
        sens.append(self.sens_fl())
        sens.append(self.sens_f())
        sens.append(self.sens_fr())
        sens.append(self.sens_l())
        sens.append(self.sens_r())

        # come up with more appropriate, scalable method for what is being done below
        if (sens[0] == 0 or sens[0] == 2) and (sens[1] == 0 or sens[1] == 2) and (sens[2] == 0 or sens[2] == 2) and (sens[3] == 0 or sens[3] == 2) and (sens[4] == 0 or sens[4] == 2):
            state = 0
        elif (sens[0] == 0 or sens[0] == 2) and (sens[1] == 0 or sens[1] == 2) and (sens[2] == 0 or sens[2] == 2) and (sens[3] == 0 or sens[3] == 2) and sens[4] == 1:
            state = 1
        elif (sens[0] == 0 or sens[0] == 2) and (sens[1] == 0 or sens[1] == 2) and (sens[2] == 0 or sens[2] == 2) and sens[3] == 1 and (sens[4] == 0 or sens[4] == 2):
            state = 2
        elif (sens[0] == 0 or sens[0] == 2) and (sens[1] == 0 or sens[1] == 2) and (sens[2] == 0 or sens[2] == 2) and sens[3] == 1 and sens[4] == 1:
            state = 3
        elif (sens[0] == 0 or sens[0] == 2) and (sens[1] == 0 or sens[1] == 2) and sens[2] == 1 and (sens[3] == 0 or sens[3] == 2) and (sens[4] == 0 or sens[4] == 2):
            state = 4
        elif (sens[0] == 0 or sens[0] == 2) and (sens[1] == 0 or sens[1] == 2) and sens[2] == 1 and (sens[3] == 0 or sens[3] == 2) and sens[4] == 1:
            state = 5
        elif (sens[0] == 0 or sens[0] == 2) and (sens[1] == 0 or sens[1] == 2) and sens[2] == 1 and sens[3] == 1 and (sens[4] == 0 or sens[4] == 2):
            state = 6
        elif (sens[0] == 0 or sens[0] == 2) and (sens[1] == 0 or sens[1] == 2) and sens[2] == 1 and sens[3] == 1 and sens[4] == 1:
            state = 7
        elif (sens[0] == 0 or sens[0] == 2) and sens[1] == 1 and (sens[2] == 0 or sens[2] == 2) and (sens[3] == 0 or sens[3] == 2) and (sens[4] == 0 or sens[4] == 2):
            state = 8
        elif (sens[0] == 0 or sens[0] == 2) and sens[1] == 1 and (sens[2] == 0 or sens[2] == 2) and (sens[3] == 0 or sens[3] == 2) and sens[4] == 1:
            state = 9
        elif (sens[0] == 0 or sens[0] == 2) and sens[1] == 1 and (sens[2] == 0 or sens[2] == 2) and sens[3] == 1 and (sens[4] == 0 or sens[4] == 2):
            state = 10
        elif (sens[0] == 0 or sens[0] == 2) and sens[1] == 1 and (sens[2] == 0 or sens[2] == 2) and sens[3] == 1 and sens[4] == 1:
            state = 11
        elif (sens[0] == 0 or sens[0] == 2) and sens[1] == 1 and sens[2] == 1 and (sens[3] == 0 or sens[3] == 2) and (sens[4] == 0 or sens[4] == 2):
            state = 12
        elif (sens[0] == 0 or sens[0] == 2) and sens[1] == 1 and sens[2] == 1 and (sens[3] == 0 or sens[3] == 2) and sens[4] == 1:
            state = 13
        elif (sens[0] == 0 or sens[0] == 2) and sens[1] == 1 and sens[2] == 1 and sens[3] == 1 and (sens[4] == 0 or sens[4] == 2):
            state = 14
        elif (sens[0] == 0 or sens[0] == 2) and sens[1] == 1 and sens[2] == 1 and sens[3] == 1 and sens[4] == 1:
            state = 15
        elif sens[0] == 1 and (sens[1] == 0 or sens[1] == 2) and (sens[2] == 0 or sens[2] == 2) and (sens[3] == 0 or sens[3] == 2) and (sens[4] == 0 or sens[4] == 2):
            state = 16
        elif sens[0] == 1 and (sens[1] == 0 or sens[1] == 2) and (sens[2] == 0 or sens[2] == 2) and (sens[3] == 0 or sens[3] == 2) and sens[4] == 1:
            state = 17
        elif sens[0] == 1 and (sens[1] == 0 or sens[1] == 2) and (sens[2] == 0 or sens[2] == 2) and sens[3] == 1 and (sens[4] == 0 or sens[4] == 2):
            state = 18
        elif sens[0] == 1 and (sens[1] == 0 or sens[1] == 2) and (sens[2] == 0 or sens[2] == 2) and sens[3] == 1 and sens[4] == 1:
            state = 19
        elif sens[0] == 1 and (sens[1] == 0 or sens[1] == 2) and sens[2] == 1 and (sens[3] == 0 or sens[3] == 2) and (sens[4] == 0 or sens[4] == 2):
            state = 20
        elif sens[0] == 1 and (sens[1] == 0 or sens[1] == 2) and sens[2] == 1 and (sens[3] == 0 or sens[3] == 2) and sens[4] == 1:
            state = 21
        elif sens[0] == 1 and (sens[1] == 0 or sens[1] == 2) and sens[2] == 1 and sens[3] == 1 and (sens[4] == 0 or sens[4] == 2):
            state = 22
        elif sens[0] == 1 and (sens[1] == 0 or sens[1] == 2) and sens[2] == 1 and sens[3] == 1 and sens[4] == 1:
            state = 23
        elif sens[0] == 1 and sens[1] == 1 and (sens[2] == 0 or sens[2] == 2) and (sens[3] == 0 or sens[3] == 2) and (sens[4] == 0 or sens[4] == 2):
            state = 24
        elif sens[0] == 1 and sens[1] == 1 and (sens[2] == 0 or sens[2] == 2) and (sens[3] == 0 or sens[3] == 2) and sens[4] == 1:
            state = 25
        elif sens[0] == 1 and sens[1] == 1 and (sens[2] == 0 or sens[2] == 2) and sens[3] == 1 and (sens[4] == 0 or sens[4] == 2):
            state = 26
        elif sens[0] == 1 and sens[1] == 1 and (sens[2] == 0 or sens[2] == 2) and sens[3] == 1 and sens[4] == 1:
            state = 27
        elif sens[0] == 1 and sens[1] == 1 and sens[2] == 1 and (sens[3] == 0 or sens[3] == 2) and (sens[4] == 0 or sens[4] == 2):
            state = 28
        elif sens[0] == 1 and sens[1] == 1 and sens[2] == 1 and (sens[3] == 0 or sens[3] == 2) and sens[4] == 1:
            state = 29
        elif sens[0] == 1 and sens[1] == 1 and sens[2] == 1 and sens[3] == 1 and (sens[4] == 0 or sens[4] == 2):
            state = 30
        else:
            state = 31
        return state

    def bumped_into_wall(self):
        if self.arena[self.coordinates[0]][self.coordinates[1]] == 1:
            return True
        return False

    def turn_right(self):  # do not change cell yet
        self.head_ang = (self.head_ang + 90) % 360
        return

    def turn_left(self):
        self.head_ang = self.head_ang - 90
        if self.head_ang == -90:  # to cater for case where facing N initially
            self.head_ang = 270
        return

    def move_forward(self):  # don't move if will go out of arena or bumped into wall
        arena_h = len(self.arena)
        if arena_h > 0:
            arena_w = len(self.arena[0])  # square matrix so self.arena[0] works
        if self.head_ang == 0:  # take one step North
            if self.coordinates[0] != 0:
                self.coordinates[0] -= 1
                if self.bumped_into_wall():  # hit wall and come back/ stay on original pos
                    self.coordinates[0] += 1
        elif self.head_ang == 90:
            if self.coordinates[1] != (arena_w - 1):
                self.coordinates[1] += 1
                if self.bumped_into_wall():
                    self.coordinates[1] -= 1
        elif self.head_ang == 180:
            if self.coordinates[0] != (arena_h - 1):
                self.coordinates[0] += 1
                if self.bumped_into_wall():
                    self.coordinates[0] -= 1
        else:
            if self.coordinates[1] != 0:
                self.coordinates[1] -= 1
                if self.bumped_into_wall():
                    self.coordinates[1] += 1
        return

    def do_nothing(self):
        # literally do nothing
        return

    def run(self):
        mystate = self.check_state()
        resp = self.response[mystate]
        if resp == 0:
            self.do_nothing()
            # print("Literally did nothing")
        elif resp == 1:
            self.move_forward()
            # print("Moved Forward")
        elif resp == 2:
            self.turn_right()
            # print("Turned Right")
        elif resp == 3:
            self.turn_left()
            # print("Turned Left")
        else:
            print("Unexpected Response Received")
        return
