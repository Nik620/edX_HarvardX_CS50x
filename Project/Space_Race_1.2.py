# -*- coding: utf-8 -*-
"""
Created on Tue Dec 15 14:23:07 2020
Released Thur Dec 17
@author: Nik P

SPACE RACE
Version 1.0
-Original release
Version 1.1
-Crash saved release
Version 1.2
-Stable file, added bomb drop funcitonality

This game is a modification of my moon race 
written in Ruby. This is the final project
for CS50's Intro to Computer Science.

The game code has been updated to funciton
with python, and modified to change the game.

This game will challenge the user to guess
a number between 1 and 10. The closer they
are, the further they will go. They will
face an opponent, who will try guessing
and beating them.

Game will feature both classic moonrace and
the newly developed space race modes.
"""


"""
INITIALIZATION
-Libraries
-Game types
-Game difficulty settings
"""

# Import libraries
import sys
import random
import time

# Game type
moon, space = "M", "S"
game_type = (moon, space) # moonrace, spacerace

# Game difficulty settings (Easy, Medium, Hard, Custom)
E_moon = {'length': 15, 'warps': 1, 'aliens': 1, 'adjust': 1}
M_moon = {'length': 30, 'warps': 2, 'aliens': 2, 'adjust': 2}
H_moon = {'length': 45, 'warps': 4, 'aliens': 4, 'adjust': 4}
C_moon = {} # custom will be up to 100, 20, 20, 10
E_space = {'length': 15, 'warps': 1, 'aliens': 1, 'adjust': 1,
           'shields': 1, 'bombs': 1, 'comets': 1}
M_space = {'length': 30, 'warps': 2, 'aliens': 2, 'adjust': 2,
           'shields': 3, 'bombs': 2, 'comets': 2}
H_space = {'length': 45, 'warps': 4, 'aliens': 4, 'adjust': 4,
           'shields': 5, 'bombs': 3, 'comets': 4}
C_space = {} # custom will be up to 100, 20, 20, 10, 10, 10, 10
game_modes = {'M':{'E':E_moon,'M':M_moon,'H':H_moon,'C':C_moon}, 
              'S':{'E':E_space,'M':M_space,'H':H_space,'C':C_space}}


"""
GAME SETUP
-Warp/Alien Position Generator
-Number and Char Check functions
-Opponent Setup
-Dice Roll
-Board Creation
-Turn Operations
-Terminate Game
""" 

# Warp and alien position generator
def position_generator(pos1, pos2, length):
    # generate a position away from earth and moon
    # pos 1 is a list of either warp/alien positions
    # pos 2 is a list of either alien/warp positions
    # length is an integer for the board length
    # returns positions for warp or alien
    position = random.randint(1, length - 2)
    if (position in pos1) or (position in pos2):
        position = position_generator(pos1, pos2, length)
    return position

# Number value check
def num_check(min_val, max_val):
    # checks if custom mode inputs are valid
    # min_val is defined minimum
    # max_val is defined maximum
    # returns the number if valid
    number = input()
    if number.isnumeric() == False:
        print('Your response must be a number between:', min_val, 'and', max_val)
        number = num_check(min_val, max_val)
    elif int(number) < min_val or int(number) > max_val:
        print('Your response must be between:', min_val, 'and', max_val)
        number = num_check(min_val, max_val)
    return int(number)

# User input check
def char_check(chars):
    # checks over the user_input to see if in chars
    # chars is a list of characters user_input must match
    # outputs the user_input if value works
    user_input = input().upper()
    if not user_input in chars:
        print("...input not recognized, please try again:")
        user_input = char_check(chars)
    return user_input.upper()

# Opponent setup
def scroll_names(names):
    # flips between a list of names
    # names is a list
    # returns a randomly selected name
    print("Choosing character...")
    for i in range(2):
        for name in names:
            # this will scroll through list of names
            sys.stdout.write('\r' + name)
            sys.stdout.flush()
            time.sleep(0.2)
    name_select = names[random.randint(0, len(names) - 1)]
    print('\r' + name_select)
    return name_select
            
# Dice roll
def dice():
    numbers = (1,2,3,4,5,6)
    number = random.randint(min(numbers), max(numbers))
    for i in range(3):
        for val in numbers:
            sys.stdout.write('\r' + str(val))
            sys.stdout.flush()
            time.sleep(0.1)
    print('\r' + "Rolled a: " + str(number))
    return number

