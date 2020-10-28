from cs50 import SQL
from sys import argv, exit

if len(argv) != 2:
    print("Usage: python roster.py house\nTry again.")
    exit(1)
else:
    # Open the students.db file to prepare for usage
    db = SQL("sqlite:///students.db")
    # Read house from command line argument and covnvert to string
    house = str(argv[1])
    # db.execute with SELECT query returns list of dicts
    for row in db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house):
        print(f"{row['first']} {row['middle'] + ' ' if row['middle'] is not None else ''}{row['last']}, born {row['birth']}")
