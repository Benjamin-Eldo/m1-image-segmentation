#include "../headers/Pixel.hpp"

Pixel::Pixel(int _x, int _y) : x(_x),y(_y) {};

Pixel::~Pixel(){}

int Pixel::getX() {
    return x;
};

int Pixel::getY() {
    return y;
};
bool operator==(const Pixel& a, const Pixel& b){
    return a.x == b.x && a.y == b.y;
}