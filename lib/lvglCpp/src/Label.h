/*
 * Label.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_LABEL_H_
#define LVGLCPP_SRC_LABEL_H_

#include "Object.h"

namespace lvgl {
class Label : public Object {
public:
	/**
	 * Create a button matrix object
	 * @param parent    pointer to an object, it will be the parent of the new button matrix
	 * @return          pointer to the created button matrix
	 */
	Label(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_label_create(parent);
		} else {
			_obj = lv_label_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}

	Label(lv_obj_t *parent, char *fmt, ...) {
		if(parent) {
			_obj = lv_label_create(parent);
		} else {
			_obj = lv_label_create(lv_scr_act());
		}
		va_list args;
		va_start(args, fmt);
		int size = vsnprintf(NULL, 0, fmt, args);
		va_end(args);
		char buffer[size + 1];
		va_start(args, fmt);
		vsnprintf(buffer, size + 1, fmt, args);
		va_end(args);
		lv_label_set_text(_obj, buffer);
		_child = NULL;
		_childs = NULL;
	}
	/**
	 * Create an empty btnMatrix object, this is useful when used as a child.
	 */
	Label() {
		_obj = NULL;
		_child = NULL;
		_childs = NULL;
	}

	~Label() {

	}

	/**
	 * Set the object pointer when used as a child.
	 */
	inline Label *setObj(lv_obj_t *obj) {
		_obj = obj;
		return this;
	}

	inline lv_obj_t *getObj() {
		return _obj;
	}


	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set a new text for a Label. Memory will be allocated to store the text by the Label.
	 * @param obj           pointer to a Label object
	 * @param text          '\0' terminated character string. NULL to refresh with the current text.
	 */
	inline Label *setText(const char * text) {
		lv_label_set_text(_obj, text);
		return this;
	}

	/**
	 * Set a new formatted text for a Label. Memory will be allocated to store the text by the Label.
	 * @param obj           pointer to a Label object
	 * @param fmt           `printf`-like format
	 * @example lv_label_set_text_fmt(label1, "%d user", user_num);
	 */
	inline Label *setText(const char * fmt, ...) {
		va_list args;
		va_start(args, fmt);
		int size = vsnprintf(NULL, 0, fmt, args);
		va_end(args);
		char buffer[size + 1];
		va_start(args, fmt);
		vsnprintf(buffer, size + 1, fmt, args);
		va_end(args);
		lv_label_set_text(_obj, buffer);
		return this;
	}

	/**
	 * Set a static text. It will not be saved by the Label so the 'text' variable
	 * has to be 'alive' while the Label exists.
	 * @param obj           pointer to a Label object
	 * @param text          pointer to a text. NULL to refresh with the current text.
	 */
	inline Label *setTextStatic(const char * text) {
		lv_label_set_text_static(_obj, text);
		return this;
	}

	/**
	 * Set the behavior of the Label with longer text then the object size
	 * @param obj           pointer to a Label object
	 * @param long_mode     the new mode from 'lv_label_long_mode' enum.
	 *                      In LV_LONG_WRAP/DOT/SCROLL/SCROLL_CIRC the size of the Label should be set AFTER this function
	 */
	inline Label *setLongMode(lv_label_long_mode_t long_mode) {
		lv_label_set_long_mode(_obj, long_mode);
		return this;
	}

	/**
	 * Enable the recoloring by in-line commands
	 * @param obj           pointer to a Label object
	 * @param en            true: enable recoloring, false: disable
	 * @example "This is a #ff0000 red# word"
	 */
	inline Label *setReColor(bool en) {
		lv_label_set_recolor(_obj, en);
		return this;
	}

	/**
	 * Set where text selection should start
	 * @param obj       pointer to a Label object
	 * @param index     character index from where selection should start. `LV_LABEL_TEXT_SELECTION_OFF` for no selection
	 */
	inline Label *setTextSelkStart(uint32_t index) {
		lv_label_set_text_sel_start(_obj, index);
		return this;
	}

	/**
	 * Set where text selection should end
	 * @param obj       pointer to a Label object
	 * @param index     character index where selection should end. `LV_LABEL_TEXT_SELECTION_OFF` for no selection
	 */
	inline Label *setTextSelEnd(uint32_t index) {
		lv_label_set_text_sel_end(_obj, index);
		return this;
	}

	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the text of a Label
	 * @param obj       pointer to a Label object
	 * @return          the text of the Label
	 */
	inline char *getText() {
		return lv_label_get_text(_obj);
	}

	/**
	 * Get the long mode of a Label
	 * @param obj       pointer to a Label object
	 * @return          the current long mode
	 */
	inline lv_label_long_mode_t getLongMode() {
		return lv_label_get_long_mode(_obj);
	}

	/**
	 * Get the recoloring attribute
	 * @param obj       pointer to a Label object
	 * @return          true: recoloring is enabled, false: disable
	 */
	inline bool getReColor() {
		return lv_label_get_recolor(_obj);
	}

	/**
	 * Get the relative x and y coordinates of a letter
	 * @param obj       pointer to a Label object
	 * @param index     index of the character [0 ... text length - 1].
	 *                  Expressed in character index, not byte index (different in UTF-8)
	 * @param pos       store the result here (E.g. index = 0 gives 0;0 coordinates if the text if aligned to the left)
	 */
	inline Label *getLetter(uint32_t char_id, lv_point_t * pos) {
		lv_label_get_letter_pos(_obj, char_id, pos);
		return this;
	}

	/**
	 * Get the index of letter on a relative point of a Label.
	 * @param obj       pointer to Label object
	 * @param pos       pointer to point with coordinates on a the Label
	 * @return          The index of the letter on the 'pos_p' point (E.g. on 0;0 is the 0. letter if aligned to the left)
	 *                  Expressed in character index and not byte index (different in UTF-8)
	 */
	inline uint32_t getLetterOn(lv_point_t * pos_in) {
		return lv_label_get_letter_on(_obj, pos_in);
	}

	/**
	 * Check if a character is drawn under a point.
	 * @param obj       pointer to a Label object
	 * @param pos       Point to check for character under
	 * @return          whether a character is drawn under the point
	 */
	inline bool isCharUnderPos(lv_point_t * pos) {
		return lv_label_is_char_under_pos(_obj, pos);
	}

	/**
	 * @brief Get the selection start index.
	 * @param obj       pointer to a Label object.
	 * @return          selection start index. `LV_LABEL_TEXT_SELECTION_OFF` if nothing is selected.
	 */
	inline uint32_t getTextSelectionStart() {
		return lv_label_get_text_selection_start(_obj);
	}

	/**
	 * @brief Get the selection end index.
	 * @param obj       pointer to a Label object.
	 * @return          selection end index. `LV_LABEL_TXT_SEL_OFF` if nothing is selected.
	 */
	inline uint32_t getTextSelectionEnd() {
		return lv_label_get_text_selection_end(_obj);
	}

	/*=====================
	 * Other functions
	 *====================*/

	/**
	 * Insert a text to a Label. The Label text can not be static.
	 * @param obj       pointer to a Label object
	 * @param pos       character index to insert. Expressed in character index and not byte index.
	 *                  0: before first char. LV_LABEL_POS_LAST: after last char.
	 * @param txt       pointer to the text to insert
	 */
	inline Label *insertText(uint32_t pos, const char * txt) {
		lv_label_ins_text(_obj, pos, txt);
		return this;
	}

	/**
	 * Delete characters from a Label. The Label text can not be static.
	 * @param obj       pointer to a Label object
	 * @param pos       character index from where to cut. Expressed in character index and not byte index.
	 *                  0: start in from of the first character
	 * @param cnt       number of characters to cut
	 */
	inline Label *cutText(uint32_t pos, uint32_t cnt) {
		lv_label_cut_text(_obj, pos, cnt);
		return this;
	}
};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_LABEL_H_ */
