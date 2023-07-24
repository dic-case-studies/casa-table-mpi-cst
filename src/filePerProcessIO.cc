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

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int arrColSize = 5;
  if (rank != 0)
  {
    std::string outputDirPath = "out/";
    std::string tableName = outputDirPath + "table_" + std::to_string(rank) + ".casatable";
    casacore::TableDesc td("tableDesc", "1", casacore::TableDesc::Scratch);

    td.addColumn(casacore::ScalarColumnDesc<casacore::String>("name"));
    td.addColumn(casacore::ArrayColumnDesc<float>("data"));

    casacore::SetupNewTable newTable(tableName, td, casacore::Table::New);

    const int initialRows = 0;
    casacore::Table tab(newTable, initialRows);

    casacore::ScalarColumn<casacore::String> nameCol(tab, "name");
    casacore::ArrayColumn<float> dataCol(tab, "data");

    int rowsToAdd = 3;
    for (size_t i = 0; i < rowsToAdd; i++)
    {
      tab.addRow();
      nameCol.put(tab.nrow() - 1, std::to_string(i + 100 * rank));
      
      casacore::Vector<float> arrColValues(arrColSize);
      casacore::indgen(arrColValues, float(i + 200 * rank));
      dataCol.put(tab.nrow() - 1, arrColValues);
    }
  }

  MPI_Finalize();
}
