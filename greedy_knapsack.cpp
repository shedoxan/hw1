#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;


struct knapsack_item {
    int    id;
    double weight;
    double value;
    double ratio;
};

// жадный алгоритм: сортирует по убыванию value/weight и отбирает пока помещается
vector<int> greedy_knapsack(vector<knapsack_item>& items,
                            double capacity,
                            double& total_weight,
                            double& total_value) {
    for (auto& it : items)
        it.ratio = it.value / it.weight;

    sort(items.begin(), items.end(),
         [](const knapsack_item& a, const knapsack_item& b) {
             return a.ratio > b.ratio;
         });

    total_weight = 0;
    total_value  = 0;
    vector<int> selected_ids;

    for (const auto& it : items) {
        if (total_weight + it.weight <= capacity) {
            total_weight += it.weight;
            total_value  += it.value;
            selected_ids.push_back(it.id);
        }
    }
    return selected_ids;
}

int main() {
    int n;
    double capacity;
    if (!(cin >> n >> capacity)) {
        cerr << "error: invalid header (need N W)\n";
        return 1;
    }

    vector<knapsack_item> items;
    items.reserve(n);
    for (int i = 0; i < n; ++i) {
        double value, weight;
        if (!(cin >> value >> weight)) {
            cerr << "error: invalid item at line " << (i+2) << "\n";
            return 1;
        }
        items.push_back({ i, weight, value, 0.0 });
    }

    double total_weight, total_value;
    // меряем время
    auto t_start = high_resolution_clock::now();
    auto chosen  = greedy_knapsack(items, capacity, total_weight, total_value);
    auto t_end   = high_resolution_clock::now();
    double duration_ms = chrono::duration<double, milli>(t_end - t_start).count();

    cout << "total_value "   << total_value   << "\n"
         << "total_weight "  << total_weight  << "\n"
         << "duration_ms "   << duration_ms   << "\n"
         << "selected_items";
    for (int id : chosen) cout << " " << id;
    cout << "\n";

    return 0;
}