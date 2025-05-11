#include "CLI.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstdlib> // für rand(), srand()
#include <ctime>   // für time()

void CLI::run()
{
    std::string line;

    std::cout << "Game of Life CLI gestartet. Tippe 'exit' zum Beenden.\n";
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, line);

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "exit")
        {
            break;
        }
        else if (command == "create")
        {
            int w, h;
            if (iss >> w >> h)
            {
                if (game)
                    delete game; // vorherige Welt löschen, wenn vorhanden
                game = new GameOfLife(w, h);
                std::cout << "Welt mit " << w << "x" << h << " erstellt.\n";
            }
            else
            {
                std::cout << "Fehler: Benutze 'create <breite> <höhe>'\n";
            }
        }
        else if (command == "set")
        {
            int x, y, state;
            if (iss >> x >> y >> state)
            {
                if (game)
                {
                    game->set_cell(x, y, state);
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                iss.clear();
                iss.seekg(0);
                iss >> command; // skip "set"
                int p;
                if (iss >> p >> state)
                {
                    if (game)
                    {
                        int w = game->get_width();
                        int h = game->get_height();
                        if (p >= 0 && p < w * h)
                        {
                            int x1 = p % w;
                            int y1 = p / w;
                            game->set_cell(x1, y1, state);
                        }
                        else
                        {
                            std::cout << "Fehler: Ungültige 1D-Zellposition.\n";
                        }
                    }
                    else
                    {
                        std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                    }
                }
                else
                {
                    std::cout << "Fehler: Benutze 'set <x> <y> <0|1>' oder 'set <position> <0|1>'\n";
                }
            }
        }
        else if (command == "print")
        {
            int flag;
            if (iss >> flag)
            {
                print_enabled = (flag != 0);
                std::cout << "Automatisches Anzeigen nach Generationen ist "
                          << (print_enabled ? "aktiviert." : "deaktiviert.") << "\n";
            }
            else if (game)
            {
                game->print(); // falls kein Argument → direkt drucken
            }
            else
            {
                std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
            }
        }

        else if (command == "run")
        {
            int generations;
            if (iss >> generations)
            {
                if (game)
                {
                    auto start = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < generations; ++i)
                    {
                        game->evolve();
                        if (print_enabled)
                        {
                            game->print();
                            if (delay_ms > 0)
                                std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
                        }
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                    std::cout << generations << " Generation(en) in " << duration.count() << " ms.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'run <anzahl_generationen>'\n";
            }
        }
        else if (command == "delay")
        {
            int ms;
            if (iss >> ms)
            {
                delay_ms = ms;
                std::cout << "Verzögerung nach Generationen auf " << ms << " ms gesetzt.\n";
            }
            else
            {
                std::cout << "Fehler: Benutze 'delay <millisekunden>'\n";
            }
        }
        else if (command == "get")
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    int value = game->get_cell(x, y);
                    if (value != -1)
                        std::cout << "Zelle (" << x << "," << y << ") = " << value << "\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                iss.clear();
                iss.seekg(0);
                iss >> command; // skip "get"
                int p;
                if (iss >> p)
                {
                    if (game)
                    {
                        // 1D-Zugriff: p = y * width + x
                        int w = game->get_width();
                        int h = game->get_height();
                        if (p >= 0 && p < w * h)
                        {
                            int x1 = p % w;
                            int y1 = p / w;
                            int value = game->get_cell(x1, y1);
                            std::cout << "Zelle #" << p << " = Zelle(" << x1 << "," << y1 << ") = " << value << "\n";
                        }
                        else
                        {
                            std::cout << "Fehler: Ungültige 1D-Zellposition.\n";
                        }
                    }
                    else
                    {
                        std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                    }
                }
                else
                {
                    std::cout << "Fehler: Benutze 'get <x> <y>' oder 'get <position>'\n";
                }
            }
        }
        else if (command == "save")
        {
            std::string filename;
            if (iss >> filename)
            {
                if (game)
                {
                    game->save(filename);
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'save <filename>'\n";
            }
        }
        else if (command == "load")
        {
            std::string filename;
            if (iss >> filename)
            {
                if (game)
                    delete game;

                game = new GameOfLife(1, 1); // Dummy-Initialisierung
                game->load(filename);
            }
            else
            {
                std::cout << "Fehler: Benutze 'load <filename>'\n";
            }
        }
        else if (command == "glider")
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    game->set_cell((x + 1) % game->get_width(), y % game->get_height(), 1);
                    game->set_cell((x + 2) % game->get_width(), (y + 1) % game->get_height(), 1);
                    game->set_cell(x % game->get_width(), (y + 2) % game->get_height(), 1);
                    game->set_cell((x + 1) % game->get_width(), (y + 2) % game->get_height(), 1);
                    game->set_cell((x + 2) % game->get_width(), (y + 2) % game->get_height(), 1);
                    std::cout << "Glider bei (" << x << "," << y << ") gesetzt.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'glider <x> <y>'\n";
            }
        }
        else if (command == "toad")
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    int w = game->get_width();
                    int h = game->get_height();

                    game->set_cell((x + 1) % w, y % h, 1);
                    game->set_cell((x + 2) % w, y % h, 1);
                    game->set_cell((x + 3) % w, y % h, 1);
                    game->set_cell(x % w, (y + 1) % h, 1);
                    game->set_cell((x + 1) % w, (y + 1) % h, 1);
                    game->set_cell((x + 2) % w, (y + 1) % h, 1);

                    std::cout << "Toad bei (" << x << "," << y << ") gesetzt.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'toad <x> <y>'\n";
            }
        }
        else if (command == "beacon")
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    int w = game->get_width();
                    int h = game->get_height();

                    // linke obere Ecke
                    game->set_cell((x + 0) % w, (y + 0) % h, 1);
                    game->set_cell((x + 1) % w, (y + 0) % h, 1);
                    game->set_cell((x + 0) % w, (y + 1) % h, 1);
                    game->set_cell((x + 1) % w, (y + 1) % h, 1);

                    // rechte untere Ecke
                    game->set_cell((x + 2) % w, (y + 2) % h, 1);
                    game->set_cell((x + 3) % w, (y + 2) % h, 1);
                    game->set_cell((x + 2) % w, (y + 3) % h, 1);
                    game->set_cell((x + 3) % w, (y + 3) % h, 1);

                    std::cout << "Beacon bei (" << x << "," << y << ") gesetzt.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'beacon <x> <y>'\n";
            }
        }
        else if (command == "methuselah")
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    int w = game->get_width();
                    int h = game->get_height();

                    // R-Pentomino
                    game->set_cell((x + 1) % w, y % h, 1);
                    game->set_cell((x + 2) % w, y % h, 1);
                    game->set_cell(x % w, (y + 1) % h, 1);
                    game->set_cell((x + 1) % w, (y + 1) % h, 1);
                    game->set_cell((x + 1) % w, (y + 2) % h, 1);

                    std::cout << "Methuselah (R-Pentomino) bei (" << x << "," << y << ") gesetzt.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'methuselah <x> <y>'\n";
            }
        }
        else if (command == "random")
        {
            int n;
            if (iss >> n)
            {
                if (game)
                {
                    int w = game->get_width();
                    int h = game->get_height();

                    for (int i = 0; i < n; ++i)
                    {
                        int pattern = rand() % 4; // 0=glider, 1=toad, 2=beacon, 3=methuselah
                        int x = rand() % w;
                        int y = rand() % h;

                        switch (pattern)
                        {
                        case 0: // glider
                            game->set_cell((x + 1) % w, y % h, 1);
                            game->set_cell((x + 2) % w, (y + 1) % h, 1);
                            game->set_cell(x % w, (y + 2) % h, 1);
                            game->set_cell((x + 1) % w, (y + 2) % h, 1);
                            game->set_cell((x + 2) % w, (y + 2) % h, 1);
                            break;
                        case 1: // toad
                            game->set_cell((x + 1) % w, y % h, 1);
                            game->set_cell((x + 2) % w, y % h, 1);
                            game->set_cell((x + 3) % w, y % h, 1);
                            game->set_cell(x % w, (y + 1) % h, 1);
                            game->set_cell((x + 1) % w, (y + 1) % h, 1);
                            game->set_cell((x + 2) % w, (y + 1) % h, 1);
                            break;
                        case 2: // beacon
                            game->set_cell((x + 0) % w, (y + 0) % h, 1);
                            game->set_cell((x + 1) % w, (y + 0) % h, 1);
                            game->set_cell((x + 0) % w, (y + 1) % h, 1);
                            game->set_cell((x + 1) % w, (y + 1) % h, 1);
                            game->set_cell((x + 2) % w, (y + 2) % h, 1);
                            game->set_cell((x + 3) % w, (y + 2) % h, 1);
                            game->set_cell((x + 2) % w, (y + 3) % h, 1);
                            game->set_cell((x + 3) % w, (y + 3) % h, 1);
                            break;
                        case 3: // methuselah (R-pentomino)
                            game->set_cell((x + 1) % w, y % h, 1);
                            game->set_cell((x + 2) % w, y % h, 1);
                            game->set_cell(x % w, (y + 1) % h, 1);
                            game->set_cell((x + 1) % w, (y + 1) % h, 1);
                            game->set_cell((x + 1) % w, (y + 2) % h, 1);
                            break;
                        }
                    }

                    std::cout << n << " zufällige Muster platziert.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'random <anzahl>'\n";
            }
        }

        else
        {
            std::cout << "Unbekannter Befehl: " << command << "\n";
        }
    }

    std::cout << "Beendet.\n";
}

CLI::~CLI()
{
    if (game)
    {
        delete game;
    }
}
