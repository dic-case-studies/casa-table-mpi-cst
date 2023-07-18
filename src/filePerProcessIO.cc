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

#include <mpi.h>

int main(int argc, char** argv ) {

  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int arrColSize = 5;
//   if (rank != 0) {
    std::string tableName = "test_" + std::to_string(rank) + ".data";
    casacore::TableDesc td("tableDesc", "1", casacore::TableDesc::Scratch);

    td.addColumn(casacore::ScalarColumnDesc<int> ("id"));
    td.addColumn(casacore::ArrayColumnDesc<float>("arr"));

    casacore::SetupNewTable newTable(tableName, td, casacore::Table::New);

    const int initialRows = 0;
    casacore::Table tab(newTable, initialRows);

    casacore::ScalarColumn<int> idCol(tab, "id");
    casacore::ArrayColumn<float> arrCol(tab, "arr");

    casacore::Vector<float> arrColValues(arrColSize);

    int rowsToAdd = 5;
    tab.lock();
    for (size_t i = 0; i < rowsToAdd; i++)
    {
      tab.addRow();
      idCol.put(tab.nrow() - 1, i+100*rank);
      casacore::indgen(arrColValues, float(i+200*rank));
      arrCol.put(tab.nrow() - 1, arrColValues);
    }
    tab.unlock();
//   }

  MPI_Finalize();

}
