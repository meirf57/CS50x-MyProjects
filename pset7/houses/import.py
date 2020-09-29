import sys
import csv
from cs50 import SQL

# if input error of argv exit program (expect 2)
if len(sys.argv) != 2:
    print("Usage: python import.py data.csv")
    sys.exit()

# in order to execute sql code
db = SQL("sqlite:///students.db")


# opening csv file given
file = open(sys.argv[1], "r")

# clearing database so that it doesnt add more
db.execute("DELETE FROM students")


# going through csv file to insert to database
with file:

    reader = csv.DictReader(file)

    for row in reader:

        # dividing name to first, middle, last
        name = row["name"].split(" ")
        if len(name) <= 2:
            name.insert(1, None)
        row["first"] = name[0]
        row["middle"] = name[1]
        row["last"] = name[2]

        # inserting data to database students
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?,?,?,?,?)",
                   row["first"], row["middle"], row["last"], row["house"], row["birth"])