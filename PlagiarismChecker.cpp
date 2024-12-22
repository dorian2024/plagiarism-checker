/* 
Name: Umaima
Roll number: 23i-0790
DS Assignment 1
Section A*/

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const int MAX_WORDS = 1000;
const int MAX_LINE_LENGTH = 1000;

char* getText(int documentNumber);

int stringCount(char* array, char* word) {
	int count = 0;
	// find the length of the array and word
	int arrayLength = 0;
	while (array[arrayLength] != '\0') {
		arrayLength++;
	}

	int wordLength = 0;
	while (word[wordLength] != '\0') {
		wordLength++;
	}

	if (arrayLength < wordLength) {
		return 0; // the array is too short to contain the word
	}

	// look through the array and find the word
	for (int i = 0; i <= arrayLength - wordLength; i++) {
		// ceck if the substring starting at index i matches the word
		bool found = true;
		for (int j = 0; j < wordLength; j++) {
			if (array[i + j] != word[j]) {
				found = false;
				break;
			}
		}

		// ensure the word is followed by a space or end of string
		if (found && (i + wordLength == arrayLength || array[i + wordLength] == ' ')) {
			// ensure it's the start of the word or preceded by a space
			if (i == 0 || array[i - 1] == ' ') {
				count++; // word found
			}
		}
	}

	return count; // word not found
}
bool stringCompare(char* array, char* word) {
	// Find the length of the array and word manually
	int arrayLength = 0;
	while (array[arrayLength] != '\0') {
		arrayLength++;
	}

	int wordLength = 0;
	while (word[wordLength] != '\0') {
		wordLength++;
	}

	if (arrayLength < wordLength) {
		return false; // The array is too short to contain the word
	}

	// Traverse through the array and find the word
	for (int i = 0; i <= arrayLength - wordLength; i++) {
		// Check if the substring starting at index i matches the word
		bool found = true;
		for (int j = 0; j < wordLength; j++) {
			if (array[i + j] != word[j]) {
				found = false;
				break;
			}
		}

		// Ensure the word is followed by a space or end of string
		if (found && (i + wordLength == arrayLength || array[i + wordLength] == ' ')) {
			// Ensure it's the start of the word or preceded by a space
			if (i == 0 || array[i - 1] == ' ') {
				return true; // Word found
			}
		}
	}

	return false; // Word not found
}


char* stringAppend(char* array, char* add) {
	// Length of the original array
	int arrayLength = 0;
	while (array[arrayLength] != '\0') {
		arrayLength++;
	}

	// Length of the array to be added
	int addLength = 0;
	while (add[addLength] != '\0') {
		addLength++;
	}

	// Allocate memory for the result
	// +1 for space, +1 for null terminator
	char* result = new char[arrayLength + addLength + 2];

	// Copy the original array to the result
	for (int i = 0; i < arrayLength; i++) {
		result[i] = array[i];
	}

	// Add a space if the original array is not empty
	if (arrayLength > 0) {
		result[arrayLength] = ' ';
		arrayLength++; // Update arrayLength to include the space
	}

	// Copy the 'add' string to the result
	for (int i = 0; i < addLength; i++) {
		result[arrayLength + i] = add[i];
	}

	// Null-terminate the new string
	result[arrayLength + addLength] = '\0';

	return result;
}


