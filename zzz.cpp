#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;

string toLower(const string& str)
{
	return boost::to_lower_copy(str);
}

unordered_map<string, string> parseDictionary(ifstream& inFile)
{
	string numWordPairsStr;
	getline(inFile, numWordPairsStr);
	int numWordPairs = stoi(numWordPairsStr);

	unordered_map<string, string> dictMap;

	for (int i{0}; i < numWordPairs; ++i)
	{
		string wordPairsStr;
		vector<string> wordPairsVec;

		getline(inFile, wordPairsStr);
		boost::split(wordPairsVec, wordPairsStr, boost::is_any_of(" "));
		dictMap[toLower(wordPairsVec[0])] = toLower(wordPairsVec[1]);
	}

	return dictMap;
}

bool areWordsSynonyms(unordered_map<string, string>& reverseDictMap, const std::unordered_map<string, string>& dictMap, string word1, string word2)
{
	if (word1 == word2)
	{
		return true;
	}

	string word = word1;

	while(true)
	{
		auto it = dictMap.find(word);

		if (it != dictMap.end())
		{
			word = it->second;
			if (word == word2)
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
}

void processTestPairs(ifstream& inFile, vector<string>& out)
{
	auto dictMap = parseDictionary(inFile);

	string numTestPairsStr;
	getline(inFile, numTestPairsStr);
	int numTestPairs = stoi(numTestPairsStr);

	for (int i{0}; i < numTestPairs; ++i)
	{
		string testPairsStr;
		std::vector<string> testPairsVec;

		getline(inFile, testPairsStr);
		boost::split(testPairsVec, testPairsStr, boost::is_any_of(" "));

		bool areSynonyms;

		string word1 = toLower(testPairsVec[0]);
		string word2 = toLower(testPairsVec[1]);

		unordered_map<string, string> reverseDictMap;

		areSynonyms = areWordsSynonyms(reverseDictMap, dictMap, word1, word2);
		if (!areSynonyms)
		{
			areSynonyms = areWordsSynonyms(reverseDictMap, dictMap, word2, word1);
		}

		string line = areSynonyms ? "synonyms" : "different";
		out.push_back(line);
	}


}

int main()
{
	// read and process input file
	string inFileName = "input.txt";
	ifstream inFile;
	inFile.open(inFileName);

	vector<string> output;

	if (inFile.is_open())
	{
		string numTestCasesStr;

		getline(inFile, numTestCasesStr);
		int numTestCases = stoi(numTestCasesStr);

		for (int i{0}; i < numTestCases; ++i)
		{
			processTestPairs(inFile, output);
		}

		inFile.close();
	}
	else
	{
		cout << "Cannot open file: " << inFileName << endl;
	}

	// write and save output file
	string outFileName = "output.txt";
	ofstream outFile;
	outFile.open(outFileName);

	if (outFile.is_open())
	{
		for (auto out : output)
		{
			outFile << out << endl;
		}

		outFile.close();
	}
	else
	{
		cout << "Could not create file: " << outFileName << endl;
	}

	return 0;
}
