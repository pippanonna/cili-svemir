#include "decisiontree.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <set>

Node::Node() {
    //konstruktor
}


void Node::addNode(string attribute, int headerPos, bool isLeaf) {
    this->attribute = attribute;
    this->isLeaf = isLeaf;
    this->headerPos = headerPos;
}

void Node::addChild(string attValue, shared_ptr<Node> child) {
    children.insert_or_assign(attValue, child);
}

void Node::print(int level, string parentBranches) {
    if (level != 0) parentBranches.append(" ");
    if (isLeaf) {
        parentBranches.append(attribute);
        cout << parentBranches << endl;
    } else {
        parentBranches.append(to_string(level + 1) + ":" + attribute + "=");
        for (auto i : children) {
            Node n = *i.second;
            string branch = parentBranches + i.first;
            n.print(level + 1, branch);
        }
    }

}

string Node::getPrediction(vector<string> testData) {
    string prediction = "";
    bool end = false;
    Node n = (*this);

    while(!end) {
        if (n.isLeaf) {
            prediction.append(n.attribute);
            end = true;
        } else {
            string val = testData[n.headerPos];
            auto rep = n.children.find(val);
            if (rep == n.children.end()) {
                n = (*n.children[mostCommonValue]);
            } else {
                n = (*(*rep).second);
            }
        }

    }

    return prediction;
}




ID3::ID3() {
    // konstruktor
}

ID3::~ID3() {
    
}


void ID3::fit(string filename, int level) {
    readFile(filename, true);

    vector<int> X;
    for (int i = 0; i < header.size() - 1; i++) {
        X.push_back(i);
    }

    vector<int> dataset;
    for (int i = 0; i < trainDataset.size(); i++) {
        dataset.push_back(i);
    }
    this->tree = algorithm(dataset, dataset, X, level);

    this->printTree();
}

string ID3::predict(string filename) {
    set<string> uniqueValues;
    unordered_map<string, int> valueReps;
    readFile(filename, false);
    double accuracy = 0.0;
    string predictions = "[PREDICTIONS]:";

    for (int i = 0; i < this->testDataset.size(); i++) {
        string predVal = (*tree).getPrediction(testDataset[i]);
        string trueVal = testDataset[i].back();
        uniqueValues.insert(trueVal);
        valueReps[(trueVal+predVal)]++;
        predictions.append(" " + predVal);
        if (predVal.compare(trueVal) == 0) {
            accuracy += 1.0;
        }
    }
    accuracy /= testDataset.size();
    cout << predictions << endl;
    printf("[ACCURACY]: %6.5f\n", accuracy);

    cout << "[CONFUSION_MATRIX]:" << endl;
    for (auto i : uniqueValues) {
        bool first = true;
        for (auto j : uniqueValues) {
            if (first) {
                first = false;
            } else {
                cout << " ";
            }
            auto rep = valueReps.find(i+j);
            if (rep == valueReps.end()) {
                cout << 0;
            } else {
                cout << (*rep).second;
            }
        }
        cout << endl;
    }


    return predictions;
}

void ID3::readFile(string filename, bool isTrainData) {
    ifstream file;
    file.open(filename);

    if(file.is_open()) {
        bool first = true;
        string line;
        while(getline(file, line)) {
            if (first) {
                first = false;
                if (isTrainData) {
                    string headerVal;
                    stringstream ss(line);
                    while(getline(ss, headerVal, ',')) {
                        this->header.push_back(headerVal);
                    }
                }
            } else {
                string feature;
                vector<string> featureRow;
                stringstream ss(line);
                while(getline(ss, feature, ',')) {
                    featureRow.push_back(feature);
                } 
                if (isTrainData) {
                    this->trainDataset.push_back(featureRow);
                } else {
                    this->testDataset.push_back(featureRow);
                }
            }
        }
        
        file.close();
    }
}

shared_ptr<Node> ID3::algorithm(vector<int> subDataset, vector<int> parentDataset, vector<int> X, int level) {
    string v;
    Node *n = new Node();
    if (subDataset.empty()) {
        v = mostCommonLabel(parentDataset, header.size() - 1); // v = argmax(Dparent, v)
        n->addNode(v, header.size() - 1, true);
        return shared_ptr<Node>(n); // return Leaf(v)
    }

    v = mostCommonLabel(subDataset, header.size() - 1);
    bool isSame = true;
    for (int i = 0; i < subDataset.size(); i++) {
        if (this->trainDataset[subDataset[i]].back().compare(v) != 0) isSame = false;
    }
    if (X.empty() || isSame || level == 0) {
        n->addNode(v, header.size() - 1, true);
        return shared_ptr<Node>(n);
    }

    // x <-- argmaxˇx€X(IG(D, x))
    unordered_map<string, vector<int>> values;
    double E = entropy(subDataset), IG = 0.0;
    int x = 0;
    for (int i = 0; i < X.size(); i++) {
        unordered_map<string, vector<int>> hash;
        double currIG = E;
        for (int j = 0; j < subDataset.size(); j++) {
            hash[this->trainDataset[subDataset[j]][X[i]]].push_back(subDataset[j]);
        }
        for (int j = 0; j < trainDataset.size(); j++) {
            if (hash.count(trainDataset[j][X[i]]) == 0) {
                hash[trainDataset[j][X[i]]];
            }
        }
        for (auto i : hash) {
            currIG -= (i.second.size() * 1.0 / subDataset.size()) * entropy(i.second);
        }
        if ((currIG == IG && header[X[i]].compare(header[x]) < 0) || (currIG > IG)) {
            x = X[i];
            IG = currIG;
            values = hash;
        }
    }


    n->addNode(this->header[x], x, false);
    string a = mostCommonLabel(subDataset, x);
    n->setMostCommonValue(mostCommonLabel(subDataset, x));
    if (level >= 0) level--;
    for (auto val : values) {
        vector<int> newX;
        for (int i = 0; i < X.size(); i++) {
            if (X[i] != x) newX.push_back(X[i]);
        }
		
        shared_ptr<Node> t = this->algorithm(val.second, subDataset, newX, level);
        n->addChild(val.first, t);
    }

    return shared_ptr<Node>(n);
}

string ID3::mostCommonLabel(vector<int> ds, int pos) {
    unordered_map<string, vector<int>> hash;
    for (int i = 0; i < ds.size(); i++) {
        hash[this->trainDataset[ds[i]][pos]].push_back(ds[i]);
    }

    int maxNum = 0;
    string mostCommon = "";
    for (auto i : hash) {
        
        if ((maxNum == i.second.size() && mostCommonLabel(i.second, header.size() - 1).compare(mostCommonLabel(hash[mostCommon], header.size() - 1)) < 0) || maxNum < i.second.size()) {
            mostCommon = i.first;
            maxNum = i.second.size();
        }
    }

    return mostCommon;
}

double ID3::entropy(vector<int> ds) {
    
    unordered_map<string, int> hash;
    double entropy = 0;
    for (int i = 0; i < ds.size(); i++) {
        hash[this->trainDataset[ds[i]].back()]++;
    }
    for (auto i : hash) {
        double p = i.second * 1.0 / ds.size();
        if (p > 0) entropy -= p * log2(p);
    }
    return entropy;
}

void ID3::printTree() {
    cout << "[BRANCHES]:" << endl;
    (*tree).print(0, "");
}
