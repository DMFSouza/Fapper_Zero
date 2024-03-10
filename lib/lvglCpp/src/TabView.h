/*
 * tabView.h
 *
 *  Created on: Nov 25, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_TABVIEW_H_
#define LVGLCPP_SRC_TABVIEW_H_

#include "ButtonMatrix.h"
#include "Object.h"

namespace lvgl {

class TabView : public Object {
protected:
	ButtonMatrix *_btns;
public:
	TabView(lv_obj_t *parent = NULL, lv_dir_t tabPos = LV_DIR_TOP, lv_coord_t tabSize = 40) {
		if(parent) {
			_obj = lv_tabview_create(parent, tabPos, tabSize);
		} else {
			_obj = lv_tabview_create(lv_scr_act(), tabPos, tabSize);
		}
		_btns = new ButtonMatrix();
		_btns->setObj(lv_tabview_get_tab_btns(_obj));
		_child = NULL;
		_childs = (lv_obj_t **)calloc(1, sizeof(lv_obj_t *));
	}

	~TabView() {
		// TODO Auto-generated destructor stub
	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	inline 	ButtonMatrix *getBtns() {
		return _btns;
	}

	inline TabView *addTab(const char *name) {
		uint32_t i = 0;
		for(; i < (uint32_t)-1; i++) {
			if(!_childs[i])
				break;
		}
		_childs = (lv_obj_t **)realloc(_childs, sizeof(lv_obj_t *) * (i + 2));
		_childs[i + 1] = NULL;
		_childs[i] = lv_tabview_add_tab(_obj, name);
		return this;
	}

	inline lv_obj_t *getTabObj(uint32_t id) {
		int i = 0;
		for(; _childs[i] != NULL; i++);
		if(id < i) {
			return _childs[i];
		}
		return NULL;
	}

	inline lv_obj_t *getTabObj(const char *name) {
		int i = 0;
		for(; _childs[i] != NULL; i++);
		for(int j = 0; j < i; j++) {
			const char *txt = lv_btnmatrix_get_btn_text((const lv_obj_t *)_btns->getObj(), j);
			if(!strcmp(txt, name))
				return _childs[j];
		}
		return NULL;
	}



	inline TabView *renameTab(uint32_t tab_id, const char * new_name) {
		lv_tabview_rename_tab(_obj, tab_id, new_name);
		return this;
	}
	inline lv_obj_t *getContent() {
		return lv_tabview_get_content(_obj);
	}
	inline lv_obj_t *getTabBtns() {
		return lv_tabview_get_tab_btns(_obj);
	}
	inline TabView *setAct(uint32_t id, lv_anim_enable_t anim_en) {
		lv_tabview_set_act(_obj, id, anim_en);
		return this;
	}
	inline uint16_t getTabAct() {
		return lv_tabview_get_tab_act(_obj);
	}
};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_TABVIEW_H_ */
