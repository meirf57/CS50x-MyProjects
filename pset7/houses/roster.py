import sys
import csv
from cs50 import SQL

# if input error of argv exit program (expect 2)
if len(sys.argv) != 2:
    print("Usage: python import.py House")
    sys.exit()

# in order to execute sql code
db = SQL("sqlite:///students.db")

# saving house name given
house = (sys.argv[1]).capitalize()

# creating list of items in db
ans = db.execute(f"SELECT first,middle,last,birth FROM students WHERE house = (?) ORDER BY last,first", house)

# looping trough list and printing items
res = []
for a in ans:
    for b in a.values():
        res.append(b)
    if res[1] == None:
        res[1] = " "
        x = "".join(res[0:3])
    else:
        x = res[0]+" "+res[1]+" "+res[2]
    print(f"{x}, born {res[-1]}")
    res.clear()