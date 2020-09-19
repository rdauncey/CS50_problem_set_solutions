from sys import argv, exit
import csv
import cs50


def main():

    # Check correct usage
    if len(argv) != 2:
        print("Usage: roster.py house")
        exit(1)

    # Make sure house given in correct format
    house = argv[1].lower().capitalize()

    # Check command line argument is a valid house
    houses = ["Gryffindor", "Slytherin", "Hufflepuff", "Ravenclaw"]
    if house not in houses:
        print("Give valid house name.")
        exit(1)

    # Open students database
    db = cs50.SQL("sqlite:///students.db")

    # Get sorted list of students in given house; rows is a list of dictionaries, each of which represents a student
    rows = db.execute("SELECT first, middle, last, birth FROM students WHERE house = :hs ORDER BY last, first", hs=house)

    # Print students
    for row in rows:
        if row['middle'] is not None:
            print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")

        else:
            print(f"{row['first']} {row['last']}, born {row['birth']}")

    exit(0)


main()
