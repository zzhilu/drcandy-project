#include "board.h"
#include <fstream>



Board::Board(int width, int height)
{
    m_width = width;
    m_height = height;

    m_cells.resize(m_height);

    for (int y = 0; y < m_height; y++)
    {
        m_cells[y].resize(m_width);

        for (int x = 0; x < m_width; x++)
        {
            m_cells[y][x] = nullptr;
        }
    }
}

Board::~Board()
{
    // Primera entrega: no ownership of dynamic memory here
}

Candy* Board::getCell(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        return nullptr;
    }

    return m_cells[y][x];
}

void Board::setCell(Candy* candy, int x, int y)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        return;
    }

    m_cells[y][x] = candy;
}

int Board::getWidth() const
{
    return m_width;
}

int Board::getHeight() const
{
    return m_height;
}

bool Board::shouldExplode(int x, int y) const
{
    Candy* current = getCell(x, y);

    if (current == nullptr)
    {
        return false;
    }

    int count;

    // Horizontal
    count = 1;

    for (int nx = x - 1; nx >= 0; nx--)
    {
        Candy* other = getCell(nx, y);

        if (other != nullptr && other->getType() == current->getType())
        {
            count++;
        }
        else
        {
            break;
        }
    }

    for (int nx = x + 1; nx < m_width; nx++)
    {
        Candy* other = getCell(nx, y);

        if (other != nullptr && other->getType() == current->getType())
        {
            count++;
        }
        else
        {
            break;
        }
    }

    if (count >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    // Vertical
    count = 1;

    for (int ny = y - 1; ny >= 0; ny--)
    {
        Candy* other = getCell(x, ny);

        if (other != nullptr && other->getType() == current->getType())
        {
            count++;
        }
        else
        {
            break;
        }
    }

    for (int ny = y + 1; ny < m_height; ny++)
    {
        Candy* other = getCell(x, ny);

        if (other != nullptr && other->getType() == current->getType())
        {
            count++;
        }
        else
        {
            break;
        }
    }

    if (count >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    // Diagonal
    count = 1;

    for (int nx = x - 1, ny = y - 1; nx >= 0 && ny >= 0; nx--, ny--)
    {
        Candy* other = getCell(nx, ny);

        if (other != nullptr && other->getType() == current->getType())
        {
            count++;
        }
        else
        {
            break;
        }
    }

    for (int nx = x + 1, ny = y + 1; nx < m_width && ny < m_height; nx++, ny++)
    {
        Candy* other = getCell(nx, ny);

        if (other != nullptr && other->getType() == current->getType())
        {
            count++;
        }
        else
        {
            break;
        }
    }

    if (count >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    // Diagonal
    count = 1;

    for (int nx = x - 1, ny = y + 1; nx >= 0 && ny < m_height; nx--, ny++)
    {
        Candy* other = getCell(nx, ny);

        if (other != nullptr && other->getType() == current->getType())
        {
            count++;
        }
        else
        {
            break;
        }
    }

    for (int nx = x + 1, ny = y - 1; nx < m_width && ny >= 0; nx++, ny--)
    {
        Candy* other = getCell(nx, ny);

        if (other != nullptr && other->getType() == current->getType())
        {
            count++;
        }
        else
        {
            break;
        }
    }

    if (count >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    std::vector<Candy*> exploded;
    bool changed = true;

    while (changed)
    {
        changed = false;

        std::vector<std::vector<bool> > toExplode;
        toExplode.resize(m_height);

        for (int y = 0; y < m_height; y++)
        {
            toExplode[y].resize(m_width);

            for (int x = 0; x < m_width; x++)
            {
                toExplode[y][x] = false;
            }
        }

        // 1. Mark
        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                if (shouldExplode(x, y))
                {
                    toExplode[y][x] = true;
                    changed = true;
                }
            }
        }

        // 2. Delete 
        if (changed)
        {
            for (int y = 0; y < m_height; y++)
            {
                for (int x = 0; x < m_width; x++)
                {
                    if (toExplode[y][x])
                    {
                        exploded.push_back(m_cells[y][x]);
                        m_cells[y][x] = nullptr;
                    }
                }
            }

            // 3. Drop 
            for (int x = 0; x < m_width; x++)
            {
                int writeY = m_height - 1;

                for (int y = m_height - 1; y >= 0; y--)
                {
                    if (m_cells[y][x] != nullptr)
                    {
                        m_cells[writeY][x] = m_cells[y][x];

                        if (writeY != y)
                        {
                            m_cells[y][x] = nullptr;
                        }

                        writeY--;
                    }
                }

                while (writeY >= 0)
                {
                    m_cells[writeY][x] = nullptr;
                    writeY--;
                }
            }
        }
    }

    return exploded;
}

bool Board::dump(const std::string& output_path) const
{
    std::ofstream file(output_path);

    if (!file.is_open())
    {
        return false;
    }

    file << m_width << " " << m_height << "\n";

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            char ch = '.';

            if (m_cells[y][x] != nullptr)
            {
                CandyType type = m_cells[y][x]->getType();

                if (type == CandyType::TYPE_RED)
                {
                    ch = 'R';
                }
                else if (type == CandyType::TYPE_BLUE)
                {
                    ch = 'B';
                }
                else if (type == CandyType::TYPE_GREEN)
                {
                    ch = 'G';
                }
                else if (type == CandyType::TYPE_YELLOW)
                {
                    ch = 'Y';
                }
                else if (type == CandyType::TYPE_PURPLE)
                {
                    ch = 'P';
                }
                else if (type == CandyType::TYPE_ORANGE)
                {
                    ch = 'O';
                }
            }

            file << ch;

            if (x < m_width - 1)
            {
                file << " ";
            }
        }

        file << "\n";
    }

    return true;
}

bool Board::load(const std::string& input_path)
{
    std::ifstream file(input_path);

    if (!file.is_open())
    {
        return false;
    }

    int newWidth;
    int newHeight;

    if (!(file >> newWidth >> newHeight))
    {
        return false;
    }

    if (newWidth <= 0 || newHeight <= 0)
    {
        return false;
    }

    std::vector<std::vector<Candy*> > newCells;
    newCells.resize(newHeight);

    for (int y = 0; y < newHeight; y++)
    {
        newCells[y].resize(newWidth);
    }

    // Static candies: they stay alive after load() finishes
    static Candy redCandy(CandyType::TYPE_RED);
    static Candy blueCandy(CandyType::TYPE_BLUE);
    static Candy greenCandy(CandyType::TYPE_GREEN);
    static Candy yellowCandy(CandyType::TYPE_YELLOW);
    static Candy purpleCandy(CandyType::TYPE_PURPLE);
    static Candy orangeCandy(CandyType::TYPE_ORANGE);

    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            char ch;
            if (!(file >> ch))
            {
                return false;
            }

            if (ch == '.')
            {
                newCells[y][x] = nullptr;
            }
            else if (ch == 'R')
            {
                newCells[y][x] = &redCandy;
            }
            else if (ch == 'B')
            {
                newCells[y][x] = &blueCandy;
            }
            else if (ch == 'G')
            {
                newCells[y][x] = &greenCandy;
            }
            else if (ch == 'Y')
            {
                newCells[y][x] = &yellowCandy;
            }
            else if (ch == 'P')
            {
                newCells[y][x] = &purpleCandy;
            }
            else if (ch == 'O')
            {
                newCells[y][x] = &orangeCandy;
            }
            else
            {
                return false;
            }
        }
    }

    m_width = newWidth;
    m_height = newHeight;
    m_cells = newCells;

    return true;
}