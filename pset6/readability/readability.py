import sys


# getting text
text = input("Text: ")

# setting counters (word is 1 for last {can improve})
ltrs = 0
words = 1
sntcs = 0
point = ['.', '!', '?']

# iterating through text to count lt/ w/ s
for i in text:
    if i.isalpha():
        ltrs = ltrs + 1
    elif i == ' ':
        words = words + 1
    elif i in point:
        sntcs = sntcs + 1

# Coleman Liau method
inl = (ltrs / words) * 100
ins = (sntcs / words) * 100
cli = round((0.0588 * inl) - (0.296 * ins) - 15.8)

# printing grade level
if cli < 1:
    print("Before Grade 1")

if cli >= 16:
    print("Grade 16+")
    sys.exit()

if cli > 1 & cli < 16:
    print(f"Grade {cli}")
    