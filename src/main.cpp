/*
 * TODO: complete this file comment.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <filesystem>
#include "SimpleGraph.h"

using namespace std;

const double kPi = 3.14159265358979323;

void Welcome();
void initPosition(SimpleGraph& graph);
/**
 * 输出文件目录
 * 用户输入文件名字
 * 使用文件名字打开指定的文件
 * 指定时间
 * 创建SimpleGraph对象
 * 调用算法Update展示
 * @brief _userMain
 * @return
 */
int main() {
    Welcome();
    /* TODO: your implementation here */

    // (1) display the files that use may input
    auto currPath = filesystem::current_path();
    string pathStr = currPath.string();
    cout << "current working dir is: " << pathStr << endl;
    for (const auto & entry : filesystem::directory_iterator(pathStr))
    {
        std::cout << entry.path().filename() << std::endl;
    }

    // (2) let user input the name of the file
    cout << "please input the name above to select a graph file you want to use" << endl;
    string userInputStr;
    while(!getline(cin, userInputStr))
    {
        cout << "input is invalid, please input the given file name" << endl;
    }

    // (3) read the file
    ifstream fs = ifstream(pathStr + "\\" + userInputStr, ios::in);
    if(!fs.fail())
    {
        cout << "successfully read the graph file!" << endl;
        try
        {
            SimpleGraph graph;
            int startPoint, endPoint;
            int numOfNodes;
            fs >> numOfNodes;
            while (fs >> startPoint >> endPoint)
            {
                //graph.nodes.push_back()
                graph.edges.push_back(Edge(startPoint, endPoint));
            }
            graph.nodes = vector<Node>(numOfNodes);
            // (4) initialize the position of nodes
            initPosition(graph);
            DrawGraph(graph);
            cout << "Please input the seconds you want the algorithm to run" << endl;
            double seconds = 5.0;

        }
        catch (...)
        {
            cout << "some error occurs, exit the program!" << endl;
            return -2;
        }
    }
    else
    {
        cout << "input is not valid, exit the program!" << endl;
        return -1;
    }

    return 0;
}

/**
 * @brief 初始化节点位置，形成圆形
 * @param graph
 */
void initPosition(SimpleGraph& graph)
{
    auto nodeSize = graph.nodes.size();
    for(decltype(nodeSize) i = 0; i < nodeSize; ++i)
    {
        graph.nodes[i].x = cos(static_cast<double>(i * 2) * kPi / nodeSize);
        graph.nodes[i].y = sin(static_cast<double>(i * 2) * kPi / nodeSize);
    }
}



/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}


