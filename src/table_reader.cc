#include <casacore/tables/Tables/Table.h>
#include <casacore/casa/Arrays/Vector.h>
#include <casacore/tables/DataMan/MemoryStMan.h>
#include <casacore/tables/DataMan/StandardStMan.h>
#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/tables/Tables/ScalarColumn.h>
#include <casacore/tables/Tables/SetupNewTab.h>
#include <casacore/tables/Tables/TabVecMath.h>
#include <casacore/tables/Tables/TableDesc.h>
#include <casacore/tables/Tables.h>
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <table-number>";
    exit(1);
  }

  std::string tablePath = "out/table_" + std::string(argv[1]) + ".casatable";
  
  // Check if the table exists
  if (casacore::Table::isReadable(tablePath)) {
      std::cout << "Table exists." << std::endl;
  } else {
      std::cout << "Table does not exist." << std::endl;
      exit(1);
  }

  casacore::Table tab(tablePath);

  casacore::ScalarColumn<casacore::String> nameCol(tab, "name");
  casacore::ArrayColumn<float> dataCol(tab, "data");
  
  std::cout << "Number of rows: " << tab.nrow() << std::endl;
  for (int i = 0; i < tab.nrow(); i++) {
    std::cout << "Name Col: " << nameCol.get(i) << " ";
    std::cout << "Data Col: " << dataCol.get(i);
    std::cout << std::endl;
  }

}