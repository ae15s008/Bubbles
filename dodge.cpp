#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <conio.h>
#include <windows.h>

int    ii;
int    jj;
int    kk;

int    scrWid;
int    scrHyt;

char  *screen;

int    FPS;
DWORD  DELAY;

char   inpChar;

double score;
int    nFrames;

int    nBadGuys;
int    nBullets;

int    mBadGuys;
int    mBullets;

int    frequency;

bool   exitCond;

bool   expr1;
bool   expr2;

typedef struct strRoot
{
    struct strSpr *badGuys;
    struct strSpr *bullets;
}Bad;

typedef struct strSpr
{
    char   _chr;

    int    xPos;
    int    yPos;

    int    xVel;
    int    yVel;

    int    time;
    double life;

    struct strSpr *next;
}Spr;

Spr *player;
Bad *baddys;

void  fullscreen();
void  setupVars();
void  clearScreen();
void  printScreen();
void  createSpr(Spr *spr, char c, int x, int y, int u, int v, double l);
void  createBadGuy();
void  writeSprs();
void  writeSpr(Spr *spr);
void  updateSprs();
void  updateSpr(Spr *spr);
Spr  *destroyBadGuy(Spr *spr);
Spr  *destroyBullet(Spr *spr);
void  intConstrain(int *input, int lower, int upper);
void  evalInput();
void  checkCollisions();

int main()
{
    setupVars();

    while(!exitCond)
    {
        if(nFrames % frequency == 0)
            createBadGuy();

        clearScreen();

        writeSprs();

        printScreen();

        evalInput();

        updateSprs();

        Sleep(DELAY);

        if(nBadGuys > mBadGuys)
            mBadGuys = nBadGuys;

        if(nBullets > mBullets)
            mBullets = nBullets;

        nFrames ++;
    }

    updateSpr(player);
    player->_chr = 'X';
    clearScreen();
    writeSprs();
    printScreen();

    printf("\n\n Press <SPACE> to exit");
    while(!kbhit())
        if(_getch() == ' ')
            break;

    return(score);
}

void setupVars()
{
    ii        = 0;
    jj        = 0;
    kk        = 0;

    scrWid    = 76;
    scrHyt    = 20;

    screen    = (char *) calloc((scrWid + 4) * (scrHyt + 4) + 1, sizeof(char));

    FPS       = 120;
    DELAY     = (DWORD) (1000 / (double) FPS);

    inpChar   = '\0';

    player    = (Spr *) calloc(1, sizeof(Spr));

    nBadGuys  = 0;
    nBullets  = 0;

    mBadGuys  = 0;
    mBullets  = 0;

    baddys    = (Bad *) calloc(0, sizeof(Bad));

    score     = 0;
    nFrames   = 0;

    frequency = 5;

    exitCond = false;

    ii = scrWid / 2 + rand() % 2;
    jj = scrHyt / 2 + rand() % 2;
    createSpr(player, 'O', ii, jj, 0, 0, -1);

    srand(time(NULL));
}

