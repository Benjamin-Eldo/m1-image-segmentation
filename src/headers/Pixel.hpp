#pragma once

class Pixel{
private:
    int x;
    int y;
public:
    Pixel(int x, int y);
    ~Pixel();
    int getX();
    int getY();
    friend bool operator==(const Pixel& a, const Pixel& b);
    friend bool operator< (const Pixel& a, const Pixel& b) { return a.x < b.x && a.y < b.y;}
    friend bool operator> (const Pixel& a, const Pixel& b) { return a.x > b.x && a.y > b.y; }
    friend bool operator<=(const Pixel& a, const Pixel& b) { return !(a.x > b.x && a.y > b.y); }
    friend bool operator>=(const Pixel& a, const Pixel& b) { return !(a.x < b.x && a.y < b.y); }
};