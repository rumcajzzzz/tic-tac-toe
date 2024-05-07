#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

// !!!!!!!!
// Przez to ze uzyte system("cls"), to tylko kompilowac na systemie windooows
// !!!!!!!!

    // DO LATWIEJSZEGO POSLUGIWANIA SIE UI
struct menustruct
{
    void show()
    {
        std::cout << "1 - show table \n";
        std::cout << "2 - show results \n";
        std::cout << "3 - show player stats \n";
        std::cout << "4 - clear console \n";
        std::cout << "5 - exit \n";
        std::cout << "\n############################## \n \n";
        std::cout << "Choose option: ";
    }
};

struct graczstruct
{
    std::string indeks;
    char symbol;
    int l_wygrane, l_remisy, l_przegrane, punkty;
};

std::vector<graczstruct> wektorGraczy;

// BOOLE DO STATYSTYK
bool czyWygrana(const std::string& plansza, char gracz)
{
    return (plansza[0] == gracz && plansza[1] == gracz && plansza[2] == gracz) ||
        (plansza[3] == gracz && plansza[4] == gracz && plansza[5] == gracz) ||
        (plansza[6] == gracz && plansza[7] == gracz && plansza[8] == gracz) ||
        (plansza[0] == gracz && plansza[3] == gracz && plansza[6] == gracz) ||
        (plansza[1] == gracz && plansza[4] == gracz && plansza[7] == gracz) ||
        (plansza[2] == gracz && plansza[5] == gracz && plansza[8] == gracz) ||
        (plansza[0] == gracz && plansza[4] == gracz && plansza[8] == gracz) ||
        (plansza[2] == gracz && plansza[4] == gracz && plansza[6] == gracz);
}

bool czyRemis(const std::string& plansza)
{
    for (char pole : plansza)
    {
        if (pole == '.')
            return false;
        else
            return true;
    }
}

bool ktoryLepszy(const graczstruct& a, const graczstruct& b)
{
    return a.punkty > b.punkty;
}
///////////////////


// VOIDY DO WCZYTOW
void wczytajGraczy()
{
    std::ifstream players("players.txt");
    if (!players.is_open())
    {
        std::cerr << "Nie mo�na otworzy� pliku players.txt \n";
        return;
    }

    graczstruct gracz;

    while (players >> gracz.indeks >> gracz.symbol)
    {
        gracz.l_wygrane = 0;
        gracz.l_remisy = 0;
        gracz.l_przegrane = 0;
        gracz.punkty = 0;
        wektorGraczy.push_back(gracz);
    }
    players.close();
}

void wyswietlStatystyki()
{
    std::cout << "\n############### Statystyki graczy ############### \n \n";
    std::cout << "  Id   Sign  Wins  Draws  Loses  Points\n";

    std::sort(wektorGraczy.begin(), wektorGraczy.end(), ktoryLepszy);

    for (const auto& gracz : wektorGraczy)
    {
        std::cout << gracz.indeks << "  "
            << gracz.symbol << "     "
            << gracz.l_wygrane << "    "
            << gracz.l_remisy << "    "
            << gracz.l_przegrane << "     "
            << gracz.punkty << "\n";
    }
}

void wyswietlTabele()
{
    std::ifstream plik("tictactoe.txt");

    if (!plik.is_open())
    {
        std::cerr << "Nie mozna otworzyc pliku... \n";
    }
    std::string linia;
    while (std::getline(plik, linia))
    {
        std::cout << linia << "\n";
    }
    plik.close();
}

///////////////////


int main()
{
    std::ifstream tictactoe("tictactoe.txt");

    wczytajGraczy();

    std::string plansza;

    // GATHERING INFO Z TXT
    while (std::getline(tictactoe, plansza))
    {
        for (int i = 0; i < 3; ++i)
        {
            std::string line;
            std::getline(tictactoe, line);
            if (!line.empty())
                plansza += line;
        }

        char gracz = '\0';
        for (char znak : plansza)
        {
            if (znak != '.' && znak != ' ')
            {
                gracz = znak;
                break;
            }
        }


        int index = -1;
        for (size_t i = 0; i < wektorGraczy.size(); ++i)
        {
            if (wektorGraczy[i].symbol == gracz)
            {
                index = i;
                break;
            }
        }

        if (index != -1)
        {
            if (czyWygrana(plansza, gracz))
            {
                wektorGraczy[index].l_wygrane++;
                wektorGraczy[index].punkty += 3;
            }
            else if (!czyRemis(plansza))
            {
                wektorGraczy[index].l_przegrane++;
            }
            else
            {
                wektorGraczy[index].l_remisy++;
                wektorGraczy[index].punkty += 1;
            }
        }
    }

    menustruct Menu;
    int wybor;
    Menu.show();

    // BASIC SWITCH/DO WHILE LOOP MENU
    do
    {
        std::cin >> wybor;
        switch (wybor)
        {
        case 1:
            std::system("cls");
            wyswietlStatystyki();
            std::cout << "\n############################## \n \n";
            Menu.show();
            break;

        case 2:
            std::system("cls");
            wyswietlTabele();
            std::cout << "\n############################## \n \n";
            Menu.show();
            break;

        case 3:
        {

            std::system("cls");
            std::cout << "Enter player id: ";
            int playerId;
            std::cin >> playerId;

            bool found = false;
            for (const auto& gracz : wektorGraczy)
            {
                if (std::stoi(gracz.indeks) == playerId)
                {
                    std::system("cls");
                    std::cout << "Id: " << gracz.indeks << "\n";
                    std::cout << "Symbol: " << gracz.symbol << "\n";
                    std::cout << "Wins: " << gracz.l_wygrane << "\n";
                    std::cout << "Draws: " << gracz.l_remisy << "\n";
                    std::cout << "Losses: " << gracz.l_przegrane << "\n";
                    std::cout << "Points: " << gracz.punkty << "\n";
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                std::cout << "Gracz o nr. indeksu " << playerId << " nie istnieje! \n";
            }
            std::cout << "\n############################## \n \n";
            Menu.show();
            break;
        }
        case 4:
            std::system("cls");
            Menu.show();
            break;

        case 5:
            std::system("cls");
            std::cout << "Do nastepnego! :) \n";
            break;

        default:
            std::cout << "Nieprawidlowa opcja. Sprobuj ponownie.\n";
            break;

        }
    } while (wybor != 5);


    return 0;
}
