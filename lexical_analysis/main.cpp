#include <fstream>
#include "lexicalAnalysis.h"

using namespace std;

int main(int argc, char const *argv[])
{
    ofstream out("output.txt");
    lexicalAnalysis lexical("testfile.txt");
    while (lexical.hasSym())
    {
        token symbol = lexical.getSym();
        out << symbol.getName() << " " << symbol.getValue() << endl;
    }
    return 0;
}
