#include <vector>
#include <string>
#include <set>

using namespace std;

class Clause {
    private:
        set<string> literals;
        bool isGoal = false;
        Clause *parent1 = nullptr;
        Clause *parent2 = nullptr;
    public:
        Clause();
        Clause(string literalsStr);
        Clause(set<string> literals);
        ~Clause();
        string toString();
        bool isTautology();
        bool contains(Clause *other);
        int size() {
            return literals.size();
        }
        bool operator<(const Clause& other) const {
            return literals < other.literals;
        }
        set<string> getLiterals() {
            return this->literals;
        }
        bool derive(Clause *other, Clause *newClause);
        static string negateLiteral(string literal);
        void setParents(Clause *parent1, Clause *parent2) {
            this->parent1 = parent1;
            this->parent2 = parent2;
        }
        Clause *getParent1() {
            return parent1;
        }
        Clause *getParent2() {
            return parent2;
        }
        void setGoal(bool val) { this->isGoal = true; }
        bool isGoal() {
            return isGoal;
        }
        
};

class Resolution {
    private:
        vector<Clause> clauses;
    public:
    Resolution(string clausesPath);
    void resolution();
    void cooking(string cookbook);
    void printResolution();
    bool checkRedundancy(Clause c, vector<Clause> list);
    void deduction(Clause goal);
};