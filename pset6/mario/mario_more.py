# this is a function to prompt for a digit
def number():
    global num
    num = input("height: ")
    if num.isdigit() == False:
        num = 0
    num = int(num)

    
# using function
number()
# checking that digit is 1-8
while num < 1 or num > 8:
    number()

# for inerloop
sign = num - 1

# printing # in pyramid shape
for i in range(num):
    for j in range(sign):
        print(" ", end="")
    x = num - sign
    print(f"{'#' * x}  {'#' * x}")
    sign = sign - 1