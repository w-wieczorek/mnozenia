#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <experimental/generator>

using namespace std::experimental;
using namespace std;

class solution {
public:
    virtual float bound() = 0;
    virtual bool is_complete() = 0;
    virtual generator<solution*> branch() = 0;
    virtual string as_string() = 0;
};

solution* b_and_b(solution* start_point) {
    float best_cost = numeric_limits<float>::infinity();
    solution* best_solution = nullptr;
    auto cmp = [](solution* left, solution* right) { return left->bound() > right->bound(); };
    priority_queue<solution*, vector<solution*>, decltype(cmp)> Z(cmp);
    Z.push(start_point);
    while (!Z.empty()) {
        solution* w = Z.top();
        Z.pop();
        if (w->bound() < best_cost) {
            for (solution* son : w->branch()) {
                if (son->is_complete()) {
                    if (son->bound() < best_cost) {
                        best_cost = son->bound();
                        best_solution = son;
                    }
                    else delete son;
                }
                else {
                    if (son->bound() < best_cost) Z.push(son);
                    else delete son;
                }
            }
        }
        delete w;
    }
    return best_solution;
}

class table : public solution {
public:
    table(const vector<int> &v, int x) {
        t = v;
        goal = x;
        bound_computed = false;
    }

    float bound() {
        if (!bound_computed) {
            int counter = 0;
            int x = t.back();
            while (x < goal) {
                x <<= 1;  // x = x + x
                ++counter;
            }
            bound_value = t.size() + counter;
            bound_computed = true;
        }
        return bound_value;
    }

    bool is_complete() {
        return find(t.begin(), t.end(), goal) != t.end();
    }

    generator<solution*> branch() {
        int last = t.back();
        if (last < goal) {
            for (int i : t) {
                vector<int> t2(t);
                t2.push_back(i + last);
                co_yield new table(t2, goal);
            }
        }
    }

    string as_string() {
        stringstream result;
        for (int i : t)
            result << i << " ";
        return result.str();
    }

private:
    vector<int> t;
    int goal;
    bool bound_computed;
    float bound_value;
};

int main() {
    solution* start_point = new table({1}, 299);
    solution* sol = b_and_b(start_point);
    cout << sol->as_string() << endl;
    delete sol;
    return 0;
}
