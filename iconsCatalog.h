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

#ifndef ICONSCATALOG_H
#define ICONSCATALOG_H

#include <QtWidgets>
#include <QIcon>
#include <QObject>
#include <QXmlDefaultHandler>

class iconsCatalog
{
public:
    iconsCatalog();
    void assignIcon(QTreeWidgetItem *item);
    QIcon getIconFor(QString elementName, QString itemStatus);

    QIcon andIcon;
    QIcon addIcon;
    QIcon addTopIcon;
    QIcon copyIcon;
    QIcon cutIcon;
    QIcon componentPendingIcon;
    QIcon componentPlannedIcon;
    QIcon componentValidatedIcon;
    QIcon componentReviewIcon;
    QIcon copyBranchIcon;
    QIcon coverIcon;
    QIcon clearHighlightsIcon;
    QIcon combineIcon;
    QIcon deleteIcon;
    QIcon downIcon;
    QIcon designerIcon;
    QIcon fillIcon;
    QIcon failedIcon;
    QIcon findFailedIcon;
    QIcon findPassedIcon;
    QIcon findPendingIcon;
    QIcon findPlannedIcon;
    QIcon findReviewIcon;
    QIcon findUnsupportedIcon;
    QIcon htmlIcon;
    QIcon leftIcon;
    QIcon modulePendingIcon;
    QIcon modulePlannedIcon;
    QIcon moduleValidatedIcon;
    QIcon moduleReviewIcon;
    QIcon missingIcon;
    QIcon openIcon;
    QIcon pictureIcon;
    QIcon pickColumnHeadersIcon;
    QIcon pickRowHeadersIcon;
    QIcon pendingIcon;
    QIcon plannedIcon;
    QIcon pickCollectionIcon;
    QIcon pathIcon;
    QIcon pasteIcon;    
    QIcon pickIcon;
    QIcon rightIcon;
    QIcon randomIcon;
    QIcon reviewIcon;
    QIcon statsIcon;
    QIcon saveIcon;
    QIcon saveAsIcon;
    QIcon searchIcon;
    QIcon testGroupIcon;
    QIcon testCollectionIcon;
    QIcon testCombinationIcon;
    QIcon testSequenceIcon;
    QIcon testCrossCheckerIcon;
    QIcon unsupportedIcon;
    QIcon undoIcon;
    QIcon upIcon;
    QIcon validatedIcon;

private:

};

#endif // ICONSCATALOG_H