# Board setup for each turn (Moonrace only)
def set_board_M(character, length, w_pos, w_mov, 
                a_pos, a_mov):
    # sets up player board to play
    # character is a class, of the player
    # length is an int, length of board
    # w_pos is a list, position of warps
    # w_mov is an int, the adjustment due to warps
    # a_pos is a list, posiiton of aliens
    # a_mov is an int, the adjustment due to warps
    
    # setup movement, this is after diceroll
    movement = 0
    # returns a list for the game board
    board = []
    # space setup
    for i in range(0, length + 1): board.append(SPACE)
    # moon and earth setup
    board[0] = EARTH
    board[length] = MOON
    # check for warp and alien positions
    for i in w_pos: board[i] = WARP
    for j in a_pos: board[j] = ALIEN
    # collision with warp or alien
    if board[character.getPosition()] == WARP:
        print("Warp", w_mov, "spaces forward.")
        movement += w_mov
    elif board[character.getPosition()] == ALIEN:
        print("Alien pushes back", a_mov)
        movement -= a_mov
    # rocket position
    character.updatePosition(movement)
    board[character.getPosition()] = ROCKET
    if character.getPosition() == length:
        board[length] = MOON_FINISH
    if character.getPosition() == 0:
        board[0] = EARTH_START
    # print board
    for i in board: sys.stdout.write(i)
    sys.stdout.flush()
    print('')
    return character

# Board setup for each turn (Spacerace only)
def set_board_S(character, length, w_pos, w_mov, 
                a_pos, a_mov, c_pos, b_pos):
    # sets up player board to play
    # character is a class, of the player
    # length is an int, length of board
    # w_pos is a list, position of warps
    # w_mov is an int, the adjustment due to warps
    # a_pos is a list, posiiton of aliens
    # a_mov is an int, the adjustment due to warps
    # c_pos is a list, position of comets
    # b_pos is a list, position of bombs
    # returns updated character
    
    # setup movement, this is after diceroll
    movement = 0
    # returns a list for the game board
    board = []
    bomb_used = False
    # space setup
    for i in range(0, length + 1): board.append(SPACE)
    # moon and earth setup
    board[0] = EARTH
    board[length] = PLANET
    # check for warp and alien positions
    for w in w_pos: board[w] = WARP
    for a in a_pos: board[a] = ALIEN
    for c in c_pos: board[c] = COMET
    for b in b_pos: board[b] = BOMB
    # collision with warp
    if board[character.getPosition()] == WARP:
        print("Warp", w_mov, "spaces forward.", WARP)
        movement += w_mov
    # collision with alien
    elif board[character.getPosition()] == ALIEN:
        print("Alien pushes back", a_mov, ALIEN)
        movement -= a_mov
    # collision with comet
    elif board[character.getPosition()] == COMET:
        print("Ran into a comet.", COMET, "\nAttempting to blast it:")
        time.sleep(0.1)
        if len(character.getBombs()) > 0:
            character.useBomb()
            bomb_used = True
        else:
            character.useBomb()
            character.breakShield()
    # collision with bomb
    elif board[character.getPosition()] == BOMB:
        print("Ran into a bomb...", BOMB)
        character.breakShield()
        b_pos.remove(character.getPosition())
    # rocket position
    character.updatePosition(movement)
    board[character.getPosition()] = ROCKET
    # bomb drop
    if len(character.getBombs()) > 0 and \
        character.getPosition() != 0 and \
            character.getPosition != length and \
                bomb_used == False:
        print("Drop a bomb (Y/N)")
        if character.isHuman() == True:
            prompt = char_check(["Y", "N", ""])
        else:
            prompt = random.choice("YNNN") # 25% chance of bomb
            print(prompt)
        if prompt == "Y":
            character.useBomb()
            board[character.getPosition()] = BOMB_DROP
            print(len(character.getBombs()), "bomb(s) remain.", BOMB)
            b_pos.append(character.getPosition())
    # graphics for start and finish
    if character.getPosition() == length:
        board[length] = MOON_FINISH
    if character.getPosition() == 0:
        board[0] = EARTH_START
    # print board
    for i in board: sys.stdout.write(i)
    sys.stdout.flush()

    print('')
    return character, b_pos
    
# Turn operations
def turn(mode, character, length):
    # switches between player and bot
    # mode is a string either for moonrace or spacerace
    # name is a string of either player or bot
    # position is an int of player's position
    # switches to next players turn
    print(character.getName(), "your turn...")
    if mode == moon:
        sys.stdout.write('Press (enter) to roll the dice: ')
        input()
        number = dice()
    else:
        print("Shields available:")
        for i in character.getShields(): sys.stdout.write(i)
        print('')
        sys.stdout.flush()
        print("Bombs available:")
        for i in character.getBombs(): sys.stdout.write(i)
        print('')
        sys.stdout.write('\nPick a number from 1 to 6: ')
        if character.isHuman() == True:
            guess = num_check(1, 6)
        else:
            guess = random.randint(1,6)
            print('\n', guess)
        # rolling metrics
        number = dice()
        if guess == number:
            number += 2
            print("Bonus +2 move")
        print("Move", number)
    character.updatePosition(number)
    return character

