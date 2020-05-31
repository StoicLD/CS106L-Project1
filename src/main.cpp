/*
 * TODO: complete this file comment.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <filesystem>
#include "SimpleGraph.h"

using namespace std;

const double kPi = 3.14159265358979323;
const double kRepel = 10e-3;
const double kAtract = 10e-3;

void Welcome();
void initPosition(SimpleGraph& graph);
void UpdateOnceAlgorithm(SimpleGraph& graph);
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
            InitGraphVisualizer(graph);

            DrawGraph(graph);
            //vector<Node> netForces(graph.nodes.size());
            time_t startTime = time(NULL);

            while (true)
            {
                //                if(difftime(time(NULL), startTime) > 0.01)
                //                {
                startTime = time(NULL);
                UpdateOnceAlgorithm(graph);
                DrawGraph(graph);
                //}
            }

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

/**
 * @brief 更新一次
 * @param graph 需要更新的图
 * @param netForces 当前合力
 */
void UpdateOnceAlgorithm(SimpleGraph& graph)
{
    // 必须maintain一个Δx和Δy的当前合力vector
    // 每次更新的时候用会更新当前的合力，然后移动一个合力的大小距离
    // (1) O(N2)的两层for循环，计算每个点的排斥力
    // (2) 遍历Edge计算吸引力
    // (3) 更新位置
    vector<Node> netForces(graph.nodes.size());

    auto numOfNodes = graph.nodes.size();
    auto numOfEdges = graph.edges.size();
    for(decltype (numOfNodes) i = 0; i < numOfNodes; i++)
    {
        Node& node1 = graph.nodes[i];

        for(decltype (numOfNodes) j = i + 1; j < numOfNodes; j++)
        {
            Node& node2 = graph.nodes[j];
            // 同时更新两个节点
            double beSqrt = sqrt((node1.x - node2.x) * (node1.x - node2.x) + (node1.y - node2.y) * (node1.y - node2.y));
            double repelForce = kRepel;
            if(beSqrt != 0.0)
                repelForce = kRepel / beSqrt;
            //相当于以node1为原点
            double theta = atan2(node2.y - node1.y, node2.x - node1.x);
            netForces[i].x -= repelForce * cos(theta);
            netForces[i].y -= repelForce * sin(theta);
            netForces[j].x += repelForce * cos(theta);
            netForces[j].y += repelForce * sin(theta);
        }
    }

    for(decltype (numOfEdges) i = 0; i < numOfEdges; i++)
    {
        auto nIndex1 = graph.edges[i].start;
        auto nIndex2 = graph.edges[i].end;

        Node& node1 = graph.nodes[nIndex1];
        Node& node2 = graph.nodes[nIndex2];
        double beSqrt = sqrt((node1.x - node2.x) * (node1.x - node2.x) + (node1.y - node2.y) * (node1.y - node2.y));

        double atractForce = kAtract * beSqrt;
        double theta = atan2(node2.y - node1.y, node2.x - node1.x);
        netForces[nIndex1].x += atractForce * cos(theta);
        netForces[nIndex1].y += atractForce * sin(theta);
        netForces[nIndex2].x -= atractForce * cos(theta);
        netForces[nIndex2].y -= atractForce * sin(theta);
    }

    for(decltype (numOfNodes) i = 0; i < numOfNodes; i++)
    {
        graph.nodes[i].x += netForces[i].x;
        graph.nodes[i].y += netForces[i].y;
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


