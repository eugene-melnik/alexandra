/*************************************************************************************************
 *                                                                                                *
 *  file: effectdropshadow.h                                                                      *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
 *                                                                                                *
 *  Alexandra is free software; you can redistribute it and/or modify it under the terms of the   *
 *  GNU General Public License as published by the Free Software Foundation; either version 2 of  *
 *  the License, or (at your option) any later version.                                           *
 *                                                                                                *
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 *  See the GNU General Public License for more details.                                          *
 *                                                                                                *
 *  You should have received a copy of the GNU General Public License along with this program.    *
 *  If not, see <http://www.gnu.org/licenses/>.                                                   *
 *                                                                                                *
  *************************************************************************************************/

#ifndef EFFECTDROPSHADOW_H
#define EFFECTDROPSHADOW_H

#include <QColor>
#include <QGraphicsDropShadowEffect>

class EffectDropShadow : public QGraphicsDropShadowEffect
{
    public:
        EffectDropShadow( QObject* parent = nullptr );

        EffectDropShadow( qreal offsetX,
                          qreal offsetY,
                          const QColor& color = QColor( "black" ),
                          QObject* parent = nullptr );
};

#endif // EFFECTDROPSHADOW_H
