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

int main() {

  casacore::Table tab("test.data");

  casacore::ScalarColumn<int> idCol(tab, "id");
  casacore::ArrayColumn<float> arrCol(tab, "arr");

  for (int i = 0; i < tab.nrow(); i++) {
    std::cout << "ID Col: " << idCol.get(i) << " ";
    std::cout << "Arr Col: " << arrCol.get(i);
    std::cout << std::endl;
  }

}