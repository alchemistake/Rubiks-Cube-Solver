#include "cube.h"
#include <vector>
#include <sstream>
#include <regex>
#include <map>

Cube::Cube()
{
    CornerOrientation cornerOrientation;
    CornerPermutation cornerPermutation;
    EdgeOrientation edgeOrientation;
    EdgePermutation edgePermutation;
    coState = cornerOrientation.array_to_index(std::vector<int>{ 0, 0, 0, 0, 0, 0, 0, 0 });
    cpState = cornerPermutation.array_to_index(std::vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7 });
    eoState = edgeOrientation.array_to_index(std::vector<int>{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    epState1 = edgePermutation.array_to_index(std::vector<int>{ 0, 1, 2, 3, 4, 5 });
    epState2 = edgePermutation.array_to_index(std::vector<int>{ 6, 7, 8, 9, 10, 11 });
    edges = Edges({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 12);

    cornerOrientation.buildTransitionTable();
    coTransition = cornerOrientation.getTransitionTable();

    cornerPermutation.buildTransitionTable();
    cpTransition = cornerPermutation.getTransitionTable();

    edgeOrientation.buildTransitionTable();
    eoTransition = edgeOrientation.getTransitionTable();

    edgePermutation.buildTransitionTable();
    epTransition = edgePermutation.getTransitionTable();
}

Cube::Cube(std::string scramble) : Cube()
{
    std::map<char, int> moveNames{{'U', 0}, {'D', 1}, {'R', 2}, {'L', 3}, {'F', 4}, {'B', 5}};
    std::map<char, int> repeatNames{{'\'', 3}, {'2', 2}};

    std::smatch sm;
    std::regex r("([UDRLFB][2']?)");
    std::regex_iterator<std::string::iterator> it(scramble.begin(), scramble.end(), r);
    std::regex_iterator<std::string::iterator> end;

    while (it != end)
    {
        std::string match = it->str();
        int move = moveNames[match[0]];
        int repeats = 1;
        if (match.size() > 1)
            repeats = repeatNames[match[1]];

        for (int i = 0; i < repeats; i++)
            apply_move(move);

        ++it;
    }
}

void Cube::apply_move(int move)
{
    coState = coTransition[coState][move];
    cpState = cpTransition[cpState][move];
    eoState = eoTransition[eoState][move];
    epState1 = epTransition[epState1][move];
    epState2 = epTransition[epState2][move];
    edges.apply_move(move);
}

bool Cube::is_solved()
{
    return cpState == 0 && coState == 0 && eoState == 0 && epState1 == 0 && epState2 == 366288;
}
