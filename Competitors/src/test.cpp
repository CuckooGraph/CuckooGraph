#include "GeneralGraph.hpp"
#include "TestAlgorithm.hpp"

#ifdef HAVE_LIVEGRAPH
#include "LiveGraphInterface.hpp"
#endif

#ifdef HAVE_SORTLEDTON
#include "SortledtonInterface.hpp"
#endif

#ifdef HAVE_SPRUCE
#include "SpruceInterface.hpp"
#endif

#include <string>
#include <time.h>
#include <assert.h>
#include <chrono>
#include <time.h>

using namespace std;


// ./test cmd repeat_time dataset_path
int main(int argc, char *argv[]) {
    GeneralGraph *g;
    string path = argv[3];
    int repeatTime = stoi(argv[2]);
    g->LoadData(path);
    
    #ifdef HAVE_LIVEGRAPH
    cout << "\n\n\t\tTEST: LiveGraph\n\n" << endl;
    g = new LiveGraphInterface(path, true, true);
    #endif

    #ifdef HAVE_SORTLEDTON
    cout << "\n\n\t\tTEST: Sortledton\n\n" << endl;
    g = new SortledtonInterface(path, true, true);
    #endif

    #ifdef HAVE_SPRUCE
    cout << "\n\n\t\tTEST: Spruce\n\n" << endl;
    g = new SpruceInterface(path, true, true);
    #endif

    string cmd = argv[1];
    unordered_map<string, function<void(GeneralGraph *)>> funcs;
    funcs["lcc"] = Lcc;
    funcs["bc"] = Bc;
    if (funcs[cmd] != nullptr) {
        for (int i = 0; i < repeatTime; i++)
            funcs[cmd](g);
    } else {
        cerr << "Wrong command!" << endl;
    }

    delete g;
}
