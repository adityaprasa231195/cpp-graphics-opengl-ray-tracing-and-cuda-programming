#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include "this.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

template <typename T>
T larger (T a , T b){
	return a>b ? a : b;
}

 

int main() {
	   
	   	auto s = std::make_unique<char[]>(5);

s[0] = 'l';
s[1] = 'i';
s[2] = 'k';
s[3] = 'e';
s[4] = '\0';

char *p=s.release();


delete [] p;

cout<<larger(2,3);










	   
    
    return 0;
}