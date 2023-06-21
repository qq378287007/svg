#include "poly.h"

static char *readline(char *string, FILE *infile)
{
    char *result;
    do
    {
        result = fgets(string, 256, infile);

        while ((*result != '\0') && (*result != '#') && (*result != '.') && (*result != '+') && (*result != '-') && ((*result < '0') || (*result > '9')))
            result++;
    } while ((*result == '#') || (*result == '\0'));
    return result;
}

static char *findfield(char *string)
{
    char *result = string;

    while ((*result != '\0') && (*result != '#') && (*result != ' ') && (*result != '\t'))
        result++;
    while ((*result != '\0') && (*result != '#') && (*result != '.') && (*result != '+') && (*result != '-') && ((*result < '0') || (*result > '9')))
        result++;

    if (*result == '#')
        *result = '\0';

    return result;
}

void readNode(const char *file, vector<node> &nodes)
{
    char inputline[256];

    FILE *fp = fopen(file, "r");
    char *stringptr = readline(inputline, fp);
    int number = (int)strtol(stringptr, &stringptr, 0);
    (int)strtol(stringptr, &stringptr, 0);//dim
    (int)strtol(stringptr, &stringptr, 0);//num_attr
    (int)strtol(stringptr, &stringptr, 0);//bound_mark

    nodes.clear();
    nodes.reserve(number);
    for (int i = 0; i < number; i++)
    {
        stringptr = readline(inputline, fp);
        (int)strtol(stringptr, &stringptr, 0);//order_number
        double x = (double)strtod(stringptr, &stringptr);
        double y = (double)strtod(stringptr, &stringptr);
        int bound_mark = (int)strtol(stringptr, &stringptr, 0);

        nodes.push_back(node{x,y,bound_mark});
    }

    fclose(fp);
}


/*
void Widget::readEdge(const QString &fileName)
{
    QString pathName = QFileInfo(fileName).path();
    QString baseName = QFileInfo(fileName).completeBaseName();
    QString file = pathName + QDir::separator() + baseName + ".edge";

    if (!QFileInfo(file).isFile())
        return;

    readNode(fileName);

    char inputline[256];
    char *stringptr;

    FILE *fp = fopen(file.toStdString().c_str(), "r");

    stringptr = readline(inputline, fp);
    edges.number = (int)strtol(stringptr, &stringptr, 0);
    edges.bound_mark = (int)strtol(stringptr, &stringptr, 0);

    edges.edges.reserve(edges.number);
    for (int i = 0; i < edges.number; i++)
    {

        stringptr = readline(inputline, fp);
        int order_number = (int)strtol(stringptr, &stringptr, 0);
        int x = (int)strtol(stringptr, &stringptr, 0);
        int y = (int)strtol(stringptr, &stringptr, 0);
        int bound_mark = (int)strtol(stringptr, &stringptr, 0);

        edges.edges.append(edge_one{order_number, x, y, bound_mark});
    }

    fclose(fp);
}

void Widget::readEle(const QString &fileName)
{
    QString pathName = QFileInfo(fileName).path();
    QString baseName = QFileInfo(fileName).completeBaseName();
    QString file = pathName + QDir::separator() + baseName + ".edge";

    if (!QFileInfo(file).isFile())
        return;

    readNode(fileName);

    char inputline[256];
    char *stringptr;

    FILE *fp = fopen(fileName.toStdString().c_str(), "r");

    stringptr = readline(inputline, fp);
    eles.number = (int)strtol(stringptr, &stringptr, 0);
    eles.dim = (int)strtol(stringptr, &stringptr, 0);
    eles.bound_mark = (int)strtol(stringptr, &stringptr, 0);

    eles.eles.reserve(eles.number);
    for (int i = 0; i < eles.number; i++)
    {
        stringptr = readline(inputline, fp);
        int order_number = (int)strtol(stringptr, &stringptr, 0);
        int x = (int)strtol(stringptr, &stringptr, 0);
        int y = (int)strtol(stringptr, &stringptr, 0);
        int z = (int)strtol(stringptr, &stringptr, 0);

        eles.eles.append(ele_one{order_number, x, y, z});
    }

    fclose(fp);
}

void Widget::readAll(const QString &fileName)
{
    QString pathName = QFileInfo(fileName).path();
    QString baseName = QFileInfo(fileName).completeBaseName();

    QString nodeFile = pathName + QDir::separator() + baseName + ".node";
    readNode(nodeFile);

    QString edgeFile = pathName + QDir::separator() + baseName + ".edge";
    readEdge(edgeFile);

    QString eleFile = pathName + QDir::separator() + baseName + ".ele";
    readEle(eleFile);
}

void Widget::readPoly(const QString &fileName)
{
    QString pathName = QFileInfo(fileName).path();
    QString baseName = QFileInfo(fileName).completeBaseName();
    QString file = pathName + QDir::separator() + baseName + ".poly";

    if (!QFileInfo(file).isFile())
        return;

    char inputline[256];
    char *stringptr;

    FILE *fp = fopen(file.toStdString().c_str(), "r");

    stringptr = readline(inputline, fp);
    nodes.number = (int)strtol(stringptr, &stringptr, 0);
    nodes.dim = (int)strtol(stringptr, &stringptr, 0);
    nodes.num_attr = (int)strtol(stringptr, &stringptr, 0);
    nodes.bound_mark = (int)strtol(stringptr, &stringptr, 0);

    if (nodes.number == 0)
    {
        readNode(fileName);
    }
    else
    {
        //
        nodes.nodes.reserve(nodes.number);
        for (int i = 0; i < nodes.number; i++)
        {
            stringptr = readline(inputline, fp);
            int order_number = (int)strtol(stringptr, &stringptr, 0);
            double x = (double)strtod(stringptr, &stringptr);
            double y = (double)strtod(stringptr, &stringptr);
            int bound_mark = (int)strtol(stringptr, &stringptr, 0);

            nodes.nodes.append(node_one{order_number, x, y, bound_mark});
        }
    }

    //
    stringptr = readline(inputline, fp);
    edges.number = (int)strtol(stringptr, &stringptr, 0);
    edges.bound_mark = (int)strtol(stringptr, &stringptr, 0);

    edges.edges.reserve(edges.number);
    for (int i = 0; i < edges.number; i++)
    {

        stringptr = readline(inputline, fp);
        int order_number = (int)strtol(stringptr, &stringptr, 0);
        int x = (int)strtol(stringptr, &stringptr, 0);
        int y = (int)strtol(stringptr, &stringptr, 0);
        int bound_mark = (int)strtol(stringptr, &stringptr, 0);

        edges.edges.append(edge_one{order_number, x, y, bound_mark});
    }

    //
    stringptr = readline(inputline, fp);
    holes.number = (int)strtol(stringptr, &stringptr, 0);

    holes.holes.reserve(holes.number);
    for (int i = 0; i < holes.number; i++)
    {

        stringptr = readline(inputline, fp);
        int order_number = (int)strtol(stringptr, &stringptr, 0);
        double x = (double)strtod(stringptr, &stringptr);
        double y = (int)strtod(stringptr, &stringptr);

        holes.holes.append(hole_one{order_number, x, y});
    }

    fclose(fp);
}
*/
