#include <vector>
#include <list>
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
    int health;
    uint8_t length;
    std::list<std::shared_ptr<snek_part>> body;

    public:

        snek(int const& length) : health(5) {
            this->length = length;
            for (int i = length - 1; i >= 0; i--) {
                body.emplace_back(std::make_shared<snek_part>(0, i));
            }
        }

        uint8_t const get_health() { return health; }
        void hurt() { health -= 1; }
        void set_health(int const& health) { this->health = health; }
        std::list<std::shared_ptr<snek_part>> const get_body() { return body; }
        std::shared_ptr<snek_part> const get_head() { return body.front(); }
        uint8_t const get_length() { return length; }

        void grow() {
            std::shared_ptr<snek_part> tail = body.back();
            body.emplace_back(std::make_shared<snek_part>(tail->row, tail->col + 1));
            length += 1;
        }

        void horizontal_move(int const& direction) {
            std::shared_ptr<snek_part> head = get_head();
            std::shared_ptr<snek_part> new_head = std::make_shared<snek_part>(head->row, head->col + direction);
            body.push_front(new_head);
            body.pop_back();
        }

        void vertical_move(int const& direction) {
            std::shared_ptr<snek_part> head = get_head();
            std::shared_ptr<snek_part> new_head = std::make_shared<snek_part>(head->row + direction, head->col);
            body.push_front(new_head);
            body.pop_back();
        }
        bool const is_dead() { return health <= 0; }
};