void clearScreen()
{
    char digits[10];

    for(ii = 0; ii < scrHyt + 4; ii ++)
        for(jj = 0; jj < scrWid + 4; jj ++)
            screen[ii * (scrWid + 4) + jj] = ' ';

    for(jj = 1; jj < scrWid + 3; jj ++)
    {
        ii = 1;
        screen[ii * (scrWid + 4) + jj] = '=';

        ii = scrHyt + 2;
        screen[ii * (scrWid + 4) + jj] = '=';
    }

    jj = scrWid + 3;
    for(ii = 0; ii < scrHyt + 4; ii ++)
        screen[ii * (scrWid + 4) + jj] = '\n';

    for(ii = 1; ii < scrHyt + 3; ii ++)
    {
        jj = 1;
        screen[ii * (scrWid + 4) + jj] = 'H';

        jj = scrWid + 2;
        screen[ii * (scrWid + 4) + jj] = 'H';
    }

    ii = 1;
    jj = 1;
    screen[ii * (scrWid + 4) + jj] = '#';

    ii = 1;
    jj = scrWid + 2;
    screen[ii * (scrWid + 4) + jj] = '#';

    ii = scrHyt + 2;
    jj = 1;
    screen[ii * (scrWid + 4) + jj] = '#';

    ii = scrHyt + 2;
    jj = scrWid + 2;
    screen[ii * (scrWid + 4) + jj] = '#';

    ii = scrHyt + 3;
    jj = 3;
    screen[ii * (scrWid + 4) + jj ++] = 'S';
    screen[ii * (scrWid + 4) + jj ++] = 'C';
    screen[ii * (scrWid + 4) + jj ++] = 'O';
    screen[ii * (scrWid + 4) + jj ++] = 'R';
    screen[ii * (scrWid + 4) + jj ++] = 'E';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = '=';
    screen[ii * (scrWid + 4) + jj ++] = ' ';

    itoa((int) score, digits, 10);

    for(kk = 0; (unsigned int) kk < strlen(digits); kk ++)
        screen[ii * (scrWid + 4) + jj ++] = digits[kk];

    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = ':';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = 'F';
    screen[ii * (scrWid + 4) + jj ++] = 'C';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = '=';
    screen[ii * (scrWid + 4) + jj ++] = ' ';

    itoa(nFrames, digits, 10);

    for(kk = 0; (unsigned int) kk < strlen(digits); kk ++)
        screen[ii * (scrWid + 4) + jj ++] = digits[kk];

    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = ':';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = 'F';
    screen[ii * (scrWid + 4) + jj ++] = 'P';
    screen[ii * (scrWid + 4) + jj ++] = 'S';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = '=';
    screen[ii * (scrWid + 4) + jj ++] = ' ';

    itoa(FPS, digits, 10);

    for(kk = 0; (unsigned int) kk < strlen(digits); kk ++)
        screen[ii * (scrWid + 4) + jj ++] = digits[kk];

    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = ':';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = 'n';
    screen[ii * (scrWid + 4) + jj ++] = 'B';
    screen[ii * (scrWid + 4) + jj ++] = 'G';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = '=';
    screen[ii * (scrWid + 4) + jj ++] = ' ';

    itoa(nBadGuys, digits, 10);

    for(kk = 0; (unsigned int) kk < strlen(digits); kk ++)
        screen[ii * (scrWid + 4) + jj ++] = digits[kk];

    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = ':';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = 'n';
    screen[ii * (scrWid + 4) + jj ++] = 'B';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = '=';
    screen[ii * (scrWid + 4) + jj ++] = ' ';

    itoa(nBullets, digits, 10);

    for(kk = 0; (unsigned int) kk < strlen(digits); kk ++)
        screen[ii * (scrWid + 4) + jj ++] = digits[kk];

    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = ':';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = 'm';
    screen[ii * (scrWid + 4) + jj ++] = 'B';
    screen[ii * (scrWid + 4) + jj ++] = 'G';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = '=';
    screen[ii * (scrWid + 4) + jj ++] = ' ';

    itoa(mBadGuys, digits, 10);

    for(kk = 0; (unsigned int) kk < strlen(digits); kk ++)
        screen[ii * (scrWid + 4) + jj ++] = digits[kk];

    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = ':';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = 'm';
    screen[ii * (scrWid + 4) + jj ++] = 'B';
    screen[ii * (scrWid + 4) + jj ++] = ' ';
    screen[ii * (scrWid + 4) + jj ++] = '=';
    screen[ii * (scrWid + 4) + jj ++] = ' ';

    itoa(mBullets, digits, 10);

    for(kk = 0; (unsigned int) kk < strlen(digits); kk ++)
        screen[ii * (scrWid + 4) + jj ++] = digits[kk];

    ii = scrHyt + 3;
    jj = scrWid + 3;
    screen[ii * (scrWid + 4) + jj] = '\0';
}

void printScreen()
{
    system("cls");

    puts(screen);
}
void createSpr(Spr *spr, char c, int x, int y, int u, int v, double l)
{
    spr->_chr = c;

    spr->xPos = x;
    spr->yPos = y;

    spr->xVel = u;
    spr->yVel = v;

    spr->life = l;
    spr->time = nFrames;

    spr->next = NULL;
}

