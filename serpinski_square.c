#include <GL/glut.h>    // glut*
#include <stdio.h>      // printf
#include <string.h>     // strcmp
#include <time.h>       // time

// Минимальный размер квадрата
const float lim = 0.0005;
// Шаг масштабирования за 1 кадр (в промилле)
const int mstep = 5;
// Масштаб самоподобия фрактала
const int mf = 3;

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
        angle = rand() % 8;
    }
}

void reshape(int w, int h)
{
    int ox, oy, s;
    // Используем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    // Reset матрицы
    glLoadIdentity();
    // Если ширина окна больше высоты
    if(w >= h)
    {
        s = h;  // сторона квадрата
        ox = (w - s) / 2;
        oy = 0;
    }
    else
    {
        s = w;
        ox = 0;
        oy = (h - s) / 2;
    }
    // Определить окно просмотра
    glViewport(ox, oy, s, s);
    // Установить 2D проекцию (по границам квадрата)
    gluOrtho2D(-0.5, 0.5, -0.5, 0.5);
    // Вернуться к модели
    glMatrixMode(GL_MODELVIEW);
}

void square(float x1, float y1, float x2, float y2)
{
    const float len = (x2 - x1) / 3;
    if(len >= lim)
    {
        const float x3 = x1 + len;
        const float y3 = y1 - len;
        const float x4 = x2 - len;
        const float y4 = y2 + len;
        glVertex2f(x3, y3);
        glVertex2f(x4, y3);
        glVertex2f(x4, y4);
        glVertex2f(x3, y4);
        square(x1, y1, x3, y3);
        square(x3, y1, x4, y3);
        square(x4, y1, x2, y3);
        square(x4, y3, x2, y4);
        square(x4, y4, x2, y2);
        square(x3, y4, x4, y2);
        square(x1, y4, x3, y2);
        square(x1, y3, x3, y4);
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
        case 0: glTranslatef(-0.5 * f, 0.5 * f, 0);
                break;
        case 1: glTranslatef(0, 0.5 * f, 0);
                break;
        case 2: glTranslatef(0.5 * f, 0.5 * f, 0);
                break;
        case 3: glTranslatef(0.5 * f, 0, 0);
                break;
        case 4: glTranslatef(0.5 * f, -0.5 * f, 0);
                break;
        case 5: glTranslatef(0, -0.5 * f, 0);
                break;
        case 6: glTranslatef(-0.5 * f, -0.5 * f, 0);
                break;
        case 7: glTranslatef(-0.5 * f, 0, 0);
                break;
    }
    // Для сглаживания
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    // Сглаживание полигонов  
    //glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Рисование треугольников
    glBegin(GL_QUADS);
        glColor3f(0, 128.0f / 255, 0);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(-0.5, -0.5);
        glColor3f(0, 0, 0);
        square(-0.5, 0.5, 0.5, -0.5);
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
        case 8:         // Backspace
            rnd_a = !rnd_a;
            break;
        case '7':
            angle = 0;
            break;
        case '8':
            angle = 1;
            break;
        case '9':
            angle = 2;
            break;
        case '6':
            angle = 3;
            break;
        case '3':
            angle = 4;
            break;
        case '2':
            angle = 5;
            break;
        case '1':
            angle = 6;
            break;
        case '4':
            angle = 7;
            break;
        case '5':
            up = !up;
            break;
    }
}

int main (int argc, char * argv[])
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    if(2 == argc && 0 == strcmp(argv[1], "-w"))
    {
        glutInitWindowSize(729, 729);
        glutCreateWindow("Квадрат Серпинского");
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
    glutMainLoop();
    return 0;
}
