#include <iostream>
#include <cmath>
#include <list>
#include <fstream>

// Adjust this
#define STARTX 0
#define STARTY 0
// Adjust this
#define TARGETX 15
#define TARGETY 15


#define ROWS 20
#define COLS 20

using namespace std;

class MapNode
{
public:
    int x, y;
    MapNode *parent;
    double g, h;
    MapNode(int x, int y, MapNode *parent, double g, double h) : x(x), y(y), parent(parent), g(g), h(h) {}

    double f() const
    {
        return h + g;
    }
};

class EnormousMazeEpicMap
{
public:
    int field[ROWS][COLS];
    EnormousMazeEpicMap(const char *file_name)
    {
        ifstream file(file_name);
        for (unsigned int i = 0; i < ROWS; ++i)
            for (unsigned int j = 0; j < COLS; ++j)
                file >> field[i][j];
    }

    double heuristic_function(int x, int y)
    {
        return sqrt(pow(x - TARGETX, 2) + pow(y - TARGETY, 2));
    }

    bool target_found(int x, int y)
    {
        return x == TARGETX && y == TARGETY;
    }

    bool check_if_field_is_empty_space(int x, int y)
    {
        return x >= 0 && y >= 0 && x < ROWS && y < COLS && field[x][y] == 0;
    }

    void get_path(MapNode *node)
    {
        while (node)
        {
            field[node->x][node->y] = 3;
            node = node->parent;
        }
    }
};

bool contains_node(list<MapNode *> &node_list, int x, int y)
{
    for (MapNode *node : node_list)
    {
        if (node->x == x && node->y == y)
            return true;
    }
    return false;
}

void a_stern(EnormousMazeEpicMap &grid)
{
    list<MapNode *> to_visit;
    list<MapNode *> visited;

    MapNode *start = new MapNode(STARTX, STARTY, nullptr, 0, grid.heuristic_function(STARTX, STARTY));
    to_visit.push_back(start);

    while (!to_visit.empty())
    {
        MapNode *current_node = to_visit.front();
        for (MapNode *node : to_visit)
        {
            if ( (node->f() == current_node->f() && rand() % 2 == 0) || node->f() < current_node->f())
                current_node = node;
        }

        to_visit.remove(current_node);
        visited.push_back(current_node);

        if (grid.target_found(current_node->x, current_node->y))
        {
            grid.get_path(current_node);
            break;
        }

        int move_y[] = {-1, 1, 0, 0};
        int move_x[] = {0, 0, -1, 1};
        
        for (int i = 0; i < 4; ++i)
        {
            int x = current_node->x + move_x[i];
            int y = current_node->y + move_y[i];

            if (!grid.check_if_field_is_empty_space(x, y) || contains_node(visited, x, y))
                continue;

            double h = grid.heuristic_function(x, y);
            double g = current_node->g + 1;
            
            MapNode *neighbour = new MapNode(x, y, current_node, g, h);

            if (!contains_node(to_visit, x, y) || g < neighbour->g)
                to_visit.push_back(neighbour);
        }
    }

    if (to_visit.empty())
        cout << "THERE IS NO WAY WE CAN REACH THE TARGET!\nGAME OVER!!!!!"
             << endl;

    for (MapNode *node : to_visit)
    {
        delete node;
    }

    for (MapNode *node : visited)
    {
        delete node;
    }
}

int main()
{
    string file_name = "grid.txt";
    EnormousMazeEpicMap grid(file_name.c_str());
    if (grid.field[STARTX][STARTY] == 5)
    {
        cout << "THE STARTING POINT IS A WALL!!! :-("
             << endl;
        for (int i = 0; i < ROWS; ++i)
        {
            for (int j = 0; j < COLS; ++j)
            {
                cout << grid.field[i][j] << " ";
            }
            cout << endl;
        }
        return 0;
    }
    a_stern(grid);
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            cout << grid.field[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
