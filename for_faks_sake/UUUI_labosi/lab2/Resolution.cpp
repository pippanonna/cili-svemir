#include "Resolution.h"
#include <iostream>
#include <regex>
#include <algorithm>
#include <fstream>
#include <set>
#include <map>
#include <utility>
#include <queue>

using namespace std;

Clause::Clause() {
    // nista
}

Clause::Clause(string literalsStr) {
    regex re(" v ");
    sregex_token_iterator first{literalsStr.begin(), literalsStr.end(), re, -1}, last;
    vector<string> tokens{first, last};

    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        literals.insert(*it);
    }
};


Clause::Clause(set<string> literals) {
    this->literals = literals;
}


string Clause::toString() {
    if (literals.empty()) return "NIL";

    string strClause = "";
    for (auto it = literals.begin(); it != literals.end(); ++it) {
        if (it != literals.begin()) {
            strClause += " v ";
        }
        strClause += *it;
    }
    return strClause;
}

bool Clause::isTautology() {
    for (string s : literals) {
        if (s.front() == '~') break;

        string comparedString = "~" + s;
        if (literals.find(comparedString) != literals.end()) {
            return true;
        }
    }
    return false;
}

bool Clause::contains(Clause *other) {
    /* cout << "This : ";
    this->printClause();
    cout << "That : ";
    other->printClause(); */
    if (other->literals.size() > this->literals.size()) {
        //printf("Velicina other %d, velicina this %d\n", other->literals.size(), this->literals.size());
        return false;
    }
    auto it = other->literals.begin();
    auto it2 = this->literals.begin();
    int matched = 0;

    while(it != other->literals.end() && it2 != this->literals.end()) {
        // cout << *it << ", " << *it2 << endl;
        if ((*it).compare((*it2)) > 0) {
            // cout << "Nismo jos dosli...\n";
            --it;
        } else if ((*it).compare((*it2)) < 0) {
            // cout << "Ne poklapaju se literali " << (*it) << " i " << (*it2) << endl;
            return false;
        } else {
            matched++;
        }
        ++it2;
        ++it;
    }


    if (matched != other->literals.size()) {
        // cout << "Match unsuccesful\n";
        return false;
    }
    // cout << "True!\n";
    return true;
}

bool Clause::derive(Clause *other, Clause *newClause) {
    // cout << "This: " << this->toString() << endl;
    // cout << "That: " << other->toString() << endl;
    for (string lit : other->getLiterals()) {
        string comparedStr = negateLiteral(lit);
        if (literals.find(comparedStr) != literals.end()) {
            // cout << "Lit: " << lit << ", neg: " << comparedStr << endl;

            set<string> derivedLit = other->getLiterals();
            derivedLit.insert(literals.begin(), literals.end());
            derivedLit.erase(comparedStr);
            derivedLit.erase(lit);
            newClause->literals = derivedLit;
            newClause->parent1 = this;
            newClause->parent2 = other;
            return true;
        }
        
    }
    return false;
}

string Clause::negateLiteral(string literal) {
    if (literal.front() == '~') {
        return literal.substr(1);
    }
    return "~" + literal;   
}



bool sortBySize(Clause a, Clause b) {
    return a.size() < b.size();
}

bool compare(Clause a, Clause b) {
    if (a.size() != b.size()) return a.size() < b.size();
    auto it = a.getLiterals().begin(), itEnd = a.getLiterals().end(), it2 = b.getLiterals().begin();

    while (it != itEnd) {
        if((*it).compare((*it2)) != 0) {
            return (*it).compare((*it2)) < 0;
        }
        ++it;
        ++it2;
    }

    return true;
}

bool compareForPrint(Clause a, Clause b) {
    if (a.getParent1() == nullptr && b.getParent1() == nullptr) {
        if (a.isGoal() && !b.isGoal()) return false;
        return true;
    }
    if(a.getParent1() == nullptr && b.getParent1() != nullptr) return true;
    if(b.getParent1() == nullptr && a.getParent1() != nullptr) return false;
    return true;
}

Resolution::Resolution(string clausesPath) {
    std::ifstream file;
    file.open(clausesPath);

    if(file.is_open()) {
        std::string line;
        while(std::getline(file, line)) {
            if (line.front() == '#') continue;

            for (int i = 0; line[i]!='\0'; i++) {
                if (line[i] >= 'A' && line[i] <= 'Z') {
                    line[i] = line[i] + 32;
                }
            }

            Clause c = Clause(line);
            if(!c.isTautology()) clauses.push_back(c);
        }
        
        file.close();
    }

    /* auto it = clauses.begin();

    while(it != clauses.end()) {
        checkRedundancy(*it);
        ++it;
    } */
}

void Resolution::printResolution() {
    cout << "Klauzule: " << endl;
    for (int i = 0; i < clauses.size(); i++) {
        cout << clauses.at(i).toString() << endl;
    }

}

