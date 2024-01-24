#include <iostream>
#include <memory>
#include <string>
#include <list>

#include "snek.hpp"

enum snek_directions {
    right,
    left,
    down,
    up
};

struct food {
    int row;
    int col;
    food(int const& row_limit, int const& col_limit) { generate(row_limit, col_limit); }

    void generate(int const& row_limit, int const& col_limit) {
        srand(time(0));
        this->row = rand() % row_limit;
        this->col = rand() % col_limit;
    }
};

class game {
    uint8_t score;
    uint16_t round;
    uint8_t row_limit = 5;
    uint8_t col_limit = 8;
    uint8_t difficulty = 0; 
    snek_directions last_move;
    std::unique_ptr<snek> snake;
    std::unique_ptr<food> foodie;

    public:

        game() : score(0), round(0), snake(std::make_unique<snek>(3)), foodie(std::make_unique<food>(row_limit, col_limit)), last_move(right) {}

        uint8_t const get_row_limit() { return row_limit; }
        uint8_t const get_col_limit() { return col_limit; }

        snek_directions get_last_snek_move() { return last_move; }

        bool is_snek_dead() {
            return snake->is_dead();
        }

        bool is_snek_eating_itself(int const& row, int const& col) {
            return is_snek_body(row, col, false);

        }

        bool is_snek_eating_food() {
            std::shared_ptr<snek_part> snek_coord = snake->get_head();
            return snek_coord->row == foodie->row && snek_coord->col == foodie->col;
        }

        bool const is_inbound(int row, int col) {
            return (0 <= row && row < row_limit) && (0 <= col && col < col_limit);
        }

        void move_snek(snek_directions direction) {
            std::shared_ptr<snek_part> snek_coord = snake->get_head();
            switch (direction) {
                case right:
                    if (!is_inbound(snek_coord->row, snek_coord->col + 1)) break;
                    if (is_snek_eating_itself(snek_coord->row, snek_coord->col + 1)) {
                        snake->hurt();
                        break;
                    }
                    last_move = right;
                    snake->horizontal_move(1);
                    break;
                case left:
                    if (!is_inbound(snek_coord->row, snek_coord->col - 1)) break;
                    if (is_snek_eating_itself(snek_coord->row, snek_coord->col - 1)) {
                        snake->hurt();
                        break;
                    }
                    last_move = left;
                    snake->horizontal_move(-1);
                    break;
                case down:
                    if (!is_inbound(snek_coord->row + 1, snek_coord->col)) break;
                    if (is_snek_eating_itself(snek_coord->row + 1, snek_coord->col)) {
                        snake->hurt();
                        break;
                    }
                    last_move = down;
                    snake->vertical_move(1);
                    break;
                case up:
                    if (!is_inbound(snek_coord->row - 1, snek_coord->col)) break;
                    if (is_snek_eating_itself(snek_coord->row - 1, snek_coord->col)) {
                        snake->hurt();
                        break;
                    }
                    last_move = up;
                    snake->vertical_move(-1);
                    break;
            }
            if (is_snek_eating_food()) {
                this->score += 1;
                snake->grow();
                spawn_food();
            }
        }

        void spawn_food() {
            do {
                foodie->generate(row_limit, col_limit);
            } while (is_snek_body(foodie->row, foodie->col));
        }

        bool is_snek_head(int const& row, int const& col) {
            std::shared_ptr<snek_part> snek_head = snake->get_head();
            return snek_head->row == row && snek_head->col == col;
        }

        bool is_snek_body(int const& row, int const& col, bool const& include_head = true) {
            std::list<std::shared_ptr<snek_part>> snek_body_coord = snake->get_body();
            auto startIterator = include_head ? snek_body_coord.begin() : std::next(snek_body_coord.begin());
            return std::find_if(startIterator, snek_body_coord.end(), 
            [&](std::shared_ptr<snek_part> const& snek_body_part){
                return snek_body_part->row == row && snek_body_part->col == col;
            }) != snek_body_coord.end();
        }

        void display_score_and_health() {
            std::cout << "Score: " << std::to_string(score) << "\t-\t Health: " << std::to_string(snake->get_health()) << std::endl;
        }

        void display_board() {
            for (int row = -1; row <= row_limit; row++) {
                for (int col = -1; col <= col_limit; col++) {
                    if (row < 0 || row >= row_limit) {
                        std::cout << "-";
                        continue;
                    }
                    if (col < 0 || col >= col_limit) {
                        std::cout << "|";
                        continue;
                    }
                    if (is_snek_head(row, col)) {
                        std::cout << '@';
                    } else if (is_snek_body(row, col)) {
                        std::cout << 'o';
                    } else if (row == foodie->row && col == foodie->col) {
                        std::cout << "#";
                    } else {
                        std::cout << ' ';
                    }
                }
                std::cout << std::endl;
            }
        }

        void display_game_over() {
            system("clear");
            std::cout << "GAME OVER." << std::endl;
            std::cout << "Score : " << std::to_string(score) << " Points." << std::endl;
        }

        void render() {
            system("clear");
            if (is_snek_dead()) {
                display_game_over();
                return;
            }
            display_score_and_health();
            display_board();
        }
};