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

#ifndef TREESAVER_H
#define TREESAVER_H

#include <QXmlStreamWriter>
#include <QByteArray>
#include <QtWidgets>
#include "testCAD.h"
#include "sequencer/sequencerWindow.h"

#include <QTextStream>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class treeSaver
{
public:
    treeSaver(QTreeWidget *treeWidget, QWidget *parent);

    QString filePath;
    bool saveTreeAs();
    bool saveTreeTo(QString fileName);
    bool exportTreeToImage();

private:
    bool writeTreeTo(QIODevice *device);
    void addItemToStream(QTreeWidgetItem *item);
    void addCollectionDesignToStream(QTreeWidgetItem *item);
    void addCombinationDesignToStream(QTreeWidgetItem *item);
    void addSequenceDesignToStream(QTreeWidgetItem *item);
    void addCrossCheckDesignToStream(QTreeWidgetItem *item);

    QTreeWidget *treeWidget;
    QWidget *parentWidget;
    QXmlStreamWriter *xmlWriter;

};

#endif
