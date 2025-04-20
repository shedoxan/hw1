#include <bits/stdc++.h>
#include <filesystem>

struct Item {
    long long value;
    long long weight;
};

struct Result {
    std::string filename;
    std::string algorithm;        // "ratio" или "value_over_W"
    long long total_value = 0;
    long long total_weight = 0;
    long long duration_ms = 0;
    std::vector<long long> selected_values;
    std::vector<long long> selected_weights;
};

// Жадный выбор по заданному критерию
Result greedy_knapsack(std::vector<Item> items,
                       long long W,
                       const std::string &file_name,
                       const std::string &algo_name)
{
    auto start = std::chrono::high_resolution_clock::now();

    if (algo_name == "ratio") {
        std::sort(items.begin(), items.end(), [](auto &a, auto &b) {
            double ra = double(a.value) / a.weight;
            double rb = double(b.value) / b.weight;
            if (ra == rb) return a.value > b.value;
            return ra > rb;
        });
    } else { // value_over_W
        double invW = 1.0 / double(W);
        std::sort(items.begin(), items.end(), [&](auto &a, auto &b) {
            return a.value * invW > b.value * invW;
        });
    }

    Result res{file_name, algo_name};
    long long cur_w = 0;
    for (auto &it : items) {
        if (cur_w + it.weight <= W) {
            cur_w += it.weight;
            res.total_value  += it.value;
            res.total_weight += it.weight;
            res.selected_values.push_back(it.value);
            res.selected_weights.push_back(it.weight);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    res.duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return res;
}

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Путь к папке с данными
    std::filesystem::path data_dir = "data";
    if (argc >= 2) data_dir = argv[1];
    if (!std::filesystem::exists(data_dir) || !std::filesystem::is_directory(data_dir)) {
        std::cerr << "Directory not found: " << data_dir << "\n";
        return 1;
    }

    std::vector<Result> results;

    // Обработка каждого файла в директории
    for (auto &entry : std::filesystem::directory_iterator(data_dir)) {
        if (!entry.is_regular_file()) continue;

        std::ifstream fin(entry.path());
        if (!fin.is_open()) {
            std::cerr << "Cannot open file: " << entry.path() << "\n";
            continue;
        }

        size_t N; long long W;
        fin >> N >> W;
        std::vector<Item> items(N);
        for (size_t i = 0; i < N; ++i) {
            fin >> items[i].value >> items[i].weight;
        }

        std::string fname = entry.path().filename().string();
        results.push_back(greedy_knapsack(items, W, fname, "ratio"));
        results.push_back(greedy_knapsack(items, W, fname, "value_over_W"));
    }

    // 1) Человекочитаемый вывод в консоль
    for (auto &r : results) {
        std::cout << "File: " << r.filename
                  << "   Algorithm: " << r.algorithm << "\n";
        std::cout << "  Total value: " << r.total_value
                  << "   Total weight: " << r.total_weight
                  << "   Time: " << r.duration_ms << " ms\n";

        std::cout << "  Selected values: ";
        for (size_t i = 0; i < r.selected_values.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << r.selected_values[i];
        }
        std::cout << "\n";

        std::cout << "  Selected weights: ";
        for (size_t i = 0; i < r.selected_weights.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << r.selected_weights[i];
        }
        std::cout << "\n\n";
    }

    // 2) Запись того же формата в results.csv
    std::ofstream fout("results.csv");
    if (!fout.is_open()) {
        std::cerr << "Cannot open results.csv for writing\n";
        return 1;
    }

    for (const auto &r : results) {
        fout << "File: " << r.filename
             << "   Algorithm: " << r.algorithm << "\n";
        fout << "  Total value: " << r.total_value
             << "   Total weight: " << r.total_weight
             << "   Time: " << r.duration_ms << " ms\n";

        fout << "  Selected values: ";
        for (size_t i = 0; i < r.selected_values.size(); ++i) {
            if (i) fout << ", ";
            fout << r.selected_values[i];
        }
        fout << "\n";

        fout << "  Selected weights: ";
        for (size_t i = 0; i < r.selected_weights.size(); ++i) {
            if (i) fout << ", ";
            fout << r.selected_weights[i];
        }
        fout << "\n\n";
    }

    std::cerr << "Results written to results.csv\n";
    return 0;
}
