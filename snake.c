#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <sys/time.h>

#define SCREEN_LINE     25
#define SCREEN_ROW         80
#define GAME_HEIGHT        (SCREEN_LINE-1)
#define GAME_WIDTH        (SCREEN_ROW-1)
#define CHAR_SNAKE        '*'
#define CHAR_FOOD         'O'
#define CHAR_BLANK         ' '

typedef struct snake_struct {
    int x, y;
    struct snake_struct *next;
    struct snake_struct *prev;
} snake_t, *snake_tp;

typedef struct food_struct {
    int x, y;
} food_t, *food_tp;

int             dir_x, dir_y;
snake_tp         head, tail;
food_t             food;

int set_ticker(int n_msec)
{
    struct itimerval timeset;
    long n_sec, n_usec;

    n_sec = n_msec / 1000;
    n_usec = (n_msec % 1000) * 1000L;

    timeset.it_interval.tv_sec = n_sec;
    timeset.it_interval.tv_usec = n_usec;

    timeset.it_value.tv_sec = n_sec;
    timeset.it_value.tv_usec = n_usec;

    return setitimer(ITIMER_REAL, &timeset, NULL);
}

void sig_alrm(int singo)
{
    set_ticker(500);
    snake_move();
}


int main()
{
    init();
    while(1) {
        key_ctl();
    }
    game_over();
    return 0;
}


void init()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    srand(time(0));

    dir_x = 1;
    dir_y = 0;

    head = (snake_tp)malloc(sizeof(snake_t));
    head->x = rand() % SCREEN_ROW;
    head->y = rand() % SCREEN_LINE;
    head->next = (snake_tp)malloc(sizeof(snake_t));
    tail = head->next;
    tail->prev = head;
    tail->x = head->x - dir_x;
    tail->y = head->y - dir_y;

    do {
        food.x = rand() % SCREEN_ROW;
        food.y = rand() % SCREEN_LINE;
        move(food.y, food.x);
    } while ((char)inch() == CHAR_SNAKE);

    move(head->y, head->x);
    addch(CHAR_SNAKE);
    move(food.y, food.x);
    addch(CHAR_FOOD);
    refresh();

    signal(SIGALRM, sig_alrm);
    set_ticker(500);
}


 void key_ctl()
    {
        int ch = getch();
        switch (ch) {
            case 'W':
            case 'w':
                if (dir_x == 0)
                    break;
                dir_x = 0;
                dir_y = -1;
                break;
            case 'S':
            case 's':
                if (dir_x == 0)
                    break;
                dir_x = 0;
                dir_y = 1;
                break;
            case 'A':
            case 'a':
                if (dir_y == 0)
                    break;
                dir_y = 0;
                dir_x = -1;
                break;
            case 'D':
            case 'd':
                if (dir_y == 0)
                    break;
                dir_y = 0;
                dir_x = 1;
                break;
            case ' ':
                set_ticker(0);
                do {
                    ch = getch();
                } while(ch != ' ');
                set_ticker(500);
                break;
            case 'Q':
            case 'q':
                game_over();
                break;
            default:break;
        }
    }


//### 7. 定时移动贪吃蛇
//这个函数是被我们的信号处理函数定时调用的，完成了移动贪吃蛇并判断是否吃到了食物或咬到自己，以及进行相关数据结构的更新。

void snake_move()
{
    snake_tp p, tmp;

    for (p = tail; p != head; p = p->prev) {
        p->x = p->prev->x;
        p->y = p->prev->y;
    }
    p->x += dir_x;
    p->y += dir_y;

    if (head->x > GAME_WIDTH)
        head->x = 0;
    if (head->x < 0)
        head->x = GAME_WIDTH;
    if (head->y > GAME_HEIGHT)
        head->y = 0;
    if (head->y < 0)
        head->y = GAME_HEIGHT;

    move(head->y, head->x);
    if ((char)inch() == CHAR_SNAKE) { //eat self
        game_over();
    }
    if ((char)inch() == CHAR_FOOD) { //eat food
        move(head->y, head->x);
        addch(CHAR_SNAKE);
        refresh();

        tmp = (snake_tp)malloc(sizeof(snake_t));
        tmp->x = head->x + dir_x;
        tmp->y = head->y + dir_y;
        tmp->next = head;
        head->prev = tmp;
        head = tmp;

        do {
            food.x = rand() % SCREEN_ROW;
            food.y = rand() % SCREEN_LINE;
            move(food.y, food.x);
        } while (((char)inch()) == CHAR_SNAKE);
        move(food.y, food.x);
        addch(CHAR_FOOD);
        refresh();
    }

    move(head->y, head->x);
    addch(CHAR_SNAKE);
    refresh();
    move(tail->y, tail->x);
    addch(CHAR_BLANK);
    refresh();
}
//8. 释放资源
//在游戏结束时，我们要释放掉自己占用的资源-宝贵的内存。虽然我们的程序很小，而且系统会帮我们做这些，但是养成好的习惯总是会受益的。

void free_snake()
{
    snake_tp tmp;
    while (head) {
        tmp = head;
        head = tmp->next;
        free(tmp);
    }
}

int game_over()
{

    sleep(1);
    endwin();
    free_snake();
    exit(0);
//end
}
