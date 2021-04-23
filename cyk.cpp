#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

int main(int argc, char** argv){

	if(argc != 3){
		cout << "Please run the program with two parameters:" << endl;
		cout << "./CYK.exe <language-file> <test-string>" << endl;
		cout << "For example," << endl;
		cout << "./CYK.exe language.txt aabbb" << endl;
		return 1;
	}

	//cout << "Input file name: ";
	//string filename;
	//getline(cin, filename);
	string filename = argv[1];

	ifstream infile(filename);
	
	char** rules;
	int ruleCount;
	
	if(infile.good()){
		infile >> ruleCount;
		infile.ignore();
		rules = new char*[ruleCount];
		for(int i = 0; i < ruleCount; i++){
			string line;
			getline(infile, line);
			rules[i] = new char[3];
			rules[i][0] = line[0];
			rules[i][1] = line[3];
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
	
	//string inputString;
	//cout << "Enter test string: ";
	//getline(cin, inputString);
	string inputString = argv[2];
	
	int n = inputString.length();
	
	// [column][row]
	// column + n*row
	string* matrix = new string[n*n];
	
	for(int i = 0; i < n; i++){
		for(int rule = 0; rule < ruleCount; rule++){
			if(rules[rule][1] == inputString[i] && rules[rule][2] == ' '){
				matrix[n*i+i] += rules[rule][0];
			}
		}
	}
	
	for(int step = 2; step <= n; step++){
		for(int i = 1; i <= n-step+1; i++){
			for(int k = i; k <= i+step-2; k++){
				int row1 = i-1;
				int col1 = k-1;
				int row2 = k;
				int col2 = i+step-2;
				string pro1 = matrix[n*row1+col1];
				string pro2 = matrix[n*row2+col2];
				for(int a = 0; a < pro1.length(); a++){
					for(int b = 0; b < pro2.length(); b++){
						for(int c = 0; c < ruleCount; c++){
							if(pro1[a] == rules[c][1] && pro2[b] == rules[c][2]){
								int destRow = i-1;
								int destCol = i+step-2;
								matrix[n*destRow+destCol] += rules[c][0];
							}
						}
					}
				}
			}
		}
	}
	
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			string chars = matrix[n*i+j];
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
	
	for(int i = 0; i < n; i++){
		delete [] rules[i];
	}
	delete [] rules;
	delete [] matrix;
	
	return 0;
}


