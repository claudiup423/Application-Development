#include "route_planner.h"
#include <algorithm>
using std::reverse;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;
    start_node = &model.FindClosestNode(start_x, start_y);
    end_node = &model.FindClosestNode(end_x, end_y);
    
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {

    return node->distance(*end_node);

}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

    current_node->FindNeighbors(); 
    float current_g = current_node -> g_value; // Asigne g_value to current value
    current_node->visited = true;

    for (int i = 0; i <  current_node -> neighbors.size(); i++)
        {
            RouteModel::Node* neighbor = current_node -> neighbors[i];
            neighbor -> parent = current_node;
            neighbor -> g_value = current_g + neighbor -> distance(*current_node);
            neighbor -> h_value = CalculateHValue(neighbor);
            open_list.push_back(neighbor);
            neighbor -> visited = true;
        }
}

bool CheckNode(RouteModel::Node* first_node, RouteModel::Node* second_node)
    {
        float sum_first_node = first_node -> g_value + first_node -> h_value;
        float sum_second_node = second_node -> g_value + second_node -> h_value;

        if(sum_first_node > sum_second_node)
        {
            return 1;
        }
        return 0;
    }

RouteModel::Node *RoutePlanner::NextNode() 
{
    sort (open_list.begin(), open_list.end(), CheckNode);
    RouteModel::Node* next = open_list.back();

    return next;
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    while (current_node -> x != start_node -> x && current_node -> y != start_node ->  y)
    {
      path_found.push_back(*current_node);
      distance = distance + current_node -> distance(*current_node -> parent);
      current_node = current_node -> parent;
    }

    path_found.push_back(*current_node);
    reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}

void RoutePlanner::AStarSearch() {

    RouteModel::Node *current_node = nullptr;
    open_list.push_back(start_node);

    while(open_list.size() > 0) 
    {
        current_node = NextNode();
        open_list.pop_back();
        if (current_node->x == end_node->x && current_node->y == end_node->y) {
            m_Model.path = ConstructFinalPath(current_node);
            break;
        }
        AddNeighbors(current_node);           
    }

}