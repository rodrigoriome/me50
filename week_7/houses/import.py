from sys import argv, exit
from cs50 import SQL
import csv

if len(argv) != 2:
    print("Please provide a valid spreadsheet")
    exit(1)

db = SQL("sqlite:///students.db")

with open(argv[1], "r") as file:
    reader = csv.DictReader(file)
    for row in reader:
        names = row['name'].split(' ')
        if len(names) == 2:
            names.insert(1, None)
        db.execute("INSERT INTO students (first, middle, last, house, birth) values (?, ?, ?, ?, ?)", names[0], names[1], names[2], row["house"], row["birth"])
