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

  if (rank == 0) {
    casacore::TableDesc td("tableDesc", "1", casacore::TableDesc::Scratch);

    td.addColumn(casacore::ScalarColumnDesc<int> ("id"));
    td.addColumn(casacore::ArrayColumnDesc<float>("arr"));

    casacore::SetupNewTable newTable("test_0.data", td, casacore::Table::New);

    // casacore::StandardStMan stman;
    // newTable.bindAll(stman);

    const int nrows = 2;
    casacore::Table tab(newTable, nrows);

    casacore::ScalarColumn<int> idCol(tab, "id");
    casacore::ArrayColumn<float> arrCol(tab, "arr");

    casacore::Vector<float> arrColValues(nrows);

    for (int i = 0; i < nrows; i++) {
      idCol.put(i, rank*i+10);
      casacore::indgen(arrColValues, float(rank*i+20));
      arrCol.put(i, arrColValues);
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank != 0) {
    casacore::Table tab("test_0.data",
      casacore::TableLock::LockOption::UserNoReadLocking,
      casacore::Table::TableOption::Update
    );

    casacore::ScalarColumn<int> idCol(tab, "id");
    casacore::ArrayColumn<float> arrCol(tab, "arr");

    int rowsToAdd = 8;
    casacore::Vector<float> arrColValues(rowsToAdd);
    for (size_t i = 0; i < rowsToAdd; i++)
    {
      tab.lock();
      tab.addRow();
      idCol.put(tab.nrow() - 1, i+100*rank);
      casacore::indgen(arrColValues, float(i+200*rank));
      arrCol.put(tab.nrow() - 1, arrColValues);
      tab.unlock();
    }
  }

  MPI_Finalize();

}
