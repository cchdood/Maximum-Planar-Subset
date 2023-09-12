// **************************************************************************
//  File       [main.cpp]
//  Author     [YU-AN, CHEN]
//  Synopsis   [The main program of PA2]
// **************************************************************************

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class MPS_tool {
    public:
        MPS_tool(vector<int>&); // constructor
        void Record_MPS(vector<int>&, vector<int>&, int, int);
    private:
        vector<vector<bool> > MPS_SubsetCut;
        vector<vector<int> > MPS_SetSize;
        
};

void MergeSort(vector<int>&);
void MergeSortSubVector(vector<int>&, int, int);
void Merge(vector<int>&, int, int, int, int);

void help_message() {
    cout << "usage: mps <input_file> <output_file>" << endl;
}

int main(int argc, char* argv[])
{

    //////////// read the input file /////////////

    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);
    int size;
    int endpoint_1;
    int endpoint_2;
    fin >> size;
	vector<int> data(size);

    for (int i = 0; i < size / 2; i++) {
        fin >> endpoint_1 >> endpoint_2;    // suppose point a is connected to point b
		data[endpoint_1] = endpoint_2;      // we have data[a] = b
		data[endpoint_2] = endpoint_1;      // and data[b] = a, so we can find a pair in O(1)
    }
    
    cout << "{";
    for (int i = 0; i < data.size(); i++) {
    	cout << data[i];
    	if (i != data.size() - 1) cout << ", ";
	}
	cout << "}" << endl;
    
    //////////// Find MPS using bottom-up DP ////////////////
    vector<int> output;                                  // initialize a vector used for storing the result
    MPS_tool mps(data);                                  // building MPS DP table
    mps.Record_MPS(data, output, 0, data.size() - 1);    // write down the MPS[0][2n-1] into output
    
	cout << "{";
    for (int i = 0; i < output.size(); i++) {
    	cout << output[i];
    	if (i != output.size() - 1) cout << ", ";
	}
	cout << "}" << endl;
	
    MergeSort(output);

    //////////// write the output file ///////////
    fout << output.size() << endl;
    for (int i = 0; i < output.size(); i++)
        fout << output[i] << " " << data[output[i]] << endl;
    fin.close();
    fout.close();
    return 0;
}

// **************************************************************************
MPS_tool::MPS_tool(vector<int>& vec) {

    vector<vector<bool> > S(vec.size(), vector<bool> (vec.size()));
    vector<vector<int> > C(vec.size(), vector<int> (vec.size()));
    MPS_SubsetCut = S;
    MPS_SetSize = C;
    
	for (int i = 0; i < vec.size(); i++) {
        MPS_SubsetCut[i][i] = false;
        MPS_SetSize[i][i] = 0;
    }

    for (int gap = 1; gap < vec.size(); gap++) {
    	for (int i = 0; i + gap < vec.size(); i++) {
    		int j = i + gap;
    		MPS_SubsetCut[i][j] = false;
    		MPS_SetSize[i][j] = MPS_SetSize[i][j-1];
    		if (vec[j] < j && vec[j] >= i) {
    			int alt;
				if (vec[j] == i) alt = MPS_SetSize[i][j] + 1;
				else alt = MPS_SetSize[i][vec[j] - 1] + MPS_SetSize[vec[j] + 1][j - 1] + 1;
    			if (MPS_SetSize[i][j] < alt) {
    				MPS_SetSize[i][j] = alt;
    				MPS_SubsetCut[i][j] = true;
				}
			}
		}
	}
}

void MPS_tool::Record_MPS(vector<int>& data, vector<int>& result, int i, int j) {
	if (i >= j) return;
	bool k = MPS_SubsetCut[i][j];
	if (!k) Record_MPS(data, result, i, j-1);
	else {
		result.push_back(data[j]);
		Record_MPS(data, result, i, data[j] - 1);
		Record_MPS(data, result, data[j] + 1, j);
	}
}

void MergeSort(vector<int>& data){
    MergeSortSubVector(data, 0, data.size() - 1);
}

void MergeSortSubVector(vector<int>& data, int low, int high) {

    if (low < high) {
    	int middle1 = (low + high) / 2;
    	int middle2 = middle1 + 1;
    	MergeSortSubVector(data, low, middle1);
    	MergeSortSubVector(data, middle2, high);
    	Merge(data, low, middle1, middle2, high);
	}
}

void Merge(vector<int>& data, int low, int middle1, int middle2, int high) {
	
    std::vector<int> result;
    int Leftindex = low;
    int Rightindex = middle2;
    
    while (Leftindex <= middle1 && Rightindex <= high) {
    	if (data[Leftindex] < data[Rightindex]) {
    		result.push_back(data[Leftindex]);
    		Leftindex++;
		}
		else {
			result.push_back(data[Rightindex]);
			Rightindex++;
		}
	}
	
	if (Leftindex == middle2) {
		while (Rightindex <= high) {
			result.push_back(data[Rightindex]);
			Rightindex++;
		}
	}
	
	else {
		while (Leftindex <= middle1) {
    		result.push_back(data[Leftindex]);
    		Leftindex++;
		}
	}
	
	int paste_pos = low;
	for (int i = 0; i < result.size(); i ++) {
		data[paste_pos] = result[i];
		paste_pos++;
	}

}
// **************************************************************************
