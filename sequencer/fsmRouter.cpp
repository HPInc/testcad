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

#include "fsmRouter.h"

fsmRouter::fsmRouter(diagram *testDiagram, int switchLength)
{
    Diagram = testDiagram;
    structure = Diagram->getStructure();
    switchN = switchLength;
    switchSequences = getSwitchesBuffer();

    loadFsmExtremes();
    generateTests();
    calculateStats();

}
//--------------------------------------------------------------------------------------------------------------------------------

switchSequence fsmRouter::getBestRouteFrom(fsmRouter::directionFrom direction,
                                           switchSequence singleRoute,
                                           sequencesList allRoutes)
{
    switchSequence routeBuff;
    sequencesList directedRoutes;
    int n = 0;
    QList<int> covered;
    int maxCovered;
    switchSequence bestRoute;
    sequencesList routes;
    routes.sequences.append(singleRoute);
    int attempts = 0;
    int maxAttempts = pow(switchSequences.at(fsmRouter::nSwitchesIndex).sequences.size(), 2);

    while((directedRoutes.sequences.size() == 0) && (n < allRoutes.sequences.size())){
        attempts++;

        for(int i = 0; i < switchSequences.at(fsmRouter::zeroSwitchesIndex).sequences.size(); i++){
            maxCovered = 0;
            bestRoute.switches.clear();

            for(int j = 0; j < routes.sequences.size(); j++){
                routeBuff = tryToRouteHeading(direction,
                                              routes.sequences.at(j),
                                              switchSequences.at(fsmRouter::zeroSwitchesIndex).sequences.at(i));

                if(routeBuff.switches.size() > 0){
                    covered = switchesCovered(routeBuff, switchN);

                    if(covered.size() > maxCovered){
                        maxCovered = covered.size();
                        bestRoute = routeBuff;

                    }
                }
            }

            addIfMissing(bestRoute, &routes);
            directedRoutes = extractRoutesHeading(direction, routes);

            if (directedRoutes.sequences.size()>0){
                n++;

            }
        }

        if(attempts > maxAttempts) //Limit iterations to an estimated maximum to avoid infinite State Machines
            return routeBuff;
    }

    return getHigherCoverageRoute(directedRoutes);

}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::sortByCoverage(statsByTest *stats)
{
    std::vector<int> sortedCoverages;
    std::vector<int>::iterator it = sortedCoverages.begin();

    for (int i = 0; i < stats->switchesCovered.size(); i++){
        sortedCoverages.insert(it,stats->switchesCovered.at(i));
        it = sortedCoverages.begin();

    }

    std::sort(sortedCoverages.begin(),sortedCoverages.end(), higherCoverage);
    statsByTest sortedStats;
    sequencesList sortedTests;

    for(unsigned int i = 0; i < sortedCoverages.size(); i++){

        for(int j = 0; j < stats->switchesCovered.size(); j++){

            if (stats->switchesCovered.at(j) == sortedCoverages.at(i)){
                sortedStats.testIndexes.append(stats->testIndexes.at(j));
                sortedStats.switchesCovered.append(stats->switchesCovered.at(j));
                sortedStats.statesCoverage.append(stats->statesCoverage.at(j));
                sortedStats.transitionsCoverage.append(stats->transitionsCoverage.at(j));
                sortedStats.switchesCoverage.append(stats->switchesCoverage.at(j));
                sortedStats.missingZeroSwitches.append(stats->missingZeroSwitches.at(j));
                sortedStats.missingNSwitches.append(stats->missingNSwitches.at(j));

                sortedTests.sequences.append(tests.sequences.at(sortedStats.testIndexes.last()));

                stats->testIndexes.removeAt(j);
                stats->switchesCovered.removeAt(j);
                stats->statesCoverage.removeAt(j);
                stats->transitionsCoverage.removeAt(j);
                stats->switchesCoverage.removeAt(j);
                stats->missingZeroSwitches.removeAt(j);
                stats->missingNSwitches.removeAt(j);

                break;
            }
        }
    }

    stats->assign(sortedStats);

    tests = sortedTests;

}
//--------------------------------------------------------------------------------------------------------------------------------