void writeSprs()
{
    Spr *tmp = NULL;

    tmp = baddys->bullets;
    while(tmp != NULL)
    {
        writeSpr(tmp);
        tmp = tmp->next;
    }

    tmp = baddys->badGuys;
    while(tmp != NULL)
    {
        writeSpr(tmp);
        tmp = tmp->next;
    }

    writeSpr(player);
}

void writeSpr(Spr *spr)
{
    ii = spr->yPos + 1;
    jj = spr->xPos + 1;

    screen[ii * (scrWid + 4) + jj] = spr->_chr;
}

void updateSprs()
{
    Spr *tmp = NULL;

    updateSpr(player);

    intConstrain(&(player->xPos), 1, scrWid);
    intConstrain(&(player->yPos), 1, scrHyt);

    tmp = baddys->badGuys;
    while(tmp != NULL)
    {
        updateSpr(tmp);

        expr1 = (tmp->xPos < 0) || (tmp->xPos > scrWid + 1);
        expr2 = (tmp->yPos < 0) || (tmp->yPos > scrHyt + 1);

        if(expr1 || expr2)
        {
            score += 0.2;
            tmp = destroyBadGuy(tmp);
        }
        else
            tmp = tmp->next;
    }

    tmp = baddys->bullets;
    while(tmp != NULL)
    {
        updateSpr(tmp);

        expr1 = (tmp->xPos < 0) || (tmp->xPos > scrWid + 1);
        expr2 = (tmp->yPos < 0) || (tmp->yPos > scrHyt + 1);

        if(expr1 || expr2)
        {
            score += 0.05;
            tmp = destroyBullet(tmp);
        }
        else
            tmp = tmp->next;
    }

    checkCollisions();
}

void updateSpr(Spr *spr)
{
    spr->xPos += spr->xVel;
    spr->yPos += spr->yVel;
}

Spr *destroyBadGuy(Spr *spr)
{
    Spr *tmp = baddys->badGuys;

    nBadGuys --;

    if(tmp == spr)
        baddys->badGuys = tmp->next;
    else
        while(tmp->next != spr)
            tmp = tmp->next;

    tmp->next = spr->next;

    free(spr);

    return(tmp->next);
}

Spr *destroyBullet(Spr *spr)
{
    Spr *tmp = baddys->bullets;

    nBullets --;

    if(tmp == spr)
        baddys->bullets = tmp->next;
    else
        while(tmp->next != spr)
            tmp = tmp->next;

    tmp->next = spr->next;

    free(spr);

    return(tmp->next);
}

void intConstrain(int *input, int lower, int upper)
{
    if(*input < lower)
        *input = lower;

    if(*input > upper)
        *input = upper;
}

void evalInput()
{
    if(_kbhit())
        inpChar = _getch();

    if((inpChar == 'w') || (inpChar == 'W') || (inpChar == '8'))
    {
        player->xVel =  0;
        player->yVel = -1;
    }
    else if((inpChar == 'a') || (inpChar == 'A') || (inpChar == '4'))
    {
        player->xVel = -1;
        player->yVel =  0;
    }
    else if((inpChar == 'x') || (inpChar == 'X') || (inpChar == '2'))
    {
        player->xVel =  0;
        player->yVel =  1;
    }
    else if((inpChar == 'd') || (inpChar == 'D') || (inpChar == '6'))
    {
        player->xVel =  1;
        player->yVel =  0;
    }
    else if((inpChar == 'q') || (inpChar == 'Q') || (inpChar == '7'))
    {
        player->xVel = -1;
        player->yVel = -1;
    }
    else if((inpChar == 'e') || (inpChar == 'E') || (inpChar == '9'))
    {
        player->xVel =  1;
        player->yVel = -1;
    }
    else if((inpChar == 'z') || (inpChar == 'Z') || (inpChar == '1'))
    {
        player->xVel = -1;
        player->yVel =  1;
    }
    else if((inpChar == 'c') || (inpChar == 'C') || (inpChar == '3'))
    {
        player->xVel =  1;
        player->yVel =  1;
    }
    else if((inpChar == 's') || (inpChar == 'S') || (inpChar == '5'))
    {
        player->xVel =  0;
        player->yVel =  0;
    }
    else if((inpChar == 'k') || (inpChar == 'K'))
    {
        exitCond = true;
    }
}

