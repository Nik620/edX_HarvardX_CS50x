# will import student data from csv into a db
import csv
import sys
from cs50 import SQL

# user input
if len(sys.argv) != 2:
    print("Usage: python import.py file.csv")
    exit(1)
students = list(csv.reader(open(sys.argv[1], 'r'), delimiter=','))
students.remove(['name','house','birth'])

# converting file
def name_separator(name):
    names = name.split()
    return names

# parsing name each row
for student in students:
    student[0] = name_separator(student[0])

# database creation
db = SQL("sqlite:///students.db")
for student in students:
    if len(student[0]) == 2:
        db.execute("INSERT INTO students (first, last, house, birth) \
            VALUES(?, ?, ?, ?)", student[0][0], student[0][1], \
            student[1], student[2])
    if len(student[0]) == 3:
        db.execute("INSERT INTO students (first, middle, last, house, birth) \
            VALUES(?, ?, ?, ?, ?)", student[0][0], student[0][1], \
            student[0][2], student[1], student[2])
