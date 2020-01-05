from cs50 import get_string

# Prevent user entering empty string
while True:
    text = get_string("Text: ")
    if text:
        break

letters = 0
words = 1
sentences = 0

# Get characters info
for i, char in enumerate(text):
    if char in [".", "!", "?", ":"]:
        sentences += 1
    elif char.isalpha():
        letters += 1
        if text[i - 1] == " ":
            words += 1

# Calculate the readability score
L = letters * 100 / words
S = sentences * 100 / words
index = round(0.0588 * L - 0.296 * S - 15.8)

# Print result
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade", index)
