# sys to use exit func
import sys

# getting number
num = input("Number: ")

# if num is word exit
if num.isdigit() == False:
    sys.exit("INVALID")

# reversing input to go through later
rev = ''.join(reversed(num))

# creating and filling up list firts char then to int
list1 = []
list2 = []
list1.extend(rev[::2])
list2.extend(rev[1::2])
list1 = [int(i) for i in list1]
list2 = [int(i) for i in list2]

# * evry other number
cc2 = 0
for i in list2:
    cc2 = cc2 + i * 2

# adding it all up
cc1 = cc2
for i in list1:
    cc1 = cc1 + i

# if modulo = 0 then check to see the type of cc and print
if cc1 % 10:
    if int(num[0]) == 4:
        sys.exit("VISA")

    amex = [34, 37]
    if int(num[0] + num[1]) in amex:
        sys.exit("AMEX")

    masterc = [51, 52, 53, 54, 55]
    if int(num[0] + num[1]) in masterc:
        sys.exit("MASTERCARD")
else:
    print("INVALID")

# couldnt figure out why 4111111111111111 didnt comeout as visa b4
if cc1 % 10 == 0:
    if int(num[0]) == 4:
        sys.exit("VISA")