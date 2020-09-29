import sys
import csv

# if input error of argv exit program (expect 3)
if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    sys.exit()

# opening and saving data file
file = open(str(sys.argv[1]), 'r')

# opening and aving data file to dict
people = {}
with file:
    reader = csv.DictReader(file)
    # print(list(reader))
    for row in reader:
        people[row['name']] = [row['AGATC'], row['AATG'], row['TATC']]


# opening and saving text file
seqt = open(str(sys.argv[2]), 'r')
text = seqt.read()
lent = len(text)

# function to check text file for sequence of letters


def check(word):
    a2 = 1
    a5 = 1
    a4 = 0
    lenw = len(word)
    for i in range(lent + 1):
        j = i + lenw
        k = j + lenw
        l = i - lenw
        if text[i:j] == word:
            a3 = 1
            while text[i:k] == word * 2:
                a2 = a2 + 1
                if text[l:j] != word * 2:
                    a2 = a2 - 1
                i = i + lenw
                if a2 > a3:
                    a4 = a2
                else:
                    a4 = a3
            if a3 > a4:
                a4 = a3
            i = j
    if a4 > 1:
        a4 = a4 + 1
    return a4


# using function to check these letters
c1 = str(check("AGATC"))
c2 = str(check("AATG"))
c3 = str(check("TATC"))

# saving sequence of text file to list
c4 = [c1, c2, c3]
# print(c4)
# print(people)

# function to compare 2 lists


def cmp(a, b):
    return (a > b) - (a < b)


# comparing own list to data
for i in people:
    equal = cmp(people.get(i), c4)
    if equal == 0:
        print(i)
        sys.exit()

print("No match")