/*
 * Roller.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_ROLLER_H_
#define LVGLCPP_SRC_ROLLER_H_

#include "Object.h"

namespace lvgl {
class Roller : public Object {
public:
	Roller(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_roller_create(parent);
		} else {
			_obj = lv_roller_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	~Roller() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set the options on a roller
	 * @param obj       pointer to roller object
	 * @param options   a string with '\n' separated options. E.g. "One\nTwo\nThree"
	 * @param mode      `LV_ROLLER_MODE_NORMAL` or `LV_ROLLER_MODE_INFINITE`
	 */
	inline Roller *setOptions(const char * options, lv_roller_mode_t mode) {
		lv_roller_set_options(_obj, options, mode);
		return this;
	}

	/**
	 * Set the selected option
	 * @param obj       pointer to a roller object
	 * @param sel_opt   index of the selected option (0 ... number of option - 1);
	 * @param anim_en   LV_ANIM_ON: set with animation; LV_ANOM_OFF set immediately
	 */
	inline Roller *setSelected(uint16_t sel_opt, lv_anim_enable_t anim) {
		lv_roller_set_selected(_obj, sel_opt, anim);
		return this;
	}

	/**
	 * Set the height to show the given number of rows (options)
	 * @param obj       pointer to a roller object
	 * @param row_cnt   number of desired visible rows
	 */
	inline Roller *setVisibleRowCount(uint8_t row_cnt) {
		lv_roller_set_visible_row_count(_obj, row_cnt);
		return this;
	}

	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the index of the selected option
	 * @param obj       pointer to a roller object
	 * @return          index of the selected option (0 ... number of option - 1);
	 */
	inline uint16_t getSelected() {
		return lv_roller_get_selected((const lv_obj_t *)_obj);
	}

	/**
	 * Get the current selected option as a string.
	 * @param obj       pointer to ddlist object
	 * @param buf       pointer to an array to store the string
	 * @param buf_size  size of `buf` in bytes. 0: to ignore it.
	 */
	inline Roller *getSelectedStr(char * buf, uint32_t buf_size) {
		lv_roller_get_selected_str((const lv_obj_t *)_obj, buf, buf_size);
		return this;
	}


	/**
	 * Get the options of a roller
	 * @param obj       pointer to roller object
	 * @return          the options separated by '\n'-s (E.g. "Option1\nOption2\nOption3")
	 */
	inline const char *getOptions() {
		return lv_roller_get_options((const lv_obj_t *)_obj);
	}

	/**
	 * Get the total number of options
	 * @param obj   pointer to a roller object
	 * @return      the total number of options
	 */
	inline uint16_t getOptionCnt() {
		return lv_roller_get_option_cnt((const lv_obj_t *)_obj);
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_ROLLER_H_ */
