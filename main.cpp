#include <iostream>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>

#include "game.hpp"

struct allocation_metrics {
    uint32_t total_allocated = 0;
    uint32_t total_freed = 0;
    uint32_t current_usage() { return total_allocated - total_freed; }

};

static allocation_metrics a_metrics;

static void print_memory_usage() {
    std::cout << "Memory Usage :\t\t" << a_metrics.current_usage() / 1000 << "\tMbytes" << std::endl;
    std::cout << "Memory Allocated :\t" << a_metrics.total_allocated  / 1000<< "\tMbytes" << std::endl;
    std::cout << "Memory Freed :\t\t" << a_metrics.total_freed / 1000 << "\tMbytes" << std::endl;
}

void* operator new(size_t size) {
    a_metrics.total_allocated += size;
    return malloc(size);
}

void operator delete(void* memory) {
    a_metrics.total_freed += sizeof(memory);
    free(memory);
} 

char getch(bool& keyPressed) {
    char buf = 0;
    struct termios old = {0};
    struct termios new_term = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    new_term = old;
    new_term.c_lflag &= ~ICANON;
    new_term.c_lflag &= ~ECHO;
    new_term.c_cc[VMIN] = 0;
    new_term.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &new_term) < 0)
        perror("tcsetattr ICANON");
    ssize_t bytesRead = read(0, &buf, 1);
    if (bytesRead < 0) {
        perror ("read()");
        keyPressed = false;
    } else if (bytesRead == 0) {
        // No key pressed
        keyPressed = false;
    } else {
        keyPressed = true;
    }
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return buf;
}

int chose_difficulty() {
    system("clear");
    std::cout << "Difficulty ? (1: Easy, 2: Medium, 3: Hard)" << std::endl;
    int tmp_difficulty = 2;
    std::cin >> tmp_difficulty;
    if (tmp_difficulty == 1) return 1;
    if (tmp_difficulty == 3) return 3;
    else return 2;
}

int main() {
    {
        int difficulty = chose_difficulty();
        game* g =  new game();
        g->render();
        char key;
        bool keyPressed = false;
        while (!g->is_snek_dead()) {
            key = getch(keyPressed);
            if (keyPressed) {
                switch (key) {
                    case 'z':
                        g->move_snek(up);
                        break;
                    case 'q':
                        g->move_snek(left);
                        break;
                    case 's':
                        g->move_snek(down);
                        break;
                    case 'd':
                        g->move_snek(right);
                        break;
                }
            } else {
                g->move_snek(g->get_last_snek_move());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * (10 / difficulty)));
            g->render();
            print_memory_usage();
        }
        delete g;
        print_memory_usage();
    }

    return 0;
}