def quit_game(prompt):
    if prompt == 'Q' or prompt == 'E':
        sys.exit()

"""
GRAPHICS
-Board items
-Astronaut graphics
-Robot graphics
-Player setup
"""

# Board items
EARTH, MOON, PLANET, SPACE = '🌎', '🌕', '🪐', '  '
ROCKET, WARP, ALIEN = '🚀', '🌌', '🛸'
PLAYER_M, PLAYER_F, BOT = '👨‍🚀', '👩‍🚀', '🤖'
SHIELD, BOMB, COMET = '🛡', '💣', '☄️'
EARTH_START, BOMB_DROP = '🚀🌎', '💣🚀'
MOON_FINISH, PLANET_FINISH ='🌕🚀', '🪐🚀'
NAMES = ['Viktor', '101101', 'Ivanov', '010100', 
         'Hector', '000101', 'Charli', '100011']

# Astronaut graphics
astronaut_file = open("Astronaut.txt", "r")
astronaut_image = astronaut_file.read()
astronaut_file.close()

# Robot graphics
robot_file = open("Robot.txt", "r")
robot_image = robot_file.read()
robot_file.close()

# Title Screen
cover_file = open("Cover.txt", "r")
cover_image = cover_file.read()
cover_file.close()

# Game Over Screen
dead_file = open("Dead.txt", "r")
dead_image = dead_file.read()
dead_file.close()

# Player setup
def player_setup():
    # Creates a new user to play
    # No input
    # Outputs a player name (string)
    print("What's your name?")
    user = input()
    if user == "": user = "Player"
    print("\nWhat is your gender (M/F/O)?")
    gender = char_check(["M", "F", "O", ""])
    if gender == 'M' or gender == 'O':
        user += PLAYER_M
    elif gender == 'F' or gender == '':
        user += PLAYER_F
    return user

# Moonrace mode player
class player_moon(object):
    # Replaces old definition to better store player data
    # name is a string for player name
    # position is an int of the current player position
    # image is the text file picture of player
    # length is an int, length of the field
    def __init__(self, image, length):
        self.name = player_setup()
        self.position = 0
        self.image = image
        self.length = length
    def getImage(self):
        return self.image
    def getName(self):
        # returns the name of the player
        return self.name
    def getPosition(self):
        # returns the position of the player
        return self.position
    def updatePosition(self, movement):
        # updates position based on movement & board len
        if (self.position + movement) >= self.length:
            self.position = self.length
        elif (self.position + movement) <= 0:
            self.position = 0
        else:
            self.position += movement

# Moonrace mode computer
class computer_moon(player_moon):
    # Replaces old definition to create computer
    def __init__(self, image, length):
        self.name = scroll_names(NAMES) + BOT
        self.position = 0
        self.image = image
        self.length = length

# Spacerace mode player
class player_space(player_moon):
    # Adds data to player_moon for space version
    # name is a string for player name
    # position is an int of the current player position
    # bombs is a list of bomb emojis
    # shields is a list of shield emojis
    # status is a bool of player losing all shields
    # image is a picture of the player
    # length is an int, length of the board
    # human is a bool, if computer then fals
    def __init__(self, bombs, shields, image, length):
        self.name = player_setup()
        self.position = 0
        self.bombs = bombs
        self.shields = shields
        self.status = True
        self.image = image
        self.length = length
        self.human = True
    def isHuman(self):
        # returns if player is a computer
        return self.human
    def getBombs(self):
        # returns a string of bombs
        return self.bombs
    def useBomb(self):
        # returns a boolean if bomb launch successful
        if len(self.bombs) == 0:
            print("Sorry, no bombs left..", BOMB)
            return False
        else:
            print("Used a bomb", self.bombs.pop())
            return True
    def getShields(self):
        # returns a list of shields
        return self.shields
    def breakShield(self):
        # returns one less shield
        print("Lost a shield", self.shields.pop())
        if len(self.shields) == 0:
            print("Sorry, no shields left..", SHIELD, "\nPlayer died..")
            self.status = False
    def getStatus(self):
        return self.status
    def printStatus(self):
        if self.status == False:
            print(self.name, "is dead and lost in space..")
        else:
            print()

