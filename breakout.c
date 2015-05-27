//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
   srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
    

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // keep playing until game over
    restart:
    waitForClick();
    double velocityX = drand48();
    double velocityY = 0.2;
    while (lives > 0 && points <= 50)
        GEvent paddlemov = getNextEvent(MOUSE_EVENT);
        if (paddlemov != NULL)
        {
            if (getEventType(paddlemov) == MOUSE_MOVED)
            {
                
                double x = getX(paddlemov) - getWidth(paddle) / 2;
                setLocation(paddle, x, 500);
            }
        }
        
        
        move(ball, velocityX, velocityY);
        if ( getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityX = - velocityX;
        }
        else if(getX(ball) <= 0)
        {
            velocityX = -velocityX;
        }
        else if ( getY(ball) + getHeight(ball) >= getHeight(window) )
        {
            setLocation(ball, 190, 290);
            lives--;
            goto restart;
        }
        else if(getY(ball) <= 0)
        {
            velocityY = -velocityY;
        }
        GObject object = detectCollision(window, ball);
        if (object != NULL)
        {
        if (strcmp(getType(object), "GRect") == 0)
        {
            velocityY = -velocityY;
            if(object != paddle)
            {
            removeGWindow(window, object);
            points++;
            updateScoreboard(window, label, points);
            }
       }
       }
        pause(1);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */

void initBricks(GWindow window)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            GRect block = newGRect(j * 40, i * 20, 35, 5);
            setFilled(block, true);
            switch (i)
            {
                case 1:
                setColor(block, "RED");
                break;
                case 2:
                setColor(block, "YELLOW");
                break;
                case 3:
                setColor(block, "GREEN");
                break;
                case 4:
                setColor(block, "BLUE");
                break;
                default:
                setColor(block, "00FFFF");
            }
            add(window, block);
            
        } 
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(190, 290, 20,20);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{   
    GRect paddle = newGRect(150, 500, 100, 10);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window, paddle);
    return paddle;


}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
