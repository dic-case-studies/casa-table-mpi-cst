from casatools import table
from casatools import image
import numpy as np
import shutil
import os
import sys

if __name__ == '__main__':

    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} table_full_path fits_dir_path")
        sys.exit(1)

    table_path = sys.argv[1]
    fits_dir_path = sys.argv[2]

    # Open the CASA table
    tbl = table()
    tbl.open(table_path)
    col_names = tbl.colnames()
    
    # image object instance
    im = image()

    for row in tbl.row():
        pol_name = row[col_names[0]]
        # getting "data" column from the required row with matching "name"
        pol_tbl = tbl.taql(f"SELECT data from {tbl.name()} t1 WHERE t1.name == '{pol_name}'")

        pol_data_casa_table = np.array(pol_tbl.row()[0]["data"])

        fits_file_path = os.path.join(fits_dir_path,pol_name)
        fits_file_path = fits_file_path + ".fits"
        casa_file_path = fits_file_path + ".casaimg"
        print(fits_file_path)
        
        im.fromfits(outfile=casa_file_path, infile=fits_file_path)
        im.close()

        im.open(casa_file_path)
        fits_image_data = im.getchunk()
        im.close()

        # print(image_data)

        assert(np.equal(pol_data_casa_table.all(),fits_image_data.all()))

        shutil.rmtree(casa_file_path)
        

    # Close the table
    tbl.close()