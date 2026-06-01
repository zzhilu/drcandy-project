#include "game.h"
#include <random>
#include <fstream>
#include <string>
#include "graphics.h"
#include "candy.h"
#include "util.h"

Game::Game()
{
    m_frameCounter = 0;
    m_score = 0;
    m_gameOver = false;
}

Game::~Game()
{
    // Implement your code here
}

void Game::update(const Controller& controller)
{
    if (!m_gameOver)
    {
        int blockX = m_block.getX();
        int blockY = m_block.getY();

        if (controller.isLeftPressed())
        {
            bool canMove = true;
            int newX = blockX - 1;

            for (int i = 0; i < 3; i++)
            {
                int y = blockY + i;

                if (newX < 0 || newX >= m_board.getWidth())
                {
                    canMove = false;
                }
                else if (y >= 0 && y < m_board.getHeight())
                {
                    if (m_board.getCell(newX, y) != nullptr)
                    {
                        canMove = false;
                    }
                }
            }

            if (canMove)
            {
                m_block.moverIzquierda();
            }
        }

        if (controller.isRightPressed())
        {
            bool canMove = true;
            int newX = blockX + 1;

            for (int i = 0; i < 3; i++)
            {
                int y = blockY + i;

                if (newX < 0 || newX >= m_board.getWidth())
                {
                    canMove = false;
                }
                else if (y >= 0 && y < m_board.getHeight())
                {
                    if (m_board.getCell(newX, y) != nullptr)
                    {
                        canMove = false;
                    }
                }
            }

            if (canMove)
            {
                m_block.moverDerecha();
            }
        }

        if (controller.isKey1Pressed())
        {
            m_block.girar();
        }

        if (controller.isKey2Pressed())
        {
            dump(getDataDirPath() + "/save.txt");
        }

        m_frameCounter++;

        bool shouldFall = false;

        if (m_frameCounter >= 60)
        {
            shouldFall = true;
            m_frameCounter = 0;
        }

        if (controller.isDownPressed())
        {
            shouldFall = true;
        }

        if (shouldFall)
        {
            bool canFall = true;

            blockX = m_block.getX();
            blockY = m_block.getY();

            for (int i = 0; i < 3; i++)
            {
                int newY = blockY + i + 1;

                if (newY >= m_board.getHeight())
                {
                    canFall = false;
                }
                else if (newY >= 0)
                {
                    if (m_board.getCell(blockX, newY) != nullptr)
                    {
                        canFall = false;
                    }
                }
            }

            if (canFall)
            {
                m_block.caer();
            }
            else
            {
                if (m_block.getY() < 0)
                {
                    m_gameOver = true;
                }
                else
                {
                    for (int i = 0; i < 3; i++)
                    {
                        int x = m_block.getX();
                        int y = m_block.getY() + i;

                        Candy* blockCandy = m_block.getCandy(i);

                        if (blockCandy != nullptr)
                        {
                            Candy* newCandy = new Candy(blockCandy->getType());
                            m_board.setOwnedCandy(newCandy, x, y);
                        }
                    }

                    std::vector<Candy*> exploded = m_board.explodeAndDrop();
                    m_score = m_score + exploded.size();

                    m_block.reiniciar();
                }
            }
        }
    }
}


void Game::render(GraphicManager & graphics)
    {
        const int board_padding = 3;
        const int boardX = CANDY_IMAGE_WIDTH * board_padding;
        const int boardY = CANDY_IMAGE_HEIGHT * board_padding;

        graphics.drawRectangle(
            boardX,
            boardY,
            CANDY_IMAGE_WIDTH * m_board.getWidth(),
            CANDY_IMAGE_HEIGHT * m_board.getHeight(),
            5,150,150,150);

        for (int y = 0; y < m_board.getHeight(); y++)
        {
            for (int x = 0; x < m_board.getWidth(); x++)
            {
                Candy* candy = m_board.getCell(x, y);

                if (candy != nullptr)
                {
                    int drawX = boardX + x * CANDY_IMAGE_WIDTH;
                    int drawY = boardY + y * CANDY_IMAGE_HEIGHT;

                    graphics.drawImage(candy->getResourceName(), drawX, drawY);
                }
            }
        }

        if (!m_gameOver)
        {
            for (int i = 0; i < 3; i++)
            {
                Candy* candy = m_block.getCandy(i);
                int x = m_block.getX();
                int y = m_block.getY() + i;

                if (candy != nullptr && y >= 0)
                {
                    int drawX = boardX + x * CANDY_IMAGE_WIDTH;
                    int drawY = boardY + y * CANDY_IMAGE_HEIGHT;

                    graphics.drawImage(candy->getResourceName(), drawX, drawY);
                }
            }
        }

        graphics.drawImage("img/logo_small.png", 10, 10);

        graphics.drawText(
            "Movement: [Down] [Left] [Right]  --  Buttons: [Q] rotate [W] save  --  Exit [ESC]",25,
            700,20,100,100,100);

        graphics.drawText(
            "Score: " + std::to_string(m_score),450,10,50,125,200,125);

        if (m_gameOver)
        {
            graphics.drawText("GAME OVER",180,250,60,200,0,0);
        }
 }

