#include <iostream>
#include <thread>
#include <chrono>

#include "game.hpp"

struct allocation_metrics {
    uint32_t total_allocated = 0;
    uint32_t total_freed = 0;

    uint32_t current_usage() { return total_allocated - total_freed; }
};

static allocation_metrics a_metrics;

static void print_memory_usage() {
    std::cout << "Memory usage : " << a_metrics.current_usage() << " bytes" << std::endl;
}

void* operator new(size_t size) {
    a_metrics.total_allocated += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
    a_metrics.total_freed += size;
    free(memory);
} 

void move_in_circle(game& g) {
    for (int i = 0; i < g.get_col_limit(); i++) {
        g.render();
        g.move_snek(right);
        g.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (int i = 0; i < g.get_row_limit(); i++) {
        g.render();
        g.move_snek(down);
        g.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (int i = 0; i < g.get_col_limit(); i++) {
        g.render();
        g.move_snek(left);
        g.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (int i = 0; i < g.get_row_limit(); i++) {
        g.render();
        g.move_snek(up);
        g.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    {
        game* g =  new game();
        char move;
        g->render();
        std::cin >> move;
        switch (move)
        {
        case 'z':
            g->move_snek(up);
            break;
        case 's':
            g->move_snek(down);
            break;
        case 'd':
            g->move_snek(right);
            break;
        case 'q':
            g->move_snek(left);
            break;
        default:
            break;
        }
        g->move_snek(left);
        g->render();
        delete g;
        print_memory_usage();
    }
    print_memory_usage();

    return 0;
}