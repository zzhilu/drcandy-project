#include <filesystem>
#include <iostream>
#include <string>

#include "board.h"
#include "candy.h"
#include "test.h"
#include "util.h"

using namespace std;

void mostrarResultado(const string& nombre, bool ok, int& total, int& aprobados)
{
    total++;

    if (ok)
    {
        aprobados++;
        cout << "[OK] " << nombre << endl;
    }
    else
    {
        cout << "[ERROR] " << nombre << endl;
    }
}

bool test()
{
    int total = 0;
    int aprobados = 0;
    bool todoOk = true;

    // 1. Constructor, getWidth y getHeight
    {
        Board b(4, 3);
        bool ok = true;

        if (b.getWidth() != 4)
        {
            ok = false;
        }

        if (b.getHeight() != 3)
        {
            ok = false;
        }

        mostrarResultado("Constructor, getWidth y getHeight", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 2. Tablero vacio
    {
        Board b(4, 3);
        bool ok = true;

        if (b.getCell(0, 0) != nullptr)
        {
            ok = false;
        }

        if (b.getCell(3, 2) != nullptr)
        {
            ok = false;
        }

        mostrarResultado("Tablero vacio", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 3. getCell con coordenadas invalidas
    {
        Board b(4, 3);
        bool ok = true;

        if (b.getCell(-1, 0) != nullptr)
        {
            ok = false;
        }

        if (b.getCell(0, -1) != nullptr)
        {
            ok = false;
        }

        if (b.getCell(4, 0) != nullptr)
        {
            ok = false;
        }

        if (b.getCell(0, 3) != nullptr)
        {
            ok = false;
        }

        mostrarResultado("getCell con coordenadas invalidas", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 4. setCell y getCell
    {
        Board b(4, 3);
        Candy c(CandyType::TYPE_ORANGE);
        bool ok = true;

        b.setCell(&c, 2, 1);

        if (b.getCell(2, 1) != &c)
        {
            ok = false;
        }

        mostrarResultado("setCell y getCell validos", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 5. setCell con coordenadas invalidas
    {
        Board b(4, 3);
        Candy c(CandyType::TYPE_RED);
        bool ok = true;

        b.setCell(&c, -1, 0);
        b.setCell(&c, 0, -1);
        b.setCell(&c, 4, 0);
        b.setCell(&c, 0, 3);

        if (b.getCell(0, 0) != nullptr)
        {
            ok = false;
        }

        if (b.getCell(3, 2) != nullptr)
        {
            ok = false;
        }

        mostrarResultado("setCell con coordenadas invalidas", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 6. shouldExplode en celda vacia 
    {
        Board b(4, 3);
        bool ok = true;

        if (b.shouldExplode(0, 0))
        {
            ok = false;
        }

        if (b.shouldExplode(-1, 0))
        {
            ok = false;
        }

        if (b.shouldExplode(4, 0))
        {
            ok = false;
        }

        if (b.shouldExplode(0, 3))
        {
            ok = false;
        }

        mostrarResultado("shouldExplode en casos invalidos", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 7. Explosion horizontal
    {
        Board b(5, 5);
        Candy c1(CandyType::TYPE_RED);
        Candy c2(CandyType::TYPE_RED);
        Candy c3(CandyType::TYPE_RED);
        bool ok = true;

        b.setCell(&c1, 1, 2);
        b.setCell(&c2, 2, 2);
        b.setCell(&c3, 3, 2);

        if (!b.shouldExplode(1, 2))
        {
            ok = false;
        }

        if (!b.shouldExplode(2, 2))
        {
            ok = false;
        }

        if (!b.shouldExplode(3, 2))
        {
            ok = false;
        }

        mostrarResultado("Explosion horizontal", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 8. Explosion vertical
    {
        Board b(5, 5);
        Candy c1(CandyType::TYPE_BLUE);
        Candy c2(CandyType::TYPE_BLUE);
        Candy c3(CandyType::TYPE_BLUE);
        bool ok = true;

        b.setCell(&c1, 2, 1);
        b.setCell(&c2, 2, 2);
        b.setCell(&c3, 2, 3);

        if (!b.shouldExplode(2, 1))
        {
            ok = false;
        }

        if (!b.shouldExplode(2, 2))
        {
            ok = false;
        }

        if (!b.shouldExplode(2, 3))
        {
            ok = false;
        }

        mostrarResultado("Explosion vertical", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 9. Explosion diagonal principal
    {
        Board b(5, 5);
        Candy c1(CandyType::TYPE_GREEN);
        Candy c2(CandyType::TYPE_GREEN);
        Candy c3(CandyType::TYPE_GREEN);
        bool ok = true;

        b.setCell(&c1, 1, 1);
        b.setCell(&c2, 2, 2);
        b.setCell(&c3, 3, 3);

        if (!b.shouldExplode(1, 1))
        {
            ok = false;
        }

        if (!b.shouldExplode(2, 2))
        {
            ok = false;
        }

        if (!b.shouldExplode(3, 3))
        {
            ok = false;
        }

        mostrarResultado("Explosion diagonal principal", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 10. Explosion diagonal 
    {
        Board b(5, 5);
        Candy c1(CandyType::TYPE_YELLOW);
        Candy c2(CandyType::TYPE_YELLOW);
        Candy c3(CandyType::TYPE_YELLOW);
        bool ok = true;

        b.setCell(&c1, 1, 3);
        b.setCell(&c2, 2, 2);
        b.setCell(&c3, 3, 1);

        if (!b.shouldExplode(1, 3))
        {
            ok = false;
        }

        if (!b.shouldExplode(2, 2))
        {
            ok = false;
        }

        if (!b.shouldExplode(3, 1))
        {
            ok = false;
        }

        mostrarResultado("Explosion diagonal secundaria", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 11. explodeAndDrop simple
    {
        Board b(3, 4);

        Candy red1(CandyType::TYPE_RED);
        Candy red2(CandyType::TYPE_RED);
        Candy red3(CandyType::TYPE_RED);
        Candy blue(CandyType::TYPE_BLUE);

        bool ok = true;

        b.setCell(&red1, 0, 3);
        b.setCell(&red2, 1, 3);
        b.setCell(&red3, 2, 3);
        b.setCell(&blue, 0, 2);

        vector<Candy*> exploded = b.explodeAndDrop();

        if (exploded.size() != 3)
        {
            ok = false;
        }

        if (b.getCell(0, 3) != &blue)
        {
            ok = false;
        }

        if (b.getCell(1, 3) != nullptr)
        {
            ok = false;
        }

        if (b.getCell(2, 3) != nullptr)
        {
            ok = false;
        }

        mostrarResultado("explodeAndDrop simple", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 12. Reaccion en cadena
    {
        Board b(3, 4);

        Candy blue1(CandyType::TYPE_BLUE);
        Candy blue2(CandyType::TYPE_BLUE);
        Candy blue3(CandyType::TYPE_BLUE);

        Candy red1(CandyType::TYPE_RED);
        Candy red2(CandyType::TYPE_RED);
        Candy red3(CandyType::TYPE_RED);

        bool ok = true;

        b.setCell(&red1, 0, 3);
        b.setCell(&red2, 1, 3);
        b.setCell(&red3, 2, 3);

        b.setCell(&blue1, 0, 1);
        b.setCell(&blue2, 1, 1);
        b.setCell(&blue3, 2, 2);

        vector<Candy*> exploded = b.explodeAndDrop();

        if (exploded.size() != 6)
        {
            ok = false;
        }

        for (int y = 0; y < b.getHeight(); y++)
        {
            for (int x = 0; x < b.getWidth(); x++)
            {
                if (b.getCell(x, y) != nullptr)
                {
                    ok = false;
                }
            }
        }

        mostrarResultado("Reaccion en cadena", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 13. dump y load
    {
        Board b(4, 3);
        Candy red(CandyType::TYPE_RED);
        Candy orange(CandyType::TYPE_ORANGE);
        Candy purple(CandyType::TYPE_PURPLE);

        bool ok = true;

        b.setCell(&red, 0, 0);
        b.setCell(&orange, 2, 1);
        b.setCell(&purple, 3, 2);

        string path = getDataDirPath() + "/test_board.txt";
        std::filesystem::remove(path);

        if (!b.dump(path))
        {
            ok = false;
        }

        Board loaded(1, 1);

        if (!loaded.load(path))
        {
            ok = false;
        }

        if (loaded.getWidth() != 4)
        {
            ok = false;
        }

        if (loaded.getHeight() != 3)
        {
            ok = false;
        }

        if (loaded.getCell(0, 0) == nullptr)
        {
            ok = false;
        }
        else if (loaded.getCell(0, 0)->getType() != CandyType::TYPE_RED)
        {
            ok = false;
        }

        if (loaded.getCell(2, 1) == nullptr)
        {
            ok = false;
        }
        else if (loaded.getCell(2, 1)->getType() != CandyType::TYPE_ORANGE)
        {
            ok = false;
        }

        if (loaded.getCell(3, 2) == nullptr)
        {
            ok = false;
        }
        else if (loaded.getCell(3, 2)->getType() != CandyType::TYPE_PURPLE)
        {
            ok = false;
        }

        if (loaded.getCell(1, 0) != nullptr)
        {
            ok = false;
        }

        std::filesystem::remove(path);

        mostrarResultado("dump y load", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    // 14. load con archivo inexistente
    {
        Board b(2, 2);
        bool ok = true;

        string path = getDataDirPath() + "/file_that_does_not_exist_12345.txt";
        std::filesystem::remove(path);

        if (b.load(path))
        {
            ok = false;
        }

        mostrarResultado("load con archivo inexistente", ok, total, aprobados);

        if (!ok)
        {
            todoOk = false;
        }
    }

    cout << endl;
    cout << "Resumen: " << aprobados << " de " << total << " tests superados." << endl;

    if (todoOk)
    {
        cout << "Resultado final: TODO CORRECTO." << endl;
    }
    else
    {
        cout << "Resultado final: HAY ERRORES." << endl;
    }

    return todoOk;
}