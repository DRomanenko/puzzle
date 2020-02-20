#include <vector>
#include <random>
#include <algorithm>

class board {
public:
    /*** Default constructor. ***/
    board () : height(0),
               width(0),
               empty_pos({0, 0}),
               manhattan_distance(0) {
        state.reserve(0);
    }

    /*** Copy constructor. ***/
    board (const std::vector<std::vector<unsigned short>> &other) : state(other),
                                                                    height(other.size()),
                                                                    width(!other.empty() ? other[0].size() : 0),
                                                                    empty_pos(find_space(other)),
                                                                    manhattan_distance(calc_m_d(other, empty_pos)) {
    }

    /*** Assignment constructor 1 ***/
    explicit board (const short size) : height(size),
                                        width(size) {
        generate_random_state(state, size, size, empty_pos, manhattan_distance);
    }

    /*** Assignment constructor 2 ***/
    board (const unsigned short height, const unsigned short width) : height(height),
                                                                      width(width) {
        generate_random_state(state, height, width, empty_pos, manhattan_distance);
    }

    /*** Returns the size of the board. ***/
    std::pair<unsigned short, unsigned short> size () const {
        return {height, width};
    }

    /*** Returns the number of blocks in the wrong places. ***/
    unsigned short hamming () const {
        unsigned int hamming_distance = 0;
        for (unsigned short i = 0; i < height; ++i) {
            for (unsigned short q = 0; q < width; ++q) {
                if (state[i][q] != i * width + q + 1 && empty_pos != std::make_pair(i, q)) {
                    ++hamming_distance;
                }
            }
        }
        return hamming_distance;
    }

    /*** Returns the sum of the Manhattan distance between the blocks and the target. ***/
    unsigned short manhattan () const {
        return manhattan_distance;
    }

    /*** Returns whether this board is a target. ***/
    bool is_goal () const {
        return hamming() == 0;
    }

    /*** Returns whether this arrangement of elements is solvable. ***/
    bool is_solvable () const {
        if (!height || !width) {
            return true;
        }
        std::vector<unsigned short> linear;
        linear.reserve(height * width);
        for (const auto &u : state) {
            for (auto v : u) {
                linear.push_back(v);
            }
        }
        int inv_count = 0;
        for (unsigned short i = 0; i < height * width - 1; ++i) {
            for (unsigned short q = i + 1; q < height * width; ++q) {
                if (linear[q] != height * width && linear[i] != height * width && linear[i] > linear[q]) {
                    ++inv_count;
                }
            }
        }
        int pos = height - empty_pos.first;
        if (width % 2 == 0) {
            return pos % 2 == 0 ? inv_count % 2 != 0 : inv_count % 2 == 0;
        } else {
            return inv_count % 2 == 0;
        }
    }

    bool operator!= (const board &other) const {
        return state != other.state;
    }

    bool operator== (const board &other) const {
        return state == other.state;
    }

    friend bool operator< (const board &board_1, const board &board_2) {
        if (board_1.manhattan() != board_2.manhattan()) {
            return board_1.manhattan() < board_2.manhattan();
        }
        if (board_1.height != board_2.height || board_1.width != board_2.width) {
            return false;
        }
        for (int i = 0; i < board_1.height; ++i) {
            for (int q = 0; q < board_1.width; ++q) {
                if (board_1.state[i][q] != board_2.state[i][q]) {
                    return board_1.state[i][q] < board_2.state[i][q];
                }
            }
        }
        return false;
    }

    friend bool operator> (const board &board_1, const board &board_2) {
        return board_2 < board_1;
    }

    const std::vector<unsigned short> &operator[] (unsigned short index) const {
        return state[index];
    }

    /*** Output methods and operators for textual representation of strings. ***/
    std::string to_string () const {
        std::string ans;
        for (const auto &line : state) {
            for (auto number : line) {
                if (number != height * width) {
                    ans += (number > 9 ? "" : " ") + std::to_string(number) + " ";
                } else {
                    ans += " # ";
                }
            }
            ans += '\n';
        }
        return ans;
    }

    friend std::ostream &operator<< (std::ostream &out_stream, const board &b) {
        return out_stream << b.to_string();
    }

    std::vector<std::vector<unsigned short>> get_board () const {
        return state;
    }

    std::pair<unsigned short, unsigned short> get_empty_pos () const {
        return empty_pos;
    }

private:
    std::vector<std::vector<unsigned short>> state;
    unsigned short height, width;
    std::pair<unsigned short, unsigned short> empty_pos;
    unsigned int manhattan_distance;

    static unsigned int calc_m_d (const std::vector<std::vector<unsigned short>> &state,
                                  std::pair<unsigned short, unsigned short> &empty_pos) {
        unsigned int manhattan_distance = 0;
        for (unsigned short i = 0; i < state.size(); ++i) {
            for (unsigned short q = 0; q < state[i].size(); ++q) {
                if (state[i][q] != i * state[i].size() + q + 1 && empty_pos != std::make_pair(i, q)) {
                    int x = (state[i][q] - 1) / state.size(), y = (state[i][q] - 1) % state[i].size();
                    manhattan_distance += abs(i - x) + abs(q - y);
                }
            }
        }
        return manhattan_distance;
    }

    static std::pair<unsigned short, unsigned short>
    find_space (const std::vector<std::vector<unsigned short>> &state) {
        std::pair<unsigned short, unsigned short> empty_pos;
        for (unsigned short i = 0; i < state.size(); ++i) {
            for (unsigned short q = 0; q < state[i].size(); ++q) {
                if (state[i][q] == state.size() * state[i].size()) {
                    empty_pos = {i, q};
                }
            }
        }
        return empty_pos;
    }

    static void generate_random_state (std::vector<std::vector<unsigned short>> &state,
                                       unsigned short height, unsigned short width,
                                       std::pair<unsigned short, unsigned short> &empty_pos,
                                       unsigned int &manhattan_dist) {
        std::vector<unsigned short> permutation(height * width);
        std::iota(permutation.begin(), permutation.end(), 1);
        std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));

        state.resize(height);
        for (unsigned short i = 0; i < height; ++i) {
            for (unsigned short q = 0; q < width; ++q) {
                state[i].push_back(permutation[i * width + q]);
                if (state[i][q] == height * width) { manhattan_dist = calc_m_d(state, empty_pos = {i, q}); }
            }
        }
    }
};