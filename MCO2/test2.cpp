#include "Lib.h"
#include <cstdlib>
#include "SymbolTable.h"

int main(){

    SymbolTable symbol_table;
    symbol_table.Initialize();

    symbol_table.Insert(0, "VAR_A", 10);
    symbol_table.Insert(1, "VAR_B", 20);
    symbol_table.Update(0, 15);
    symbol_table.Update("VAR_B", 25);
    int value_a = symbol_table.Fetch("VAR_A");
    int value_b = symbol_table.Fetch("VAR_B");
    int size = symbol_table.GetSize();
    cout << "Value of VAR_A: " << value_a << endl;
    cout << "Value of VAR_B: " << value_b << endl;
    cout << "Size of symbol table: " << size << endl;
    symbol_table.Update(0, 30);
    value_a = symbol_table.Fetch("VAR_A");
    cout << "Updated Value of VAR_A: " << value_a << endl;

    return 0;
}