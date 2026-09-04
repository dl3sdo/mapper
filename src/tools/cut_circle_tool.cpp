/*
 *    Copyright 2026 Matthias Kühlewein
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

#include "cut_circle_tool.h"

#include <Qt>
#include <QtGlobal>
#include <QCursor>
#include <QEvent>
#include <QFlags>
#include <QGuiApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QString>

#include "settings.h"
#include "core/map.h"
#include "core/map_part.h"
#include "core/map_view.h"
#include "core/objects/object.h"
#include "core/symbols/point_symbol.h"
#include "core/symbols/symbol.h"
#include "core/renderables/renderable.h"
#include "gui/map/map_widget.h"
#include "undo/object_undo.h"
#include "undo/undo.h"

namespace OpenOrienteering {


CutCircleTool::CutCircleTool(MapEditorController* editor, QAction* tool_action)
 : MapEditorToolBase { scaledToScreen(QCursor{ QString::fromLatin1(":/images/cursor-cut.png"), 11, 11 }), Other, editor, tool_action }
 , user_click_tolerance { Settings::getInstance().getSetting(Settings::MapEditor_ClickToleranceMM).toDouble() / 10.0}
 , renderables { new MapRenderables(map()) }
{
	// nothing
}


CutCircleTool::~CutCircleTool()
{
	deletePreviewObject();
}



void CutCircleTool::initImpl()
{
	updateDirtyRect();
	updateStatusText();
}



// This function contains translations. Keep it close to the top of the file so
// that line numbers remain stable here when changing other parts of the file.
void CutCircleTool::updateStatusText()
{
	QString text;
	if (!editingInProgress())
		text = tr("<b>Click</b> on a point circle: Create cut point, click on another to create the gap. <b>Click</b> on a gap in a point circle: Remove gap. ");
	
	setStatusBarText(text);
}

void CutCircleTool::objectSelectionChangedImpl()
{
	updateDirtyRect();
	deactivate();
}


/*
bool CutCircleTool::mouseDoubleClickEvent(QMouseEvent* event, const MapCoordF& map_coord, MapWidget* widget)
{
	return false;
}


void CutCircleTool::leaveEvent(QEvent* event)
{

}


void CutCircleTool::focusOutEvent(QFocusEvent* event)
{

}



void CutCircleTool::mouseMove()
{
	//updateHoverState(cur_pos_map);
}
*/

void CutCircleTool::clickPress()
{
	if (map()->getNumSelectedObjects() != 1)
	{
		deactivate();
		return;
	}
	const auto point = map()->getFirstSelectedObject();
	int angle;
	int point_element_index = this->point_element_index;
	if (getCircleAngle(point, &angle, point_element_index))
	{
		//qDebug("\nangle: %i",angle);
		//qDebug("\ngap?: %i",point->asPoint()->getCutCircle().isAngleGap(angle) ? 1 : 0);
		if (point->asPoint()->getCutCircle().isAngleGap(angle))
		{
			// removing the gap
			auto new_point = point->duplicate();
			new_point->asPoint()->getCutCircle().deleteGap(angle);
			replaceObject(point, new_point);
			updateDirtyRect();
			//deactivate();
		}
		return;
	}
	auto coords = point->asPoint()->getCoordF();
	auto symbol = point->getSymbol()->asPoint();
	double radius = (symbol->getInnerRadius() + symbol->getOuterWidth() / 2) / 1000.0;

	//auto norma = click_pos_map - coords;
	//qDebug("\nradius: %f",norma.angle());	// gegen Uhrzeigersinn
	// statt distanceTo Quadrat verwenden?
	const auto click_tolerance = qMax(symbol->getOuterWidth() / 2000.0, user_click_tolerance);
	if (qAbs(click_pos_map.distanceTo(coords) - radius) < click_tolerance)
	{
		deactivate();
	}
	
	//deactivate();
}

bool CutCircleTool::getCircleAngle(Object* obj, int* angle, int& point_element_index) const
{
	auto point = static_cast<PointObject*>(obj);
	auto symbol = obj->getSymbol()->asPoint();
	if (point_element_index < 0)	// search
	{
		// first use base part
		if (symbol->isCircle())
		{
			if (checkCircleAngle(point->getCoordF(), symbol, (symbol->isRotatable() ? point->getRotation() : 0.0), angle))
			{
				point_element_index = 0;
				return true;
			}
		}
		// else use sub elements
		for (int i = 0; i < symbol->getNumElements(); ++i)
		{
			auto element_symbol = symbol->getElementSymbol(i);
			if (element_symbol->getType() == Symbol::Point && element_symbol->asPoint()->isCircle())
			{
				Q_ASSERT(symbol->getElementObject(i)->getType() == Object::Point);
				
				auto element_object = symbol->getElementObject(i)->asPoint();
				auto element_coords = element_object->getCoordF();
				double rotation = 0.0;
				if (symbol->isRotatable())
				{
					rotation = point->getRotation();
					element_coords.rotate(-point->getRotation());
				}
				if (checkCircleAngle(point->getCoordF() + element_coords, element_symbol->asPoint(), rotation, angle))
				{
					point_element_index = i + 1;
					return true;
				}
			}
		}
		return false;
	}
/*	if (point_element_index > 0)
	{
		symbol = symbol->getElementSymbol(point_element_index - 1)->asPoint();
		auto element_object = symbol->getElementObject(point_element_index - 1);
	}
	*/
	return false;
}

