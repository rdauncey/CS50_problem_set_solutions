from cs50 import get_string
from numpy import round


def main():

    text = get_string("Text: ")

    # Word, letter and sentence count
    words, letters, sentences = count_wls(text)

    # Avg letters & sentences per 100 words: count * (100 / words)
    L = letters * (100 / words)
    S = sentences * (100 / words)

    # Coleman-Liau index:
    cl = 0.0588 * L - 0.296 * S - 15.8
    cl = int(round(cl))

    if cl > 15:
        print("Grade 16+")
    elif cl < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {cl}")


# Can all be done in one function, which is faster because it only loops through once


def count_wls(text):
    """Input string of text, output is a tuple of word count, letter count,
    sentence count"""

    words = 0
    letters = 0
    sentences = 0

    indicators = [".", "?", "!"]

    for c in text:

        # Word count
        if c == " ":
            words += 1

        # Letter count
        elif c.isalpha():
            letters += 1

        # Sentence count
        elif c in indicators:
            sentences += 1

    # Add one to word count to include the final word of the text
    words += 1

    return words, letters, sentences


# Alternatively, for more reusable code we can have the functions separate


def count_words(text):
    """Input a string of text, output is word count"""

    word_count = 0

    # Space indicates end of a word
    for c in text:
        if c == " ":
            word_count += 1

    # There is no space at the end of the text, so must + 1
    word_count += 1
    return word_count


def count_letters(text):
    """Input a string of text, output is the count of upper and lower
    case letters"""

    letter_count = 0

    # For each character, check it is a letter
    for c in text:
        if c.isalpha():
            letter_count += 1

    return letter_count


def count_sentences(text):
    """Input a string of text, output is a count of the sentences, defined
    as any sequence of chars ending in a period, exclamation mark or question mark"""

    indicators = [".", "?", "!"]
    sentence_count = 0

    for c in text:
        if c in indicators:
            sentence_count += 1

    return sentence_count


main()