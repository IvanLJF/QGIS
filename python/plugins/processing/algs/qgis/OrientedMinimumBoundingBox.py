# -*- coding: utf-8 -*-

"""
***************************************************************************
    OrientedMinimumBoundingBox.py
    ---------------------
    Date                 : June 2015
    Copyright            : (C) 2015, Loïc BARTOLETTI
    Email                : coder at tuxfamily dot org
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Loïc BARTOLETTI'
__date__ = 'June 2015'
__copyright__ = '(C) 2015, Loïc BARTOLETTI'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

from qgis.PyQt.QtCore import QVariant
from qgis.core import (QgsField,
                       QgsFields,
                       QgsFeatureSink,
                       QgsGeometry,
                       QgsFeature,
                       QgsWkbTypes,
                       QgsFeatureRequest,
                       QgsApplication,
                       QgsProcessingUtils)
from processing.algs.qgis.QgisAlgorithm import QgisAlgorithm
from processing.core.GeoAlgorithmExecutionException import GeoAlgorithmExecutionException
from processing.core.parameters import ParameterVector
from processing.core.parameters import ParameterBoolean
from processing.core.outputs import OutputVector
from processing.tools import dataobjects


class OrientedMinimumBoundingBox(QgisAlgorithm):

    INPUT_LAYER = 'INPUT_LAYER'
    BY_FEATURE = 'BY_FEATURE'

    OUTPUT = 'OUTPUT'

    def group(self):
        return self.tr('Vector general tools')

    def __init__(self):
        super().__init__()

    def initAlgorithm(self, config=None):
        self.addParameter(ParameterVector(self.INPUT_LAYER,
                                          self.tr('Input layer')))
        self.addParameter(ParameterBoolean(self.BY_FEATURE,
                                           self.tr('Calculate OMBB for each feature separately'), True))

        self.addOutput(OutputVector(self.OUTPUT, self.tr('Oriented_MBBox'), datatype=[dataobjects.TYPE_VECTOR_POLYGON]))

    def name(self):
        return 'orientedminimumboundingbox'

    def displayName(self):
        return self.tr('Oriented minimum bounding box')

    def processAlgorithm(self, parameters, context, feedback):
        layer = QgsProcessingUtils.mapLayerFromString(self.getParameterValue(self.INPUT_LAYER), context)
        byFeature = self.getParameterValue(self.BY_FEATURE)

        if byFeature and layer.geometryType() == QgsWkbTypes.PointGeometry and layer.featureCount() <= 2:
            raise GeoAlgorithmExecutionException(self.tr("Can't calculate an OMBB for each point, it's a point. The number of points must be greater than 2"))

        if byFeature:
            fields = layer.fields()
        else:
            fields = QgsFields()
        fields.append(QgsField('area', QVariant.Double))
        fields.append(QgsField('perimeter', QVariant.Double))
        fields.append(QgsField('angle', QVariant.Double))
        fields.append(QgsField('width', QVariant.Double))
        fields.append(QgsField('height', QVariant.Double))

        writer = self.getOutputFromName(self.OUTPUT).getVectorWriter(fields, QgsWkbTypes.Polygon, layer.crs(), context)

        if byFeature:
            self.featureOmbb(layer, context, writer, feedback)
        else:
            self.layerOmmb(layer, context, writer, feedback)

        del writer

    def layerOmmb(self, layer, context, writer, feedback):
        req = QgsFeatureRequest().setSubsetOfAttributes([])
        features = QgsProcessingUtils.getFeatures(layer, context, req)
        total = 100.0 / layer.featureCount() if layer.featureCount() else 0
        newgeometry = QgsGeometry()
        first = True
        for current, inFeat in enumerate(features):
            if first:
                newgeometry = inFeat.geometry()
                first = False
            else:
                newgeometry = newgeometry.combine(inFeat.geometry())
            feedback.setProgress(int(current * total))

        geometry, area, angle, width, height = newgeometry.orientedMinimumBoundingBox()

        if geometry:
            outFeat = QgsFeature()

            outFeat.setGeometry(geometry)
            outFeat.setAttributes([area,
                                   width * 2 + height * 2,
                                   angle,
                                   width,
                                   height])
            writer.addFeature(outFeat, QgsFeatureSink.FastInsert)

    def featureOmbb(self, layer, context, writer, feedback):
        features = QgsProcessingUtils.getFeatures(layer, context)
        total = 100.0 / layer.featureCount() if layer.featureCount() else 0
        outFeat = QgsFeature()
        for current, inFeat in enumerate(features):
            geometry, area, angle, width, height = inFeat.geometry().orientedMinimumBoundingBox()
            if geometry:
                outFeat.setGeometry(geometry)
                attrs = inFeat.attributes()
                attrs.extend([area,
                              width * 2 + height * 2,
                              angle,
                              width,
                              height])
                outFeat.setAttributes(attrs)
                writer.addFeature(outFeat, QgsFeatureSink.FastInsert)
            else:
                feedback.pushInfo(self.tr("Can't calculate an OMBB for feature {0}.").format(inFeat.id()))
            feedback.setProgress(int(current * total))
