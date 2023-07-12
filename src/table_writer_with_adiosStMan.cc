#include <casacore/tables/Tables/Table.h>
#include <casacore/casa/Arrays/Vector.h>
#include <casacore/tables/DataMan/Adios2StMan.h>
#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/tables/Tables/ScalarColumn.h>
#include <casacore/tables/Tables/SetupNewTab.h>
#include <casacore/tables/Tables/TabVecMath.h>
#include <casacore/tables/Tables/TableDesc.h>
#include <casacore/tables/Tables.h>
#include <iostream>

#include <mpi.h>

using namespace casacore;

void write() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int arrColSize = 10;

  // Using BP4, don't know as of now how to read BP5 through table_reader.
  casacore::Adios2StMan stman(MPI_COMM_WORLD, "BP4");
  casacore::TableDesc td("tableDesc", "1", casacore::TableDesc::Scratch);

  td.addColumn(casacore::ScalarColumnDesc<int> ("id"));
  td.addColumn(casacore::ArrayColumnDesc<float>("arr", casacore::IPosition(1, arrColSize)));

  casacore::SetupNewTable newTable("test.data", td, casacore::Table::New);

  newTable.bindAll(stman);

  const int nrows = 5;
  casacore::Table tab(MPI_COMM_WORLD, newTable, nrows);

  casacore::ScalarColumn<int> idCol(tab, "id");
  casacore::ArrayColumn<float> arrCol(tab, "arr");

  casacore::Vector<float> arrColValues(arrColSize);

  if (rank == 0) {
    for (int i = 0; i < nrows; i++) {
      idCol.put(i, i+10);
      casacore::indgen(arrColValues, static_cast<float>(i+20));
      arrCol.put(i, arrColValues);
    }
  }

  // //does not write to bp file, why? (only change is addRow)
  // if (rank != 0) {
  //   for (int i = 0; i < nrows; i++) {
  //     tab.addRow();
  //     idCol.put(tab.nrow()-1, rank*i+10);
  //     casacore::indgen(arrColValues, float(rank*i+20));
  //     arrCol.put(tab.nrow()-1, arrColValues);
  //   }
  // }
}

int main(int argc, char** argv ) {
  MPI_Init(&argc, &argv);

  write();

  MPI_Finalize();
}