bool CutCircleTool::checkCircleAngle(const MapCoordF coords, const PointSymbol* symbol, double rotation, int* angle) const
{
	const double radius = (symbol->getInnerRadius() + symbol->getOuterWidth() / 2) / 1000.0;
	const double click_tolerance = qMax(symbol->getOuterWidth() / 2000.0, user_click_tolerance);
	if (qAbs(click_pos_map.distanceTo(coords) - radius) < click_tolerance)
	{
		auto normalized_click_pos = click_pos_map - coords;
		auto temp = qRadiansToDegrees(-normalized_click_pos.angle() - rotation);
		if (temp < 0)
			temp += 360;
		*angle = (int)(temp * 160);
		return true;
	}
	return false;
}

/*
void CutCircleTool::clickRelease()
{
	
}


*/
void CutCircleTool::updatePreviewObjects()
{
/*
	Q_ASSERT(!path_tool);
	if (editingInProgress() && qAbs(drag_end_len - drag_start_len) >= 0.001f)
	{
		if (preview_path)
		{
			renderables->removeRenderablesOfObject(preview_path, false);
		}
		else
		{
			preview_path = new PathObject { Map::getCoveringCombinedLine() };
		}
		
		preview_path->clearCoordinates();
		preview_path->appendPathPart(edit_object->parts()[drag_part_index]);
		if (reverse_drag)
			preview_path->changePathBounds(0, drag_end_len, drag_start_len);
		else
			preview_path->changePathBounds(0, drag_start_len, drag_end_len);
		
		preview_path->update();
		renderables->insertRenderablesOfObject(preview_path);
	}
	else
	{
		deletePreviewObject();
	}
		
	updateDirtyRect();
*/
}


void CutCircleTool::deletePreviewObject()
{
/*
	if (preview_path)
	{
		renderables->removeRenderablesOfObject(preview_path, false);
		delete preview_path;
		preview_path = nullptr;
	}
*/
}


int CutCircleTool::updateDirtyRectImpl(QRectF& rect)
{
/*
	auto map = this->map();
	map->includeSelectionRect(rect);
	
	if (editingInProgress())
	{
		edit_object->includeControlPointsRect(rect);
	}
	else if (map->selectedObjects().size() <= max_objects_for_handle_display)
	{
		for (const auto* object : map->selectedObjects())
			object->includeControlPointsRect(rect);
	}
	
	if (path_tool && path_tool_rect.isValid())
		rectIncludeSafe(rect, path_tool_rect);
	
	return rect.isValid() ? 6 : -1;
*/
	return -1;
}


void CutCircleTool::drawImpl(QPainter* painter, MapWidget* widget)
{
/*
	auto map = this->map();
	map->drawSelection(painter, true, widget, nullptr);
	
	if (editingInProgress())
	{
		pointHandles().draw(painter, widget, edit_object, hover_point, false);
	}
	else if (map->selectedObjects().size() <= max_objects_for_handle_display)
	{
		for (const auto* object: map->selectedObjects())
		{
			auto hover_point = MapCoordVector::size_type { hover_object == object ? this->hover_point : no_point };
			pointHandles().draw(painter, widget, object, hover_point, false);
		}
	}
	
	if (preview_path)
	{
		const MapView* map_view = widget->getMapView();
		painter->save();
		painter->translate(widget->width() / 2.0 + map_view->panOffset().x(),
						   widget->height() / 2.0 + map_view->panOffset().y());
		painter->setWorldTransform(map_view->worldTransform(), true);
		
		RenderConfig config = { *map, map_view->calculateViewedRect(widget->viewportToView(widget->rect())), map_view->calculateFinalZoomFactor(), RenderConfig::Tool, 0.5 };
		renderables->draw(painter, config);
		
		painter->restore();
	}
	
	if (path_tool)
		path_tool->draw(painter, widget);
*/
}

void CutCircleTool::replaceObject(Object* object, Object* new_object) const
{
	auto map = this->map();
	auto map_part = map->getCurrentPart();
	
	auto add_step = new AddObjectsUndoStep(map);
	add_step->addObject(map_part->findObjectIndex(object), object);
	map->removeObjectFromSelection(object, false);
	map->releaseObject(object);
	
	auto delete_step = new DeleteObjectsUndoStep(map);
	map->addObject(new_object);
	map->addObjectToSelection(new_object, false);
	delete_step->addObject(map_part->findObjectIndex(new_object));
	
	auto undo_step = new CombinedUndoStep(map);
	undo_step->push(add_step);
	undo_step->push(delete_step);
	map->push(undo_step);
	
	map->emitSelectionChanged();
}

}  // namespace OpenOrienteering