class Documents {
	int numberOfDocuments;
	char** documents;
	char** stopWords;
	int* docLength;  //null character included
	int punctuationCount;
	int* stopWordLength;
	int numberOfStopWords;
	int uniqueWordCount;
	const char punctuation[23] = { '(', ')', '.','~', '!', '@', '#', '$', '%', '^', '_', '+', '=', '"', ';', ':', '/', '?', '>', ',', '<', '\n', '&'}; //extra spaces and 's not included here
public:
	int** frequency;
	char** uniqueWords;
	double** cosineSimilarities;
	Documents() {
		numberOfDocuments = 0;
		documents = NULL;
		stopWords = NULL;
		docLength = NULL;
		stopWordLength = NULL;
		punctuationCount = 23;
	}
	char** getUniqueWords() {
		return uniqueWords;
	}
	int getUniqueWordCount() {
		return uniqueWordCount;
	}
	int** getFrequency() {
		return frequency;
	}
	int getNumberOfDocuments() {
		return numberOfDocuments;
	}
	void setNumberOfDocuments(int num) {
		numberOfDocuments = num;
	}
	void setStopWords(int count, char* temp) {
		if (stopWords != NULL) {
			return;
		}
		//count the number of words
		for (int i = 0; i < count; i++) {
			// Skip over spaces
			while (i < count && temp[i] == ' ') {
				i++;
			}
			//if a word is found
			if (i < count && temp[i] != ' ') {
				numberOfStopWords++;
				while (i < count && temp[i] != ' ') {
					i++; //find next word
				}
			}
		}
		stopWords = new char* [numberOfStopWords];
		stopWordLength = new int[numberOfStopWords];
		int length = 0; // length of each word
		int index = 0;  // index of stop word

		for (int i = 0; i < count; i++) {
			// skip over leading spaces
			while (i < count && temp[i] == ' ') {
				i++;
			}

			// count word length
			int start = i;
			while (i < count && temp[i] != ' ') {
				i++;
			}
			length = i - start; // Length of the word

			if (length > 0 && index < numberOfStopWords) {
				// allocate memory for this word
				stopWords[index] = new char[length + 1];  // +1 for null terminator
				stopWordLength[index] = length;

				// copy
				for (int k = 0; k < length; k++) {
					stopWords[index][k] = temp[start + k];
				}
				stopWords[index][length] = '\0';

				index++; // Move to the next word
			}
		}
	}
	char** getStopWords() {
		return stopWords;
	} 
	void setDocuments(char** paths) {
		documents = new char* [numberOfDocuments];
		docLength = new int[numberOfDocuments];
		int index = 0;
		for (int i = 0; i < numberOfDocuments; i++) {
			char* path = paths[index];
			ifstream file(path);
			//count number of characters in the doc
			int count = 0;
			char c;
			while (file.get(c)) {
				count++;
			}
			if (!file.is_open()) {
				cerr << "Error: could not open the file" << endl;
			}
			//this is supposed to reset filestream to the beginning
			file.clear();
			file.seekg(0, ios::beg);
			//copy text
			documents[index] = new char[count]; //plus one for null character
			file.getline(documents[index], count + 1);
			documents[index][count] = '\0';
			docLength[index] = count + 1; //null character included
			index++;
			file.close();
		}
	}
	char* getDocument(int docNum) {
		return documents[docNum - 1];
	}

	void removePunctuationMarks(int num) {
		//in punctuation array:
		for (int i = 0; i < punctuationCount; i++) {
			removeAllCharacters(documents[num], punctuation[i], num);
		}
		//extra spaces:
		for (int i = 0; i < docLength[num] - 1; i++) {
			//if first index is a space
			char ch = documents[num][i];
			if (ch == ' ' && i == 0) {
				removeChar(documents[num], i, num);
			}
			else if (ch == ' ' && documents[num][i + 1] == ' ' && i < docLength[num]) {
				removeChar(documents[num], i, num);
				i--;
			}
			// 's cases:
			else if (i < docLength[num] - 2 && ch == '\'' && documents[num][i + 1] == 's' && documents[num][i + 2] == ' ') {
				removeChar(documents[num], i, num); //for '
				removeChar(documents[num], i, num); // for s
			}
			//if last index is a space
			else if (i == docLength[num] - 1 && ch == ' ') {
				removeChar(documents[num], i, num);
				i--;
			}
		}
	}
	//removes all instances of a character specified in parameters
	void removeAllCharacters(char*& doc, char ch, int docNum) {
		int index = 0; // the position in the new string
		int count = 0;// length of doc
		while (doc[count] != '\0') {
			count++;
		}
		//copy new array without the ch
		for (int i = 0; i < count; i++) {
			if (doc[i] != ch) { 
				doc[index++] = doc[i];
			}
		}
		doc[index] = '\0';
		// update the doc length
		docLength[docNum] = index;
	}
	//removes the character at a specific index:
	void removeChar(char*& doc, int index, int docNum) {
		//check bounds
		if (index > docLength[docNum]) {
			return;
		}
		int current = 0; // the position in the new string
		int count = 0;// length of doc
		while (doc[count] != '\0') {
			count++;
		}
		//copy new array without the specified index
		for (int i = 0; i < count; i++) {
			if (i != index) {
				doc[current++] = doc[i];
			}
		}
		doc[current] = '\0';
		// update the doc length
		docLength[docNum] = current;
	}
	void convertLowercase(int num) {
		for (int i = 0; i < docLength[num]; i++) {
			char ch = documents[num][i];
			if (ch >= 65 && ch <= 95) {
				documents[num][i] += 32;
			}
		}
	}
	void removeWords(char*& array, int& arrayLength, char* word, int wordLength) {

		for (int i = 0; i < arrayLength; i++) {
			bool match = true;

			// Ensure there's enough space in the array to check for the full word
			if ((i == 0 || array[i - 1] == ' ') && (i + wordLength <= arrayLength)) {
				// Check for matching word
				for (int j = 0; j < wordLength; j++) {
					if (array[i + j] != word[j]) {
						match = false;
						break;
					}
				}

				// Check if it's a full word match (either end of string or space after the word)
				if (match && (i + wordLength == arrayLength || array[i + wordLength] == ' ' || array[i + wordLength] == '\0')) {
					// Start the shift process
					int start = i + wordLength; // start of the part to shift
					if (start < arrayLength && array[start] == ' ') {
						start++; // also remove the space after the word
					}

					// Perform the shifting manually
					int shiftLength = start - i; // number of characters to remove
					for (int k = i; k < arrayLength - shiftLength; k++) {
						array[k] = array[k + shiftLength];
					}

					// Update the array length
					arrayLength -= shiftLength;
					if (array[arrayLength - 1] == ' ') {
						arrayLength--;
					}
					array[arrayLength] = '\0';

					// Adjust the index to check for consecutive words
					i--;
				}
			}
		}
	}

