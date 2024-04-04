#include "SearchAndRescue.hpp"
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

SearchAndRescue::SearchAndRescue(string fileName)
{
    terrain = new int *[4];
    string line;
    ifstream infile(fileName);
    if (infile.is_open())
    {
        for (int i = 0; getline(infile, line); i++)
        {
            terrain[i] = new int[4];
            stringstream ss(line);
            string s;
            for (int j = 0; getline(ss, s, ' '); j++)
            {
                terrain[i][j] = stoi(s);
            }
        }
    }
    path = vector<State *>();
}

SearchAndRescue::~SearchAndRescue()
{
    State *to_del = path.back();
    delete to_del;
    path.pop_back();

    for (int i = 0; i < 4; i++)
    {
        delete terrain[i];
    }
    delete terrain;
}

void SearchAndRescue::possibleActions(State *current)
{
    // TODO
    if (current->y + 1 <= 3)
        current->possible_actions.push_back("up");
    if (current->y - 1 >= 0)
        current->possible_actions.push_back("down");
    if (current->x - 1 >= 0)
        current->possible_actions.push_back("left");
    if (current->x + 1 <= 3)
        current->possible_actions.push_back("right");
}

State *SearchAndRescue::result(State *current, string action)
{
    // TODO
    State *newState = new State{current->x, current->y, current->saved_people, action, vector<string>()};
    if (action == "up")
        newState->y += 1;
    else if (action == "down")
        newState->y -= 1;
    else if (action == "left")
        newState->x -= 1;
    else if (action == "right")
        newState->x += 1;

    return newState;
}

vector<State *> SearchAndRescue::expand(State *current)
{
    // TODO
    possibleActions(current);
    // Run through all possible actions of this state, put them into the result function, add them to the state ptr vector
    vector<State *> expansion;
    if (current->possible_actions.size() == 4)
    {
        for (int i = 0; i < 4; i++)
        {
            expansion.push_back(result(current, current->possible_actions.at(i)));
        }
    }
    if (current->possible_actions.size() == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            expansion.push_back(result(current, current->possible_actions.at(i)));
        }
    }
    if (current->possible_actions.size() == 2)
    {
        for (int i = 0; i < 2; i++)
        {
            expansion.push_back(result(current, current->possible_actions.at(i)));
        }
    }
    if (current->possible_actions.size() == 1)
    {
        for (int i = 0; i < 1; i++)
        {
            expansion.push_back(result(current, current->possible_actions.at(i)));
        }
    }

    return expansion;
}

bool SearchAndRescue::iterativeDeepeningWrapper(State *start)
{
    // TODO
    bool found = false;
    int depth_limit = STARTING_DEPTH;
    while (!found)
    {
        found = iterativeDeepeningSearch(start, depth_limit);
        if (found)
        {
            return true;
        }
        else
        {
            depth_limit++;
        }
    }
    return false;
}

bool SearchAndRescue::iterativeDeepeningSearch(State *current, int depth_limit)
{
    if (depth_limit == 0)
    {
        return false;
    }
    if (isGoal(current))
    {
        return true;
    }
    if (terrain[current->x][current->y] == 2)
    {
        current->saved_people += 1;
        terrain[current->x][current->y] = 1;
    }

    vector<State *> expansion = expand(current);

    for (int i = 0; i < expansion.size(); i++)
    {

        int original_saved_people = expansion.at(i)->saved_people;
        int original_terrain = terrain[expansion.at(i)->x][expansion.at(i)->y];

        path.push_back(expansion.at(i));
        if (iterativeDeepeningSearch(expansion.at(i), depth_limit - 1))
        {
            return true;
        }
        else
        {
            path.pop_back();
            if (expansion.at(i)->saved_people > original_saved_people)
            {
                expansion.at(i)->saved_people -= 1;
            }
            if (terrain[expansion.at(i)->x][expansion.at(i)->y] != original_terrain)
            {
                terrain[expansion.at(i)->x][expansion.at(i)->y] = 2;
            }
            delete expansion.at(i);
            expansion.at(i) = nullptr;
            expansion.erase(expansion.begin() + i);
        }
    }
    return false;
}

void SearchAndRescue::printPath()
{
    // TODO
    for (const auto &state : path)
    {
        cout << "x: " << state->x << "\ty: " << state->y << "\tprev_action: " << state->prev_action << "\n";
    }
}

bool SearchAndRescue::isGoal(State *current)
{
    // TODO
    bool goal = false;
    if (current->saved_people == PEOPLE_TO_SAVE && terrain[current->x][current->y] == 3)
    {
        goal = true;
    }
    return goal;
}

void SearchAndRescue::printTerrain()
{
    // TODO
    for (int i = 3; i >= 0; i--)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << terrain[j][i] << " ";
        }
        cout << "\n";
    }
}
