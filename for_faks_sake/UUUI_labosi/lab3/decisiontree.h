#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

using namespace std;

class Node {
private:
    string attribute; // znacajka
    int headerPos;
    bool isLeaf;
    string mostCommonValue;
    map<string, shared_ptr<Node>> children; // vrijednosti odluke -> nove znacajke/klasifikacijske odluke (isLeaf)
public:
    Node();
    void addNode(string attribute, int headerPos, bool isLeaf);
    void addChild(string attValue, shared_ptr<Node> child);
    void print(int level, string parentBranches);
    string getPrediction(vector<string> testData);
    void setMostCommonValue(string mostCommonValue) { this->mostCommonValue = mostCommonValue; }
    string getMostCommonValue() { return this->mostCommonValue; }
};

class ID3 {
private:
    vector<string> header;
    vector<vector<string>> trainDataset;
    vector<vector<string>> testDataset;
    shared_ptr<Node> tree;
    void readFile(string filename, bool isTrainData);
    shared_ptr<Node> algorithm(vector<int> subDataset, vector<int> parentDataset, vector<int> X, int level);
    string mostCommonLabel(vector<int> dataset, int pos);
    double entropy(vector<int> ds);
public:
    ID3();
    ~ID3();
    void fit(string filename, int level);
    string predict(string filename);
    void printTree();

};
