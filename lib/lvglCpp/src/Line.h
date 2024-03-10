/*
 * line.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_LINE_H_
#define LVGLCPP_SRC_LINE_H_

#include "Object.h"

namespace lvgl {
class Line : public Object {
public:
	Line(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_line_create(parent);
		} else {
			_obj = lv_line_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}

	~Line() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set an array of points. The line object will connect these points.
	 * @param obj           pointer to a line object
	 * @param points        an array of points. Only the address is saved, so the array needs to be alive while the line exists
	 * @param point_num     number of points in 'point_a'
	 */
	inline Line *setPoints(const lv_point_t points[], uint16_t point_num) {
		lv_line_set_points(_obj, points, point_num);
		return this;
	}

	/**
	 * Enable (or disable) the y coordinate inversion.
	 * If enabled then y will be subtracted from the height of the object,
	 * therefore the y = 0 coordinate will be on the bottom.
	 * @param obj       pointer to a line object
	 * @param en        true: enable the y inversion, false:disable the y inversion
	 */
	inline Line *setYInvert(bool en) {
		lv_line_set_y_invert(_obj, en);
		return this;
	}

	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the y inversion attribute
	 * @param obj       pointer to a line object
	 * @return          true: y inversion is enabled, false: disabled
	 */
	inline bool getYInvert() {
		return lv_line_get_y_invert((const lv_obj_t *)_obj);
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_LINE_H_ */
