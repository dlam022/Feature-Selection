#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iomanip>

using namespace std;

vector<vector <double> > dataSet;
vector<double> featureset;


void forward(vector<double>, int);
void backward(vector<double>, int);
void loadData(string);
void printfeatures(vector<double>);
void printdata(vector<vector<double> >);
double crossvalidation(vector<int>, int, vector<double>, int );
vector<double> labelfunction(vector<vector<double> >);
vector<vector<double> > removeFunction(vector<vector<double> >);
double accuracy(vector<vector<double> >, vector<int>, int, vector<double>, int);
vector<vector<double> > removeFunctions(vector<vector<double> >, vector<int>, int);
vector<vector<double> > removefromSet(vector<vector<double> >, vector<int>, int);





int main() {
    string filename;


    cout << "Welcome to Derick Lam Feature Selection Algorithm." << endl;
    cout << "File name" << endl;
    cin >> filename;

    ifstream fileInput(filename.c_str());
    if(!fileInput.is_open()) {
        cout << "Could not open the file" << endl;
    }

    loadData(filename);
    vector<double> labels = labelfunction(dataSet);
    vector<vector<double> > twoFeatures = removeFunction(dataSet);

    cout << filename << endl;
    cout << endl;   
    
    // int features;
    // cout << "Please enter total numbers of features: ";
    // cin >> features;
    // cout << features << endl;
    // cout << endl;

    cout << "Type the number of algorithm you want to run." << endl;
    cout << endl;
    cout << endl;

    int choice;
    bool choicecheck = false;
    

    cout << "1 - Forward Selection" << endl;
    cout << "2 - Backward Elimination" << endl;

    cin >> choice;
    cout << choice << endl;

    if(choice == 1 || choice == 2) {
        choicecheck = true;
    }

    while(!choicecheck) {
        cout << "try again" << endl;
        cout << "1- Forward Selection" << endl;
        cout << "2- Backward Elimination" << endl;
        cin >> choice;
        if(choice == 1 || choice == 2) {
            choicecheck = true;
        }
        else{
            choicecheck = false;
        }
    }

    if(choice == 1) {
        forward(labels, choice);
    }

    if(choice == 2) {
        backward(labels, choice);
    }



   return 0;

}

void loadData(string filename) {
    ifstream file(filename.c_str());
    string line;
    string feature;
    
    while(getline(file, line)) {
        stringstream ss(line);
        while(ss >> feature) {
            featureset.push_back(stod(feature)); //converts data to doubles
        }
        dataSet.push_back(featureset);
        featureset.clear(); //clear data to get the next row of data
    }

    file.close();
}

vector<double> labelfunction(vector<vector<double> > data) {
    string label;
    string line;
    vector<double> classlabel;
    for(unsigned int i = 0; i < data.size(); i++) {
        classlabel.push_back(data[i][0]); 
    }
    return classlabel;
}

vector<vector<double> > removeFunction(vector<vector<double> > data) {
    for(unsigned int i = 0; i < data.size(); i++) {
        data[i].erase(data[i].begin());
    }
    return data;
}

vector<vector<double> > removeFunctions(vector<vector<double> > val, vector<int> currentset, int featureadd) {
    vector<int> columnskeep = currentset;
    columnskeep.push_back(featureadd);
    for(unsigned int i = 0; i < val.size(); i++) {
        for(unsigned int j = 0; j < val[i].size(); j++) {
            if(find(columnskeep.begin(), columnskeep.end(), j) == columnskeep.end()) {
                val[i][j] = 0;
            }
        }
    }
    return val;
}

vector<vector<double> > removefromSet(vector<vector<double> >val, vector<int> currentset, int featureremove) {
    vector<int> columnskeep = currentset;
    columnskeep.erase(remove(columnskeep.begin(), columnskeep.end(), featureremove), columnskeep.end());
    for(unsigned int i = 0; i < val.size(); i++) {
        for(unsigned int j = 0; j < val[i].size(); j++) {
            if(find(columnskeep.begin(), columnskeep.end(), j) == columnskeep.end()) {
                val[i][j] = 0;
            }
        }
    }
    return val;
}


double accuracy(vector<vector<double> >val, vector<int> currentset, int featuretoaddorremove, vector<double> classLabels, int choice) {
    if(choice == 1) {
        val = removeFunctions(val, currentset, featuretoaddorremove);
    }

    else if(choice == 2) {
        val = removefromSet(val, currentset, featuretoaddorremove);
    }

    double accuracy = 0.0;
    double distance = 0.0;
    double label;
    double nearestdistance;
    double nearestlocation;
    double nearestlabel;
    double classified = 0.0;
    vector<double> objectclassify;
    vector<double> objectcompare;
    

    for(unsigned int i = 0; i < val.size(); i++) {
        objectclassify = val[i];
        label = classLabels[i];
        nearestdistance = 4294920212;
        nearestlocation = 4294920212;

        for(unsigned int k = 0; k < val.size(); k++) {
            if(k != i) {
                distance = 0.0;
                objectcompare = val[k];
                for(unsigned int l = 0; l < objectclassify.size(); l++) {
                    distance += pow(objectclassify[l] - objectcompare[l],2);
                }

                distance = sqrt(distance);
                if(distance < nearestdistance) {
                    nearestdistance = distance;
                    nearestlocation = k;
                    nearestlabel = classLabels[k];
                }

            }
        }
        if(label == nearestlabel) {
            classified++;
        }


    }

    accuracy = classified / val.size();
    return accuracy;



}