void createBullet(int xPos, int yPos, int xVel, int yVel)
{
    Spr *tmp = baddys->bullets;

    if(tmp != NULL)
    {
        while(tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = (Spr *) calloc(1, sizeof(Spr));
        tmp = tmp->next;
    }
    else
    {
        baddys->bullets = (Spr *) calloc(1, sizeof(Spr));
        tmp = baddys->bullets;
    }

    createSpr(tmp, '.', xPos, yPos, xVel, yVel, -1);
}

void createBadGuy()
{
    int  xPos = 0;
    int  yPos = 0;

    int  yVel = 0;
    int  xVel = 0;

    char _chr = ' ';

    int r = rand() % 3;

    Spr *tmp = baddys->badGuys;

    if(tmp != NULL)
    {
        while(tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = (Spr *) calloc(1, sizeof(Spr));
        tmp = tmp->next;
    }
    else
    {
        baddys->badGuys = (Spr *) calloc(1, sizeof(Spr));
        tmp = baddys->badGuys;
    }

    if(r < 0)
    {
        exit(-1);
    }
    else if(r < 1)
    {
        do
        {
            xPos = rand() % (scrWid - 1) + 1;
            yPos = rand() % (scrHyt - 1) + 1;

            expr1 = abs(xPos - player->xPos) < 1;
            expr2 = abs(yPos - player->yPos) < 1;
        }
        while(expr1 || expr2);

        xVel = 0;
        yVel = 0;

        _chr = '*';

        createSpr(tmp, _chr, xPos, yPos, xVel, yVel, 0.25);
    }
    else if(r < 2)
    {
        xPos = rand() % 2 * (scrWid + 1);
        yPos = rand() % (scrHyt - 1) + 1;

        yVel = 0;
        if(xPos < scrWid / 2)
            xVel =  1;
        else
            xVel = -1;

        _chr = '-';

        createSpr(tmp, _chr, xPos, yPos, xVel, yVel, -1);
    }
    else if(r < 3)
    {
        xPos = rand() % (scrWid - 1) + 1;
        yPos = rand() % 2 * (scrHyt + 1);

        xVel = 0;
        if(yPos < scrHyt / 2)
            yVel =  1;
        else
            yVel = -1;

        _chr = '|';

        createSpr(tmp, _chr, xPos, yPos, xVel, yVel, -1);
    }

    nBadGuys ++;
}

void checkCollisions()
{
    Spr  *tmp1    = NULL;

    tmp1 = baddys->badGuys;
    while(tmp1 != NULL)
    {
        expr1 = player->xPos == tmp1->xPos;
        expr2 = player->yPos == tmp1->yPos;

        if(expr1 && expr2)
            exitCond = true;

        tmp1 = tmp1->next;
    }

    tmp1 = baddys->bullets;
    while(tmp1 != NULL)
    {
        expr1 = player->xPos == tmp1->xPos;
        expr2 = player->yPos == tmp1->yPos;

        if(expr1 && expr2)
            exitCond = true;

        tmp1 = tmp1->next;
    }

    tmp1 = baddys->badGuys;
    while(tmp1 != NULL)
    {
        expr1 = tmp1->life >= 0;
        expr2 = (nFrames - tmp1->time) / (double) FPS >= tmp1->life;

        if(expr1 && expr2)
        {
            createBullet(tmp1->xPos, tmp1->yPos,  1,  0);
            createBullet(tmp1->xPos, tmp1->yPos,  1, -1);
            createBullet(tmp1->xPos, tmp1->yPos,  0, -1);
            createBullet(tmp1->xPos, tmp1->yPos, -1, -1);
            createBullet(tmp1->xPos, tmp1->yPos, -1,  0);
            createBullet(tmp1->xPos, tmp1->yPos, -1,  1);
            createBullet(tmp1->xPos, tmp1->yPos,  0,  1);
            createBullet(tmp1->xPos, tmp1->yPos,  1,  1);

            nBullets += 8;

            tmp1 = destroyBadGuy(tmp1);
        }

        tmp1 = tmp1->next;
    }
}