	void removeStopWords(int num) {
		for (int i = 0; i < 10; i++) {
			removeWords(documents[num], docLength[num], stopWords[i], stopWordLength[i]);
		}
	}
	void generateFrequencies() {
		uniqueWords = new char*[MAX_WORDS];
		for (int i = 0; i< MAX_WORDS; i++) {
			uniqueWords[i] = new char[MAX_LINE_LENGTH];
		}
		uniqueWordCount = 0;
		//create 1d array of unique words
		char* uniqueArray = new char[1];
		uniqueArray[0] = '\0';

		char* temp = new char[MAX_LINE_LENGTH];
		int tempCount = 0;
		for (int i = 0;i < numberOfDocuments; i++) {
			for (int index = 0; index < docLength[i]; index++) {
				tempCount = 0;
					while (documents[i][index] != ' ') {
						temp[tempCount] = documents[i][index];
						tempCount++;
						index++;
					}
					temp[tempCount] = '\0';//word
					//if it hasnt been counted already add it to the array:
					if (!stringCompare(uniqueArray, temp))
					{
						uniqueArray = stringAppend(uniqueArray, temp);
						uniqueWordCount++;
					}
			}
		}
		//turn it into a 2d array:
		int* uniqueWordLength = new int[uniqueWordCount];
		int length = 0; // length of each word
		int index = 0;  // index of stop word
		int count = 0;
		while (uniqueArray[count] != '\0') {
			count++;
		}
		for (int i = 0; i < count; i++) {
			// skip over leading spaces
			while (i < count && uniqueArray[i] == ' ') {
				i++;
			}

			// count word length
			int start = i;
			while (i < count && uniqueArray[i] != ' ') {
				i++;
			}
			length = i - start; // Length of the word

			if (length > 0 && index < uniqueWordCount) {
				// allocate memory for this word
				uniqueWords[index] = new char[length + 1];  // +1 for null terminator
				uniqueWordLength[index] = length;

				// copy
				for (int k = 0; k < length; k++) {
					uniqueWords[index][k] = uniqueArray[start + k];
				}
				uniqueWords[index][length] = '\0';

				index++; // Move to the next word
			}
		}
		//find frequency
		frequency = new int*[numberOfDocuments];
		for (int i = 0; i < numberOfDocuments; i++) {
			frequency[i] = new int[uniqueWordCount];
			for (int j = 0; j < uniqueWordCount; j++) {
				frequency[i][j] = stringCount(documents[i], uniqueWords[j]);
			}
		}
	}

	void setCosineSimilarities(double**& similarities, int** documentFrequency) {
		documentFrequency = frequency;
		similarities = cosineSimilarities;
		int dotProduct = 0;
		double magnitude1 = 0;
		double magnitude2 = 0;
		//create array
		cosineSimilarities = new double* [numberOfDocuments];
		for (int i = 0; i < numberOfDocuments; i++) {
			cosineSimilarities[i] = new double[numberOfDocuments];

		}
		for (int i = 0; i < numberOfDocuments; i++) {
			for (int j = 0; j < numberOfDocuments; j++) {
				dotProduct = 0;
				//for dot product
				for (int k = 0; k < uniqueWordCount; k++) {
					dotProduct += documentFrequency[i][k] * documentFrequency[j][k];
					magnitude1 += documentFrequency[i][k] * documentFrequency[i][k]; //or just use pow function
					magnitude2 += documentFrequency[j][k] * documentFrequency[j][k];
				}
				if (i == j) {
					cosineSimilarities[i][j] = 100;
				}
				else {
					magnitude1 = sqrt(magnitude1);
					magnitude2 = sqrt(magnitude2);
					if (magnitude1 != 0 && magnitude2 != 0) {
						cosineSimilarities[i][j] = std::round(dotProduct / (magnitude1 * magnitude2) * 100.0);
						//similarities[i][j] = std::round(similarities[i][j]);
					}
					else {
						cosineSimilarities[i][j] = 0;
					}

				}
				magnitude1 = 0;
				magnitude2 = 0;
			}
		}
	}
};
Documents docs;


