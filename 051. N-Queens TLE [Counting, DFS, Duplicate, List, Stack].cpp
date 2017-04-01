#include <vector>
#include <algorithm>
#include <list>

using namespace std;

// NOTE(Wuxiang): You would like to avoid store a board structure in this problem.
// Because the queen collision detection can be done in constant time even you don't
// have a structure for a board. You don't want to waste time in maintain the
// validity of the board structure.
//
// NOTE(Wuxiang): Last executed input -- 8.
class Solution
{
public:
    list<vector<pair<int, int>>> queenArrangementList;
    vector<pair<int, int>> queenArrangementCurrent;
    vector<vector<string>> queenResultList;
    vector<string>         queenResultTemplate;

    bool canQueenAttack(int rowIndexQueen, int columnIndexQueen, int rowIndexTarget, int columnIndexTarget)
    {
        int rowDiff = rowIndexQueen - rowIndexTarget;
        int columnDiff = columnIndexQueen - columnIndexTarget;

        return rowDiff == 0
               || columnDiff == 0
               || rowDiff == columnDiff
               || rowDiff == -columnDiff;
    }

    void chooseQueen(int rowOffset, int columnOffset, int rowNum, int columnNum)
    {
        // Termination condition
        if (queenArrangementCurrent.size() == rowNum)
        {
            queenArrangementList.push_back(queenArrangementCurrent);

            return;
        }

        // NOTE(Wuxiang): The problem for this routine is that search space for
        // each recursion is overlapped. You cannot avoid duplication easily in
        // this case.
        //
        // See "051. N-Queens AC [Counting, DFS, Stack]" for minor improvement can
        // easily fix this.
        for (int rowIndex = rowOffset; rowIndex < rowNum; ++rowIndex)
        {
            for (int columnIndex = columnOffset; columnIndex < columnNum; ++columnIndex)
            {
                if (queenArrangementCurrent.empty())
                {
                    queenArrangementCurrent.push_back(make_pair(rowIndex, columnIndex));

                    chooseQueen(0, 0, rowNum, columnNum);

                    queenArrangementCurrent.pop_back();
                }
                else
                {
                    bool queenCanAttackIsNone = true;

                    for (int queenIndex = 0; queenIndex < int(queenArrangementCurrent.size()); ++queenIndex)
                    {
                        pair<int, int> q = queenArrangementCurrent[queenIndex];

                        if (canQueenAttack(q.first, q.second, rowIndex, columnIndex))
                        {
                            queenCanAttackIsNone = false;
                            break;
                        }
                    }

                    if (queenCanAttackIsNone)
                    {
                        queenArrangementCurrent.push_back(make_pair(rowIndex, columnIndex));

                        chooseQueen(0, 0, rowNum, columnNum);

                        queenArrangementCurrent.pop_back();
                    }
                }
            }
        }
    }

    void removeArrangementDuplicates()
    {
        typedef list<vector<pair<int, int>>>::iterator Iter;

        for (Iter iter = queenArrangementList.begin(); iter != queenArrangementList.end(); ++iter)
        {
            vector<pair<int, int>>& queenArrangement = *iter;

            sort(queenArrangement.begin(), queenArrangement.end());
        }

        queenArrangementList.sort();

        Iter iterCurrernt = queenArrangementList.begin();
        ++iterCurrernt;
        for (; iterCurrernt != queenArrangementList.end(); ++iterCurrernt)
        {
            Iter iterPrevious = iterCurrernt;
            --iterPrevious;

            if (*iterPrevious == *iterCurrernt)
            {
                queenArrangementList.erase(iterPrevious);
            }
        }
    }

    void prepareResult(int rowNum, int columnNum)
    {
        typedef list<vector<pair<int, int>>>::iterator Iter;

        queenResultTemplate.assign(rowNum, string(columnNum, '.'));

        for (Iter iter = queenArrangementList.begin(); iter != queenArrangementList.end(); ++iter)
        {
            vector<string> queenArrangementResult = queenResultTemplate;

            for (int queenIndex = 0; queenIndex < int(iter->size()); ++queenIndex)
            {
                pair<int, int> q = (*iter)[queenIndex];
                queenArrangementResult[q.first][q.second] = 'Q';
            }

            queenResultList.push_back(queenArrangementResult);
        }
    }

    vector<vector<string>> solveNQueens(int n)
    {
        int rowNum = n;
        int columnNum = n;

        chooseQueen(0, 0, rowNum, columnNum);

        removeArrangementDuplicates();

        prepareResult(rowNum, columnNum);

        return queenResultList;
    }
};