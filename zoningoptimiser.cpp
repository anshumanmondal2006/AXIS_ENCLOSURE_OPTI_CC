#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <numeric>

using namespace std;

struct Zone
{
    double posX, posY, penalty;
    int zoneId;
};

struct Vertex
{
    double x, y;
    Vertex(double x = 0, double y = 0) : x(x), y(y) {}
};

class ZoningOptimizer
{
private:
    vector<Zone> allZones;
    int totalZones, requiredZones;
    double minTotalPenalty;
    vector<Vertex> optimalBoundary;

    vector<Vertex> buildRect(const vector<int> &chosen)
    {
        if (chosen.empty())
            return {};

        double lx = allZones[chosen[0]].posX;
        double rx = allZones[chosen[0]].posX;
        double by = allZones[chosen[0]].posY;
        double ty = allZones[chosen[0]].posY;

        for (size_t idx_i = 1; idx_i < chosen.size(); ++idx_i)
        {
            int idx = chosen[idx_i];
            if (allZones[idx].posX < lx)
                lx = allZones[idx].posX;
            else if (allZones[idx].posX > rx)
                rx = allZones[idx].posX;

            if (allZones[idx].posY < by)
                by = allZones[idx].posY;
            else if (allZones[idx].posY > ty)
                ty = allZones[idx].posY;
        }

        return {Vertex(lx, by), Vertex(rx, by), Vertex(rx, ty), Vertex(lx, ty)};
    }

    double getPerimeter(const vector<int> &picked)
    {
        if (picked.empty())
            return 0;

        double lx = allZones[picked[0]].posX, rx = lx;
        double by = allZones[picked[0]].posY, ty = by;

        size_t i = 1;
        while (i < picked.size())
        {
            int idx = picked[i];
            lx = (allZones[idx].posX < lx) ? allZones[idx].posX : lx;
            rx = (allZones[idx].posX > rx) ? allZones[idx].posX : rx;
            by = (allZones[idx].posY < by) ? allZones[idx].posY : by;
            ty = (allZones[idx].posY > ty) ? allZones[idx].posY : ty;
            i++;
        }

        return 2 * ((rx - lx) + (ty - by));
    }

    void testSelection(const vector<int> &combo)
    {
        if ((int)combo.size() < requiredZones)
            return;

        double perim = getPerimeter(combo);
        double zonePenalty = 0.0;
        for (auto it = combo.cbegin(); it != combo.cend(); ++it)
        {
            zonePenalty += allZones[*it].penalty;
        }

        double combinedCost = perim + zonePenalty;
        if (combinedCost < minTotalPenalty)
        {
            minTotalPenalty = combinedCost;
            optimalBoundary = buildRect(combo);
        }
    }

    vector<int> greedySelection()
    {
        vector<int> chosenZones;
        vector<bool> tagged(totalZones, false);

        vector<int> zoneIdx(totalZones);
        iota(zoneIdx.begin(), zoneIdx.end(), 0);
        sort(zoneIdx.begin(), zoneIdx.end(), [&](int a, int b)
             { return allZones[a].penalty < allZones[b].penalty; });

        int count = 0;
        while (count < requiredZones && count < totalZones)
        {
            chosenZones.push_back(zoneIdx[count]);
            tagged[zoneIdx[count]] = true;
            ++count;
        }

        for (int i = requiredZones; i < totalZones && i < requiredZones + 10; ++i)
        {
            int idx = zoneIdx[i];
            if (allZones[idx].penalty < 0)
            {
                vector<int> temp(chosenZones);
                temp.push_back(idx);

                double curCost = getPerimeter(chosenZones);
                for (int j = 0; j < (int)chosenZones.size(); ++j)
                {
                    curCost += allZones[chosenZones[j]].penalty;
                }

                double altCost = getPerimeter(temp);
                for (int j = 0; j < (int)temp.size(); ++j)
                {
                    altCost += allZones[temp[j]].penalty;
                }

                if (altCost < curCost)
                    chosenZones = temp;
            }
        }

        return chosenZones;
    }

    vector<int> gatherNegatives()
    {
        vector<int> negatives;
        for (int i = 0; i < totalZones; ++i)
        {
            if (allZones[i].penalty < 0)
                negatives.push_back(i);
        }

        if ((int)negatives.size() >= requiredZones)
            return negatives;

        vector<int> positives;
        for (int i = 0; i < totalZones; ++i)
        {
            if (allZones[i].penalty >= 0)
                positives.push_back(i);
        }

        sort(positives.begin(), positives.end(), [&](int a, int b)
             { return allZones[a].penalty < allZones[b].penalty; });

        size_t idx = 0;
        while (negatives.size() < (size_t)requiredZones && idx < positives.size())
        {
            negatives.push_back(positives[idx]);
            ++idx;
        }

        return negatives;
    }

public:
    ZoningOptimizer(vector<Zone> &zones, int minRequired)
        : allZones(zones), totalZones((int)zones.size()), requiredZones(minRequired), minTotalPenalty(1e18) {}

    pair<double, vector<Vertex>> optimize()
    {
        vector<int> s1 = gatherNegatives();
        testSelection(s1);

        vector<int> s2 = greedySelection();
        testSelection(s2);

        vector<int> ids(totalZones);
        iota(ids.begin(), ids.end(), 0);
        sort(ids.begin(), ids.end(), [&](int a, int b)
             { return allZones[a].penalty < allZones[b].penalty; });
        vector<int> s3(ids.begin(), ids.begin() + requiredZones);
        testSelection(s3);

        return {minTotalPenalty, optimalBoundary};
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ifstream fin("input02.txt");
    if (!fin)
    {
        cerr << "Cannot open input02.txt" << endl;
        return 1;
    }

    int zoneCount, k;
    fin >> zoneCount >> k;
    vector<Zone> regionData(zoneCount);
    for (int i = 0; i < zoneCount; ++i)
    {
        fin >> regionData[i].posX >> regionData[i].posY >> regionData[i].penalty;
    }
    fin.close();

    ZoningOptimizer planner(regionData, k);
    pair<double, vector<Vertex>> outcome = planner.optimize();

    cout << fixed << setprecision(6) << outcome.first << "\n";
    for (size_t i = 0; i < outcome.second.size(); ++i)
    {
        size_t j = (i + 1) % outcome.second.size();
        cout << fixed << setprecision(6)
             << outcome.second[i].x << " " << outcome.second[i].y << " "
             << outcome.second[j].x << " " << outcome.second[j].y << "\n";
    }

    return 0;
}
