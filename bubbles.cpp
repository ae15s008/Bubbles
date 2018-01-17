#include <math.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <graphics.h>

double    bound    = 10;
double    maxVel   = 3;
double    minRad   = 40;
double    maxRad   = 120;
double    buffer   = 0;
const int FPS      = 60;
int       nSeconds = -1;
const int nCircles = 10;

typedef struct strCircle
{
    double x1;
    double x2;

    double r;

    double u1;
    double u2;

    double m;

    int    c;
}Circle;

bool ifCollide(Circle circle1, Circle circle2)
{
    double x11 = circle1.x1 + circle1.u1, x12 = circle1.x2 + circle1.u2;
    double x21 = circle2.x1 + circle2.u1, x22 = circle2.x2 + circle2.u2;

    double r1  = circle1.r , r2  = circle2.r;

    if(sqrt(pow(x11 - x21, 2) + pow(x12 - x22, 2)) <= r1 + r2 + buffer)
        return(true);
    else
        return(false);
}

void setupCircles(struct strCircle *circle_)
{
    int  ii   = 0;
    int  jj   = 0;
    int  temp = 0;
    bool flag = true;

    for(ii = 0; ii < nCircles; ii ++)
    {
        flag = true;
        while(flag)
        {
            temp = bound + circle_[ii].r;
            circle_[ii].x1 = rand() % (int) (getmaxx() - temp) + temp;
            circle_[ii].x2 = rand() % (int) (getmaxy() - temp) + temp;

            circle_[ii].r  = rand() % (int) (maxRad - minRad + 1) + minRad;

            if(ii == 0)
                break;

            for(jj = 0; jj < ii; jj ++)
            {
                flag = ifCollide(circle_[ii], circle_[jj]);

                if(flag)
                    break;
            }
        }
        circle_[ii].u1 = rand() % (int) (2 * maxVel + 1) - maxVel;
        circle_[ii].u2 = rand() % (int) (2 * maxVel + 1) - maxVel;

        circle_[ii].m  = M_PI * circle_[ii].r * circle_[ii].r;

        circle_[ii].c  = rand() % getmaxcolor();
    }
}

void drawCircle(Circle *circle_)
{
    int val = (int) circle_->r + 5;

    // setcolor(circle_->c);
    circle(circle_->x1, circle_->x2, circle_->r);

    circle_->x1 += circle_->u1;
    circle_->x2 += circle_->u2;

    if((circle_->x1 >= getmaxx() - val) && (circle_->u1 >= 0))
        circle_->u1 *= -1;


    if((circle_->x1 <= val) && (circle_->u1 <= 0))
        circle_->u1 *= -1;


    if((circle_->x2 >= getmaxy() - val) && (circle_->u2 >= 0))
        circle_->u2 *= -1;

    if((circle_->x2 <= val) && (circle_->u2 <= 0))
        circle_->u2 *= -1;
}

void checkCollisions(Circle *circle_)
{
    int ii, jj;

    double m1, m2;
    double x11, x12, x21, x22;

    double u11, u12, u21, u22;
    double up1, uP1, up2, uP2;

    double vp1, vP1, vp2, vP2;
    double v11, v12, v21, v22;

    double norm, denom;

    double chi[2], eta[2];

    for(ii = 0; ii < nCircles - 1; ii ++)
        for(jj = ii + 1; jj < nCircles; jj ++)
            if(ifCollide(circle_[ii], circle_[jj]))
            {
                m1  = circle_[ii].m;
                m2  = circle_[jj].m;

                x11 = circle_[ii].x1;
                x12 = circle_[ii].x2;

                x21 = circle_[jj].x1;
                x22 = circle_[jj].x2;

                u11 = circle_[ii].u1;
                u12 = circle_[ii].u2;

                u21 = circle_[jj].u1;
                u22 = circle_[jj].u2;

                norm = sqrt(pow(x21 - x11, 2) + pow(x22 - x12, 2));

                chi[0] = (x21 - x11) / norm;
                chi[1] = (x22 - x12) / norm;

                denom = pow(chi[0], 2) + pow(chi[1], 2);

                eta[0] = -chi[1] / denom;
                eta[1] =  chi[0] / denom;

                up1 = u11 * chi[0] + u12 * chi[1];
                uP1 = u11 * eta[0] + u12 * eta[1];

                up2 = u21 * chi[0] + u22 * chi[1];
                uP2 = u21 * eta[0] + u22 * eta[1];

                vp1 = (up1 * (m1 - m2) + 2 * m2 * up2) / (m1 + m2);
                vP1 = uP1;

                vp2 = (up2 * (m2 - m1) + 2 * m1 * up1) / (m1 + m2);
                vP2 = uP2;

                v11 = vp1 * chi[0] + vP1 * eta[0];
                v12 = vp1 * chi[1] + vP1 * eta[1];

                v21 = vp2 * chi[0] + vP2 * eta[0];
                v22 = vp2 * chi[1] + vP2 * eta[1];

                circle_[ii].u1 = v11;
                circle_[ii].u2 = v12;

                circle_[jj].u1 = v21;
                circle_[jj].u2 = v22;
            }
}

int main()
{
    int         ii         = 0;

    const DWORD DELAY      = 1000 / (double) FPS;

    int         counter = 0;
    const int   maxFrames  = nSeconds * FPS;
    double      percent    = 0.00;

    char        text[200];

    FILE *ptrFile;
    ptrFile = fopen("Vars.dat", "w");

    double xEnergy = 0;
    double yEnergy = 0;

    srand(time(NULL));

    Circle *circles = (Circle *) calloc(nCircles, sizeof(Circle));

    initwindow(1600, 800);

    setupCircles(circles);

    counter = 0;
    while(((counter < maxFrames) || (maxFrames <= 0)) && !_kbhit())
    {
        cleardevice();

        percent = 100 * counter / (double) maxFrames;

        if(maxFrames > 0)
            sprintf(text, "%d / %d (%06.2lf%%)", counter, maxFrames, percent);
        else
            sprintf(text, "%d", counter);

        outtextxy(10, 10, text);

        if(maxFrames > 0)
        {
            xEnergy = 0;
            yEnergy = 0;
            for(ii = 0; ii < nCircles; ii ++)
            {
                xEnergy += circles[ii].m * circles[ii].u1 * circles[ii].u1;
                yEnergy += circles[ii].m * circles[ii].u2 * circles[ii].u2;
            }

            fprintf(ptrFile, "\n%lf %lf", xEnergy, yEnergy);
        }

        for(ii = 0; ii < nCircles; ii ++)
            drawCircle(&circles[ii]);

        checkCollisions(circles);

        Sleep(DELAY);

        counter ++;
    }

    closegraph();

    if(maxFrames > 0)
    {
        system("plotter.gp");
        system("\"Energy Budget.png\"");
    }

    return(0);
}
