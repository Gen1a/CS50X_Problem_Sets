from sys import argv, exit
import csv

def main():
    # Check valid input
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)
    # Load CSV file
    with open(argv[1], "r") as csv_file:
        reader = csv.reader(csv_file) # pointer to csv_file loaded in memory
        # Extract a list of available sequences without name
        sequences = next(reader)[1:] # next() iterates row per row over file where 'reader' points, starting from column 1 till last column
        print(sequences)

        # Keep CSV file open while loading txt file
        with open(argv[2], "r") as txt_file:
            # store DNA-sequence in string
            dna = txt_file.read()
            # Calculate longest STR repeats
            max_repeats = [get_longest_str_repeats(seq, dna) for seq in sequences]
            # Statements below results in the same as the 1 line of code above
            # max_repeats = []
            # for seq in sequences:
            #     maximum = get_longest_str_repeats(seq, dna)
            #     max_repeats.append(maximum)
        # Print whether or not there's a match
        print_match(reader, max_repeats)


# Print method receives pointer to CSV-file and array with calculated max repeats
def print_match(reader, max_repeats):
    for row in reader:
        name = row[0]
        # Initiate array which converts string values to int values
        values = [int(value) for value in row[1:]]
        # Comparison between values in both arrays
        if values == max_repeats:
            print(name)
            return
    print("No match")


def get_longest_str_repeats(sequence, dna):
    # Store sequence length to lower memory usage in this function
    sequence_length = len(sequence)
    i = 0
    j = sequence_length
    max_repeat = 0

    for x in range(len(dna)):
        # Compare substrings with sequence
        if dna[i : j] == sequence:
            # Reset counter to 0
            counter = 0
            # Keep searching for consecutive repeats
            while dna[i : j] == sequence:
                counter += 1
                i += sequence_length
                j += sequence_length
                if counter > max_repeat:
                    max_repeat = counter
        else:
            i += 1
            j += 1

    return max_repeat


#statement to initiate main function
if __name__ == "__main__":
    main()