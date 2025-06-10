#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

const int MAX_CONTAINER = 20;
const int MAX_MICRO = 55;

const int dRow[4] = {-1, 0, 1, 0};
const int dCol[4] = {0, -1, 0, 1};

int containerSize, experimentCount;
int cultureBoard[MAX_CONTAINER][MAX_CONTAINER];
int newCultureBoard[MAX_CONTAINER][MAX_CONTAINER];
bool visited[MAX_CONTAINER][MAX_CONTAINER];
int connectedComponentCount[MAX_MICRO];
int microSize[MAX_MICRO];
pair<int, int> microBoundingStart[MAX_MICRO], microBoundingEnd[MAX_MICRO];

bool isOutOfBounds(int row, int col) {
    return !(0 <= row && row < containerSize && 0 <= col && col < containerSize);
}

void dfsMarkComponent(int row, int col, int microId) {
    visited[row][col] = true;
    for (int dir = 0; dir < 4; dir++) {
        int newRow = row + dRow[dir];
        int newCol = col + dCol[dir];
        if (isOutOfBounds(newRow, newCol)) continue;
        if (visited[newRow][newCol]) continue;
        if (cultureBoard[newRow][newCol] != microId) continue;
        dfsMarkComponent(newRow, newCol, microId);
    }
}

void removeMicroorganism(int microId) {
    for (int row = 0; row < containerSize; row++) {
        for (int col = 0; col < containerSize; col++) {
            if (cultureBoard[row][col] == microId) {
                cultureBoard[row][col] = 0;
            }
        }
    }
}

void arrangeMicroorganisms(int microId, int injectionRow1, int injectionCol1, int injectionRow2, int injectionCol2) {
    for (int row = 0; row < containerSize; row++) {
        for (int col = 0; col < containerSize; col++) {
            visited[row][col] = false;
        }
    }
    for (int id = 1; id <= microId; id++) {
        connectedComponentCount[id] = 0;
    }
    for (int row = injectionRow1; row < injectionRow2; row++) {
        for (int col = injectionCol1; col < injectionCol2; col++) {
            cultureBoard[row][col] = microId;
        }
    }
    for (int row = 0; row < containerSize; row++) {
        for (int col = 0; col < containerSize; col++) {
            if (cultureBoard[row][col] == 0) continue;
            if (visited[row][col]) continue;
            int currentMicroId = cultureBoard[row][col];
            connectedComponentCount[currentMicroId]++;
            dfsMarkComponent(row, col, currentMicroId);
        }
    }
    for (int id = 1; id <= microId; id++) {
        if (connectedComponentCount[id] >= 2) {
            removeMicroorganism(id);
        }
    }
}

void relocateMicroorganisms(int microCount) {
    for (int row = 0; row < containerSize; row++) {
        for (int col = 0; col < containerSize; col++) {
            newCultureBoard[row][col] = 0;
        }
    }
    for (int id = 1; id <= microCount; id++) {
        microSize[id] = 0;
        microBoundingStart[id] = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
        microBoundingEnd[id] = {0, 0};
    }
    for (int row = 0; row < containerSize; row++) {
        for (int col = 0; col < containerSize; col++) {
            int currentMicroId = cultureBoard[row][col];
            if (currentMicroId == 0) continue;
            microSize[currentMicroId]++;
            microBoundingStart[currentMicroId].first = min(microBoundingStart[currentMicroId].first, row);
            microBoundingStart[currentMicroId].second = min(microBoundingStart[currentMicroId].second, col);
            microBoundingEnd[currentMicroId].first = max(microBoundingEnd[currentMicroId].first, row);
            microBoundingEnd[currentMicroId].second = max(microBoundingEnd[currentMicroId].second, col);
        }
    }
    vector<pair<int, int>> relocationOrder;
    for (int id = 1; id <= microCount; id++) {
        if (microSize[id] == 0) continue;
        relocationOrder.push_back({-microSize[id], id});
    }
    sort(relocationOrder.begin(), relocationOrder.end());
    for (auto &orderPair : relocationOrder) {
        int currentMicroId = orderPair.second;
        pair<int,int> boundingStart = microBoundingStart[currentMicroId];
        pair<int,int> boundingEnd = microBoundingEnd[currentMicroId];
        int clusterRowCount = boundingEnd.first - boundingStart.first + 1;
        int clusterColCount = boundingEnd.second - boundingStart.second + 1;
        for (int newRow = 0; newRow <= containerSize - clusterRowCount; newRow++) {
            bool placedForThisRow = false;
            for (int newCol = 0; newCol <= containerSize - clusterColCount; newCol++) {
                bool canPlace = true;
                for (int dr = 0; dr < clusterRowCount; dr++) {
                    for (int dc = 0; dc < clusterColCount; dc++) {
                        int originalRow = boundingStart.first + dr;
                        int originalCol = boundingStart.second + dc;
                        if (cultureBoard[originalRow][originalCol] != currentMicroId)
                            continue;
                        if (newCultureBoard[newRow + dr][newCol + dc] != 0) {
                            canPlace = false;
                            break;
                        }
                    }
                    if (!canPlace) break;
                }
                if (canPlace) {
                    for (int dr = 0; dr < clusterRowCount; dr++) {
                        for (int dc = 0; dc < clusterColCount; dc++) {
                            int originalRow = boundingStart.first + dr;
                            int originalCol = boundingStart.second + dc;
                            if (cultureBoard[originalRow][originalCol] != currentMicroId)
                                continue;
                            newCultureBoard[newRow + dr][newCol + dc] = currentMicroId;
                        }
                    }
                    placedForThisRow = true;
                    break;
                }
            }
            if (placedForThisRow)
                break;
        }
    }
    for (int row = 0; row < containerSize; row++) {
        for (int col = 0; col < containerSize; col++) {
            cultureBoard[row][col] = newCultureBoard[row][col];
        }
    }
}

void calculateExperimentResult(int microCount) {
    bool isAdjacent[MAX_MICRO][MAX_MICRO] = {false};
    for (int row = 0; row < containerSize; row++) {
        for (int col = 0; col < containerSize; col++) {
            if (cultureBoard[row][col] == 0) continue;
            for (int dir = 0; dir < 4; dir++) {
                int adjRow = row + dRow[dir];
                int adjCol = col + dCol[dir];
                if (isOutOfBounds(adjRow, adjCol)) continue;
                if (cultureBoard[adjRow][adjCol] == 0) continue;
                if (cultureBoard[row][col] != cultureBoard[adjRow][adjCol]) {
                    int idA = cultureBoard[row][col];
                    int idB = cultureBoard[adjRow][adjCol];
                    isAdjacent[idA][idB] = true;
                    isAdjacent[idB][idA] = true;
                }
            }
        }
    }
    int experimentScore = 0;
    for (int idA = 1; idA <= microCount; idA++) {
        for (int idB = idA + 1; idB <= microCount; idB++) {
            if (isAdjacent[idA][idB]) {
                experimentScore += microSize[idA] * microSize[idB];
            }
        }
    }
    cout << experimentScore << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> containerSize >> experimentCount;
    for (int row = 0; row < containerSize; row++) {
        for (int col = 0; col < containerSize; col++) {
            cultureBoard[row][col] = 0;
        }
    }
    for (int experimentId = 1; experimentId <= experimentCount; experimentId++) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        arrangeMicroorganisms(experimentId, r1, c1, r2, c2);
        relocateMicroorganisms(experimentId);
        calculateExperimentResult(experimentId);
    }
    return 0;
}
