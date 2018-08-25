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

#include "iconsCatalog.h"
#include "testCAD.h"

iconsCatalog::iconsCatalog()
{
    componentPendingIcon = QIcon(":/icons/componentPending.png");
    componentPlannedIcon = QIcon(":/icons/planned.png");
    componentValidatedIcon = QIcon(":/icons/componentValidated.png");
    componentReviewIcon = QIcon(":/icons/componentReview.png");
    modulePendingIcon = QIcon(":/icons/modulePending.png");
    modulePlannedIcon = QIcon(":/icons/planned.png");
    moduleValidatedIcon = QIcon(":/icons/moduleValidated.png");
    moduleReviewIcon= QIcon(":/icons/moduleReview.png");
    pendingIcon = QIcon(":/icons/pending.png");
    plannedIcon = QIcon(":/icons/planned.png");
    validatedIcon = QIcon(":/icons/validated.png");
    reviewIcon = QIcon(":/icons/review.png");
    failedIcon = QIcon(":/icons/failed.png");
    missingIcon = QIcon(":/icons/missingIcon.png");
    unsupportedIcon = QIcon(":/icons/unsupported.png");
    saveIcon = QIcon(":/icons/saveIcon.png");
    saveAsIcon = QIcon(":/icons/saveAsIcon.png");
    copyBranchIcon = QIcon(":/icons/copyBranchIcon.png");
    pathIcon = QIcon(":/icons/pathIcon.png");
    copyIcon = QIcon(":/icons/copyIcon.png");
    pasteIcon = QIcon(":/icons/pasteIcon.png");
    cutIcon = QIcon(":/icons/cutIcon.png");
    findFailedIcon = QIcon(":/icons/findFailedIcon.png");
    findPassedIcon = QIcon(":/icons/findPassedIcon.png");
    findPendingIcon = QIcon(":/icons/findPendingIcon.png");
    findPlannedIcon = QIcon(":/icons/findPlannedIcon.png");
    findReviewIcon = QIcon(":/icons/findReviewIcon.png");
    findUnsupportedIcon = QIcon(":/icons/findUnsupportedIcon.png");
    searchIcon =QIcon(":/icons/search.png");
    statsIcon = QIcon(":/icons/statsIcon.png");
    deleteIcon = QIcon(":/icons/deleteIcon.png");
    undoIcon = QIcon(":/icons/undoIcon.png");
    upIcon = QIcon(":/icons/upIcon.png");
    downIcon = QIcon(":/icons/downIcon.png");
    addIcon = QIcon(":/icons/addIcon.png");
    addTopIcon = QIcon(":/icons/addTopIcon.png");
    testGroupIcon = QIcon(":/icons/testGroupIcon.png");
    testCombinationIcon = QIcon(":/icons/combinationIcon.png");
    testSequenceIcon = QIcon(":/icons/sequenceIcon.png");
    leftIcon = QIcon(":/icons/leftIcon.png");
    rightIcon = QIcon(":/icons/rightIcon.png");
    pickIcon = QIcon(":/icons/pickIcon.png");
    combineIcon = QIcon(":/icons/combineIcon.png");
    randomIcon = QIcon(":/icons/randomIcon.png");
    fillIcon = QIcon(":/icons/fillIcon.png");
    randomIcon = QIcon(":/icons/randomIcon.png");
    htmlIcon = QIcon(":/icons/htmlIcon.png");
    designerIcon = QIcon(":/icons/designerIcon.png");
    openIcon = QIcon(":/icons/openIcon.png");
    testCrossCheckerIcon = QIcon(":/icons/crossCheckerIcon.png");
    pickColumnHeadersIcon = QIcon(":/icons/pickColHeadersIcon.png");
    pickRowHeadersIcon = QIcon(":/icons/pickRowHeadersIcon.png");
    coverIcon = QIcon(":/icons/coverIcon.png");
    clearHighlightsIcon = QIcon(":/icons/clearHighlightsIcon.png");
    andIcon = QIcon(":/icons/andIcon.png");
    testCollectionIcon = QIcon(":/icons/collectionIcon.png");
    pickCollectionIcon = QIcon(":/icons/pickCollectionIcon.png");
    pictureIcon = QIcon(":/icons/pictureIcon.png");

}
//----------------------------------------------------------------------------------------------------

void iconsCatalog::assignIcon(QTreeWidgetItem *item)
{
    QVariant elementName = item->data(0,Qt::UserRole);
    QVariant itemStatus = item->data(1,Qt::UserRole);
    QIcon iconToSet = getIconFor(elementName.toString(), itemStatus.toString());

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setIcon(0, iconToSet);

}
//----------------------------------------------------------------------------------------------------

QIcon iconsCatalog::getIconFor(QString elementName, QString itemStatus)
{
    if (elementName == TAG_TYPE_ENTRY){
        if (itemStatus == STRING_STATUS_PENDING){
             return pendingIcon;

        }else if (itemStatus == STRING_STATUS_PLANNED){
             return plannedIcon;

        }else if (itemStatus == STRING_STATUS_VALIDATED){
             return validatedIcon;

        }else if (itemStatus == STRING_STATUS_REVIEW){
             return reviewIcon;

        }else if (itemStatus == STRING_STATUS_FAILED){
             return failedIcon;

        }else if (itemStatus == STRING_STATUS_UNSUPPORTED){
             return unsupportedIcon;

        }
    }else if (elementName == TAG_TYPE_COMPONENT){
        if (itemStatus == STRING_STATUS_PENDING){
             return componentPendingIcon;

        }else if (itemStatus == STRING_STATUS_PLANNED){
            return componentPlannedIcon;

        }else if (itemStatus == STRING_STATUS_VALIDATED){
            return componentValidatedIcon;

        }else if (itemStatus == STRING_STATUS_REVIEW){
            return componentReviewIcon;

        }else if (itemStatus == STRING_STATUS_FAILED){
            return failedIcon;

        }else if (itemStatus == STRING_STATUS_UNSUPPORTED){
            return unsupportedIcon;

        }
     }else if (elementName == TAG_TYPE_MODULE){
        if (itemStatus == STRING_STATUS_PENDING){
             return modulePendingIcon;

        }else if (itemStatus == STRING_STATUS_PLANNED){
            return modulePlannedIcon;

        }else if (itemStatus == STRING_STATUS_VALIDATED){
            return moduleValidatedIcon;

        }else if (itemStatus == STRING_STATUS_REVIEW){
            return moduleReviewIcon;

        }else if (itemStatus == STRING_STATUS_FAILED){
            return failedIcon;

        }else if (itemStatus == STRING_STATUS_UNSUPPORTED){
            return unsupportedIcon;

        }
     }else if (elementName == TAG_TYPE_TEST_GROUP){
        return testGroupIcon;

     }else if (elementName == TAG_TYPE_TEST_COLLECTION){
        return testCollectionIcon;

     }else if (elementName == TAG_TYPE_TEST_COMBINATION){
        return testCombinationIcon;

     }else if (elementName == TAG_TYPE_TEST_SEQUENCE){
        return testSequenceIcon;

     }else if (elementName == TAG_TYPE_TEST_CROSSCHECK){
        return testCrossCheckerIcon;

     }else{
        return missingIcon;

    }

    return missingIcon;
}



