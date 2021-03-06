
/***************************************************************************
                              qgshostedrdsbuilder.cpp
                              -----------------------
  begin                : July, 2008
  copyright            : (C) 2008 by Marco Hugentobler
  email                : marco dot hugentobler at karto dot baug dot ethz dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgshostedrdsbuilder.h"
#include "qgslogger.h"
#include "qgsmslayercache.h"
#include "qgsrasterlayer.h"
#include "qgscoordinatereferencesystem.h"

#include <QDomElement>

QgsHostedRDSBuilder::QgsHostedRDSBuilder(): QgsMSLayerBuilder()
{

}

QgsMapLayer *QgsHostedRDSBuilder::createMapLayer( const QDomElement &elem,
    const QString &layerName,
    QList<QTemporaryFile *> &filesToRemove,
    QList<QgsMapLayer *> &layersToRemove,
    bool allowCaching ) const
{
  Q_UNUSED( filesToRemove );

  if ( elem.isNull() )
  {
    return nullptr;
  }

  QString uri = elem.attribute( QStringLiteral( "uri" ), QStringLiteral( "not found" ) );
  if ( uri == QLatin1String( "not found" ) )
  {
    QgsDebugMsg( "Uri not found" );
    return nullptr;
  }
  else
  {
    QgsDebugMsg( "Trying to get hostedrds layer from cache with uri: " + uri );
    QgsRasterLayer *rl = nullptr;
    if ( allowCaching )
    {
      rl = qobject_cast<QgsRasterLayer *>( QgsMSLayerCache::instance()->searchLayer( uri, layerName ) );
    }
    if ( !rl || !rl->isValid() )
    {
      QgsDebugMsg( "hostedrds layer not in cache, so create and insert it" );
      rl = new QgsRasterLayer( uri, layerNameFromUri( uri ) );
      if ( allowCaching )
      {
        QgsMSLayerCache::instance()->insertLayer( uri, layerName, rl );
      }
      else
      {
        layersToRemove.push_back( rl );
      }
    }

    clearRasterSymbology( rl );

    //projection
    if ( rl )
    {
      QString epsg = elem.attribute( QStringLiteral( "epsg" ) );
      if ( !epsg.isEmpty() )
      {
        bool conversionOk;
        int epsgnr = epsg.toInt( &conversionOk );
        if ( conversionOk )
        {
          //set spatial ref sys
          QgsCoordinateReferenceSystem srs = QgsCoordinateReferenceSystem::fromOgcWmsCrs( QStringLiteral( "EPSG:%1" ).arg( epsgnr ) );
          rl->setCrs( srs );
        }
      }
    }

    return rl;
  }
}
