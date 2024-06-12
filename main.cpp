#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<map>
#include<set>
#include<cmath>

using namespace std;

vector<string> readStringsFromFile(string path);
unsigned int getNumberOfStates(vector<string>& input);
set<char> getuniqueSymbols(vector<string>& input);
vector<map<char,unsigned int>> calculateSimbolFrec(vector<string>& input);
vector<map<char,double>> calculateSimbolProbs(vector<string>& input, vector<map<char,unsigned int>>& frecMaps, set<char>& symbols);
vector<vector<double>> calculateTransitionProbs(vector<string>& input, vector<map<char,unsigned int>>& frecMaps, set<char>& symbols);
 


int main(){
    auto input = readStringsFromFile("../input/file.txt");
    auto symbols = getuniqueSymbols(input);
    auto repetitionMap = calculateSimbolFrec(input);
    auto em_probabilities = calculateSimbolProbs(input, repetitionMap, symbols);
    auto transProbs = calculateTransitionProbs(input, repetitionMap, symbols);
    cout << "Number of states: " << repetitionMap.size() << endl;
    cout << "Emission Probabilities: \n";

    int counter = 0;
    for(auto& rep: repetitionMap){ //cols
        for(auto& symbol : symbols){ //symbols
            cout << "bm" << counter+1 << symbol << "=";
            cout << em_probabilities[counter][symbol] << "\t";
        }
        cout << endl;
        counter++;
    }cout << endl;

    cout << "Probabilities: \n";
    counter = 0;
    for(auto& rep: repetitionMap){ //cols
        for(auto& symbol : symbols){ //symbols
            cout << "P" << counter+1 << symbol << "=";
            cout << em_probabilities[counter][symbol] /(1.0/20.0) << "\t";
        }
        cout << endl << endl;
        counter++;
    }cout << endl;

    cout << "Scores: \n";
    counter = 0;
    for(auto& rep: repetitionMap){ //cols
        for(auto& symbol : symbols){ //symbols
            cout << "S" << counter+1 << symbol << "=";
            cout << log10(em_probabilities[counter][symbol] /(1.0/20.0)) << "\t";
        }
        cout << endl << endl;
        counter++;
    }cout << endl;

    cout << "Transition Probabilities: \n";
    counter = 1;
    for(auto& transition : transProbs){
        cout <<"PM"<<counter<<"M"<<counter+1<<"=";
        cout << transition[0] << "\t";
        cout <<"PM"<<counter<<"D"<<counter+1<<"=";
        cout << transition[1] << "\t";
        cout <<"PM"<<counter<<"I"<<counter+1<<"=";
        cout << transition[2] << "\n";
    }
    
}

vector<vector<double>> calculateTransitionProbs(vector<string>& input, vector<map<char,unsigned int>>& frecMaps, set<char>& symbols){
    double size = double(input.size());
    /*Complexity: O(n)*/
    vector<vector<double>> r;
    auto counter = 0;
    auto prevSize =1;
    for(auto& stateMap: frecMaps){
        vector<double> trans;
        auto temp = (stateMap.find('-')!=stateMap.end()) ? stateMap['-'] : 0;
        trans.push_back((size - temp+1) / (size+prevSize)); //Match
        trans.push_back((temp+1)/ (size+prevSize)); //Delete
        trans.push_back(0.1); //Insert
        if(counter != 0) r.emplace_back(trans);
        prevSize = stateMap.size();
        counter++;
    }
    return r;
}

vector<map<char,double>> calculateSimbolProbs(vector<string>& input, vector<map<char,unsigned int>>& frecMaps, set<char>& symbols){
    /*Complexity: O(n)*/
    vector<map<char,double>> r;
    for(auto& stateMap : frecMaps){
        map<char,double> probMap;
        for(auto& symbol : symbols){
            probMap[symbol] = (stateMap.find(symbol) != stateMap.end()) ? (double(stateMap[symbol])+1.0)/(input.size()+20.0): (0.0+1.0)/27.0;
        }
        r.emplace_back(probMap);
    }
    return r;
}

vector<map<char,unsigned int>> calculateSimbolFrec(vector<string>& input){
    /*Complexity: O(n*m)*/
    unsigned int size = input[0].size();
    vector<map<char,unsigned int>> r;
    r.reserve(size);
    for(auto i = 0; i < size; i++){
        map<char,unsigned int> repetitionPerChar;
        for( auto& string : input){
            repetitionPerChar[string[i]]++;
        }
        /*Delete states with more than half of gaps*/
        if(repetitionPerChar.find('-') == repetitionPerChar.end() ||
            repetitionPerChar['-'] < input.size()/2){
            
            r.emplace_back(repetitionPerChar);
        }
    }
    return r;
}
set<char> getuniqueSymbols(vector<string>& input){
    /*Complexity: O(n*m)*/
    set<char> r;
    for(auto& str : input)
        for(auto& c: str)
            r.insert(c);
    return r;
}

unsigned int getNumberOfStates(vector<string>& input){
    /*Complexity: O(n*m)*/
    unsigned int size = input[0].size();
    unsigned int numGaps = 0;
    unsigned int numStates = 0;
    for(auto i = 0; i < size; i++){
        for( auto& string : input){
            if (string[i] == '-')numGaps++;
        }
        if (numGaps < size/2)numStates++;
        numGaps=0;
    }
    return numStates;
}
vector<string> readStringsFromFile(string path){
    ifstream inputFile(path);
    vector<string> r;
    if(!inputFile.is_open()){
        cout << "Error al abrir el archivo de strings\n";
        return vector<string>();
    }
    else{
        string temp;
        while (getline(inputFile, temp))
            r.emplace_back(temp);
        inputFile.close();
        return r;
    }
}