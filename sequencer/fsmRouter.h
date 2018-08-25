/*********************************************************************************

  MIT License

  Copyright © 2018 HP Development Company, L.P.

  Permission  is  hereby granted,  free  of  charge, to any person obtaining  a
  copy of this software  and  associated  documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation the
  rights to use, copy, modify,  merge,  publish, distribute, sublicense, and/or
  sell copies  of the Software,  and to permit persons  to whom the Software is
  furnished to do so,subject to the following conditions:

  The above copyright  notice and this  permission notice shall  be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE  IS PROVIDED  "AS IS", WITHOUT WARRANTY OF  ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING  BUT NOT  LIMITED  TO THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

  IN  NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES  OR  OTHER  LIABILITY,  WHETHER IN  AN ACTION  OF  CONTRACT, TORT  OR 
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
  OR OTHER DEALINGS IN THE SOFTWARE.    

*********************************************************************************/

#ifndef FSMROUTER_H
#define FSMROUTER_H

#include <QString>
#include <QList>
#include <algorithm>
#include "diagram.h"

class testSwitch{
public:
    int stateID = 0;
    QString stateText = "";
    int transitionID = 0;
    QString transitionText = "";

};
//--------------------------------------------------------------------------------------------------------------------------------

class switchSequence{
public:
    QList<testSwitch> switches;

};
//--------------------------------------------------------------------------------------------------------------------------------

class sequencesList{
public:
    QList<switchSequence> sequences;

};
//--------------------------------------------------------------------------------------------------------------------------------

class statsByTest {
public:
    QList<int> testIndexes;
    QList<int> switchesCovered;
    QStringList statesCoverage;
    QStringList transitionsCoverage;
    QStringList switchesCoverage;
    QList<sequencesList> missingZeroSwitches;
    QList<sequencesList> missingNSwitches;

    void assign(statsByTest replacement);
};
//--------------------------------------------------------------------------------------------------------------------------------

class fsmRouter
{
public:
    fsmRouter(diagram *testDiagram = 0, int switchLength = 0);

    sequencesList tests;
    statsByTest testsStats;
    QList<sequencesList> switchSequences;
    void showDebugInfo(sequencesList Sequences);

    enum switchesIndex{
        zeroSwitchesIndex,
        oneSwitchesIndex,
        nSwitchesIndex

    };

    enum directionFrom{
        startToSequence,
        sequenceToEnd,

    };

private:
    int switchN;
    void sortByCoverage(statsByTest *stats);
    static bool higherCoverage(int a, int b);
    void generateTests();
    void calculateStats();
    void loadFsmExtremes();
    int statesCoveredCount(switchSequence testSequence);
    int transitionsCoveredCount(switchSequence testSequence);
    QList<int> switchesCovered(switchSequence route, int switchLength);
    testSwitch finalSwitch;
    diagram *Diagram;
    diagramStructure structure;
    sequencesList sequencesToRoute;
    sequencesList getSwitchesFromState(int stateID);
    bool loadSwitches();
    QList<sequencesList> getSwitchesBuffer();
    sequencesList getSwitchesFrom(sequencesList fromSwitches, int length);
    sequencesList getSwitchesAfter(switchSequence sequence);
    int getStateAfterSequence(switchSequence sequence);
    QString getTextFrom(int ID);
    int initialStateID;
    QList<int> finalTransitions;
    switchSequence coupleInOrder(switchSequence sequenceA, switchSequence sequenceB);
    switchSequence tryToCouple(switchSequence sequenceA, switchSequence sequenceB);
    void appendIfMissing(switchSequence singleSequence, sequencesList *Sequences);
    void completeFrom(sequencesList *getFrom, sequencesList *addTo);
    bool isWithin(switchSequence singleSequence, sequencesList *Sequences);
    switchSequence getBestRouteFrom(directionFrom direction, switchSequence singleRoute, sequencesList allRoutes);
    void addIfMissing(switchSequence route, sequencesList *routeList);
    sequencesList startingRoutesIn(sequencesList routes);
    sequencesList endingRoutesIn(sequencesList routes);
    switchSequence getHigherCoverageRoute(sequencesList routes);
    sequencesList extractRoutesHeading(fsmRouter::directionFrom direction, sequencesList allRoutes);
    switchSequence tryToRouteHeading(fsmRouter::directionFrom direction, switchSequence routeToComplete, switchSequence nSwitchSequence);
    int getContained(QList<int> argument1, QList<int> argument2);
    sequencesList leaveOptimalRoutes(sequencesList routes);
    sequencesList interLink(sequencesList sequencesToLink);
    sequencesList completeRoutes(sequencesList sequencesToRoute);
    void addFinalStateTo(sequencesList *linkedSequences);

};

#endif // FSMROUTER_H
