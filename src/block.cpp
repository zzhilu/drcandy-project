#include "block.h"

#include <cstdlib>

Block::Block()
{
    for (int i = 0; i < 3; i++)
    {
        m_candies[i] = nullptr;
    }

    reiniciar();
}

Block::~Block()
{
    for (int i = 0; i < 3; i++)
    {
        delete m_candies[i];
        m_candies[i] = nullptr;
    }
}

void Block::reiniciar()
{
    for (int i = 0; i < 3; i++)
    {
        delete m_candies[i];

        int randomValue = std::rand() % 6;

        if (randomValue == 0)
        {
            m_candies[i] = new Candy(CandyType::TYPE_RED);
        }
        else if (randomValue == 1)
        {
            m_candies[i] = new Candy(CandyType::TYPE_BLUE);
        }
        else if (randomValue == 2)
        {
            m_candies[i] = new Candy(CandyType::TYPE_GREEN);
        }
        else if (randomValue == 3)
        {
            m_candies[i] = new Candy(CandyType::TYPE_YELLOW);
        }
        else if (randomValue == 4)
        {
            m_candies[i] = new Candy(CandyType::TYPE_PURPLE);
        }
        else
        {
            m_candies[i] = new Candy(CandyType::TYPE_ORANGE);
        }
    }

    m_x = 5;
    m_y = -3;
}

void Block::setPosicion(int x, int y)
{
    m_x = x;
    m_y = y;
}
void Block::setCandy(int index, CandyType type)
{
    if (index >= 0 && index < 3)
    {
        delete m_candies[index];
        m_candies[index] = new Candy(type);
    }
}

void Block::girar()
{
    Candy* temp = m_candies[0];

    m_candies[0] = m_candies[1];
    m_candies[1] = m_candies[2];
    m_candies[2] = temp;
}

void Block::moverIzquierda()
{
    m_x--;
}

void Block::moverDerecha()
{
    m_x++;
}

void Block::caer()
{
    m_y++;
}

int Block::getX() const
{
    int x = m_x;
    return x;
}

int Block::getY() const
{
    int y = m_y;
    return y;
}

Candy* Block::getCandy(int index) const
{
    Candy* candy = nullptr;

    if (index >= 0 && index < 3)
    {
        candy = m_candies[index];
    }

    return candy;
}

bool Block::operator==(const Block& other) const
{
    bool iguales = true;

    if (m_x != other.m_x || m_y != other.m_y)
    {
        iguales = false;
    }

    for (int i = 0; i < 3 && iguales; i++)
    {
        Candy* candy = m_candies[i];
        Candy* otherCandy = other.m_candies[i];

        if (candy == nullptr && otherCandy != nullptr)
        {
            iguales = false;
        }
        else if (candy != nullptr && otherCandy == nullptr)
        {
            iguales = false;
        }
        else if (candy != nullptr && otherCandy != nullptr)
        {
            if (candy->getType() != otherCandy->getType())
            {
                iguales = false;
            }
        }
    }

    return iguales;
}