from cs50 import get_float


# asking for amount due
def due():
    global a_due
    a_due = input("Changed owed: ")
    tmp = a_due.replace('.', '')
    if tmp.isdigit() == False:
        a_due = 0
    a_due = float(a_due)


# using function and check
due()
while a_due < 0.001:
    due()


# multiplying by 100 for easier math
change = int(a_due * 100)


# setting counter
coins = 0


# counting coins to give back
while change > 24:
    change -= 25
    coins += 1
while change > 9:
    change -= 10
    coins += 1
while change > 4:
    change -= 5
    coins += 1
while change > 0:
    change -= 1
    coins += 1

print(coins)