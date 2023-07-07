from casatools import table

def read_and_print_casa_table(table_path):
    # Open the CASA table
    tbl = table()
    tbl.open(table_path)

    # Get the table column names
    column_names = tbl.colnames()

    # Print the column names
    print("Column names:")
    print(column_names)

    # Iterate over the table rows
    for row in tbl:
        # Iterate over the table columns
        for column_name in column_names:
            print(f"{column_name}: {row[column_name]}")

    # Close the table
    tbl.close()

if __name__ == '__main__':
    import sys

    if len(sys.argv) < 2:
        print("Please provide the path to the CASA table.")
        sys.exit(1)

    table_path = sys.argv[1]
    read_and_print_casa_table(table_path)