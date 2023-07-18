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

  casacore::Table tab("test_" + std::string(argv[1]) + ".data");

  casacore::ScalarColumn<int> idCol(tab, "id");
  casacore::ArrayColumn<float> arrCol(tab, "arr");

  std::cout << "Number of rows: " << tab.nrow() << std::endl;
  for (int i = 0; i < tab.nrow(); i++) {
    std::cout << "ID Col: " << idCol.get(i) << " ";
    std::cout << "Arr Col: " << arrCol.get(i);
    std::cout << std::endl;
  }

}