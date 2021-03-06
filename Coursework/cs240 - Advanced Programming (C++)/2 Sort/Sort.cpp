/*
 * Sort.cpp
 *
 *  Created on: Jun 28, 2010
 *      Author: mccain
 */
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

bool r_desc = false;
bool i_ignoreCase = false;
bool n_numeric = false;

struct Line {
	char* original;
	char* key;
	~Line(){
		delete original;
		delete key;
	}
};

int compareLines(const void* a, const void* b){
		Line* y;
		Line* x;
		if (r_desc){
			y = *((Line**)a);
			x = *((Line**)b);
		}
		else{
			x = *((Line**)a);
			y = *((Line**)b);
		}
		if (i_ignoreCase){
			return strcasecmp(x->key, y->key); //case insensitive
		}
		if (n_numeric){
			int xint = atoi(x->key);
			int yint = atoi(y->key);
			if (xint < yint){
				return -1;
			}
			else if (xint == yint){
				return 0;
			}
			return 1;
		}
		return strcmp(x->key, y->key);
}

void scanFile (char* filename, Line* lineArr[], int& length, int field){

	ifstream file;
	char curLine[1024];

	file.open(filename);

	for (int i = 0; file.getline(curLine, sizeof(curLine)); i++){
		//cout << strlen(curLine) << endl;
		if (strlen(curLine) == 0){
			i--;
			continue;
		}
		lineArr[i] = new Line;
		length++; //used to delete later
		lineArr[i]->original = new char[1024];
		strcpy (lineArr[i]->original, curLine);

		lineArr[i]->key = new char[1024];

		char* curr;

		int curField = 1;
		curr = strtok (curLine, " \t");
		while (curr != NULL && curField < field){
			//cout << curr << endl;
			curr = strtok (NULL, " \t");
			curField++;
		}
		//cout << "key: " << curr << endl;
		strcpy(lineArr[i]->key, curr);
		//cout << "scanned: " << curLine << endl;
		//cout << "scanned: " << lineArr[i]->original << endl;

		/*int tokens = 0;
		bool ws = true;
		bool copyKey = false;
		int k = 0;
		for (int j = 0; curLine[j] != '\0'; j++){
			if (curLine[j] == ' ' && !ws){
				cout << "space between...";
				ws = true;
				if (copyKey){
					lineArr[i]->key[k] = '\0';
					break;
				}
			}
			else if (curLine[j] != ' ' && ws){
				cout << "new word at " << curLine[j] << "...";
				ws = false;
				tokens++;
				if (tokens == field){
					cout << "it's the token...";
					copyKey = true;
					lineArr[i]->key[k] = curLine[j];
					k++;
				}
			}
			else if (copyKey){
				if (tokens == field){
					lineArr[i]->key[k] = curLine[j];
					k++;
				}
			}
		}
		cout << "key: " << lineArr[i]->key << endl;
		*/


	}


}
int main (int argc, char* argv[]){
	int field;
	int length = 0;
	char* filename;
	Line* lineArr[1024];

	if (argc == 4){
		if (strchr(argv[1], 'r') != NULL){
			r_desc = true;
		}
		if (strchr(argv[1], 'i') != NULL){
			i_ignoreCase = true;
		}
		if (strchr(argv[1], 'n') != NULL){
			n_numeric = true;
		}
		field = atoi(argv[2]);
		filename = argv[3];
	}
	else if (argc == 3)
	{
		field = atoi(argv[1]);
		filename = argv[2];
	}

	else{
		cout << "USAGE:sort240 [options] field file" << endl;

	}

	scanFile(filename, lineArr, length, field);
	qsort(lineArr, length, sizeof(lineArr[0]), compareLines);
	for (int i = 0; i < length; i++){
		cout << lineArr[i]->original << endl;
	}

	for (int i = 0; i < length; i++){
		delete lineArr[i];
	}
	return 0;
}