void forward(vector<double> labels, int choice) {
    vector<int> currentfeatures;
    vector<int> bestfeatures;
    double bestacc = 0.0;
    double localacc = 0.0;
    double totalacc;

    vector<vector<double> > val;

    val = removeFunction(dataSet);
    cout << "this dataset has " << val[0].size() << " features (not including the class attribute), with 100 instances" << endl;
    cout << "beginning search" << endl << endl;

    for(unsigned int i = 0; i < val[0].size(); i++) {
        int featurelevel = -1;
        int bestfeature = -1;
        bestacc = 0.0;
    

       // cout << "beginning search" << endl << endl;

        for(unsigned int k = 0; k < val[0].size(); ++k) {

            if(find(currentfeatures.begin(), currentfeatures.end(), k) == currentfeatures.end()) {
                //cout << "hello";
                localacc = accuracy(val, currentfeatures, k, labels, choice);
                cout << "Using features(s) {" << k + 1 << "} with the current accuracy of: " << localacc << "%, features {" ;
                for(unsigned int l = 0; l < currentfeatures.size(); l++) {
                    cout << currentfeatures[l] + 1 << ", ";
                }
                

                cout << k + 1 << "} " << endl;

                if(localacc > bestacc) {
                    bestacc = localacc;
                    featurelevel = k;
                }

                if(localacc > totalacc) {
                    totalacc = localacc;
                    bestfeature = k;
                }
            
            }        
    

        }
        cout << "Adding " << featurelevel + 1;
        cout << " with an accuracy of " << bestacc << "%" << endl << endl;
        currentfeatures.push_back(featurelevel);
        if(bestfeature >= 0) {
            bestfeatures.push_back(bestfeature);
        }
        bestfeature = -1;
    }

    cout << endl;
    cout << endl;
    cout << "finish search.  The best feature subset is { ";
    for (unsigned int i = 0; i < bestfeatures.size(); i++) {
        cout << bestfeatures[i] + 1<< " ";
    }
    cout << "}, which has an accuracy of " << totalacc << "%" << endl;



    


}

void backward(vector<double> label, int choice) {
    vector<vector<double> > val;
    vector<int> currentfeatures;
    vector<int> bestfeatures;
    vector<int> printfeatures;
    double bestacc = 0.0;
    double localacc = 0.0;
    double totalacc;

    
    val = removeFunction(dataSet);
    cout << "this dataset has " << val[0].size() << " features (not including the class attribute), with 100 instances" << endl;
    cout << "beginning search" << endl << endl;

    for(unsigned int i = 0; i < val[0].size(); i++) {
        currentfeatures.push_back(i);
    }

    for(unsigned int i = 0; i < val[0].size(); i++) {
        int removal = -1;
        bestacc = 0.0;

        for(unsigned int k = 0; k < val[0].size(); k++) {
            if(find(currentfeatures.begin(), currentfeatures.end(), k) != currentfeatures.end()) {
                localacc = accuracy(val, currentfeatures, k, label, choice);
                cout << "Using features(s) {" << k + 1 << "} with the current accuracy of: " << localacc << "% " <<  "using features {";
                printfeatures = currentfeatures;
                printfeatures.erase(remove(printfeatures.begin(), printfeatures.end(), k), printfeatures.end());
                for(unsigned int l = 0; l < printfeatures.size(); l++) {
                    if(l < printfeatures.size() - 1) {
                        cout << printfeatures[l] + 1 << ", ";
                    }
                    else{
                        cout << printfeatures[l] + 1;
                    }
                }
                cout << "}" << endl;
                if(localacc > bestacc) {
                    bestacc = localacc;
                    removal = k;
                }

                if(localacc > totalacc) {
                    totalacc = localacc;
                    bestfeatures = currentfeatures;
                    bestfeatures.erase(remove(bestfeatures.begin(), bestfeatures.end(), k), bestfeatures.end());
                }
            }

        }
        cout << "removing " << removal + 1 << " " << endl;
        currentfeatures.erase(remove(currentfeatures.begin(), currentfeatures.end(), removal), currentfeatures.end());
    }

    cout << endl;
    cout << endl;
    cout << "best features to use: {";
    for(unsigned int i = 0; i < bestfeatures.size(); i++) {
        cout << bestfeatures[i] + 1 << " ";
    }
    cout << "}" << endl;

    cout << endl << "The best accuracy from this data set is: " << totalacc << "%" << endl;

}
