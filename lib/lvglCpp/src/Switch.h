/*
 * Switch.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_SWITCH_H_
#define LVGLCPP_SRC_SWITCH_H_

#include "Object.h"

namespace lvgl {
class Switch : public Object {
public:
	Switch(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_switch_create(parent);
		} else {
			_obj = lv_switch_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	~Switch() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_SWITCH_H_ */