# Spacerace mode computer
class computer_space(player_space):
    # Creates the computer for space version
    def __init__(self, bombs, shields, image, length):
        self.name = scroll_names(NAMES) + BOT
        self.position = 0
        self.bombs = bombs
        self.shields = shields
        self.status = True
        self.image = image
        self.length = length
        self.human = False


"""
GAME EXPERIENCE INITIALIZATION
-Title screen
-Game setup screens (mode, difficulty, custom)
-Game setup finalization
-Start screen
-Player input
-Opponent Setup
"""

# Title screen
print("\033[H\033[J") # clears screen
print(cover_image)
print("Space Race Version 1.2")
print("Released December 17, 2020")
print("Developed by Nik P. All images created by Nik P.")
print("\nPress (enter) to continue.")
input()


# Game setup screen
print("\033[H\033[J") # clears screen
print("GAME SETUP\n")
print("Select game mode (type M or S):")
print("(M) for Moonrace\n(S) for Spacerace")
mode = char_check([moon, space, ""])
if mode == "": 
    mode = moon
    print("Default Moonrace selected.")
print("\nSelect game difficulty (type E, M, H, or C):")
print("(E) Easy\n(M) Medium\n(H) Hard\n(C) Custom")
difficulty = char_check(["E", "M", "H", "C", ""])
if difficulty == "":
    difficulty = "E"
    print("Default easy mode selected.")

# Custom mode setup screen
if difficulty == 'C':
    print("\033[H\033[J") # clears screen
    print("GAME SETUP\nCustom Mode")
    print("\nWhat length would you like (10-100)?")
    response = num_check(10, 100)
    maximum = int(response * .25)
    game_modes[mode][difficulty]['length'] = response
    print("\nWarps desired (1-" + str(maximum) + "):")
    response = num_check(1, maximum)
    game_modes[mode][difficulty]['warps'] = response
    print("\nAliens desired (1-" + str(maximum) + "):")
    response = num_check(1, maximum)
    game_modes[mode][difficulty]['aliens'] = response
    print("\nWarp/Alien adjustment distance (1-10):")
    response = num_check(1, 10)
    game_modes[mode][difficulty]['adjust'] = response
    if mode == space:
        print("\nHow many shields would you like (1-10)")
        response = num_check(1, 10)
        game_modes[mode][difficulty]['shields'] = response
        print("\nHow many bombs would you like (1-10)")
        response = num_check(1, 10)
        game_modes[mode][difficulty]['bombs'] = response
        print("\nHow many comets would you like (1-" + str(maximum) + "):")
        response = num_check(1, maximum)
        game_modes[mode][difficulty]['comets'] = response

# Game setup finalization
winning_number = game_modes[mode][difficulty]["length"]
warp_qty = game_modes[mode][difficulty]["warps"]
warp_move = game_modes[mode][difficulty]["adjust"]
alien_qty = game_modes[mode][difficulty]["aliens"]
alien_move = game_modes[mode][difficulty]["adjust"]
warp_pos = []
alien_pos = []
for i in range(0, warp_qty): warp_pos.append(position_generator(
        warp_pos, alien_pos, winning_number))
for i in range(0, alien_qty): alien_pos.append(position_generator(
        alien_pos, warp_pos, winning_number))
if mode == space:
    comet_qty = game_modes[mode][difficulty]["comets"]
    shield_qty = game_modes[mode][difficulty]["shields"]
    bomb_qty = game_modes[mode][difficulty]["bombs"]
    comet_pos = []
    for i in range(0, comet_qty): comet_pos.append(position_generator(
            alien_pos, warp_pos, winning_number))
    shields = [SHIELD] * shield_qty
    bombs = [BOMB] * bomb_qty
    
# Start screen
print('\n' + str(winning_number), "spaces to victory.\n")
print('Press (enter) to begin or (Q/E) to quit.')
prompt = input().upper()
quit_game(prompt)
print("\033[H\033[J") # clears screen

# Player 1 Name input & opponent setup
print("Player 1 setup:")


# Opponent setup
if mode == moon:
    user = player_moon(astronaut_image, winning_number)
    print("\nWelcome", user.getName())
    print("\n\nFinding your opponent...")
    computer = computer_moon(robot_image, winning_number)
    print('\nYour opponent is:', computer.getName())
    print('\nPress (enter) to continue.')
    input()
elif mode == space:
    user = player_space(bombs.copy(), shields.copy(), astronaut_image,
                        winning_number) # found this caused duplicates w/o cpy
    users = [user]
    for i in range(2, 5):
        print("Player", i, "desired (Y/N)?")
        response = char_check(["Y", "N", ""])
        if response == "": response = "N"
        if response == "Y":
            print("Player", i, "setup:")
            users.append(player_space(bombs.copy(), shields.copy(), 
                                      astronaut_image, winning_number))
            # users.append(player_setup())
        else: break
    if len(users) == 1:
        "\n\nFinding your opponent..."
        users.append(computer_space(bombs, shields, 
                                    robot_image, winning_number))
        print('\nYour opponent is:', users[1].getName())
        

