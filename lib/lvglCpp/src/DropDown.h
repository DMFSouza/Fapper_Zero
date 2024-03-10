/*
 * DropDown.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_DROPDOWN_H_
#define LVGLCPP_SRC_DROPDOWN_H_

#include "Object.h"

namespace lvgl {
class DropDown : public Object {
public:
	DropDown(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_dropdown_create(parent);
		} else {
			_obj = lv_dropdown_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	~DropDown() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set text of the drop-down list's button.
	 * If set to `NULL` the selected option's text will be displayed on the button.
	 * If set to a specific text then that text will be shown regardless of the selected option.
	 * @param obj       pointer to a drop-down list object
	 * @param txt       the text as a string (Only its pointer is saved)
	 */
	inline DropDown *setText(const char * txt) {
		lv_dropdown_set_text(_obj, txt);
		return this;
	}

	/**
	 * Set the options in a drop-down list from a string.
	 * The options will be copied and saved in the object so the `options` can be destroyed after calling this function
	 * @param obj       pointer to drop-down list object
	 * @param options   a string with '\n' separated options. E.g. "One\nTwo\nThree"
	 */
	inline DropDown *setOptions(const char * options) {
		lv_dropdown_set_options(_obj, options);
		return this;
	}

	/**
	 * Set the options in a drop-down list from a static string (global, static or dynamically allocated).
	 * Only the pointer of the option string will be saved.
	 * @param obj       pointer to drop-down list object
	 * @param options   a static string with '\n' separated options. E.g. "One\nTwo\nThree"
	 */
	inline DropDown *setOptionsStatic(const char * options) {
		lv_dropdown_set_options_static(_obj, options);
		return this;
	}

	/**
	 * Add an options to a drop-down list from a string.  Only works for non-static options.
	 * @param obj       pointer to drop-down list object
	 * @param option    a string without '\n'. E.g. "Four"
	 * @param pos       the insert position, indexed from 0, LV_DROPDOWN_POS_LAST = end of string
	 */
	inline DropDown *addOptions(const char * option, uint32_t pos) {
		lv_dropdown_add_option(_obj, option, pos);
		return this;
	}

	/**
	 * Clear all options in a drop-down list.  Works with both static and dynamic options.
	 * @param obj       pointer to drop-down list object
	 */
	inline DropDown *clearOptions() {
		lv_dropdown_clear_options(_obj);
		return this;
	}

	/**
	 * Set the selected option
	 * @param obj       pointer to drop-down list object
	 * @param sel_opt   id of the selected option (0 ... number of option - 1);
	 */
	inline DropDown *setSelected(uint16_t sel_opt) {
		lv_dropdown_set_selected(_obj, sel_opt);
		return this;
	}

	/**
	 * Set the direction of the a drop-down list
	 * @param obj       pointer to a drop-down list object
	 * @param dir       LV_DIR_LEFT/RIGHT/TOP/BOTTOM
	 */
	inline DropDown *setDirection(lv_dir_t dir) {
		lv_dropdown_set_dir(_obj, dir);
		return this;
	}

	/**
	 * Set an arrow or other symbol to display when on drop-down list's button. Typically a down caret or arrow.
	 * @param obj       pointer to drop-down list object
	 * @param symbol    a text like `LV_SYMBOL_DOWN`, an image (pointer or path) or NULL to not draw symbol icon
	 * @note angle and zoom transformation can be applied if the symbol is an image.
	 * E.g. when drop down is checked (opened) rotate the symbol by 180 degree
	 */
	inline DropDown *setSymbol(const void * symbol) {
		lv_dropdown_set_symbol(_obj, symbol);
		return this;
	}

	/**
	 * Set whether the selected option in the list should be highlighted or not
	 * @param obj       pointer to drop-down list object
	 * @param en        true: highlight enabled; false: disabled
	 */
	inline DropDown *setSelectedHighlight(bool en) {
		lv_dropdown_set_selected_highlight(_obj, en);
		return this;
	}

	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the list of a drop-down to allow styling or other modifications
	 * @param obj       pointer to a drop-down list object
	 * @return          pointer to the list of the drop-down
	 */
	inline lv_obj_t *getList() {
		return lv_dropdown_get_list(_obj);
	}

	/**
	 * Get text of the drop-down list's button.
	 * @param obj   pointer to a drop-down list object
	 * @return      the text as string, `NULL` if no text
	 */
	inline const char *getText() {
		return lv_dropdown_get_text(_obj);
	}

	/**
	 * Get the options of a drop-down list
	 * @param obj       pointer to drop-down list object
	 * @return          the options separated by '\n'-s (E.g. "Option1\nOption2\nOption3")
	 */
	inline const char *getOptions() {
		return lv_dropdown_get_options(_obj);
	}

	/**
	 * Get the index of the selected option
	 * @param obj       pointer to drop-down list object
	 * @return          index of the selected option (0 ... number of option - 1);
	 */
	inline uint16_t getSelected() {
		return lv_dropdown_get_selected(_obj);
	}

	/**
	 * Get the total number of options
	 * @param obj       pointer to drop-down list object
	 * @return          the total number of options in the list
	 */
	inline uint16_t getOptionCnt() {
		return lv_dropdown_get_option_cnt(_obj);
	}

	/**
	 * Get the current selected option as a string
	 * @param obj       pointer to drop-down object
	 * @param buf       pointer to an array to store the string
	 * @param buf_size  size of `buf` in bytes. 0: to ignore it.
	 */
	inline DropDown *getSelectedStr( char * buf, uint32_t buf_size) {
		lv_dropdown_get_selected_str(_obj, buf, buf_size);
		return this;
	}

	/**
	 * Get the index of an option.
	 * @param obj       pointer to drop-down object
	 * @param option    an option as string
	 * @return          index of `option` in the list of all options. -1 if not found.
	 */
	inline int32_t getOptionIndex(const char * option) {
		return lv_dropdown_get_option_index(_obj, option);
	}

	/**
	 * Get the symbol on the drop-down list. Typically a down caret or arrow.
	 * @param obj       pointer to drop-down list object
	 * @return          the symbol or NULL if not enabled
	 */
	inline const char *getSymbol() {
		return lv_dropdown_get_symbol(_obj);
	}

	/**
	 * Get whether the selected option in the list should be highlighted or not
	 * @param obj       pointer to drop-down list object
	 * @return          true: highlight enabled; false: disabled
	 */
	inline bool getSelectedHighlight() {
		return lv_dropdown_get_selected_highlight(_obj);
	}

	/**
	 * Get the direction of the drop-down list
	 * @param obj       pointer to a drop-down list object
	 * @return          LV_DIR_LEF/RIGHT/TOP/BOTTOM
	 */
	inline lv_dir_t getDirection() {
		return lv_dropdown_get_dir(_obj);
	}

	/*=====================
	 * Other functions
	 *====================*/

	/**
	 * Open the drop.down list
	 * @param obj       pointer to drop-down list object
	 */
	inline DropDown *dropDownOpen() {
		lv_dropdown_open(_obj);
		return this;
	}

	/**
	 * Close (Collapse) the drop-down list
	 * @param obj       pointer to drop-down list object
	 */
	inline DropDown *dropDownClose() {
		lv_dropdown_close(_obj);
		return this;
	}

	/**
	 * Tells whether the list is opened or not
	 * @param obj       pointer to a drop-down list object
	 * @return          true if the list os opened
	 */
	inline bool isOpen() {
		return lv_dropdown_is_open(_obj);
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_DROPDOWN_H_ */
