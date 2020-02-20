#include <iostream>
#include <deque>
#include <set>
#include <map>

#include "board.hpp"

class solver {
public:
    /*** Assignment constructor ***/
    explicit solver (const board &new_board) {
        transition = {{-1, 0},
                      {0,  -1},
                      {1,  0},
                      {0,  1}};

        if (!new_board.is_solvable()) {
            boards = {};
            return;
        }

        std::map<const board, const board> save_order_boards;
        std::set<board> open, closed;
        open.insert(new_board);

        while (!open.empty()) {
            auto current = *open.begin();
            if (!current.is_goal()) {
                open.erase(current);
                closed.insert(current);
                for (auto[transition_1, transition_2] : transition) {
                    std::pair<unsigned short, unsigned short> curr_empty_pos = current.get_empty_pos();
                    int i = curr_empty_pos.first + transition_1, q = curr_empty_pos.second + transition_2;
                    if (i >= 0 && i < current.size().first && q >= 0 && q < current.size().second) {
                        std::vector<std::vector<unsigned short>> help = current.get_board();
                        std::swap(help[curr_empty_pos.first][curr_empty_pos.second], help[i][q]);
                        board next = board(help);
                        if (!closed.count(next) && !open.count(next)) {
                            save_order_boards.insert({next, current});
                            open.insert(next);
                        }
                    }
                }
            } else {
                auto help = save_order_boards.find(current);
                boards.push_front(current);
                while (help != save_order_boards.end()) {
                    boards.push_front(help->second);
                    help = save_order_boards.find(help->second);
                }
                return;
            }
        }
    }

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = board;
        using difference_type = board;
        using pointer = board *;
        using reference = board &;

        explicit iterator (const std::_Deque_iterator<board, board &, board *> &b) {
            ptr = b;
        }

        board &operator+ (int x) {
            return *(ptr + x);
        }

        board &operator- (int x) {
            return *(ptr - x);
        }

        board &operator++ () {
            return *ptr++;
        }

        board &operator-- () {
            return *ptr--;
        }

        const board operator++ (int) {
            return *++ptr;
        }

        const board operator-- (int) {
            return *--ptr;
        }

        bool operator== (const iterator &other) {
            return ptr == other.ptr;
        }

        bool operator!= (const iterator &other) {
            return ptr != other.ptr;
        }

        board &operator* () const {
            return *ptr;
        }

        const std::_Deque_iterator<board, board &, board *> operator-> () const {
            return ptr;
        }

    private:
        std::_Deque_iterator<board, board &, board *> ptr;
    };

    /*** Returns the number of moves that lead to the solution. ***/
    const int moves () const {
        return boards.size();
    }

    iterator begin () {
        return iterator(boards.begin());
    }

    iterator end () {
        return iterator(boards.end());
    }

private:
    std::vector<std::pair<short, short>> transition;
    std::deque<board> boards;
};