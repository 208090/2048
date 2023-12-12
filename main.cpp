#include <iostream>
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <conio.h>

using namespace std;

enum colors 
{
    BLACK, BLUE, GREEN, CYAN, RED, PURPLE, YELLOW, GREY,
    LIGHTGREY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
    LIGHTPURPLE, LIGHTYELLOW, WHITE
};

void setConsoleColor(int textColor, int bgColor) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}

int randN(int start, int end)
{
    return rand() % (end - start) + start;
}

int randN(int n1, int n2, int percent)
{
    if ((rand() % 100) < percent)
        return n1;
    else
        return n2;
}

enum Direction
{
    Left = 75,
    Right = 77,
    Up = 72,
    Down = 80
};

struct Cell
{
    int x;
    int y;
    int value;

    Cell() 
    { 
        x = -1;
        y = -1;
        value = -1;
    }

    Cell(int x, int y, int value)
    {
        this->x = x;
        this->y = y;
        this->value = value;
    }

    void setValue(int value)
    {
        this->value = value;
    }

    void display()
    {
        cout << setw(4) << value;
    }

    bool isEmpty()
    {
        return value == 0;
    }

    bool isEqualTo(Cell cell)
    {
        return value == cell.value;
    }
};

struct Game
{
    Cell *cells;
    Cell *prevCells;
    int size;
    int score = 0;
    int highscore;
    int moves = 0;

    bool isMoved()
    {
        for (int i = 0; i < size * size; i++)
        {
            if (!cells[i].isEqualTo(prevCells[i]))
            {
                moves += 1;
                return true;
            }
        }
        return false;
    }

    void Doubled(int points)
    {
        score += points;
    }

    bool isCellsFull()
    {
        for (int i = 0; i < size * size; i++)
        {
            if (cells[i].isEmpty())
                return false;
        }
        return true;
    }

    Cell &findCell(int x, int y)
    {
        for (int i = 0; i < size * size; i++)
        {
            if (cells[i].x == x && cells[i].y == y)
                return cells[i];
        }
        return Cell();
    }

    Cell &randEmptyCell()
    {
        if (isCellsFull())
            return Cell();
        int i;
        do
        {
            i = randN(0, size * size);
        } while (!cells[i].isEmpty());
        return cells[i];
    }

    void create(int size)
    {
        this->size = size;
        cells = new Cell[size * size];
        prevCells = new Cell[size * size];

        for (int x = 0, c = 0; x < size; x++)
        {
            for (int y = 0; y < size; y++, c++)
            {
                cells[c] = Cell(x, y, 0);
            }
        }
    }

    void display()
    {
        for (int i = 0, c = 0; i < size + 1; i++)
        {
            for (int j = 0; j < size; j++)
                cout << " ----";
            cout << endl;
            if (i == size)
                break;
            for (int j = 0; j < size + 1; j++, c++)
            {
                cout << "|";
                if (j == size)
                    break;
                if (cells[c].isEmpty())
                    cout << setw(4) << " ";
                else
                {
                    setConsoleColor(LIGHTGREEN, BLUE);
                    cells[c].display();
                    setConsoleColor(GREY, BLACK);
                }
            }
            cout << endl;
        }
        cout << "Score: " << score << "  Highscore: " << highscore << endl;
        cout << "Moves: " << moves << endl;
    }

    void handleKey()
    {
        copy(cells, cells + size * size, prevCells);
        _getch();
        int ch = _getch();
        switch (ch)
        {
        case Left:
            moveCells(Left);
            break;
        case Right:
            moveCells(Right);
            break;
        case Up:
            moveCells(Up);
            break;
        case Down:
            moveCells(Down);
            break;
        }
    }

    void moveCells(Direction dir)
    {
        int start = dir == Left || dir == Up ? 0 : size - 1;
        int end = start == 0 ? size : -1;

        for (int x = 0; x < size; x++)
        {
            for (int y = start; y != end; (start < end ? y++ : y--))
            {
                Cell &c1 = dir == Left || dir == Right ? findCell(x, y) : findCell(y, x);
                if (c1.isEmpty())
                    continue;
                for (int k = y + (start == 0 ? -1 : 1); k != (end == -1 ? size : -1); (start < end ? k-- : k++))
                {
                    Cell &c2 = dir == Left || dir == Right ? findCell(x, k) : findCell(k, x);
                    Cell &c3 = dir == Left ?  findCell(x, k + 1) :
                               dir == Right ? findCell(x, k - 1) :
                               dir == Up ?    findCell(k + 1, x) :
                                              findCell(k - 1, x);
                    if (c1.isEqualTo(c2))
                    {
                        c2.setValue(c2.value * 2);
                        c1.setValue(0);
                        Doubled(c2.value);
                    }
                    else if (!c2.isEmpty() && c3.isEmpty())
                    {
                        c3.setValue(c1.value);
                        c1.setValue(0);
                    }
                    else if (k == start && c2.isEmpty())
                    {
                        c2.setValue(c1.value);
                        c1.setValue(0);
                    }
                }
            }
        }
    }

    void spawnCells(int amount)
    {
        for (int i = 0; i < amount; i++)
        {
            randEmptyCell().setValue(randN(2, 4, 85));
        }
    }

    bool isOver()
    {
        if (!isCellsFull())
            return false;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                Cell &center = findCell(i, j);
                Cell &right = findCell(i, j + 1);
                Cell &bottom = findCell(i + 1, j);

                if (center.isEqualTo(right) || center.isEqualTo(bottom))
                    return false;
            }
            return true;
        }
    }

    bool isWin()
    {
        for (int i = 0; i < size * size; i++)
        {
            if (cells[i].value == 2048)
                return true;
        }
        return false;
    }
};

void main()
{
    setlocale(LC_ALL, "rus");

    srand(time(0));

    bool playAgain = true;
    int highscore = 0;

    do
    {
        Game game;
        game.create(4);
        game.spawnCells(2);
        game.highscore = highscore;
        game.display();

        do
        {
            game.handleKey();
            if (game.isMoved())
                game.spawnCells(1);
            if (game.score > highscore)
                game.highscore = game.score;
            system("cls");
            game.display();
        } while (!game.isWin() && !game.isOver());

        if (game.isOver())
        {
            setConsoleColor(LIGHTRED, BLACK);
            cout << "You won!" << endl;
            setConsoleColor(GREY, BLACK);
        }
        else if (game.isWin())
        {
            setConsoleColor(LIGHTGREEN, BLACK);
            cout << "You lost!" << endl;
            setConsoleColor(GREY, BLACK);
        }

        cout << "Play again? y/n\n";
        char ch;
        do
        {
            cin >> ch;
            if (ch != 'y' && ch != 'n')
                cout << "Incorrect!\n";
        } while (ch != 'y' && ch != 'n');

        switch (ch)
        {
        case 'y':
            highscore = game.score;
            system("cls");
            break;
        case 'n':
            playAgain = false;
            break;
        }
    } while (playAgain);
}
