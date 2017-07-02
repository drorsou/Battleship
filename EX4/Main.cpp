#include "Matrix2d.h"
#include <ctype.h>
#include <iostream>

using namespace std;


template<typename Groups>
void print(const Groups& all_groups) {
	for (const auto& groupType : all_groups) {
		cout << groupType.first << ":" << endl;
		for (const auto& groupOfType : groupType.second) {
			for (const auto& coord : groupOfType) {
				cout << "{ ";
				for (auto i : coord) { // was: for( int i : coord)
					cout << i << ' ';
				}
				cout << "} ";
			}
			cout << endl;
		}
	}
}

int main() {
	//Matrix2d<char> m = { { 'a', 'A', 'a' },{ 'B', 'a', 'B' },{ 'B', 'a', 'B' } };
	Matrix2d<char> m = { { 'A', 'A', 'b', 'A' },{ 'b', 'b', 'b', 'A' },{ 'A', 'A', 'A', 'A' },{ 'b', 'b', 'b', 'b'} };
	auto all_groups = m.groupValues([](auto i) {return islower(i) ? 'L' : 'U'; });
	print(all_groups);
}