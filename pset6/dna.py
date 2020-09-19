import csv
from sys import argv, exit


def main():

    # Check for correct command line usage
    if len(argv) != 3:
        print("Usage: dna.py data.csv sequence.txt")
        exit(1)

    # Open the csv file and extract the header row
    csv_file = open(argv[1], newline='')
    reader = csv.reader(csv_file)
    header_row = next(reader)

    # Open the txt file and read into memory as string variable sequence
    sequence_file = open(argv[2], "r")
    sequence = sequence_file.read()

    # Create list to store the max lengths of each STR that appears in sequence
    str_list = []

    # For each STR referenced in the header row (neglect the first entry which is 'names')
    # find the max length of consecutive repeats of each STR
    for i in range(1, len(header_row)):
        str_list.append(str_count(sequence, header_row[i]))

    # For each row in the csv file, compare the [1:] entries to sequence_list
    # If there is a match, we print the name (row[0]) of the match; we assume there is
    # at most one match.
    # Note as we have already read the first row, this reads row 2 onwards
    found = False
    for row in reader:
        # compare
        if row[1:] == str_list:
            print(row[0])
            found = True

    # If no match found, print no match
    if found is False:
        print("No match")

    sequence_file.close()
    exit(0)


def str_count(seq, str_code):
    """Input the DNA sequence and the STR to be considered, output is the max length
    of a set of consecutive instances of the STR in the sequence, as a *string*"""

    # Get length of the STR in question and initialise counts and the index i
    length = len(str_code)
    counts = [0]
    i = 0

    while i < len(seq):

        # Check for an instance of the STR
        if seq[i:i + length] == str_code:

            count = 1
            end = False

            # Loop through to see how many consecutive repeats there are - check
            # blocks size of the length of the STR
            while end is False:
                if (i + length) > len(seq):
                    end = True

                i = i + length

                if seq[i:i + length] == str_code:
                    count += 1

                else:
                    end = True

            # Add the length of this set of repeats to counts
            counts.append(count)

        # If no instance, move to next index
        else:
            i += 1

    # Find the max length of a set of consecutive repeats, and convert to string
    # as the numbers in the csv are stored as strings
    str_count = str(max(counts))

    return str_count


main()