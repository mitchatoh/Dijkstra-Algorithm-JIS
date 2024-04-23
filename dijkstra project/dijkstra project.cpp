#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

// These are the dimensions of the window and the grid (rows and columns) used to model Dijkstra's algorithm (hence INF)



const int WIDTH = 1600;
const int HEIGHT = 1600;
const int ROWS = 40;
const int COLS = 40;
const int CELL_SIZE = 40;
const int INF = std::numeric_limits<int>::max();


// Defines the coordinates in the grid and stores its position. 

struct Node {
    int x;
    int y;
    bool obstacle; //to distinguish obstacles from nodes 
    bool visited; // mark nodes as visited during map traversal
    std::vector<Node*> neighbors;
    Node* prev;
};

// draws the grid 

class GridMap {
public:
    GridMap() {
        // Initialize grid
        for (int i = 0; i < ROWS; ++i) {
            std::vector<Node*> row;
            for (int j = 0; j < COLS; ++j) {
                Node* node = new Node();
                node->x = j;
                node->y = i;
                node->obstacle = false;
                node->visited = false;
                node->prev = nullptr;
                grid.push_back(node);
            }
        }

        // Connects neighboring nodes
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                Node* node = getNode(i, j);
                if (i > 0) node->neighbors.push_back(getNode(i - 1, j)); // Up
                if (i < ROWS - 1) node->neighbors.push_back(getNode(i + 1, j)); // Down
                if (j > 0) node->neighbors.push_back(getNode(i, j - 1)); // Left
                if (j < COLS - 1) node->neighbors.push_back(getNode(i, j + 1)); // Right
            }
        }
    }

    Node* getNode(int row, int col) const {
        return grid[row * COLS + col];
    }

    //allows user to set obstacles 
    void setObstacle(int row, int col) {
        getNode(row, col)->obstacle = true;
    }

    void reset() {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                Node* node = getNode(i, j);
                node->visited = false;
                node->prev = nullptr;
            }
        }
    }

    // attaining the optimal path between two nodes
    std::vector<Node*> getShortestPath(Node* start, Node* end) {
        std::vector<Node*> path;
        Node* current = end;
        while (current != nullptr) {
            path.push_back(current);
            current = current->prev;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // stores nodes on the grid
private:
    std::vector<Node*> grid;
};

//implementation of Dijkstra's algorithm

void dijkstra(GridMap& grid, Node* start, Node* end1, Node* end2) {
    start->visited = true;
    std::queue<Node*> q;
    q.push(start);

    // To check if the neighbour node is one of the end points


    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current == end1 || current == end2)
            return; // stops when one end point is reached 

        for (Node* neighbor : current->neighbors) {
            if (!neighbor->visited && !neighbor->obstacle) {
                neighbor->visited = true;
                neighbor->prev = current;
                q.push(neighbor);
            }
        }
    }
}

// Chooses the nearest end point from the starting node (Essentially the shorter path of the two)

Node* findClosestEndpoint(Node* start, Node* end1, Node* end2) {
    int dist1 = std::abs(start->x - end1->x) + std::abs(start->y - end1->y);
    int dist2 = std::abs(start->x - end2->x) + std::abs(start->y - end2->y);
    return (dist1 < dist2) ? end1 : end2;
}

