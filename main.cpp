#include <iostream>
#include <cmath>
#include <ctime>
#include <cassert>

#include "solver.hpp"

static void do_tests (int n) {
    std::cout << "Test 1 (Empty input):\n";
    board b_0 = board({});
    std::cout << "Passed!\n-----------------\n";

    std::cout << "Test 2 (Unsolvable(small)):\n";
    board b_1 = board({{2, 1, 4, 3}});
    std::cout << b_1.to_string() << '\n';
    if (!b_1.is_solvable()) {
        std::cout << "Passed!\n";
    } else {
        std::cout << "Failed\n";
        assert(false);
    }
    std::cout << "-----------------\n";

    std::cout << "Test 3 (Unsolvable(big)):\n";
    board b_2 = board({{2,  10, 11, 20, 9},
                       {15, 14, 22, 17, 25},
                       {6,  12, 23, 13, 5},
                       {7,  16, 18, 21, 4},
                       {24, 19, 1,  8,  3}});
    std::cout << b_2.to_string() << '\n';
    if (!b_2.is_solvable()) {
        std::cout << "Passed!\n";
    } else {
        std::cout << "Failed\n";
        assert(false);
    }
    std::cout << "-----------------\n";

    std::cout << "Test 4 (Solvable(small)):\n";
    board b_3 = board({{1, 2, 3},
                       {4, 9, 5},
                       {7, 8, 6}});
    std::cout << b_3.to_string() << "\nSolution:\n";
    solver s(b_3);
    for (const auto &i : s) {
        std::cout << i << '\n';
    }
    if (s.moves() > 0) {
        std::cout << "Moves: " << s.moves() << "\nPassed!\n";
    } else {
        std::cout << "Failed\n";
        assert(false);
    }
    std::cout << "-----------------\n";

    std::cout << "Test 5 (Solvable(big)):\n";
    board b_4 = board({{6,  1,  21, 15, 14},
                       {19, 10, 9,  25, 16},
                       {24, 18, 5,  8,  23},
                       {13, 2,  20, 11, 22},
                       {7,  12, 17, 4,  3}});
    std::cout << b_4.to_string() << '\n';
    solver s_1(b_4);
    if (s_1.moves() > 0) {
        std::cout << "Moves: " << s_1.moves() << "\nPassed!\n";
    } else {
        std::cout << "Failed\n";
        assert(false);
    }
    std::cout << "-----------------\n";

    for (int i = 6; i <= n; ++i) {
        std::cout << "Test " << i << " (Random):\n";
        unsigned short h = std::mt19937(std::random_device()())() % 4;
        unsigned short w = std::mt19937(std::random_device()())() % 4;
        board b_random = board(h + 1, w + 1);
        std::cout << b_random.to_string() << '\n';
        solver s_random(b_random);
        std::cout << "Solvable: " << (b_random.is_solvable() ? "Yes" : "No") << '\n';

        if (s_random.moves() > 20) {
            std::cout << "Moves: " << s_random.moves() << '\n';
            std::cout << "Passed!\n-----------------\n";
            continue;
        }

        if (b_random.is_solvable()) {
            std::cout << "Solution:\n";
            for (const auto &u : s_random) {
                std::cout << u << '\n';
            }
            std::cout << "Moves: " << s_random.moves() << "\nPassed!\n";
        } else {
            std::cout << "Passed!\n";
        }
        std::cout << "-----------------\n";
    }
}

int main () {
    do_tests(1000);
    return 0;
}