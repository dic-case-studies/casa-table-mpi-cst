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

// 2 ways to set shape for an array column
// 1.
// td.addColumn(casacore::ArrayColumnDesc<float>("arr", casacore::IPosition(1, arrColSize)));
// 2.
// td.addColumn(casacore::ArrayColumnDesc<float>("arr", 1, casacore::ColumnDesc::Direct));
// newTable.setShapeColumn("arr", casacore::IPosition(1, arrColSize));

void write(int nrows) {
  int rank;
  int nprocs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  // Create tableDescriptor, columnDescriptor
  casacore::TableDesc td("tableDesc", "1", casacore::TableDesc::Scratch);
  td.addColumn(casacore::ScalarColumnDesc<int> ("id"));
  td.addColumn(casacore::ArrayColumnDesc<float>("arr", 1, casacore::ColumnDesc::Direct));

  // Create setupTable and specify array column shape
  int arrColSize = 10;
  casacore::SetupNewTable setupTable("test_0.data", td, casacore::Table::New);
  setupTable.setShapeColumn("arr", casacore::IPosition(1, arrColSize));

  // Using BP4, reading BP5 through table_reader is failing.
  casacore::Adios2StMan stman(MPI_COMM_WORLD, "BP4");
  setupTable.bindAll(stman); // binding adios2StMan with the table

  casacore::Table tab(MPI_COMM_WORLD, setupTable, nrows);

  casacore::ScalarColumn<int> idCol(tab, "id");
  casacore::ArrayColumn<float> arrCol(tab, "arr");

  casacore::Vector<float> arrColValues(arrColSize);

  int rowsPerWorker = nrows / (nprocs - 1);
  if (rank != 0) {
    for (int i = 0; i < rowsPerWorker; i++) {
      int rowIdx = (rank - 1) * rowsPerWorker + i;

      idCol.put(rowIdx, rank*10+i+1);

      casacore::indgen(arrColValues, float(rank*100+i+1));
      arrCol.put(rowIdx, arrColValues);
    }
  }
}

int main(int argc, char** argv ) {
  MPI_Init(&argc, &argv);

  write(12);

  MPI_Finalize();
}
