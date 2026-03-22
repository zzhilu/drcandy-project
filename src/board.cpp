#include "board.h"
#include <memory>
#include <iostream>

Board::Board(int width, int height)
{
    // Implement your code here
}

Board::~Board()
{
    // Implement your code here
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
    // Implement your code here
    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    // Implement your code here
    return {};
}

bool Board::dump(const std::string& output_path) const
{
    // Implement your code here
    return false;
}

bool Board::load(const std::string& input_path)
{
    // Implement your code here
    return false;
}
