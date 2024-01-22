#include <vector>
struct snek_part {
    int row;
    int col;

    snek_part(int const& row, int const& col) : row(row), col(col) {}
    snek_part(snek_part const& other) : row(other.row), col(other.col) {}
    snek_part operator=(snek_part const& other) {
        if (this != &other) {
            row = other.row;
            col = other.col;
        }
        return *this;
    }
};

class snek {
    uint8_t health;
    uint8_t length;
    std::vector<std::shared_ptr<snek_part>> body;

    public:

        snek(int const& length) : health(5) {
            this->length = length;
            for (int i = length - 1; i >= 0; i--) {
                body.emplace_back(new snek_part(0, i));
            }
        }

        uint8_t const get_health() { return health; }
        void hurt() { health -= 1; }
        void set_health(int const& health) { this->health = health; }
        std::vector<std::shared_ptr<snek_part>> const get_body() { return body; }
        std::shared_ptr<snek_part> const get_head_coord() { return body[0]; }
        uint8_t const get_length() { return length; }

        void grow() {
            std::shared_ptr<snek_part> tail = body[length - 1];
            body.emplace_back(new snek_part(tail->row, tail->col + 1));
            this->length += 1;
        }

        void horizontal_move(int const& direction) {
            std::shared_ptr<snek_part> head = body[0];
            snek_part prev = *head;
            head->col += direction;
            for (int i = 1; i < this->length; i++) {
                auto tmp = *body[i];
                body[i]->col = prev.col;
                body[i]->row = prev.row;
                prev = tmp;
            }
        }

        void vertical_move(int const& direction) {
            std::shared_ptr<snek_part> head = body[0];
            snek_part prev = *head;
            head->row += direction;
            for (int i = 1; i < this->length; i++) {
                auto tmp = *body[i];
                body[i]->col = prev.col;
                body[i]->row = prev.row;
                prev = tmp;
            }
        }
        bool const is_dead() { return health <= 0; }
};