bool fsmRouter::higherCoverage(int a, int b)
{
    return (a > b);
}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::calculateStats()
{
    statsByTest stats;
    int statesCovPercent;
    int transitionsCovPercent;
    int switchesCovPercent;
    QList<int> swCovered;
    sequencesList missingSeqs;

    for(int i = 0; i < tests.sequences.size(); i++){
        stats.testIndexes.append(i);
        swCovered = switchesCovered(tests.sequences.at(i), fsmRouter::zeroSwitchesIndex);
        stats.switchesCovered.append(swCovered.size());
        switchesCovPercent = swCovered.size() * 100 / switchSequences.at(fsmRouter::zeroSwitchesIndex).sequences.size();
        missingSeqs.sequences.clear();

        for(int j = 0; j < switchSequences.at(fsmRouter::zeroSwitchesIndex).sequences.size(); j++ ){

            if (!swCovered.contains(j)){
                missingSeqs.sequences.append(switchSequences.at(fsmRouter::zeroSwitchesIndex).sequences.at(j));

            }
        }

        stats.missingZeroSwitches.append(missingSeqs);
        missingSeqs.sequences.clear();

        if (this->switchN > 0){

            if (this->switchN == 1){
                swCovered = switchesCovered(tests.sequences.at(i), fsmRouter::oneSwitchesIndex);

            }else{
                swCovered = switchesCovered(tests.sequences.at(i), fsmRouter::nSwitchesIndex);

            }

            missingSeqs.sequences.clear();

            for(int j = 0; j < switchSequences.at(fsmRouter::nSwitchesIndex).sequences.size(); j++ ){

                if (!swCovered.contains(j)){
                    missingSeqs.sequences.append(switchSequences.at(fsmRouter::nSwitchesIndex).sequences.at(j));

                }
            }
        }

        stats.missingNSwitches.append(missingSeqs);

        stats.switchesCoverage.append(QString::number(switchesCovPercent) + "%");
        statesCovPercent = statesCoveredCount(tests.sequences.at(i)) * 100 / structure.shapesInfo.size();
        stats.statesCoverage.append(QString::number(statesCovPercent) + "%");
        transitionsCovPercent = transitionsCoveredCount(tests.sequences.at(i)) * 100 / structure.connectorsInfo.size();
        stats.transitionsCoverage.append(QString::number(transitionsCovPercent) + "%");

    }

    sortByCoverage(&stats);
    testsStats = stats;
}
//--------------------------------------------------------------------------------------------------------------------------------

int fsmRouter::statesCoveredCount(switchSequence testSequence)
{
    QList<int> covIdsBuff;

    for (int i = 0; i < testSequence.switches.size(); i++){

        if (!covIdsBuff.contains(testSequence.switches.at(i).stateID)){
            covIdsBuff.append(testSequence.switches.at(i).stateID);

        }

    }

    return covIdsBuff.size();
}
//--------------------------------------------------------------------------------------------------------------------------------

int fsmRouter::transitionsCoveredCount(switchSequence testSequence)
{
    QList<int> covIdsBuff;

    for (int i = 0; i < testSequence.switches.size(); i++){

        if ((!covIdsBuff.contains(testSequence.switches.at(i).transitionID)) &&
                (testSequence.switches.at(i).transitionID) != 0){
            covIdsBuff.append(testSequence.switches.at(i).transitionID);

        }

    }

    return covIdsBuff.size();
}
//--------------------------------------------------------------------------------------------------------------------------------