//The window is created using SFML graphics and user interaction with mouse and keyboard is done here

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Dijkstra's Algorithm");

    GridMap gridMap;

    bool drawingObstacle = true;
    bool selectingStart = true;
    bool selectingEnd1 = false;
    bool selectingEnd2 = false;
    Node* start = gridMap.getNode(0, 0);
    Node* end1 = gridMap.getNode(ROWS - 1, COLS - 2);
    Node* end2 = gridMap.getNode(ROWS - 2, COLS - 1);
    std::vector<Node*> path;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            //positioning of the 1st start node using key board
            else if (event.type == sf::Event::KeyPressed) {
                if (selectingStart) {
                    if (event.key.code == sf::Keyboard::Up) {
                        if (start->y > 0)
                            start = gridMap.getNode(start->y - 1, start->x);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        if (start->y < ROWS - 1)
                            start = gridMap.getNode(start->y + 1, start->x);
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        if (start->x > 0)
                            start = gridMap.getNode(start->y, start->x - 1);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        if (start->x < COLS - 1)
                            start = gridMap.getNode(start->y, start->x + 1);
                    }
                    else if (event.key.code == sf::Keyboard::Return) {
                        selectingStart = false;
                        selectingEnd1 = true;
                    }
                } //positioning of the 1st end node using key board
                else if (selectingEnd1) {
                    if (event.key.code == sf::Keyboard::Up) {
                        if (end1->y > 0)
                            end1 = gridMap.getNode(end1->y - 1, end1->x);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        if (end1->y < ROWS - 1)
                            end1 = gridMap.getNode(end1->y + 1, end1->x);
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        if (end1->x > 0)
                            end1 = gridMap.getNode(end1->y, end1->x - 1);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        if (end1->x < COLS - 1)
                            end1 = gridMap.getNode(end1->y, end1->x + 1);
                    }
                    else if (event.key.code == sf::Keyboard::Return) {
                        selectingEnd1 = false;
                        selectingEnd2 = true;
                    }
                } //positioning of the 2nd end node using key board
                else if (selectingEnd2) {
                    if (event.key.code == sf::Keyboard::Up) {
                        if (end2->y > 0)
                            end2 = gridMap.getNode(end2->y - 1, end2->x);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        if (end2->y < ROWS - 1)
                            end2 = gridMap.getNode(end2->y + 1, end2->x);
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        if (end2->x > 0)
                            end2 = gridMap.getNode(end2->y, end2->x - 1);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        if (end2->x < COLS - 1)
                            end2 = gridMap.getNode(end2->y, end2->x + 1);
                    }
                    else if (event.key.code == sf::Keyboard::Return) {
                        selectingEnd2 = false;
                    }
                }
                else if (event.key.code == sf::Keyboard::Space) {
                    gridMap.reset();
                    Node* closestEnd = findClosestEndpoint(start, end1, end2);
                    dijkstra(gridMap, start, closestEnd, closestEnd == end1 ? end2 : end1);
                    path = gridMap.getShortestPath(start, closestEnd);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int mouseX = event.mouseButton.x / CELL_SIZE;
                    int mouseY = event.mouseButton.y / CELL_SIZE;
                    if (mouseX >= 0 && mouseX < COLS && mouseY >= 0 && mouseY < ROWS) {
                        Node* node = gridMap.getNode(mouseY, mouseX);
                        node->obstacle = !node->obstacle;
                    }
                }
            }
        }

        window.clear();

        // Draws the grid
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                Node* node = gridMap.getNode(i, j);
                sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                rect.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                if (node->obstacle) {
                    rect.setFillColor(sf::Color::Black);
                }
                else if (node == start) {
                    rect.setFillColor(sf::Color::Red);
                }
                else if (node == end1 || node == end2) {
                    rect.setFillColor(sf::Color::Yellow);
                }
                else if (node->visited) {
                    rect.setFillColor(sf::Color::Green);
                }
                else {
                    rect.setFillColor(sf::Color::White);
                }
                window.draw(rect);
            }
        }

        // finds the shortest path
        if (!path.empty()) {
            for (int i = 1; i < path.size() - 1; ++i) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f((path[i - 1]->x + 0.5f) * CELL_SIZE, (path[i - 1]->y + 0.5f) * CELL_SIZE)),
                    sf::Vertex(sf::Vector2f((path[i]->x + 0.5f) * CELL_SIZE, (path[i]->y + 0.5f) * CELL_SIZE))
                };
                line[0].color = sf::Color::Blue;
                line[1].color = sf::Color::Blue;
                window.draw(line, 2, sf::Lines);
            }
        }

        window.display();
    }

    return 0;
}
