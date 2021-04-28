#pragma once
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<algorithm>
using namespace std;

template <class T>
struct entry {
	vector<T> datas;
	vector<string> labels;
};

class kNN {
public:
	entry<vector<double>> createDataSet() {
		entry<vector<double>> res;
		ifstream testFile("C:\\WorkSpace\\VS-Code-C\\MachineLearningC\\classification\\test.txt");
		if (!testFile.is_open()) {
			cout << "Open file failure" << endl;
		}
		while (!testFile.eof()) {
			double a, b;
			string s;
			testFile >> a >> b >> s;
			vector<double> v{ a,b };
			res.datas.push_back(v);
			res.labels.push_back(s);
		}
		testFile.close();
		return res;
	}

	string classify(vector<double> input, entry<vector<double>> dataSet, int k) {
		int dataSetSize = dataSet.datas.size();
		int dataSize = dataSet.datas[0].size();
		if (dataSize != input.size()) {
			cout << "in line " << __LINE__ << ": input data size is not equal to the test data size" << endl;
			return "err";
		}

		//calculate the distances of input data and dataSet
		vector<double> distance(dataSetSize, 0);
		for (int j = 0; j < dataSetSize; j++) {
			double disOfJ = 0;
			for (int i = 0; i < dataSize; i++) {
				double dis = input[i] - dataSet.datas[j][i];
				dis = dis * dis;
				disOfJ += dis;
			}
			distance[j] = sqrt(disOfJ);
		}

		//sort the distance with increasing order
		//TODO: improve the sort algorithm
		vector<string> labels(dataSet.labels);
		for (int i = 0; i < dataSetSize; i++) {
			for (int j = i + 1; j < dataSetSize; j++) {
				if (distance[j] < distance[i]) {
					double temp = distance[i];
					distance[i] = distance[j];
					distance[j] = temp;

					string tmp = labels[i];
					labels[i] = labels[j];
					labels[j] = tmp;
				}
			}
		}


		//return the most frequent label in the first k members of the increasing list
		map<string, int> m;
		for (int i = 0; i < k; i++) {
			if (m.find(labels[i]) != m.end()) {
				m[labels[i]]++;
			}
			else {
				m.insert(pair<string, int>(labels[i], 1));
			}
		}

		int maxcount = 0;
		string ret;
		for (auto it = m.begin(); it != m.end(); it++) {
			if (it->second > maxcount) {
				maxcount = it->second;
				ret = it->first;
			}
		}

		return ret;
	}
};

void test_of_kNN1() {
	kNN k;
	entry<vector<double>> e = k.createDataSet();
	string res = k.classify(vector<double>{0.7, 0.7}, e, 3);
	cout << res << endl;
}