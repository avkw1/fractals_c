#include <GL/glut.h>    //подключаем заголовочный файл glut.h
#include <math.h>       // sqrt
#include <stdio.h>      // printf

// Масштаб
float m = 1;
// Множитель масштаба
float mm = 1.005;
// Остановить анимацию
int stop = 0;
// Угол поворота
float a = 0;

void reshape(int w, int h)
{
    // предупредим деление на ноль
    // если окно сильно перетянуто будет
    if(h == 0)
        h = 1;
    const float ratio = ((float)w) / h;

    // используем матрицу проекции
    glMatrixMode(GL_PROJECTION);

    // Reset матрицы
    glLoadIdentity();

    // определяем окно просмотра
    glViewport(0, 0, w, h);

    // установить корректную перспективу.
    //gluOrtho2D(-1, 1, 1, -1);
    gluPerspective(45, ratio, 2, 3);

    // вернуться к модели
    glMatrixMode(GL_MODELVIEW);
}

void triangle(float x1, float y1, float x2, float y2, float x3, float y3, float lim)
{
    if (x3 - x1 < lim)
        return;
    const float x4 = (x1 + x2) / 2;
    const float y4 = (y1 + y2) / 2;
    const float x5 = (x1 + x3) / 2;
    const float y5 = (y1 + y3) / 2;
    const float x6 = (x2 + x3) / 2;
    const float y6 = (y2 + y3) / 2;
    glVertex3f(x4, y4, -2.5);
    glVertex3f(x5, y5, -2.5);
    glVertex3f(x6, y6, -2.5);
    triangle(x1, y1, x4, y4, x5, y5, lim);
    triangle(x4, y4, x2, y2, x6, y6, lim);
    triangle(x5, y5, x6, y6, x3, y3, lim);
}

void display()
{
    float x = sqrt(3) / 2;
    
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    glScalef(m, m, 1);
    glRotatef(a, 0, 0, 1);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Сглаживание полигонов  
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(-x, -0.5, -2.5);
        glColor3f(0, 1, 0);
        glVertex3f(0, 1, -2.5);
        glColor3f(0, 0, 1);
        glVertex3f(x, -0.5, -2.5);
        glColor3f(0, 0, 0);
        triangle(-x, -0.5, 0, 1, x, -0.5, 0.003);
    glEnd();
    
    if(!stop)
    {
        if(m < 0.5)
            mm = 1.005;
        else if (m > 2)
            mm = 1000.0f / 1005;
        m *= mm;
        a += 0.2;
    }
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
    else if(key == 't')
        stop = !stop;
}

int main (int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
    //glutInitWindowSize(600, 600);
    //glutCreateWindow("Треугольник Серпинского");
    glutGameModeString("1920x1080:16@60");
    if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
        glutEnterGameMode();
    else
    {
        printf("Режим не поддерживается\n");
        exit(1);
    }
    
    glutSetCursor(GLUT_CURSOR_NONE);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    
    return 0;
}
