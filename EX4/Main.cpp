#include "Matrix2d.h"
#include <ctype.h>
#include <iostream>
#include "Matrix3d.h"

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
	//Matrix2d<char> m = { { 'A', 'A', 'b', 'A' },{ 'b', 'b', 'b', 'A' },{ 'A', 'A', 'A', 'A' },{ 'b', 'b', 'b', 'b'} };
	//Matrix3d<char> m = { { { 'a', 'A', 'a' },{ 'B', 'a', 'B' } } ,{ { 'a', 'A', 'a' },{ 'B', 'a', 'B' } },{ { 'a', 'A', 'a' },{ 'B', 'a', 'B' } } };
	Matrix2d<char> m = { { 'a', 'a', 'a' },{ 'a', 'B', 'a' },{ 'a', 'a', 'a' } };
	auto all_groups = m.groupValues([](auto i) {return islower(i) ? 'L' : 'U'; });
	print(all_groups);

	Matrix3d<int> m2 = { { { 1, 2, 3 },{ 1, 2 },{ 1, 2 } },{ { 1, 2 },{ 1, 2, 3, 4 } } };
	auto groups = m2.groupValues([](auto i) {return i % 3 ? "!x3" : "x3"; });
	print(groups);
}