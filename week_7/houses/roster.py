from sys import argv, exit
from cs50 import SQL

if len(argv) != 2:
    print("Please enter a house name")
    exit(1)

db = SQL("sqlite:///students.db")
query = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", argv[1])

for row in query:
    middle = " " + row['middle'] if row['middle'] else ""
    print(f"{row['first']}{middle} {row['last']}, born {row['birth']}")
