/*
 *    Copyright 2014 Thomas Schöps
 *
 *    This file is part of OpenOrienteering.
 *
 *    OpenOrienteering is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    OpenOrienteering is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with OpenOrienteering.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _OPENORIENTEERING_GPS_TEMPORARY_MARKERS_H_
#define _OPENORIENTEERING_GPS_TEMPORARY_MARKERS_H_

#include <QObject>

#include "map_coord.h"

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE
class MapWidget;
class GPSDisplay;


/** Displays temporary markers recorded with GPS. */
class GPSTemporaryMarkers : public QObject
{
Q_OBJECT
public:
	GPSTemporaryMarkers(MapWidget* widget, GPSDisplay* gps_display);
	virtual ~GPSTemporaryMarkers();
	
	void addPoint();
	void startPath();
	void stopPath();
	void clear();
	
	/// This is called from the MapWidget drawing code to draw the markers.
	void paint(QPainter* painter);
	
public slots:
	void newGPSPosition(MapCoordF coord, float accuracy);
	
private:
	void updateMapWidget();
	
	bool recording_path;
	std::vector< QPointF > points;
	std::vector< std::vector< QPointF > > paths;
	GPSDisplay* gps_display;
	MapWidget* widget;
};

#endif