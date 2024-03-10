/*
 * CheckBox.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_CHECKBOX_H_
#define LVGLCPP_SRC_CHECKBOX_H_

#include "Object.h"

namespace lvgl {
class CheckBox : public Object {
public:
	CheckBox(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_checkbox_create(parent);
		} else {
			_obj = lv_checkbox_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	virtual ~CheckBox() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set the text of a check box. `txt` will be copied and may be deallocated
	 * after this function returns.
	 * @param cb    pointer to a check box
	 * @param txt   the text of the check box. NULL to refresh with the current text.
	 */
	inline CheckBox *setText(const char * txt) {
		lv_checkbox_set_text(_obj, txt);
		return this;
	}

	/**
	 * Set the text of a check box. `txt` must not be deallocated during the life
	 * of this checkbox.
	 * @param cb    pointer to a check box
	 * @param txt   the text of the check box.
	 */
	inline CheckBox *setTextStatic(const char * txt) {
		lv_checkbox_set_text_static(_obj, txt);
		return this;
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_CHECKBOX_H_ */
