from cs50 import SQL
from sys import argv, exit
import csv

if len(argv) != 2:
    print("Usage: python import.py filename.csv\nTry again.")
    exit(1)
else:
    # Open the students.db file to prepare for usage
    db = SQL("sqlite:///students.db")
    # Create table in students.db (students table seems to be present already)
    # db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")
    # Open provided .csv-file and assign pointer
    with open(argv[1], "r") as file:
        reader = csv.DictReader(file)
        # Iterate through csv file and start writing students.db
        for row in reader:
            # split method which returns list of strings
            names = row["name"].split()
            # Write 'None' for student's middle name if not present -> shown as 'NULL' in database
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                names[0], None if len(names) == 2 else names[1], names[1] if len(names) == 2 else names[2], row["house"], int(row["birth"]))

    print("Import successful")