bool Resolution::checkRedundancy(Clause c, vector<Clause> list) {
    auto it = list.begin();

    while(it != list.end()) {
        if (c.contains(&(*it))) {
            return false;
        /* } else if ((*it).contains(&c)) {
            it = list.erase(it); */
        } else {
            ++it;
        }
    }
    return true;
}

void Resolution::resolution() {
    Clause goal = clauses.back();
    clauses.pop_back();
    goal.setGoal(true);

    deduction(goal);
}
void Resolution::cooking(string cookbook) {
    std::ifstream file;
    file.open(cookbook);

    if(file.is_open()) {
        std::string line;
        while(std::getline(file, line)) {
            if (line.front() == '#') continue;

            for (int i = 0; line[i]!='\0'; i++) {
                if (line[i] >= 'A' && line[i] <= 'Z') {
                    line[i] = line[i] + 32;
                }
            }

            char action = line.back();
            line.pop_back();
            line.pop_back();

            Clause input = Clause(line);
            if (action == '?') {
                deduction(input);
            } else if (action == '+') {
                clauses.push_back(input);
            } else if (action == '-') {
                auto it = clauses.begin();

                while(it != clauses.end()) {
                    if ((*it).getLiterals() == input.getLiterals()) {
                        it = clauses.erase(it);
                        break;
                    } else {
                        ++it;
                    }
                }
            }
        }
        
        file.close();
    }
}

void Resolution::deduction(Clause goal) {
    std::sort(clauses.begin(), clauses.end(), sortBySize);

    vector<Clause> sos;
    set<string> usedClauses;
    map<int, pair<int, int>> parents;
    
    int oldSize, newSize;
    bool proved = false;

    int ruleNo = 1;
    for(Clause clause : clauses) {
        cout << ruleNo << ". " << clause.toString() << endl;
        ruleNo++;
    }
    
    oldSize = sos.size();
    for (string lit : goal.getLiterals()) {
        sos.push_back(goal.negateLiteral(lit));
    }
    newSize = sos.size();

    for(Clause clause : sos) {
        cout << ruleNo << ". " << clause.toString() << endl;
        ruleNo++;
    }

    cout << string(10, '=') << endl;

    while(oldSize < newSize && !proved) {
        oldSize = newSize;

        int firstP, secondP;


        for (int i = 0; i < sos.size() && !proved; i++) {
            for (int j = 0; j < clauses.size() && !proved; j++) {
                Clause derivedClause = Clause();
                if (usedClauses.find(to_string(clauses.size() + i) + to_string(j)) != usedClauses.end()) continue;
                bool success = sos.at(i).derive(&(clauses.at(j)), &derivedClause);
                
                if (success) {
                    firstP = clauses.size() + i;
                    secondP = j;
                    string used = to_string(firstP) + to_string(secondP);
                    usedClauses.insert(used);
                    if (!derivedClause.isTautology() && checkRedundancy(derivedClause, sos)) {
                        sos.push_back(derivedClause);
                        /* cout << ruleNo << ". " << sos.back().toString();
                        printf(" (%d, %d)\n", firstP + 1, secondP + 1); */
                        ruleNo++;
                        if(derivedClause.size() == 0) proved = true;
                    }
                }
            }
            for (int j = 0; j < sos.size() && !proved; j++) {
                Clause derivedClause = Clause();

                if (i == j || usedClauses.find(to_string(clauses.size() + i) + to_string(clauses.size() + j)) != usedClauses.end()) continue;

                bool success = sos.at(i).derive(&(sos.at(j)), &derivedClause);
                if (success) {
                    firstP = clauses.size() + i;
                    secondP = clauses.size() + j;
                    string used = to_string(firstP) + to_string(secondP);
                    usedClauses.insert(used);
                    if (!derivedClause.isTautology() && checkRedundancy(derivedClause, sos)) {
                        sos.push_back(derivedClause);
                        /* cout << ruleNo << ". " << sos.back().toString();
                        printf(" (%d, %d)\n", firstP + 1, secondP + 1); */
                        ruleNo++;
                        if(derivedClause.size() == 0) proved = true;
                    }
                    
                }
            }
        }

        newSize = sos.size();
    }

    if (proved) {
        queue<Clause *> q;
        vector<Clause *> path;
        q.push(&sos.back());


        while(!q.empty()) {
            Clause *c = q.front();
            path.push_back(c);
            q.pop();

            if (c->getParent1() != nullptr) {
                q.push(c->getParent1());
            }
            if (c->getParent2() != nullptr) {
                q.push(c->getParent2());
            }

        }

        sort(path.rbegin(), path.rend(), compareForPrint);

        bool passed = false;
        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            cout << (*it)->toString() << endl;
            
        }
    }

    cout << string(10, '=') << endl;
    cout << "[CONCLUSION]: ";

    if (proved) {
        cout << goal.toString() << " is true" << endl;
    } else {
        cout << goal.toString() << " is unknown" << endl;
    }


}


