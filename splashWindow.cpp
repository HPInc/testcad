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

#include "splashWindow.h"

splashWindow::splashWindow(QWidget *parent) : QMainWindow(parent)
{
    QGraphicsScene *gsc = new QGraphicsScene;
    gsc->addPixmap(QPixmap(":/images/splashImage.png"));

    QGraphicsTextItem *label = gsc->addText(STRING_AUTHORS, QFont("Arial",8,QFont::Normal));
    label->setPos(20,236);
    QString labelText("Version:");
    labelText.append(" ");
    labelText.append(STRING_VERSION_NUMBER);
    label = gsc->addText(labelText, QFont("Arial",10,QFont::Bold));
    label->setPos(460,330);

    QGraphicsView *gvw = new QGraphicsView();
    gvw->setScene(gsc);
    gvw->resize(gsc->width(),gsc->height());
    gvw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gvw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setCentralWidget(gvw);
    this->resize(gsc->width(),gsc->height());
    this->setFixedSize(this->size());
    this->setWindowFlags(Qt::FramelessWindowHint);
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(closeSplash()));
    timer->start(3000);

}

void splashWindow::closeSplash()
{
    this->close();

}

