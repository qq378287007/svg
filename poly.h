#pragma once

#include <vector>
#include <string>
using namespace std;

class node
{
public:
    explicit node(double x = 0.0, double y = 0.0, int boundMark = 0)
        : m_X(x), m_Y(y), m_BoundMark(boundMark)
    {
    }
    double m_X;
    double m_Y;
    int m_BoundMark;
};


class edge
{
public:
    explicit edge(int x = 0, int y = 0, int boundMark = 0)
        : m_X(x), m_Y(y), m_BoundMark(boundMark)
    {
    }
    int m_X;
    int m_Y;
    int m_BoundMark;
};

class ele
{
public:
    explicit ele(int x = 0, int y = 0, int z = 0, int boundMark = 0)
        : m_X(x), m_Y(y), m_Z(z), m_BoundMark(boundMark)
    {
    }
    int m_X;
    int m_Y;
    int m_Z;
    int m_BoundMark;
};

class hole
{
public:
    explicit hole(double x = 0.0, double y = 0.0, int boundMark = 0)
        : m_X(x), m_Y(y), m_BoundMark(boundMark)
    {
    }
    double m_X;
    double m_Y;
    int m_BoundMark;
};

void readNode(const char *file, vector<node> &nodes);
//void readEdge(const char *file);
//void readEle(const char *file);
//void readAll(const char *file);
//void readPoly(const char *file);


