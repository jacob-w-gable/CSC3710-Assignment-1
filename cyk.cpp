
/**
 * Author: 	Jacob Gable
 * File: 	cyk.cpp
 * Class: 	CSC3710 - Foundations of Computer Science
 * 
 * Description:
 * This is a program that takes two input variables: a test string, 
 * and a file containing a grammar in Chomsky normal form. It generates 
 * a table using the CYK algorithm and prints it to the console. If the
 * first row of the table contains the start symbol, then we know that
 * the string can be generated using the grammar.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

int main(int argc, char** argv){

	// bad input detection
	if(argc != 3){
		cout << "Please run the program with two parameters:" << endl;
		cout << "./CYK.exe <language-file> <test-string>" << endl;
		cout << "For example," << endl;
		cout << "./CYK.exe language.txt aabbb" << endl;
		return 1;
	}
	
	string filename = argv[1];
	ifstream infile(filename);

	// read the input file into char** rules.
	char** rules;
	int ruleCount;
	if(infile.good()){
		infile >> ruleCount;
		infile.ignore();
		rules = new char*[ruleCount];
		for(int i = 0; i < ruleCount; i++){
			string line;
			getline(infile, line);

			// each char* is of length 3, converting the format A->BC to the array {A, B, C}.
			// a space character (or no character at all) effectively acts as the null-character lambda
			rules[i] = new char[3];
			rules[i][0] = line[0];
			if(line.length() > 3){
				rules[i][1] = line[3];
			} else {
				rules[i][1] = ' ';
			}
			if(line.length() > 4){
				rules[i][2] = line[4];
			} else {
				rules[i][2] = ' ';
			}
		}

	} else {
		cout << "Failed to open the given file." << endl;
		return 1;
	}

	string inputString = argv[2];
	int stringLength = inputString.length();
	
	// CYK Step 1: Initialize an empty matrix
	// index = row*stringLength + column
	string* matrix = new string[stringLength*stringLength];
	
	// CYK Step 2: Initialize diagonal of the matrix
	for(int i = 0; i < stringLength; i++){
		for(int rule = 0; rule < ruleCount; rule++){
			// Find the terminating rules that are valid for each index of the test string, 
			// and put them in the corresponding (index,index) slot in the diagonal of the matrix.
			if(rules[rule][1] == inputString[i] && rules[rule][2] == ' '){
				matrix[stringLength*i+i] += rules[rule][0];
			}
		}
	}
	
	// CYK Step 3: Complete matrix elements above the diagonal.
	// The outermost three for-loops iterate through each element in the matrix 
	// that are above the diagonal.
	// The current element is represented as "(row, column) = (i-1, i+step-2)"
	for(int step = 2; step <= stringLength; step++){
		for(int i = 1; i <= stringLength-step+1; i++){
			for(int k = i; k <= i+step-2; k++){
				// convert "step", "i", and "k" to row/column coordinates to simplify code
				int row1 = i-1;
				int col1 = k-1;
				int row2 = k;
				int col2 = i+step-2;

				// Get two elements relative to the main element in question. They will be
				// compared together.
				string pro1 = matrix[stringLength*row1+col1];
				string pro2 = matrix[stringLength*row2+col2];

				// given symbol A as a character in "pro1",
				//   and symbol B as a character in "pro2"...
				for(int a = 0; a < pro1.length(); a++){
					for(int b = 0; b < pro2.length(); b++){
						// ... check to see if there is a rule C,
						// such that C->AB
						for(int c = 0; c < ruleCount; c++){
							if(pro1[a] == rules[c][1] && pro2[b] == rules[c][2]){
								// if so, add the rule C to the list at the current element of the matrix
								int destRow = row1;
								int destCol = col2;

								// don't add the rule if it's already in the list
								char thisRule = rules[c][0];
								bool alreadyHas = false;
								for(int symbol = 0; symbol < matrix[stringLength*destRow+destCol].length(); symbol++){
									if(matrix[stringLength*destRow+destCol][symbol] == thisRule){
										alreadyHas = true;
									}
								}
								if(!alreadyHas){
									matrix[stringLength*destRow+destCol] += thisRule;
								}
							}
						}
					}
				}
			}
		}
	}

	// CYK algorithm complete, matrix generated. Note that the area under the diagonal does not get used.
	
	// Print each element in a fancy way
	for(int i = 0; i < stringLength; i++){
		for(int j = 0; j < stringLength; j++){
			string chars = matrix[stringLength*i+j];
			string element = "{";
			if(chars.length() > 1){
				for(int k = 0; k < chars.length()-1; k++){
					element = element + chars[k] + ",";	
				}
			}
			if(chars.length() > 0){
				element = element + chars[chars.length()-1];
			}
			element += "}";
			cout << setw(10) << left << element;
		}
		cout << "\n";
	}
	
	// Memory management
	for(int i = 0; i < ruleCount; i++){
		delete [] rules[i];
	}
	delete [] rules;
	delete [] matrix;
	
	return 0;
}


