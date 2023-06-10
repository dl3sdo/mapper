/*
 *    Copyright 2023 Matthias Kühlewein
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


#include "gui/widgets/symbol_wheel.h"

#include <vector>

#include <QtGlobal>
#include <QAction>
#include <QIcon>
#include <QLatin1String>
#include <QPixmap>
#include <QVector>

#include "core/map.h"
#include "core/symbols/symbol.h"  // IWYU pragma: keep
#include "gui/main_window.h"
#include "gui/widgets/pie_menu.h"
#include "symbol_render_widget.h"
#include "symbol_widget.h"

namespace OpenOrienteering {

SymbolWheelMenu::SymbolWheelMenu(QWidget* parent)
 : QWidget(parent)
 , map(nullptr)
{
	symbol_pie = new PieMenu(this);
}

bool SymbolWheelMenu::setupSymbolPie()
{
	if (!symbol_pie || !map || !selected_symbols.size())
		return false;
	
	constexpr int min_wheel_size = 6;
	
	for (const auto &symbol : selected_symbols)
	{
		if (symbol.index >= map->getNumSymbols() || map->getSymbol(symbol.index) != symbol.symbol)
		{
			selected_symbols.clear();
			return false;
		}
	}
	
	const auto actions = symbol_pie->actions();
	for (auto action : actions)
		symbol_pie->removeAction(action);
	const int size = selected_symbols.size();
	for (auto i = 0; i < qMax(size, min_wheel_size); ++i)
	{
		QIcon icon = {};
		if (i < size)
		{
			const auto symbol = selected_symbols.at(i);
			//auto symbol = map->getSymbol(symbol_num);
			auto icon_image = symbol.symbol->getIcon(map);
			icon.addPixmap(QPixmap::fromImage(icon_image));
		}
		
		auto action = new QAction(icon, i < size ? selected_symbols.at(i).name : QString{}, window);
		if (i < size)
			action->setStatusTip(selected_symbols.at(i).name);
		else
			action->setDisabled(true);
		connect(action, &QAction::triggered, this, [this, i](bool) { this->itemSelected(i); });
		symbol_pie->addAction(action);
	}
	
	if (symbol_pie->isEmpty())
		return false;
	return true;
}

// slot
void SymbolWheelMenu::itemSelected(int index)
{
	// https://stackoverflow.com/questions/19719397/qt-slots-and-c11-lambda
	//qDebug("%i",index);
	emit selectSymbol(selected_symbols.at(index).index);
}

void SymbolWheelMenu::addSymbolsToPieMenu(QVector<int> indexes)
//void SymbolWheelMenu::addSymbolsToPieMenu(std::vector<int> indexes)
{
	//qDebug("SLOT: %i",indexes.size());
	selected_symbols.clear();
	selected_symbols.reserve(max_wheel_size);
	for (auto selected_symbol : indexes)
	{
		const auto symbol = map->getSymbol(selected_symbol);
		const QString name_label(symbol->getNumberAsString() + QLatin1String(" ") + map->translate(symbol->getName()));
		selected_symbols.push_back({selected_symbol, symbol, name_label});
	}
}

void SymbolWheelMenu::setMap(Map* const map)
{
	this->map = map;
}

void SymbolWheelMenu::setMainWindow(MainWindow* const window)
{
	this->window = window;
}

}  // namespace OpenOrienteering
