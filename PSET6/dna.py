import csv
import sys


"""Initial Data Setup"""
# user input for desired data
if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)
# import csv and convert to database
file1 = csv.reader(open(sys.argv[1], 'r'), delimiter=',')
data = []
for line in file1:
    data.append(line)
STRlist = []
STRlist = data[0]
STRlist.remove('name')
data.remove(STRlist)
#import txt
txt_file = open(sys.argv[2], "r")
sequence = txt_file.read()
txt_file.close()


"""Subroutines"""


# DNA sequencing function
def sequencer(dna, STR_L):
    # setup decoded list
    STR_D = []
    # loop through dna for each STR
    for STR in STR_L:
        # reset
        construct = ""
        run = 0
        max_run = 0
        pos = 0
        prev = 0
        # loop through to check runs
        for i in dna:
            construct += i
            if STR in construct and pos - len(STR) == prev:
                run += 1
                construct = ""
                prev = pos
            elif STR in construct:
                run = 1
                construct = ""
                prev = pos
            if run > max_run:
                max_run = run
            pos += 1
        STR_D.append(str(max_run))
    # return data
    return STR_D


# Function to identify the unknown DNA
def identifier(STR_U, database):
    identity = ""
    for person in database:
        identity = person.pop(0)
        if person == STR_U:
            break
        else:
            identity = "No match"
    return print(identity)


"""Main Function"""
# function callouts
identifier(sequencer(sequence, STRlist), data)
