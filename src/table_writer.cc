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

  casacore::TableDesc td("tableDesc", "1", casacore::TableDesc::Scratch);

  td.addColumn(casacore::ScalarColumnDesc<int> ("id"));
  td.addColumn(casacore::ArrayColumnDesc<float>("arr"));

  casacore::SetupNewTable newTable("test.data", td, casacore::Table::New);

  casacore::StandardStMan stman;

  newTable.bindAll(stman);
  // newTable.setShapeColumn("arr", casacore::IPosition(3, 5, 5, 5));

  casacore::Table tab(newTable, 10);

  casacore::Vector<float> vec(10);

  casacore::ScalarColumn<int> idCol(tab, "id");
  casacore::ArrayColumn<float> arrCol(tab, "arr");

  for (int i = 0; i < 10; i++) {
    idCol.put(i, rank*i+100);
    casacore::indgen(vec, float(rank*i+20));
    arrCol.put(i, vec);
  }

  // casacore::MemoryStMan stman;
  // casacore::Table tab("test.ms");

  MPI_Finalize();

}