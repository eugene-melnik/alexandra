/*************************************************************************************************
 *                                                                                                *
 *  file: effectdropshadow.cpp                                                                    *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
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

#include "effectdropshadow.h"

EffectDropShadow::EffectDropShadow( QObject* parent )
    : QGraphicsDropShadowEffect( parent )
{
    this->setColor( QColor( "black" ) );
    this->setBlurRadius( 5 );
    this->setOffset( 3, 3 );
}

EffectDropShadow::EffectDropShadow( qreal offsetX, qreal offsetY, const QColor& color, QObject* parent )
    : QGraphicsDropShadowEffect( parent )
{
    this->setOffset( offsetX, offsetY );
    this->setBlurRadius( 5 );
    this->setColor( QColor(color) );
}

EffectDropShadow::EffectDropShadow( qreal offsetX, qreal offsetY, qreal blurRadius, const QColor& color, QObject *parent )
    : QGraphicsDropShadowEffect( parent )
{
    this->setOffset( offsetX, offsetY );
    this->setBlurRadius( blurRadius );
    this->setColor( QColor(color) );
}
