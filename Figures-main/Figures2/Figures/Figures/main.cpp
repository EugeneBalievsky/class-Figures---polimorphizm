#include "include/graphics.h"

#include <iostream>
#include <functional>

/*
Задание
Определить абстрактный класс "Фигура". У нее будет цвет обводки, точка центра, угол наклона.
Чисто виртуальные методы:
1. "рисовать"
2. "передвинуть фигуру на смещение (dx, dy)


Определить следующие классы-наследники:
1. Окружность
2. Прямоугольник
3. Треугольник

*/


namespace Figures {

    class Figure {
    protected:
        double x_c, y_c;
        int color;
        double alpha;

    public:
        Figure(double x_c, double y_c, int color, double alpha = 0) : x_c(x_c), y_c(y_c), color(color), alpha(alpha) {};
        virtual void draw() = 0;
        virtual void move(double dx, double dy) = 0;
        virtual void rotate(double alpha) {}
    };



    //В конструкторе передается точка центра, длина, ширина
    class Rectangle : public Figure {
    private:
        std::pair<double, double> points[4];

    public:
        Rectangle(double x, double y, int color, double width, double height) :
            Figure(x, y, color) {
            int sign_x[4] = { -1, 1, 1, -1 };
            int sign_y[4] = { -1, -1, 1, 1 };

            for (int i = 0; i < 4; ++i) {
                points[i].first = x_c + sign_x[i] * width / 2.0;
                points[i].second = y_c + sign_y[i] * height / 2.0;
            }

            /*
            points[0].first = x_c - width / 2.0;
            points[0].second = y_c - height / 2.0;

            points[1].first = x_c + width / 2.0;
            points[1].second = y_c - height / 2.0;

            points[2].first = x_c + width / 2.0;
            points[2].second = y_c + height / 2.0;

            points[3].first = x_c - width / 2.0;
            points[3].second = y_c + height / 2.0;
            */
        }


        void draw() override {
            setcolor(color);
            for (int i = 0; i < 4; ++i) {
                line(points[i].first, points[i].second, points[(i + 1) % 4].first,
                    points[(i + 1) % 4].second);
            }
        }

        void move(double dx, double dy) override {
            x_c += dx;
            y_c += dy;
            for (int i = 0; i < 4; ++i) {
                points[i].first += dx;
                points[i].second += dy;
            }

        }

        void rotate(double alpha) override {
            for (int i = 0; i < 4; ++i) {
                double oldX = points[i].first - x_c;
                double oldY = points[i].second - y_c;
                points[i].first = oldX * cos(alpha) - oldY * sin(alpha) + x_c;
                points[i].second = oldX * sin(alpha) + oldY * cos(alpha) + y_c;
            }
        }
    };

    //В конструкторе передается точка центра, радиус
    class Circle : public Figure {
    private:
        int rad;
    public:
        Circle(double x_c, double y_c, int color, int r) :
            Figure(x_c, y_c, color, 0), rad(r) {}
        void draw() override {
            setcolor(color);
            circle(x_c, y_c, rad);
        }
        void move(double dx, double dy) override {
            this->x_c += dx;
            this->y_c += dy;
        }
    };


    //В конструктор передается точка центра и 3 вершины
    class Triangle : public Figure {
    private:
        std::pair<double, double> points[3];
        using t_point = std::pair<double, double>;

    public:
        Triangle(t_point first, t_point second, t_point third, int color) :
            Figure((first.first + second.first + third.first) / 3.0,
                (first.second + second.second + third.second) / 3.0, color, 0) {
            points[0] = first;
            points[1] = second;
            points[2] = third;
        }
        void draw() override {
            setcolor(color);
            for (int i = 0; i < 3; ++i) {
                line(points[i].first, points[i].second,
                    points[(i + 1) % 3].first, points[(i + 1) % 3].second);
            }
        }
        void move(double dx, double dy) override {
            x_c += dx;
            y_c += dy;
            for (int i = 0; i < 3; ++i) {
                points[i].first += dx;
                points[i].second += dy;
            }
        }

        void rotate(double alpha) override {
            for (int i = 0; i < 3; ++i) {
                double oldX = points[i].first - x_c;
                double oldY = points[i].second - y_c;
                points[i].first = oldX * cos(alpha) - oldY * sin(alpha) + x_c;
                points[i].second = oldX * sin(alpha) + oldY * cos(alpha) + y_c;
            }
        }
    };

    class Button {
    protected:
        using act_type = std::function<void()>;

        act_type action;                            //Действие при нажатии
        std::pair<int, int> point;                  //Левая верхняя точка
        int width;
        int height;


    public:
        Button(std::pair<int, int> point, int w, int h, act_type action) : 
            point(point), width(w), height(h), action(action) {}

        bool isPressed() {     //Нажата ли кнопка
            int x = mousex();
            int y = mousey();
            return (mousebuttons() == 1 && x >= point.first && x <= point.first + width
                && y >= point.second && y <= point.second + height);
            
        }
     
        void handler()  {
            if (isPressed()) {
                action();
            }
        }
        void draw_btn() {
            setcolor(WHITE);
            bar(point.first, point.second, point.first + width, point.second + height);
        }
            
    };
    
};




int main() {
    int a = 0;

    int WIDTH = 800;
    int HEIGHT = 800;

    initwindow(WIDTH, HEIGHT, "Анимация", 0, 0, true);

    Figures::Figure* fig = nullptr;

    Figures::Button btnTriangle({ 0, HEIGHT - 50 }, 100, 50, [&]() {
        if (fig)
            delete fig;
        fig = new Figures::Triangle({ 300, 300 }, { 500, 300 }, { 400, 500 }, WHITE);
        });

    Figures::Button btnCircle({ 110, HEIGHT - 50 }, 100, 50, [&]() {
        if (fig)
            delete fig;
        fig = new Figures::Circle(WIDTH/2, HEIGHT/2, WHITE, 100);
        });
    Figures::Button btnRect({ 220, HEIGHT - 50 }, 100, 50, [&]() {
        if (fig)
            delete fig;
        fig = new Figures::Rectangle(WIDTH / 2, HEIGHT / 2, WHITE, 150, 100);
        });

    Figures::Button btnTurnRight({ WIDTH - 50, HEIGHT - 800 }, 50, 100, [&]() {
        fig->rotate(3.14 / 90);
        });

    Figures::Button btnTurnLeft({ WIDTH - 50, HEIGHT - 690 }, 50, 100, [&]() {
        fig->rotate(-3.14 / 90);
        });

   

    while (true) {
        setbkcolor(BLACK); // цвет фона
        clearviewport(); // закраска текущей страницы цветом фона

        
        btnTriangle.handler();
        btnCircle.handler();
        btnRect.handler();
        btnTurnRight.handler();
        btnTurnLeft.handler();

        btnRect.draw_btn();
        btnCircle.draw_btn();
        btnTriangle.draw_btn();
        btnTurnRight.draw_btn();
        btnTurnLeft.draw_btn();

        if (mousebuttons() == 1) {
            a = 1;
        }
        if (a == 1) {
            fig->draw();
        }
       

        

        swapbuffers(); // делаем активную страницу видимой
        delay(30); // задержка на 1/30 секунды
        if (kbhit()) break; // если нажата клавиша - завершить работу
    }

    closegraph();



    return 0;
}
  