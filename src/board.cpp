#include "board.h"
#include <fstream>



Board::Board(int width, int height)
{
    m_width = width;
    m_height = height;
   
    m_cells = new Candy * [m_width * m_height];

    if (width <= 0)
    {
        width = DEFAULT_BOARD_WIDTH;
    }

    if (height <= 0)
    {
        height = DEFAULT_BOARD_HEIGHT;
    }

    for (int i = 0; i < m_width * m_height; i++)
    {
        m_cells[i] = nullptr;
    }
}

Board::~Board()
{
    for (int i = 0; i < static_cast<int>(m_candyPropios.size()); i++)
    {
        delete m_candyPropios[i];
    }

    m_candyPropios.clear();

    delete[] m_cells;
    m_cells = nullptr;
}

Board::Board(const Board& other)
{
    m_width = other.m_width;
    m_height = other.m_height;
    m_cells = new Candy * [m_width * m_height];

    for (int i = 0; i < m_width * m_height; i++)
    {
        m_cells[i] = nullptr;
    }

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {

            Candy* otherCandy = other.getCell(x, y);
            if (otherCandy != nullptr)
            {
                Candy* newCandy = new Candy(otherCandy->getType());
                setOwnedCandy(newCandy, x, y);

            }
        }

    }
}

Board& Board::operator=(const Board& other) { 

    if (this != &other) {

        for (int i = 0; i < static_cast<int>(m_candyPropios.size()); i++)
        {
            delete m_candyPropios[i];
        }

        m_candyPropios.clear();
        delete[] m_cells;
        m_width = other.m_width;
        m_height = other.m_height;

        m_cells = new Candy * [m_width * m_height];

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                Candy* otherCandy = other.getCell(x, y);

                if (otherCandy != nullptr)
                {
                    Candy* newCandy = new Candy(otherCandy->getType());
                    setOwnedCandy(newCandy, x, y);
                }

            }

        }
    
    }


    return *this;
}


Candy* Board::getCell(int x, int y) const
{
    Candy* candy = nullptr;
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        return nullptr;
    }
    candy = m_cells[y * m_width + x];

    return candy;
}

void Board::setCell(Candy* candy, int x, int y)
{
    
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        m_cells[y * m_width + x] = candy;
    }

}

int Board::getWidth() const
{
    return m_width;
}

int Board::getHeight() const
{
    return m_height;
}

void Board::setOwnedCandy(Candy* candy, int x, int y)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        m_cells[y * m_width + x] = candy;
        m_candyPropios.push_back(candy);

    }
    else
    {
        delete candy;
    }
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
                        int index = y * m_width + x;

                        exploded.push_back(m_cells[index]);
                        m_cells[index] = nullptr;
                    }
                }
            }

            // 3. Drop 
            for (int x = 0; x < m_width; x++)
            {
                int writeY = m_height - 1;

                for (int y = m_height - 1; y >= 0; y--)
                {
                    int currentIndex = y * m_width + x;

                    if (m_cells[currentIndex] != nullptr)
                    {
                        int writeIndex = writeY * m_width + x;

                        m_cells[writeIndex] = m_cells[currentIndex];

                        if (writeY != y)
                        {
                            m_cells[currentIndex] = nullptr;
                        }

                        writeY--;
                    }
                }

                while (writeY >= 0)
                {
                    int index = writeY * m_width + x;
                    m_cells[index] = nullptr;
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
            int index = y * m_width + x;
            Candy* candy = m_cells[index];

            if (candy != nullptr)
            {
                CandyType type = candy->getType();

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
    bool result = true;
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

    Candy** newCells = new Candy * [newWidth * newHeight];
    std::vector<Candy*> newOwnedCandies;

    for (int i = 0; i < newWidth * newHeight; i++)
    {
        newCells[i] = nullptr;
    }



    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            char ch;
            if (!(file >> ch))
            {
                result = false;
            }
            Candy* candy = nullptr;
            if (ch == '.')
            {
                candy = nullptr;
            }
            else if (ch == 'R')
            {
                candy = new Candy(CandyType::TYPE_RED);

            }
            else if (ch == 'B')
            {
                candy = new Candy(CandyType::TYPE_BLUE);
            }
            else if (ch == 'G')
            {
                candy = new Candy(CandyType::TYPE_GREEN);
            }
            else if (ch == 'Y')
            {
                candy = new Candy(CandyType::TYPE_YELLOW);
            }
            else if (ch == 'P')
            {
                candy = new Candy(CandyType::TYPE_PURPLE);
            }
            else if (ch == 'O')
            {
                candy = new Candy(CandyType::TYPE_ORANGE);
            }
            else
            {
                result = false;
            }
            if (result)
            {
                int index = y * newWidth + x;

                newCells[index] = candy;

                if (candy != nullptr)
                {
                    newOwnedCandies.push_back(candy);
                }
            }
        }
    }
    if (result)
    {
        for (int i = 0; i < static_cast<int>(m_candyPropios.size()); i++)
        {
            delete m_candyPropios[i];
        }
        m_candyPropios.clear();

        delete[] m_cells;

        m_width = newWidth;
        m_height = newHeight;
        m_cells = newCells;
        m_candyPropios = newOwnedCandies;
      
    }
    return result;
}