#ifndef CLI_HPP
#define CLI_HPP

#include "GameOfLife.hpp"
#include <string>
#include <thread> // für sleep

class CLI
{
public:
    void run(); // Haupt-Loop
    ~CLI();     // Destruktor

private:
    GameOfLife *game = nullptr;
    bool print_enabled = true;
    int delay_ms = 0; // neue Variable für Verzögerung
};

#endif // CLI_HPP
