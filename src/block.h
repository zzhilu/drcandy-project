#pragma once

#define BLOCK_H

#include "candy.h"

class Block {
public:
    Block();
    ~Block();

    void reiniciar();
    void  girar();
    void moverIzquierda();
    void moverDerecha();
    void caer();

    int getX() const;
    int getY() const;

    void setPosicion(int x, int y);
    void setCandy(int index, CandyType type);

    Candy* getCandy(int index) const;

    bool operator==(const Block& other) const;
private:
    Candy* m_candies[3];
    int m_x;
    int m_y;

};