void readInput(const char* pathofInputFile) {

	char** pathsOfDocuments;
	char temp[MAX_LINE_LENGTH];

	//find and store number of documents
	//find and store paths of all the documents
	//find and store the stop words
	//store documents

	ifstream file(pathofInputFile);
	char* ignore = new char[13];
	//point towards the actual words:
	file.getline(ignore, 13, ':');
	//copy stop words
	int count = 0;
	file.getline(temp, MAX_LINE_LENGTH, '\n');
	while (temp[count]) {
		count++; //count characters
	}
	docs.setStopWords(count, temp);
	
	//now file stream points towards second row
	//save number of documents
	int num = 0;
	file >> num;
	docs.setNumberOfDocuments(num);
	file.getline(temp, MAX_LINE_LENGTH, '\n'); //to ignore the rest of the line

	////to store the paths of the documents
	num = docs.getNumberOfDocuments();
	pathsOfDocuments = new char* [num];
	count = 0;
	for (int i = 0; i < num; i++) {
		//save in temp
		file.getline(temp, MAX_LINE_LENGTH, '\n');
		while (temp[count] != '\0' && count <= MAX_LINE_LENGTH) {
			count++; //count length of each path
		}
		pathsOfDocuments[i] = new char[count + 1]; //memory allocation
		for (int j = 0; j < count; j++) {
			pathsOfDocuments[i][j] = temp[j]; //copy
		}
		pathsOfDocuments[i][count] = '\0';
		count = 0;
	}
	//store documents in document array
	docs.setDocuments(pathsOfDocuments);
	file.close();
}
int getNumberOfDocument() {
	return docs.getNumberOfDocuments();
}

char* getText(int documentNumber) {
	return docs.getDocument(documentNumber);
}


void removePunctuationMarks() {
	//remove punctutation from each doc one by one
	for (int i = 0; i < docs.getNumberOfDocuments(); i++) {
		docs.removePunctuationMarks(i);
	}
}

// Function to convert all characters in documents to lowercase
void convertUpperToLowerCase() {
	for (int i = 0; i < docs.getNumberOfDocuments(); i++) {
		docs.convertLowercase(i);
	}
}

// Function to remove stopwords from documents
void removeStopWords() {
	for (int i = 0; i < docs.getNumberOfDocuments(); i++) {
		docs.removeStopWords(i);
	}
}

// Function to generate frequencies of each unique word in each document
void generateFrequencies(char**& uniqueWords, int& uniqueCount, int**& documentFrequency) {
	docs.generateFrequencies();
	uniqueCount = docs.getUniqueWordCount();
	uniqueWords = new char* [uniqueCount];
	for (int i = 0; i < uniqueCount; i++) {
		uniqueWords[i] = docs.uniqueWords[i];
	}
	documentFrequency = docs.frequency;	
}
int getFrequency(char* word, int documentNum) {
	// -1 if not found
	char** uniqueWords = docs.getUniqueWords();
	int wordIndex = -1;
	int wordLength = 0;

	// Get the length of the word
	while (word[wordLength] != '\0') {
		wordLength++;
	}

	// Access frequency array
	int** freq = docs.frequency;

	// Iterate over unique words to find the matching word
	for (int i = 0; i < docs.getUniqueWordCount(); i++) {
		int uniqueWordLength = 0;

		// Get the length of the unique word
		while (uniqueWords[i][uniqueWordLength] != '\0') {
			uniqueWordLength++;
		}

		// Compare lengths first
		if (uniqueWordLength != wordLength) {
			continue; // Skip words of different lengths
		}

		// Compare characters of the unique word and the search word
		bool match = true;
		for (int j = 0; j < wordLength; j++) {
			if (uniqueWords[i][j] != word[j]) {
				match = false;
				break;
			}
		}

		// If words match, store the index
		if (match) {
			wordIndex = i;
			return freq[documentNum - 1][wordIndex];
		}
	}
	return -1;
}

void calculateAllCosineSimilarities(double**& similarities, int** documentFrequency) {
	docs.setCosineSimilarities(similarities, documentFrequency);
}

double similarityIn(int doucmentNum1, int documentNum2) {
	double value = docs.cosineSimilarities[doucmentNum1 - 1][documentNum2 - 1];
	return value;
}