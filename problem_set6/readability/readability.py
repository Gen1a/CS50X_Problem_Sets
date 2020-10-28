from cs50 import get_string

def main():
    while True:
        text = get_string("Text: ")
        if len(text) > 0:
            break
    index = get_grade(text)
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print("Grade", index)


def get_grade(text):
    # Coleman-Liau index: index = 0.0588 * L - 0.296 * S - 15.8
    # index = rounded to nearest whole number
    # L = average number of letters per 100 words
    # S = average number of sentences per 100 words
    # Calculate letters in string
    letters = 0
    for char in text:
        if char.isalpha():
            letters += 1
    # Calculate words in string
    words = 1
    for char in text:
        if char.isspace():
            words += 1
    # Calculate sentences in string
    sentences = 0
    for char in text:
        if char in [".", "!", "?"]:
            sentences += 1
    # Calculate and return Coleman-Liau index
    index = (0.0588 * (float(letters) * 100 / float(words))) - (0.296 * (float(sentences) * 100 / float(words))) - 15.8
    return round(index)

# statement to initiate main function
if __name__ == "__main__":
    main()