void Game::run()
{
    const int screen_width = 750;
    const int screen_height = 750;
    const int bg_red = 255;
    const int bg_green = 255;
    const int bg_blue = 255;
    runGraphicGame(*this, screen_width, screen_height, bg_red, bg_green, bg_blue);
}

bool Game::dump(const std::string& output_path) const
{
    bool result = true;

    std::ofstream file(output_path);

    if (!file.is_open())
    {
        result = false;
    }
    else
    {
        file << m_gameOver << " " << m_frameCounter << " " << m_score << "\n";

        file << m_board.getWidth() << " " << m_board.getHeight() << "\n";

        for (int y = 0; y < m_board.getHeight(); y++)
        {
            for (int x = 0; x < m_board.getWidth(); x++)
            {
                char ch = '.';
                Candy* candy = m_board.getCell(x, y);

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

                if (x < m_board.getWidth() - 1)
                {
                    file << " ";
                }
            }

            file << "\n";
        }

        file << m_block.getX() << " " << m_block.getY() << "\n";

        for (int i = 0; i < 3; i++)
        {
            char ch = '.';
            Candy* candy = m_block.getCandy(i);

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

            if (i < 2)
            {
                file << " ";
            }
        }

        file << "\n";
    }

    return result;
}

bool Game::load(const std::string& input_path)
{
    bool result = true;

    std::ifstream file(input_path);

    if (!file.is_open())
    {
        result = false;
    }
    else
    {
        int newGameOver;
        int newFrameCounter;
        int newScore;

        if (!(file >> newGameOver >> newFrameCounter >> newScore))
        {
            result = false;
        }
        else
        {
            int newWidth;
            int newHeight;

            if (!(file >> newWidth >> newHeight))
            {
                result = false;
            }
            else if (newWidth <= 0 || newHeight <= 0)
            {
                result = false;
            }
            else
            {
                Board newBoard(newWidth, newHeight);

                for (int y = 0; y < newHeight && result; y++)
                {
                    for (int x = 0; x < newWidth && result; x++)
                    {
                        char ch;

                        if (!(file >> ch))
                        {
                            result = false;
                        }
                        else
                        {
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

                            if (result && candy != nullptr)
                            {
                                newBoard.setOwnedCandy(candy, x, y);
                            }
                        }
                    }
                }

                int blockX = 0;
                int blockY = 0;
                CandyType blockTypes[3];

                if (result)
                {
                    if (!(file >> blockX >> blockY))
                    {
                        result = false;
                    }
                }

                for (int i = 0; i < 3 && result; i++)
                {
                    char ch;

                    if (!(file >> ch))
                    {
                        result = false;
                    }
                    else if (ch == 'R')
                    {
                        blockTypes[i] = CandyType::TYPE_RED;
                    }
                    else if (ch == 'B')
                    {
                        blockTypes[i] = CandyType::TYPE_BLUE;
                    }
                    else if (ch == 'G')
                    {
                        blockTypes[i] = CandyType::TYPE_GREEN;
                    }
                    else if (ch == 'Y')
                    {
                        blockTypes[i] = CandyType::TYPE_YELLOW;
                    }
                    else if (ch == 'P')
                    {
                        blockTypes[i] = CandyType::TYPE_PURPLE;
                    }
                    else if (ch == 'O')
                    {
                        blockTypes[i] = CandyType::TYPE_ORANGE;
                    }
                    else
                    {
                        result = false;
                    }
                }

                if (result)
                {
                    m_gameOver = newGameOver != 0;
                    m_frameCounter = newFrameCounter;
                    m_score = newScore;

                    m_board = newBoard;

                    m_block.setPosicion(blockX, blockY);

                    for (int i = 0; i < 3; i++)
                    {
                        m_block.setCandy(i, blockTypes[i]);
                    }
                }
            }
        }
    }

    return result;
}

bool Game::operator==(const Game& other) const
{
    bool iguales = true;

    if (m_frameCounter != other.m_frameCounter)
    {
        iguales = false;
    }
    else if (m_score != other.m_score)
    {
        iguales = false;
    }
    else if (m_gameOver != other.m_gameOver)
    {
        iguales = false;
    }
    else if (m_board.getWidth() != other.m_board.getWidth())
    {
        iguales = false;
    }
    else if (m_board.getHeight() != other.m_board.getHeight())
    {
        iguales = false;
    }
    else if (!(m_block == other.m_block))
    {
        iguales = false;
    }

    for (int y = 0; y < m_board.getHeight() && iguales; y++)
    {
        for (int x = 0; x < m_board.getWidth() && iguales; x++)
        {
            Candy* candy = m_board.getCell(x, y);
            Candy* otherCandy = other.m_board.getCell(x, y);

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
    }

    return iguales;
}