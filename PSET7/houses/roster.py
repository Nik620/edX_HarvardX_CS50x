# will produce house rosters of each group
# program command line argument with house
import csv
import sys
from cs50 import SQL

# user input
if len(sys.argv) != 2:
    print("Usage: python roster.py nameHouse")
    exit(1)
house = (sys.argv[1].lower()).title()
if house not in ['Gryffindor', 'Slytherin',\
    'Hufflepuff', 'Ravenclaw']:
    print("Usage: python roster.py nameHouse")
    exit(1)

# sorting hat converts table to list of dicts
db = SQL("sqlite:///students.db")
house = (sys.argv[1].lower()).title()
students = db.execute("SELECT * \
    FROM students WHERE house = (?)\
    ORDER BY last, first", house)

# output list
for student in students:
    if student['middle'] == None:
        print(student['first'], student['last'], student['birth'])
    else:
        print(student['first'], student['middle'],student['last'], student['birth'])
