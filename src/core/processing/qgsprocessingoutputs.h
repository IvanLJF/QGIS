/***************************************************************************
                         qgsprocessingoutputs.h
                         -------------------------
    begin                : May 2017
    copyright            : (C) 2017 by Nyall Dawson
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSPROCESSINGOUTPUTS_H
#define QGSPROCESSINGOUTPUTS_H

#include "qgis_core.h"
#include "qgis.h"
#include "qgsprocessingparameters.h"

//
// Output definitions
//

/**
 * \class QgsProcessingOutputDefinition
 * \ingroup core
 *
 * Base class for the definition of processing outputs.
 *
 * Output definitions encapsulate the properties regarding the outputs from algorithms, such
 * as generated layers or calculated values.
 *
 * \since QGIS 3.0
 */

class CORE_EXPORT QgsProcessingOutputDefinition
{

#ifdef SIP_RUN
    SIP_CONVERT_TO_SUBCLASS_CODE
    if ( sipCpp->type() == QgsProcessingOutputVectorLayer::typeName() )
      sipType = sipType_QgsProcessingOutputVectorLayer;
    else if ( sipCpp->type() == QgsProcessingOutputRasterLayer::typeName() )
      sipType = sipType_QgsProcessingOutputRasterLayer;
    else if ( sipCpp->type() == QgsProcessingOutputHtml::typeName() )
      sipType = sipType_QgsProcessingOutputHtml;
    else if ( sipCpp->type() == QgsProcessingOutputNumber::typeName() )
      sipType = sipType_QgsProcessingOutputNumber;
    else if ( sipCpp->type() == QgsProcessingOutputString::typeName() )
      sipType = sipType_QgsProcessingOutputString;
    else if ( sipCpp->type() == QgsProcessingOutputFolder::typeName() )
      sipType = sipType_QgsProcessingOutputFolder;
    SIP_END
#endif

  public:

    /**
     * Constructor for QgsProcessingOutputDefinition.
     */
    QgsProcessingOutputDefinition( const QString &name, const QString &description = QString() );

    virtual ~QgsProcessingOutputDefinition() = default;

    /**
     * Unique output type name.
     */
    virtual QString type() const = 0;

    /**
     * Returns the name of the output. This is the internal identifier by which
     * algorithms access this output.
     * @see setName()
     */
    QString name() const { return mName; }

    /**
     * Sets the \a name of the output. This is the internal identifier by which
     * algorithms access this output.
     * @see name()
     */
    void setName( const QString &name ) { mName = name; }

    /**
     * Returns the description for the output. This is the user-visible string
     * used to identify this output.
     * @see setDescription()
     */
    QString description() const { return mDescription; }

    /**
     * Sets the \a description for the output. This is the user-visible string
     * used to identify this output.
     * @see description()
     */
    void setDescription( const QString &description ) { mDescription = description; }

  protected:

    //! Output name
    QString mName;

    //! Output description
    QString mDescription;

};

//! List of processing parameters
typedef QList< const QgsProcessingOutputDefinition * > QgsProcessingOutputDefinitions;

/**
 * \class QgsProcessingOutputVectorLayer
 * \ingroup core
 * A vector layer output for processing algorithms.
  * \since QGIS 3.0
 */
class CORE_EXPORT QgsProcessingOutputVectorLayer : public QgsProcessingOutputDefinition
{
  public:

    /**
     * Constructor for QgsProcessingOutputVectorLayer.
     */
    QgsProcessingOutputVectorLayer( const QString &name, const QString &description = QString(), QgsProcessing::LayerType type = QgsProcessing::TypeVectorAny );

    /**
     * Returns the type name for the output class.
     */
    static QString typeName() { return QStringLiteral( "outputVector" ); }
    QString type() const override { return typeName(); }

    /**
     * Returns the layer type for the output layer.
     * \see setDataType()
     */
    QgsProcessing::LayerType dataType() const;

    /**
     * Sets the layer \a type for the output layer.
     * \see dataType()
     */
    void setDataType( QgsProcessing::LayerType type );

  private:

    QgsProcessing::LayerType mDataType = QgsProcessing::TypeVectorAny;
};

/**
 * \class QgsProcessingOutputRasterLayer
 * \ingroup core
 * A raster layer output for processing algorithms.
  * \since QGIS 3.0
 */
class CORE_EXPORT QgsProcessingOutputRasterLayer : public QgsProcessingOutputDefinition
{
  public:

    /**
     * Constructor for QgsProcessingOutputRasterLayer.
     */
    QgsProcessingOutputRasterLayer( const QString &name, const QString &description = QString() );

    /**
     * Returns the type name for the output class.
     */
    static QString typeName() { return QStringLiteral( "outputRaster" ); }
    QString type() const override { return typeName(); }


};

/**
 * \class QgsProcessingOutputHtml
 * \ingroup core
 * A HTML file output for processing algorithms.
  * \since QGIS 3.0
 */
class CORE_EXPORT QgsProcessingOutputHtml : public QgsProcessingOutputDefinition
{
  public:

    /**
     * Constructor for QgsProcessingOutputHtml.
     */
    QgsProcessingOutputHtml( const QString &name, const QString &description = QString() );

    /**
     * Returns the type name for the output class.
     */
    static QString typeName() { return QStringLiteral( "outputHtml" ); }
    QString type() const override { return typeName(); }

};

/**
 * \class QgsProcessingOutputNumber
 * \ingroup core
 * A numeric output for processing algorithms.
  * \since QGIS 3.0
 */
class CORE_EXPORT QgsProcessingOutputNumber : public QgsProcessingOutputDefinition
{
  public:

    /**
     * Constructor for QgsProcessingOutputNumber.
     */
    QgsProcessingOutputNumber( const QString &name, const QString &description = QString() );

    /**
     * Returns the type name for the output class.
     */
    static QString typeName() { return QStringLiteral( "outputNumber" ); }
    QString type() const override { return typeName(); }
};

/**
 * \class QgsProcessingOutputString
 * \ingroup core
 * A string output for processing algorithms.
  * \since QGIS 3.0
 */
class CORE_EXPORT QgsProcessingOutputString : public QgsProcessingOutputDefinition
{
  public:

    /**
     * Constructor for QgsProcessingOutputString.
     */
    QgsProcessingOutputString( const QString &name, const QString &description = QString() );

    /**
     * Returns the type name for the output class.
     */
    static QString typeName() { return QStringLiteral( "outputString" ); }
    QString type() const override { return typeName(); }

};

/**
 * \class QgsProcessingOutputFolder
 * \ingroup core
 * A folder output for processing algorithms.
  * \since QGIS 3.0
 */
class CORE_EXPORT QgsProcessingOutputFolder : public QgsProcessingOutputDefinition
{
  public:

    /**
     * Constructor for QgsProcessingOutputFolder.
     */

    QgsProcessingOutputFolder( const QString &name, const QString &description = QString() );

    /**
     * Returns the type name for the output class.
     */
    static QString typeName() { return QStringLiteral( "outputFolder" ); }
    QString type() const override { return typeName(); }

};

#endif // QGSPROCESSINGOUTPUTS_H


