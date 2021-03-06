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

#include "trigonometry.h"

Trigonometry::Trigonometry()
{

}
//--------------------------------------------------------------------------------------------------------------------------------

Trigonometry::~Trigonometry()
{

}
//--------------------------------------------------------------------------------------------------------------------------------

qreal Trigonometry::getDistance(QPointF FirstPoint, QPointF SecondPoint)
{
    qreal dx = FirstPoint.x() - SecondPoint.x();
    qreal dy = FirstPoint.y() - SecondPoint.y();

    return sqrt( pow(dx,2) + pow(dy,2) );
}
//--------------------------------------------------------------------------------------------------------------------------------

qreal Trigonometry::getDegreesClockWise(QPointF FirstPoint, QPointF SecondPoint)
{
    qreal dx = SecondPoint.x() - FirstPoint.x();
    qreal dy = SecondPoint.y() - FirstPoint.y();

    if (dx < 0)
    {
       return qRadiansToDegrees( qAtan(dy/dx) ) + 180;

    }else {
       return qRadiansToDegrees( qAtan(dy/dx) );

    }
}
//--------------------------------------------------------------------------------------------------------------------------------

QPointF Trigonometry::getPointFromPolar(QPointF FromPoint, qreal AngleDegreesClockWise, qreal CordLength)
{
    qreal dy = CordLength * qCos( AngleDegreesClockWise * M_PI/180.0 );
    qreal dx = qTan( AngleDegreesClockWise * M_PI/180.0 ) * dy;

    qreal newX = FromPoint.x() + dx;
    qreal newY = FromPoint.y() - dy;

    return QPointF(newX, newY);
}
