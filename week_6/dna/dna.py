from sys import argv, exit
import csv

if len(argv) != 3:
    print("Please provide a DNA database and a DNA sequence.")
    exit(1)

database = {}
srt_dict = {}

# Open database file
with open(argv[1], "r") as db_src:
    reader = csv.DictReader(db_src)
    for row in reader:
        # Person name
        person = row["name"]
        # Init person in database dict
        database[person] = {}
        # Store each SRT value into person's dict
        for srt_label in row:
            if srt_label != "name":
                # Populate SRT dict
                if srt_label not in srt_dict:
                    srt_dict[srt_label] = 0
                database[person][srt_label] = row[srt_label]

# Open sequence file
with open(argv[2], "r") as f:
    sequence = f.read()

    # Each SRT available
    for srt in srt_dict:
        # Duplicate sequence to parse it without modifying the original
        query = sequence

        # Index from last SRT ocurrence to compare distance to next
        last_index = -1

        # Compare index distances to track groups
        max_count = 0
        last_count = 0

        # While the SRT is present in the sequence
        while query.find(srt) != -1:
            # The index of last char of current SRT occurrence
            end = query.find(srt) + len(srt)

            if last_index == end - len(srt) or last_index == -1:
                # If distance from last ocurrance is equal to length of SRT or is first found
                last_count += 1
            else:
                # If distance is different, reset current counting
                last_count = 1

            if last_count > max_count:
                max_count = last_count

            # Update last index
            last_index = end

            # Replace SRT chars with something else to keep index distances comparable
            query = query.replace(srt, "." * len(srt), 1)
        # Store max count for this SRT label
        srt_dict[srt] = max_count

# Lookup each person to match SRT values
for person in database:
    is_match = True
    for srt in srt_dict:
        if int(database[person][srt]) != int(srt_dict[srt]):
            is_match = False
    # Exit early if found
    if is_match:
        break

if is_match:
    print(person)
else:
    print("No match")
