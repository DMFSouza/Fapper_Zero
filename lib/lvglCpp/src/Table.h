/*
 * Table.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_TABLE_H_
#define LVGLCPP_SRC_TABLE_H_

#include "Object.h"

namespace lvgl {
class Table : public Object {
public:
	Table(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_table_create(parent);
		} else {
			_obj = lv_table_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	~Table() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set the value of a cell.
	 * @param obj           pointer to a Table object
	 * @param row           id of the row [0 .. row_cnt -1]
	 * @param col           id of the column [0 .. col_cnt -1]
	 * @param txt           text to display in the cell. It will be copied and saved so this variable is not required after this function call.
	 * @note                New roes/columns are added automatically if required
	 */
	Table *setCellValue(uint16_t row, uint16_t col, const char * fmt, ...) {
		va_list args;
		va_start(args, fmt);
		int size = vsnprintf(NULL, 0, fmt, args);
		va_end(args);
		char buffer[size + 1];
		va_start(args, fmt);
		vsnprintf(buffer, size + 1, fmt, args);
		va_end(args);
		lv_table_set_cell_value(_obj, row, col, buffer);
		return this;
	}

	/**
	 * Set the number of rows
	 * @param obj           table pointer to a Table object
	 * @param row_cnt       number of rows
	 */
	inline Table *setRowCount(uint16_t row_cnt) {
		lv_table_set_row_cnt(_obj, row_cnt);
		return this;
	}

	/**
	 * Set the number of columns
	 * @param obj       table pointer to a Table object
	 * @param col_cnt   number of columns.
	 */
	inline Table *setColCount(uint16_t col_cnt) {
		lv_table_set_col_cnt(_obj, col_cnt);
		return this;
	}

	/**
	 * Set the width of a column
	 * @param obj       table pointer to a Table object
	 * @param col_id    id of the column [0 .. LV_TABLE_COL_MAX -1]
	 * @param w         width of the column
	 */
	inline Table *setColWidth(uint16_t col_id, lv_coord_t w) {
		lv_table_set_col_width(_obj, col_id, w);
		return this;
	}

	/**
	 * Add control bits to the cell.
	 * @param obj       pointer to a Table object
	 * @param row       id of the row [0 .. row_cnt -1]
	 * @param col       id of the column [0 .. col_cnt -1]
	 * @param ctrl      OR-ed values from ::lv_table_cell_ctrl_t
	 */
	inline Table *addCellCtrl(uint16_t row, uint16_t col, lv_table_cell_ctrl_t ctrl) {
		lv_table_add_cell_ctrl(_obj, row, col, ctrl);
		return this;
	}


	/**
	 * Clear control bits of the cell.
	 * @param obj       pointer to a Table object
	 * @param row       id of the row [0 .. row_cnt -1]
	 * @param col       id of the column [0 .. col_cnt -1]
	 * @param ctrl      OR-ed values from ::lv_table_cell_ctrl_t
	 */
	inline Table *clearCellCtrl(uint16_t row, uint16_t col, lv_table_cell_ctrl_t ctrl) {
		lv_table_clear_cell_ctrl(_obj, row, col, ctrl);
		return this;
	}

	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the value of a cell.
	 * @param obj       pointer to a Table object
	 * @param row       id of the row [0 .. row_cnt -1]
	 * @param col       id of the column [0 .. col_cnt -1]
	 * @return          text in the cell
	 */
	inline const char *getCellValue(uint16_t row, uint16_t col) {
		return lv_table_get_cell_value(_obj, row, col);
	}

	/**
	 * Get the number of rows.
	 * @param obj       table pointer to a Table object
	 * @return          number of rows.
	 */
	inline uint16_t getRowCount() {
		return lv_table_get_row_cnt(_obj);
	}

	/**
	 * Get the number of columns.
	 * @param obj       table pointer to a Table object
	 * @return          number of columns.
	 */
	inline uint16_t getColCount() {
		return lv_table_get_col_cnt(_obj);
	}

	/**
	 * Get the width of a column
	 * @param obj       table pointer to a Table object
	 * @param col       id of the column [0 .. LV_TABLE_COL_MAX -1]
	 * @return          width of the column
	 */
	inline lv_coord_t getColWidth(uint16_t col) {
		return lv_table_get_col_width(_obj, col);
	}

	/**
	 * Get whether a cell has the control bits
	 * @param obj       pointer to a Table object
	 * @param row       id of the row [0 .. row_cnt -1]
	 * @param col       id of the column [0 .. col_cnt -1]
	 * @param ctrl      OR-ed values from ::lv_table_cell_ctrl_t
	 * @return          true: all control bits are set; false: not all control bits are set
	 */
	inline bool hasCellCtrl(uint16_t row, uint16_t col, lv_table_cell_ctrl_t ctrl) {
		return lv_table_has_cell_ctrl(_obj, row, col, ctrl);
	}

	/**
	 * Get the selected cell (pressed and or focused)
	 * @param obj       pointer to a table object
	 * @param row       pointer to variable to store the selected row (LV_TABLE_CELL_NONE: if no cell selected)
	 * @param col       pointer to variable to store the selected column  (LV_TABLE_CELL_NONE: if no cell selected)
	 */
	inline Table *getSelectedCell(uint16_t * row, uint16_t * col) {
		lv_table_get_selected_cell(_obj, row, col);
		return this;
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_TABLE_H_ */