QList<int> fsmRouter::switchesCovered(switchSequence route, int switchLength)
{
    QList<int> seqsCovered;
    switchSequence toCoverBuff;
    int matchCount;
    sequencesList sequencesToCover;

    if (switchLength == 0){
        sequencesToCover = switchSequences.at(fsmRouter::zeroSwitchesIndex);

    }else if (switchLength == 1){
        sequencesToCover = switchSequences.at(fsmRouter::oneSwitchesIndex);

    }else{
        sequencesToCover = switchSequences.at(fsmRouter::nSwitchesIndex);

    }

    for(int i = 0; i < sequencesToCover.sequences.size(); i++){
        toCoverBuff = sequencesToCover.sequences.at(i);
        matchCount = 0;

        for (int k = 0; k < route.switches.size(); k++){

            if(toCoverBuff.switches.at(matchCount).stateID == route.switches.at(k).stateID &&
                toCoverBuff.switches.at(matchCount).transitionID == route.switches.at(k).transitionID){
                matchCount++;

            }else{
                matchCount = 0;

            }

            if(matchCount == switchLength + 1){
                seqsCovered.append(i);
                break;

            }
        }
    }

    return seqsCovered;
}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::generateTests()
{
    sequencesList linkedSwitches;
    linkedSwitches = switchSequences.at(fsmRouter::nSwitchesIndex);
    int countBefore = linkedSwitches.sequences.size();
    int countAfter = 0;

    while (countBefore != countAfter){
        countBefore = linkedSwitches.sequences.size();
        linkedSwitches = interLink(linkedSwitches);
        countAfter = linkedSwitches.sequences.size();

    }

    tests = completeRoutes(linkedSwitches);
}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::loadFsmExtremes()
{
    for (int n = 0; n < structure.shapesInfo.size(); n++){

        if(structure.shapesInfo.at(n).shapeType == shapeTypes::initialState){
            initialStateID = structure.shapesInfo.at(n).ID;

        }

        if(structure.shapesInfo.at(n).shapeType == shapeTypes::finalState){
            finalSwitch.stateID = structure.shapesInfo.at(n).ID;
            finalSwitch.stateText = structure.shapesInfo.at(n).text;
            finalSwitch.transitionID = 0;
            finalSwitch.transitionText = "";

        }
    }

    for (int n = 0; n < structure.connectorsInfo.size(); n++){

        if(structure.connectorsInfo.at(n).toID == finalSwitch.stateID){
            finalTransitions.append(structure.connectorsInfo.at(n).ID);

        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

switchSequence fsmRouter::tryToRouteHeading(fsmRouter::directionFrom direction, switchSequence routeToComplete, switchSequence nSwitchSequence){

    if (direction == fsmRouter::startToSequence){

        if (routeToComplete.switches.first().stateID == initialStateID){
            return routeToComplete;

        }else{
            return coupleInOrder(nSwitchSequence, routeToComplete);

        }

    }else if(direction == fsmRouter::sequenceToEnd){

        for (int i = 0; i < finalTransitions.size(); i++){

            if (routeToComplete.switches.last().transitionID == finalTransitions.at(i)){
               return routeToComplete;

            }
        }

        return coupleInOrder(routeToComplete, nSwitchSequence);

    }

    switchSequence emptySequence;
    return emptySequence;
}
//--------------------------------------------------------------------------------------------------------------------------------

switchSequence fsmRouter::getHigherCoverageRoute(sequencesList routes){
    QList<int> indexesCovered;
    int maxHit = 0;
    switchSequence bestRoute;

    for(int i = 0; i < routes.sequences.size(); i++){
        indexesCovered = switchesCovered(routes.sequences.at(i), switchN);

        if(indexesCovered.size() > maxHit){
            maxHit = indexesCovered.size();
            bestRoute = routes.sequences.at(i);
        }
    }

    return bestRoute;
}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::completeRoutes(sequencesList sequencesToRoute)
{
    switchSequence routeBuffer;
    sequencesList routedSwitches;

    for(int i = 0; i < sequencesToRoute.sequences.size(); i++){

        if(!isWithin(sequencesToRoute.sequences.at(i), &routedSwitches)){
            routeBuffer = getBestRouteFrom(fsmRouter::startToSequence,
                                           sequencesToRoute.sequences.at(i),
                                           sequencesToRoute);
            routeBuffer = getBestRouteFrom(fsmRouter::sequenceToEnd,
                                           routeBuffer,
                                           sequencesToRoute);

            if (routeBuffer.switches.size() > 0){
                addIfMissing(routeBuffer, &routedSwitches);

            }else{ //No possible rounte to end has been found (Diagram is not a FSM)
                routedSwitches.sequences.clear();
                return routedSwitches;

            }

        }
    }

    sequencesList optimalRoutes = leaveOptimalRoutes(routedSwitches);
    addFinalStateTo(&optimalRoutes);

    return optimalRoutes;

}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::leaveOptimalRoutes(sequencesList routes){
    QList<int> idxsBuff;
    QList<QList<int> > coveredIndexesByRoute;
    QList<int> idxs1;
    QList<int> idxs2;
    QList<int> containedRoutesIndexes;
    sequencesList output;
    int containedArgument;

    for (int i = 0; i < routes.sequences.size(); i++){
        idxsBuff = switchesCovered(routes.sequences.at(i), switchN);
        coveredIndexesByRoute.append(idxsBuff);

    }

    for(int i = 0; i < coveredIndexesByRoute.size(); i++){
        idxs1 = coveredIndexesByRoute.at(i);

        for(int j = i + 1; j < coveredIndexesByRoute.size(); j++){
            idxs2 = coveredIndexesByRoute.at(j);
            containedArgument = getContained(idxs1, idxs2);

            if(containedArgument == 1){
                containedRoutesIndexes.append(i);
                break;

            }else if(containedArgument == 2){
                containedRoutesIndexes.append(j);
                break;

            }
        }
    }

    for (int i = 0; i < routes.sequences.size(); i++){

        if (!containedRoutesIndexes.contains(i)){
            output.sequences.append(routes.sequences.at(i));

        }
    }

    return output;

}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::addFinalStateTo(sequencesList *linkedSequences)
{
    switchSequence sequenceBuffer;

    for(int i = 0; i < linkedSequences->sequences.size(); i++){
        sequenceBuffer = linkedSequences->sequences.at(i);
        sequenceBuffer.switches.append(finalSwitch);
        linkedSequences->sequences.replace(i, sequenceBuffer);

    }

}
//--------------------------------------------------------------------------------------------------------------------------------

int fsmRouter::getContained(QList<int> argument1, QList<int> argument2){
    int matchCount = 0;

    for (int i = 0; i < argument1.size(); i++){

        for (int j = 0; j < argument2.size(); j++){

            if (argument1.at(i) == argument2.at(j)){
                matchCount++;

                if(matchCount == argument1.size()){
                    return 1;

                }else if(matchCount == argument2.size()){
                    return 2;

                }
            }
        }
    }

    return 0;

}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::extractRoutesHeading(fsmRouter::directionFrom direction, sequencesList allRoutes){

    if (direction == fsmRouter::startToSequence){
        return startingRoutesIn(allRoutes);

    }else if(direction == fsmRouter::sequenceToEnd){
        return endingRoutesIn(allRoutes);

    }

    sequencesList emptyList;
    return emptyList;
}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::endingRoutesIn(sequencesList routes){
    sequencesList endRoutes;
    switchSequence seqBuff;

    if (routes.sequences.size() > 0){

        for(int n = 0; n < routes.sequences.size(); n++){
            seqBuff = routes.sequences.at(n);

            if (seqBuff.switches.size() > 0){

                for (int m = 0; m < structure.connectorsInfo.size(); m++){

                    if((structure.connectorsInfo.at(m).toID == finalSwitch.stateID) &&
                            (seqBuff.switches.last().transitionID == structure.connectorsInfo.at(m).ID)){
                        endRoutes.sequences.append(seqBuff);

                    }
                }
            }
        }
    }

    return endRoutes;
}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::startingRoutesIn(sequencesList routes){
    sequencesList startRoutes;
    switchSequence seqBuff;

    if (routes.sequences.size() > 0){

        for(int n = 0; n < routes.sequences.size(); n++){
            seqBuff = routes.sequences.at(n);

            if(seqBuff.switches.first().stateID == initialStateID){
                startRoutes.sequences.append(seqBuff);

            }
        }
    }

    return startRoutes;
}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::addIfMissing(switchSequence route, sequencesList *routeList){

    if((route.switches.size() > 0) && (!isWithin(route, routeList))){
        routeList->sequences.append(route);

    }
}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::interLink(sequencesList sequencesToLink)
{
    bool stillLinking;
    switchSequence switchChain;
    sequencesList linkedSws;
    long idxA;
    long idxB;
    stillLinking = true;

    while(stillLinking){
        //Try to link switches between them
        stillLinking = false;

        for (long i = 0; i < sequencesToLink.sequences.size(); i++){

            for (long j = 0; j < sequencesToLink.sequences.size(); j++){
                //Do not link between the same
                if(i != j){
                    switchChain = tryToCouple(sequencesToLink.sequences.at(i), sequencesToLink.sequences.at(j));

                    if(switchChain.switches.size() > 0){
                        appendIfMissing(switchChain, &linkedSws);
                        stillLinking = true;
                        idxA = i;
                        idxB = j;
                        break;

                    }
                }
            }

            if(stillLinking){
                break;
            }
        }

        if(stillLinking){

            if( idxA > idxB){
                sequencesToLink.sequences.removeAt(idxA);
                sequencesToLink.sequences.removeAt(idxB);

            }else{
                sequencesToLink.sequences.removeAt(idxB);
                sequencesToLink.sequences.removeAt(idxA);

            }
        }
    }

    //Add the switches left without direct connection
    completeFrom(&sequencesToLink, &linkedSws);

   return linkedSws;
}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::completeFrom(sequencesList *getFrom, sequencesList *addTo)
{
    bool isMissing;
    switchSequence seqBuff;

    for(int i = 0; i < getFrom->sequences.size(); i++){
        isMissing = true;

        for(int j = 0; j < addTo->sequences.size(); j++){
            seqBuff = getFrom->sequences.at(i);

            if(isWithin(seqBuff, addTo)){
                isMissing = false;
                break;
            }
        }

        if(isMissing){
            addTo->sequences.append(getFrom->sequences.at(i));

        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

bool fsmRouter::isWithin(switchSequence singleSequence, sequencesList *Sequences)
{
    switchSequence targetBuffer;
    int currentIndex;

    for(int i = 0; i < Sequences->sequences.size(); i++){
        targetBuffer = Sequences->sequences.at(i);
        currentIndex = 0;

        for(int j = 0; j < targetBuffer.switches.size(); j++){

            if(targetBuffer.switches.at(j).stateID == singleSequence.switches.at(currentIndex).stateID &&
                targetBuffer.switches.at(j).transitionID == singleSequence.switches.at(currentIndex).transitionID){
                currentIndex++;

            }else{
                currentIndex = 0;

            }

            if(currentIndex == singleSequence.switches.size()){
                return true;

            }
        }
    }

    return false;
}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::appendIfMissing(switchSequence singleSequence, sequencesList *Sequences)
{

    if(singleSequence.switches.size() > 0 && isWithin(singleSequence, Sequences) == false){
        Sequences->sequences.append(singleSequence);

    }

}
//--------------------------------------------------------------------------------------------------------------------------------

switchSequence fsmRouter::coupleInOrder(switchSequence sequenceA, switchSequence sequenceB)
{
    switchSequence output;
    sequencesList oneSwitches = switchSequences.at(fsmRouter::oneSwitchesIndex);
    switchSequence validSequence;

    if((sequenceA.switches.size() > 0) && (sequenceB.switches.size() > 0)){
        testSwitch aTail = sequenceA.switches.last();
        testSwitch bHead = sequenceB.switches.first();

        for(int i = 0; i < oneSwitches.sequences.size(); i++){
            validSequence = oneSwitches.sequences.at(i);

            if(aTail.stateID == validSequence.switches.first().stateID &&
               aTail.transitionID == validSequence.switches.first().transitionID &&
               bHead.stateID == validSequence.switches.last().stateID &&
               bHead.transitionID == validSequence.switches.last().transitionID){

                output.switches.append(sequenceA.switches);
                output.switches.append(sequenceB.switches);
                return output;

            }
        }
    }

    return output;
}
//--------------------------------------------------------------------------------------------------------------------------------

switchSequence fsmRouter::tryToCouple(switchSequence sequenceA, switchSequence sequenceB)
{
    switchSequence output = coupleInOrder(sequenceA, sequenceB);

    if(output.switches.size() == 0){
        output = coupleInOrder(sequenceB, sequenceA);

    }

    return output;

}
//--------------------------------------------------------------------------------------------------------------------------------

QList<sequencesList> fsmRouter::getSwitchesBuffer()
{
    sequencesList zeroSwitches;

    for(int i = 0; i < structure.connectorsInfo.size(); i++){
        testSwitch swBuff;
        swBuff.stateID = structure.connectorsInfo.at(i).fromID;
        swBuff.stateText = getTextFrom(swBuff.stateID);
        swBuff.transitionID = structure.connectorsInfo.at(i).ID;
        swBuff.transitionText = getTextFrom(swBuff.transitionID);
        switchSequence sequence;
        sequence.switches.append(swBuff);
        zeroSwitches.sequences.append(sequence);

    }

    sequencesList oneSwitches = getSwitchesFrom(zeroSwitches, 1);
    sequencesList nSwitches = getSwitchesFrom(zeroSwitches, switchN);

    QList<sequencesList> output;
    output.append(zeroSwitches);
    output.append(oneSwitches);
    output.append(nSwitches);

    return output;
}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::getSwitchesFromState(int stateID)
{
    sequencesList output;

    for(int i = 0; i < structure.connectorsInfo.size(); i++){
        switchSequence seqBuff;

        if (structure.connectorsInfo.at(i).fromID == stateID){
            testSwitch swBuff;
            swBuff.stateID = stateID;
            swBuff.stateText = getTextFrom(swBuff.stateID);
            swBuff.transitionID = structure.connectorsInfo.at(i).ID;
            swBuff.transitionText = getTextFrom(swBuff.transitionID);
            seqBuff.switches.append(swBuff);
            output.sequences.append(seqBuff);

        }
    }

    return output;
}
//--------------------------------------------------------------------------------------------------------------------------------

int fsmRouter::getStateAfterSequence(switchSequence sequence)
{
    testSwitch lastSwitch = sequence.switches.last();

    for(int i = 0; i < structure.connectorsInfo.size(); i++){

        if (structure.connectorsInfo.at(i).ID == lastSwitch.transitionID){
            return structure.connectorsInfo.at(i).toID;

        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::getSwitchesAfter(switchSequence sequence)
{
    int stateAfter = getStateAfterSequence(sequence);

    return getSwitchesFromState(stateAfter);

}
//--------------------------------------------------------------------------------------------------------------------------------

sequencesList fsmRouter::getSwitchesFrom(sequencesList fromSwitches, int length)
{
    sequencesList output;

    if(length > 0){
        sequencesList switchBuffer = fromSwitches;

        for(int i = 0; i < length; i++){
            output.sequences.clear();

            for(int j = 0; j < switchBuffer.sequences.size(); j++){
                switchSequence baseSequence = switchBuffer.sequences.at(j);
                sequencesList nextSequences = getSwitchesAfter(baseSequence);

                for(int k = 0; k < nextSequences.sequences.size(); k++){
                    switchSequence extendedSequence = baseSequence;
                    switchSequence nextSwitches = nextSequences.sequences.at(k);
                    extendedSequence.switches.append(nextSwitches.switches);
                    output.sequences.append(extendedSequence);

                }
            }

            switchBuffer = output;

        }

    }else{
        output = fromSwitches;

    }

    return output;
}
//--------------------------------------------------------------------------------------------------------------------------------

QString fsmRouter::getTextFrom(int ID)
{
    for(int i = 0; i < structure.shapesInfo.size(); i++){

        if(ID == structure.shapesInfo.at(i).ID){
            return structure.shapesInfo.at(i).text;

        }
    }

    for(int i = 0; i < structure.connectorsInfo.size(); i++){

        if(ID == structure.connectorsInfo.at(i).ID){
            return structure.connectorsInfo.at(i).text;

        }
    }

    return "";
}
//--------------------------------------------------------------------------------------------------------------------------------

void fsmRouter::showDebugInfo(sequencesList Sequences)
{
    QString debugInfo;

    for (int j  = 0; j < Sequences.sequences.size(); j++){
        switchSequence sws = Sequences.sequences.at(j);

        debugInfo.append(QString::number(j) + ": ");

        for (int k = 0; k < sws.switches.size(); k++){
            debugInfo.append(sws.switches.at(k).stateText + sws.switches.at(k).transitionText);

        }

        debugInfo.append("\n");
    }

    QMessageBox msg(QMessageBox::Warning, "Debug", debugInfo);
    msg.exec();
}
//--------------------------------------------------------------------------------------------------------------------------------

void statsByTest::assign(statsByTest replacement)
{
    testIndexes = replacement.testIndexes;
    switchesCovered = replacement.switchesCovered;
    statesCoverage = replacement.statesCoverage;
    transitionsCoverage = replacement.transitionsCoverage;
    switchesCoverage = replacement.switchesCoverage;
    missingZeroSwitches = replacement.missingZeroSwitches;
    missingNSwitches = replacement.missingNSwitches;
}
