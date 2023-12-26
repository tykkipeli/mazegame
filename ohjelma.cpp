#include <iostream>
#include "ohjelma.hpp"

using namespace std;

void lol() {
    cout << "ohjelma";
}


namespace ohjelma {

    void tulosta(int x) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < i+1; j++) lol();
            cout << endl;
        }
    }
}