"""
GAMEPLAY
-Board initialization
-Game loop
-Final results
"""
# Board initialization
print("\033[H\033[J") # clears screen
if mode == moon:
    print(user.getName())
    # Player initialization
    user = set_board_M(user, winning_number, warp_pos, 
              warp_move, alien_pos, alien_move)
    print('\n\nversus\n\n')
    print(computer.getName())
    computer = set_board_M(computer, winning_number, warp_pos,
              warp_move, alien_pos, alien_move)
    # Print out rules
    print("\n\nYou've chosen the classic Moonrace!")
    print("Rules: Roll the dice, first to", winning_number,
              "wins. Warps will move you forward, aliens back.")
if mode == space:
    # Player initialization
    print("\nGet ready players!\n\n")
    bomb_pos = []
    for player in users:
        print(player.getName())
        player = set_board_S(player, winning_number, warp_pos, 
              warp_move, alien_pos, alien_move, comet_pos, bomb_pos)
    # Print out rules
    print("\n\nYou've chosen the new Spacerace!")
    print("Rules: Guess a number between 1 and 6.")
    print("The closer you are to the roll, the")
    print("further you'll go. You also have shields")
    print("to protect you from and bombs to blow up comets.")
    print("You can leave bombs against the opponent.")
    print("Comet/bomb damage is equal to a shield.")
print("\n\nPress (enter) to continue or (Q/E) to quit.")
prompt = input().upper()
quit_game(prompt)
print("\033[H\033[J") # clears screen

# Game loop
if mode == moon:
    while True:
        # User's turn
        user = turn(mode, user, winning_number)
        user = set_board_M(user, winning_number, 
                                  warp_pos, warp_move, alien_pos, alien_move)
        print("\nThe new position is:", user.getPosition(), '\n')
        time.sleep(3)
        # Computer's turn
        computer = turn(mode, computer, winning_number)
        computer = set_board_M(computer, winning_number, 
                                      warp_pos, warp_move, alien_pos, alien_move)
        print("\nThe new position is:", computer.getPosition(), '\n')
        # clear screen anc continue
        print("\nPress (enter) to continue or (Q/E) to quit.")
        prompt = input().upper()
        quit_game(prompt)
        print("\033[H\033[J") # clears screen
        # End game
        if user.getPosition() > winning_number - 1 or\
            computer.getPosition() > winning_number - 1:
            break
if mode == space:
    play_game = True
    while play_game:
        dead_players = 0
        for player in users:
            if player.getStatus() == False:
                print(player.getName(), "is dead..")
                next
            else:
                player = turn(mode, player, winning_number)
                player, bomb_pos = set_board_S(player, winning_number,
                               warp_pos, warp_move, alien_pos, 
                               alien_move, comet_pos, bomb_pos)
            print("\nThe new position is:", player.getPosition(), '\n')
        # clear screen anc continue
        print("\nPress (enter) to continue or (Q/E) to quit.")
        prompt = input().upper()
        quit_game(prompt)
        print("\033[H\033[J") # clears screen
        # End game
        end_match = False
        for player in users:
            if player.getPosition() > winning_number - 1:
                play_game = False
                break
            if player.getStatus() == False:
                dead_players += 1
        if dead_players == len(users):
            play_game = False
            break

# Display the result
if mode == moon:
    # Player wins
    if user.getPosition() > computer.getPosition():
        print(user.getName(), "won!")
        print(user.getImage())
    # Tie
    elif user.getPosition() == computer.getPosition():
        print(user.getName(), "&", computer.getName(), "tied!")
    # Computer wins
    else:
        print(computer.getName(), "won... sorry you lost..")
        print(computer.getImage())
if mode == space:
    # Check if all players died
    if dead_players == len(users):
        print("All players have died...")
        print(dead_image)
    else:
        # Reviewing scores
        scores = []
        for player in users:
            scores.append(player.getPosition())
        high_score = max(scores)
        # Checking for ties
        winners = []
        tie = False
        for player in users:
            if high_score == player.getPosition():
                winners.append(player)
        if len(winners) >= 2:
            tie = True
        # Check Tie vs. Non-tie wins
        if tie == False:
            print(winners[0].getName(), "wins!")
            print(winners[0].getImage())
        else:
            print("Its a tie between:\n")
            for winner in winners:
                print(winner.getName())