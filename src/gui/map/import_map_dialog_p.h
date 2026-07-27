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

#ifndef OPENORIENTEERING_IMPORT_MAP_DIALOG_P_H
#define OPENORIENTEERING_IMPORT_MAP_DIALOG_P_H

#include <vector>

#include <QString>

namespace OpenOrienteering {

struct PartConfigItem {
	QString name;			// part name in import map
	QString target_name;	// part name in destination map
	bool import;
	int num_objects;
	bool import_selected;
	bool import_equal_objects;
	QString compare_map_part;
	QString object_tag_key;
	QString object_tag_value;
};

using PartConfigList = std::vector<PartConfigItem>;

}  // namespace OpenOrienteering


#endif // OPENORIENTEERING_IMPORT_MAP_DIALOG_P_H
