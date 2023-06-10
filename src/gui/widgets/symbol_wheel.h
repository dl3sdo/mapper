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


#ifndef SYMBOL_WHEEL_H
#define SYMBOL_WHEEL_H

#include <vector>

#include <QObject>
#include <QString>
#include <QVector>
#include <QWidget>

namespace OpenOrienteering {

class MainWindow;
class Map;
class PieMenu;
class Symbol;

class SymbolWheelMenu : public QWidget
{
Q_OBJECT
public:
	SymbolWheelMenu(QWidget* parent);
	
	bool setupSymbolPie();
	PieMenu* getSymbolPieMenu() const { return symbol_pie; }
	void setMap(Map* const map);
	void setMainWindow(MainWindow* const window);
	void addSymbolsToPieMenu(QVector<int> indexes);
	//void addSymbolsToPieMenu(std::vector<int> indexes);
	
	static constexpr int max_wheel_size = 8;

signals:
	void selectSymbol(int symbol);
	
private:
	struct SymbolId {
		const int index;
		const Symbol* symbol;
		const QString name;
	};
	
	std::vector<SymbolId> selected_symbols;
	PieMenu* symbol_pie;
	Map* map;
	MainWindow* window;
	
protected slots:
	void itemSelected(int index);
};

}  // namespace OpenOrienteering

#endif // SYMBOL_WHEEL_H
