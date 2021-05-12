#pragma once
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<algorithm>
#include<stdio.h>
#include<io.h>
#include<Windows.h>
using namespace std;

struct entry {
	vector<vector<double>> datas;
	vector<string> labels;
};

ostream& operator<<(ostream& os, entry e) {
	for (size_t i = 0; i < e.datas.size(); i++)
	{
		os << "entry " << i << ": [";
		for (size_t j = 0; j < e.datas[i].size(); j++)
		{
			os << e.datas[i][j] << " ";
		}
		os << "\b], labels: " << e.labels[i] << endl;
	}
	return os;
}

template<class T>
ostream& operator<<(ostream& os, vector<T> v) {
	os << "vector: [";
	for (int i = 0; i < v.size(); i++) {
		os << v[i] << " ";
	}
	os << (v.size() == 0 ? "" : "\b") << "]" << endl;
	return os;
}

class kNN {
public:
	void normalize(entry& e) {
		vector<vector<double>> datas = e.datas;
		size_t dataSetsize = datas.size();
		size_t dataNumber = datas[0].size();
		vector<double> maxValue(dataNumber, INT_MIN);
		vector<double> minValue(dataNumber, INT_MAX);

		//计算出每个参数的最大值和最小值
		for (size_t i = 0; i < dataNumber; i++)
		{
			for (size_t j = 0; j < dataSetsize; j++)
			{
				if (datas[j][i] > maxValue[i]) {
					maxValue[i] = datas[j][i];
				}
				if (datas[j][i] < minValue[i]) {
					minValue[i] = datas[j][i];
				}
			}
		}
		//计算出每个参数的区间
		vector<double> range(dataNumber);
		for (size_t i = 0; i < dataNumber; i++)
		{
			range[i] = maxValue[i] - minValue[i];
		}

		for (size_t i = 0; i < dataNumber; i++)
		{
			for (size_t j = 0; j < dataSetsize; j++)
			{
				e.datas[j][i] = (e.datas[j][i] - minValue[i]) / range[i];
			}
		}
	}

	entry loadPointData() {
		entry res;
		ifstream testFile("C:\\WorkSpace\\VS-Code-C\\MachineLearningC\\MLIA-in-CPP\\classification\\test.txt");
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

	entry loadDatingData() {
		entry res;
		ifstream datingFile("C:\\WorkSpace\\VS-Code-C\\MachineLearningC\\MLIA-in-CPP\\classification\\datingTestSet.txt");
		if (!datingFile.is_open()) {
			cout << "Open file failure" << endl;
		}
		while (!datingFile.eof()) {
			double a, b, c;
			string s;
			datingFile >> a >> b >> c >> s;
			vector<double> v{ a,b,c };
			res.datas.push_back(v);
			res.labels.push_back(s);
		}
		datingFile.close();
		this->normalize(res);
		return res;
	}



	string classify(vector<double> input, entry dataSet, int k) {
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

		//first store the labels count with a map class.
		map<string, int> m;
		for (int i = 0; i < k; i++) {
			if (m.find(labels[i]) != m.end()) {
				m[labels[i]]++;
			}
			else {
				m.insert(pair<string, int>(labels[i], 1));
			}
		}

		//return the most frequent label
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



void getDir(string path, vector<string>& filelist) {
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*.txt").c_str(), &fileinfo)) != -1) {
		do {
			filelist.push_back(fileinfo.name);
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

//my trainingSet dirPath is "C:\\WorkSpace\\VS-Code-C\\MachineLearningC\\MLIA-in-CPP\\classification\\trainingDigits"
//my testingSet dirPath is "C:\\WorkSpace\\VS-Code-C\\MachineLearningC\\MLIA-in-CPP\\classification\\testDigits"
void loadMinistSet(string dirPath, entry& trainingSet) {
	vector<string> files;
	getDir(dirPath, files);
	for (string file : files) {
		fstream trainingFile(dirPath + "\\" + file);
		if (!trainingFile.is_open()) {
			cout << "Open file failure" << endl;
		}
		int pos = file.find_first_of('_');
		string label = file.substr(0, pos);
		trainingSet.labels.push_back(label);
		vector<double> img;
		while (!trainingFile.eof()) {
			char c;
			trainingFile >> c;
			double d = atof(&c);
			img.push_back(d);
		}
		trainingSet.datas.push_back(img);
	}
	cout << "load file completed." << endl;
}


void test_of_kNNSimpleTest() {
	kNN k;
	entry e = k.loadPointData();
	string res = k.classify(vector<double>{0.7, 0.7}, e, 3);
	cout << res << endl;
}

void test_of_dating() {
	kNN k;
	entry e = k.loadDatingData();
	size_t dataSetSize = e.datas.size();
	entry testingData;
	entry trainingData;
	double hoRatio = 0.1;
	size_t m = dataSetSize * hoRatio;
	for (size_t i = 0; i < dataSetSize; i++)
	{
		if (i <= m) {
			testingData.datas.push_back(e.datas[i]);
			testingData.labels.push_back(e.labels[i]);
		}
		else {
			trainingData.datas.push_back(e.datas[i]);
			trainingData.labels.push_back(e.labels[i]);
		}
	}
	size_t totalSize = testingData.datas.size();
	size_t errNum = 0;
	for (size_t i = 0; i < testingData.datas.size(); i++)
	{
		string res = k.classify(testingData.datas[i], trainingData, 3);
		if (res != testingData.labels[i]) {
			cout << "entry " << i << ": label should be " << testingData.labels[i] << ", but the classify result is " << res << "." << endl;
			errNum++;
		}
	}
	double errRate = (double)errNum / (double)totalSize;
	cout << "Completed, the error rate is " << errRate * 100 << "%." << endl;
}

void test_of_minist() {
	string trainingPath = "C:\\WorkSpace\\VS-Code-C\\MachineLearningC\\MLIA-in-CPP\\classification\\trainingDigits";
	entry trainingSet;
	loadMinistSet(trainingPath, trainingSet);

	string testPath = "C:\\WorkSpace\\VS-Code-C\\MachineLearningC\\MLIA-in-CPP\\classification\\testDigits";
	entry testingSet;
	loadMinistSet(testPath, testingSet);


	size_t totalSize = testingSet.datas.size();
	size_t errNum = 0;
	kNN k;
	for (size_t i = 0; i < totalSize; i++) {
		string res = k.classify(testingSet.datas[i], trainingSet, 3);
		if (res != testingSet.labels[i]) {
			cout << "entry " << i << ": label should be " << testingSet.labels[i] << ", but the classify result is " << res << "." << endl;
			errNum++;
		}
		printf("progress : %d/%d, error number: %d", i+1, totalSize,errNum);
		if (i != totalSize - 1) {
			printf("\r");
		}
		else {
			printf("\n");
		}
	}
	double errRate = (double)errNum / (double)totalSize;
	cout << "Completed, the error rate is " << errRate * 100 << "%." << endl;
}
