#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

std::string toLower(const std::string& str)
{
	return boost::to_lower_copy(str);
}

int readIntegerFromFile(std::ifstream& inFile)
{
	std::string integerStr;
	std::getline(inFile, integerStr);
	return stoi(integerStr);
}

std::vector<std::string> readWordPairsFromFile(std::ifstream& inFile)
{
	std::string wordPairsStr;
	std::vector<std::string> wordPairsVec;

	getline(inFile, wordPairsStr);
	boost::split(wordPairsVec, wordPairsStr, boost::is_any_of(" "));

	return wordPairsVec;
}

std::unordered_map<std::string, std::string> parseDictionary(std::ifstream& inFile)
{
	int numWordPairs = readIntegerFromFile(inFile);

	std::unordered_map<std::string, std::string> dictMap;

	for (int i{0}; i < numWordPairs; ++i)
	{
		auto wordPairsVec = readWordPairsFromFile(inFile);
		dictMap[toLower(wordPairsVec[0])] = toLower(wordPairsVec[1]);
	}

	return dictMap;
}

bool areWordsSynonyms(const std::unordered_map<std::string, std::string>& dictMap, const std::string& word1, const std::string& word2)
{
	if (word1 == word2)
	{
		return true;
	}

	std::string word = word1;

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

void processTestPairs(std::ifstream& inFile, std::vector<std::string>& output)
{
	auto dictMap = parseDictionary(inFile);

	int numTestPairs = readIntegerFromFile(inFile);

	for (int i{0}; i < numTestPairs; ++i)
	{
		auto testPairsVec = readWordPairsFromFile(inFile);

		bool areSynonyms;

		std::string word1 = toLower(testPairsVec[0]);
		std::string word2 = toLower(testPairsVec[1]);

		areSynonyms = areWordsSynonyms(dictMap, word1, word2);
		if (!areSynonyms)
		{
			areSynonyms = areWordsSynonyms(dictMap, word2, word1);
		}

		std::string line = areSynonyms ? "synonyms" : "different";
		output.push_back(line);
	}


}

std::vector<std::string> readAndprocessFile()
{
	std::string inFileName = "input.txt";
	std::ifstream inFile;
	inFile.open(inFileName);

	std::vector<std::string> output;

	if (inFile.is_open())
	{
		int numTestCases = readIntegerFromFile(inFile);

		for (int i{0}; i < numTestCases; ++i)
		{
			processTestPairs(inFile, output);
		}

		inFile.close();
	}
	else
	{
		std::cout << "Cannot open file: " << inFileName << "\n";
	}

	return output;
}

void createOutputFile(const std::vector<std::string>& output)
{
	std::string outFileName = "output.txt";
	std::ofstream outFile;
	outFile.open(outFileName);

	if (outFile.is_open())
	{
		for (auto out : output)
		{
			outFile << out << "\n";
		}

		outFile.close();
	}
	else
	{
		std::cout << "Could not create file: " << outFileName << "\n";
	}
}

int main()
{

	auto output = readAndprocessFile();
	createOutputFile(output);

	return 0;
}
