#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> stopWords = {
    "the", "is", "a", "an", "of",
    "and", "to", "in", "on", "for"
};

bool isStopWord(string word) {

    for (string x : stopWords) {

        if (x == word) {
            return true;
        }
    }

    return false;
}

string readFile(string filename) {

    ifstream file(filename);

    if (!file) {
        cout << "\nError Opening File!\n";
        return "";
    }

    string word, content;

    while (file >> word) {
        content += word + " ";
    }

    return content;
}

string toLowerCase(string text) {

    for (int i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }

    return text;
}

map<string, int> getWordFrequency(string text) {

    map<string, int> freq;

    stringstream ss(text);

    string word;

    while (ss >> word) {

        word.erase(remove_if(word.begin(),
                             word.end(),
                             ::ispunct),
                             word.end());

        if (isStopWord(word)) {
            continue;
        }
if (word.empty()) {
    continue;
}
        freq[word]++;
    }

    return freq;
}

double calculateSimilarity(map<string, int> file1,
                           map<string, int> file2) {

    int commonWords = 0;
    int totalWords = 0;

    for (auto x : file1) {

        totalWords += x.second;

        if (file2.find(x.first) != file2.end()) {

            commonWords += min(x.second,
                               file2[x.first]);
        }
    }

    if (totalWords == 0)
        return 0;

    return (double) commonWords
           / totalWords * 100;
}

void mostRepeatedWords(map<string, int> freq) {

    int maxFreq = 0;

    for (auto x : freq) {

        if (x.second > maxFreq) {
            maxFreq = x.second;
        }
    }

    if (maxFreq <= 1) {

        cout << "\nNo Repeated Words Found\n";
    }

    else {

        cout << "\nMost Repeated Words:\n";

        for (auto x : freq) {

            if (x.second == maxFreq) {

                cout << x.first
                     << " -> "
                     << x.second
                     << " times\n";
            }
        }
    }
}

void topFrequentWords(map<string, int> freq) {

    vector<pair<string, int>> words;

    for (auto x : freq) {
        words.push_back(x);
    }

    sort(words.begin(), words.end(),

         [](pair<string, int> a,
            pair<string, int> b) {

           if (a.second == b.second)
    return a.first < b.first;

return a.second > b.second;
         });

    cout << "\nTop Frequent Words:\n";

    int limit = min(5, (int)words.size());

    for (int i = 0; i < limit; i++) {

        cout << words[i].first
             << " -> "
             << words[i].second
             << endl;
    }
}

string longestCommonWord(map<string, int> file1,
                         map<string, int> file2) {

    string longest = "";

    for (auto x : file1) {

        if (file2.find(x.first) != file2.end()) {

            if (x.first.length() >
                longest.length()) {

                longest = x.first;
            }
        }
    }

    return longest;
}

void similarityBar(double similarity) {

    cout << "\nSimilarity Bar:\n[";

    int bars = similarity / 5;

    for (int i = 0; i < bars; i++) {
        cout << "#";
    }

    for (int i = bars; i < 20; i++) {
        cout << "-";
    }

    cout << "]\n";
}

void saveReport(double similarity,
                int commonCount,
                string longestWord) {
                    
ofstream report("report.txt", ios::app);

    report << "PLAGIARISM REPORT\n";
    report << "========================\n";

    report << "Similarity Percentage: "
           << similarity << "%\n";

    report << "Common Words: "
           << commonCount << "\n";

    report << "Longest Common Word: "
           << longestWord << "\n";

    report.close();
}

int main() {

    string file1, file2;

    cout << "Enter first file name: ";
    cin >> file1;

    cout << "Enter second file name: ";
    cin >> file2;

    string text1 =
        toLowerCase(readFile(file1));

    string text2 =
        toLowerCase(readFile(file2));

    map<string, int> freq1 =
        getWordFrequency(text1);

    map<string, int> freq2 =
        getWordFrequency(text2);

    double similarity =
        calculateSimilarity(freq1, freq2);

    cout << "\n==============================";
    cout << "\nPLAGIARISM REPORT";
    cout << "\n==============================\n";

    cout << "\nSimilarity Percentage: "
         << similarity << "%\n";

    if (similarity >= 80) {

        cout << "Plagiarism Level: High\n";
    }

    else if (similarity >= 50) {

        cout << "Plagiarism Level: Moderate\n";
    }

    else {

        cout << "Plagiarism Level: Low\n";
    }

    similarityBar(similarity);

    cout << "\nTotal Unique Words in File 1: "
         << freq1.size() << endl;

    cout << "Total Unique Words in File 2: "
         << freq2.size() << endl;

    int commonCount = 0;

    cout << "\nCommon Words:\n";
    cout << "------------------\n";

    for (auto x : freq1) {

        if (freq2.find(x.first)
            != freq2.end()) {

            commonCount++;

            cout << x.first << endl;
        }
    }

    cout << "\nNumber of Common Words: "
         << commonCount << endl;

    string longestWord =
        longestCommonWord(freq1, freq2);

    cout << "\nLongest Common Word: "
         << longestWord << endl;

    cout << "\nFILE 1 ANALYSIS\n";
    mostRepeatedWords(freq1);
    topFrequentWords(freq1);

    cout << "\nFILE 2 ANALYSIS\n";
    mostRepeatedWords(freq2);
    topFrequentWords(freq2);

    saveReport(similarity,
               commonCount,
               longestWord);

    cout << "\nReport Saved Successfully "
         << "in report.txt\n";

    return 0;
}