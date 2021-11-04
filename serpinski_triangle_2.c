#include <GL/glut.h>    // glut*
#include <math.h>       // sqrt
#include <stdio.h>      // printf
#include <string.h>     // strcmp
#include <time.h>       // time

// Минимальный размер треугольника
const float lim = 0.001;
// Шаг масштабирования за 1 кадр (в промилле)
const int mstep = 5;
// Масштаб самоподобия фрактала
const int mf = 4;  // Вообще 2, но здесь 4
// Координата x правого угла треугольника
const float dx = sqrt(3) / 2;

// Масштаб
float m = 1;
// Направление движения
int up = 0;
// Остановить движение
int stop = 0;
// Угол для движения (0, 1, 2)
int angle = 0;
// Случайный выбор угла для движения
int rnd_a = 0;

void change_angle()
{
    if(rnd_a)
    {
        angle = rand() % 9;
    }
}

void reshape(int w, int h)
{
    int ox, oy, th, tw;
    // Используем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    // Reset матрицы
    glLoadIdentity();
    // Вычислить ширину треугольника при высоте h
    tw = round((float)h / dx);
    // Если ширина окна больше ширины треугольника
    if(w >= tw)
    {
        ox = (w - tw) / 2;
        oy = 0;
        th = h;
    }
    else
    {
        // Вычислить высоту треугольника при ширине w
        th = round((float)w * dx);
        ox = 0;
        oy = (h - th) / 2;
        tw = w;
    }
    // Определить окно просмотра
    glViewport(ox, oy, tw, th);
    // Установить 2D проекцию (по границам треугольника)
    gluOrtho2D(-dx, dx, -0.5, 1);
    // Вернуться к модели
    glMatrixMode(GL_MODELVIEW);
}

void triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    const float x4 = (x1 + x2) / 2;
    const float y4 = (y1 + y2) / 2;
    const float x5 = (x1 + x3) / 2;
    const float y5 = (y1 + y3) / 2;
    const float x6 = (x2 + x3) / 2;
    const float y6 = (y2 + y3) / 2;
    glVertex2f(x4, y4);
    glVertex2f(x5, y5);
    glVertex2f(x6, y6);
    if(x6 - x4 >= lim)
    {
        triangle(x4, y4, x2, y2, x6, y6);
        triangle(x1, y1, x4, y4, x5, y5);
        triangle(x5, y5, x6, y6, x3, y3);
    }
}

void display()
{
    // Очистка
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    // Масштабирование
    glScalef(m, m, 1);
    // Сдвиг
    const float f = 1.0f / m - 1;
    switch(angle)
    {
        default:
        case 0: glTranslatef(0, f, 0);
                break;
        case 1: glTranslatef(dx * f, -0.5 * f, 0);
                break;
        case 2: glTranslatef(-dx * f, -0.5 * f, 0);
                break;
        case 3: glTranslatef(-dx / 3 * f, 0.5 * f, 0);
                break;
        case 4: glTranslatef(dx / 3 * f, 0.5 * f, 0);
                break;
        case 5: glTranslatef(dx / 1.5 * f, 0, 0);
                break;
        case 6: glTranslatef(dx / 3 * f, -0.5 * f, 0);
                break;
        case 7: glTranslatef(-dx / 3 * f, -0.5 * f, 0);
                break;
        case 8: glTranslatef(-dx / 1.5 * f, 0, 0);
                break;
    }
    // Для сглаживания
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    // Сглаживание полигонов  
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Рисование треугольников
    glBegin(GL_TRIANGLES);
        glColor3f(1, 165.0f / 255, 0);
        glVertex2f(-dx, -0.5);
        glVertex2f(0, 1);
        glVertex2f(dx, -0.5);
        glColor3f(0, 0, 0);
        triangle(-dx, -0.5, 0, 1, dx, -0.5);
    glEnd();
    // Управление масштабированием
    if(!stop)
    {
        if(up)
            m *= 1000.0f / (1000 + mstep);
        else
            m *= (1000.0f + mstep) / 1000;
        if(m < 1)
        {
            m *= mf;
            change_angle();
        }
        else if(m > mf)
        {
            m /= mf;
            change_angle();
        }
    }
    // Перерисовка
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:        // Esc
            exit(0);
        case ' ':       // Space
            stop = !stop;
            break;
        case 127:       // Delete
            angle = 3;
            break;
        case 8:         // Backspace
            rnd_a = !rnd_a;
            break;
    }
}

void special(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            angle = 0;
            break;
        case GLUT_KEY_RIGHT:
            angle = 1;
            break;
        case GLUT_KEY_LEFT:
            angle = 2;
            break;
        case GLUT_KEY_DOWN:
            up = !up;
            break;
        case GLUT_KEY_INSERT:
            angle = 4;
            break;
        case GLUT_KEY_HOME:
            angle = 5;
            break;
        case GLUT_KEY_END:
            angle = 8;
            break;
        case GLUT_KEY_PAGE_UP:
            angle = 6;
            break;
        case GLUT_KEY_PAGE_DOWN:
            angle = 7;
    }
}

int main (int argc, char * argv[])
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    if(2 == argc && 0 == strcmp(argv[1], "-w"))
    {
        glutInitWindowSize(1024, 887);
        glutCreateWindow("Треугольник Серпинского");
    }
    else
    {
        glutGameModeString("1920x1080:16@60");
        if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
            glutEnterGameMode();
        else
        {
            printf("Режим не поддерживается\n");
            exit(1);
        }
        glutSetCursor(GLUT_CURSOR_NONE);
    }
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}
