from sys import argv, exit
import csv
import cs50


def main():

    # Check correct number of command line arguments
    if len(argv) != 2:
        print("Usage: import.py filename.csv")
        exit(1)

    # Open students database
    db = cs50.SQL("sqlite:///students.db")

    # Read csv file
    with open(argv[1], "r") as file:

        # Create DictReader
        reader = csv.DictReader(file)

        # Iterate over csv file
        for row in reader:

            # Parse name into first, middle, last
            first, middle, last = parse_name(row["name"])

            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       first, middle, last, row["house"], row["birth"])

    exit(0)


def parse_name(full_name):
    """Input argument is the full name as a string, output is tuple giving
    first, middle (which may be none) and last name"""

    # Determine the indexes of the full_name array of the space chars
    space_indexes = []
    for i in range(len(full_name)):
        if full_name[i] == " ":
            space_indexes.append(i)

    # We may assume space_indexes contains either one or two indexes
    first = full_name[:space_indexes[0]]

    # If only one space, there is no middle name
    if len(space_indexes) == 1:
        last = full_name[space_indexes[0] + 1:]
        middle = None

    # If two spaces, there is a middle and a last name
    elif len(space_indexes) == 2:
        middle = full_name[space_indexes[0] + 1:space_indexes[1]]
        last = full_name[space_indexes[1] + 1:]

    else:
        middle = None
        last = None

    return first, middle, last


main()