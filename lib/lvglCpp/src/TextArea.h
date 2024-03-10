/*
 * TextArea.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_TEXTAREA_H_
#define LVGLCPP_SRC_TEXTAREA_H_

#include "Object.h"

namespace lvgl {
class TextArea : public Object {
public:
	TextArea(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_textarea_create(parent);
		} else {
			_obj = lv_textarea_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	~TextArea() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*======================
	 * Add/remove functions
	 *=====================*/

	/**
	 * Insert a character to the current cursor position.
	 * To add a wide char, e.g. 'Á' use `_lv_txt_encoded_conv_wc('Á')`
	 * @param c         a character (e.g. 'a')
	 */
	inline TextArea *add(char c) {
		lv_textarea_add_char(_obj, c);
		return this;
	}

	/**
	 * Insert a text to the current cursor position
	 * @param fmt       a formated string
	 */
	TextArea *add(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		int size = vsnprintf(NULL, 0, fmt, args);
		va_end(args);
		char buffer[size + 1];
		va_start(args, fmt);
		vsnprintf(buffer, size + 1, fmt, args);
		va_end(args);
		lv_textarea_add_text(_obj, buffer);
		return this;
	}

	/**
	 * Delete a the left character from the current cursor position
	 * @param obj       pointer to a text area object
	 */
	inline TextArea *deleteChar() {
		lv_textarea_del_char(_obj);
		return this;
	}

	/**
	 * Delete the right character from the current cursor position
	 * @param obj       pointer to a text area object
	 */
	inline TextArea *deleteCharForward() {
		lv_textarea_del_char_forward(_obj);
		return this;
	}

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set the text of a text area
	 * @param obj       pointer to a text area object
	 * @param txt       pointer to the text
	 */
	TextArea *setText(const char * fmt, ...) {
		va_list args;
		va_start(args, fmt);
		int size = vsnprintf(NULL, 0, fmt, args);
		va_end(args);
		char buffer[size + 1];
		va_start(args, fmt);
		vsnprintf(buffer, size + 1, fmt, args);
		va_end(args);
		lv_textarea_set_text(_obj, buffer);
		return this;
	}

	/**
	 * Set the placeholder text of a text area
	 * @param obj       pointer to a text area object
	 * @param txt       pointer to the text
	 */
	TextArea *setPlaceholderText(const char * fmt, ...) {
		va_list args;
		va_start(args, fmt);
		int size = vsnprintf(NULL, 0, fmt, args);
		va_end(args);
		char buffer[size + 1];
		va_start(args, fmt);
		vsnprintf(buffer, size + 1, fmt, args);
		va_end(args);
		lv_textarea_set_placeholder_text(_obj, buffer);
		return this;
	}

	/**
	 * Set the cursor position
	 * @param obj       pointer to a text area object
	 * @param pos       the new cursor position in character index
	 *                  < 0 : index from the end of the text
	 *                  LV_TEXTAREA_CURSOR_LAST: go after the last character
	 */
	inline TextArea *setCursorPos(int32_t pos) {
		lv_textarea_set_cursor_pos(_obj, pos);
		return this;
	}

	/**
	 * Enable/Disable the positioning of the cursor by clicking the text on the text area.
	 * @param obj       pointer to a text area object
	 * @param en        true: enable click positions; false: disable
	 */
	inline TextArea *setCursorClickPos(bool en) {
		lv_textarea_set_cursor_click_pos(_obj, en);
		return this;
	}

	/**
	 * Enable/Disable password mode
	 * @param obj       pointer to a text area object
	 * @param en        true: enable, false: disable
	 */
	inline TextArea *setPasswordMode(bool en) {
		lv_textarea_set_password_mode(_obj, en);
		return this;
	}

	/**
	 * Set the replacement characters to show in password mode
	 * @param obj       pointer to a text area object
	 * @param bullet    pointer to the replacement text
	 */
	inline TextArea *setPasswordBullet(const char *bullet) {
		lv_textarea_set_password_bullet(_obj, bullet);
		return this;
	}

	/**
	 * Configure the text area to one line or back to normal
	 * @param obj       pointer to a text area object
	 * @param en        true: one line, false: normal
	 */
	inline TextArea *setOneLine(bool en) {
		lv_textarea_set_one_line(_obj, en);
		return this;
	}

	/**
	 * Set a list of characters. Only these characters will be accepted by the text area
	 * @param obj       pointer to a text area object
	 * @param list      list of characters. Only the pointer is saved. E.g. "+-.,0123456789"
	 */
	inline TextArea *setAcceptedChars(const char *list) {
		lv_textarea_set_accepted_chars(_obj, list);
		return this;
	}

	/**
	 * Set max length of a Text Area.
	 * @param obj       pointer to a text area object
	 * @param num       the maximal number of characters can be added (`lv_textarea_set_text` ignores it)
	 */
	inline TextArea *setMaxLength(uint32_t num) {
		lv_textarea_set_max_length(_obj, num);
		return this;
	}

	/**
	 * In `LV_EVENT_INSERT` the text which planned to be inserted can be replaced by an other text.
	 * It can be used to add automatic formatting to the text area.
	 * @param obj       pointer to a text area object
	 * @param txt       pointer to a new string to insert. If `""` no text will be added.
	 *                  The variable must be live after the `event_cb` exists. (Should be `global` or `static`)
	 */
	inline TextArea *setInsertReplace(const char * txt) {
		lv_textarea_set_insert_replace(_obj, txt);
		return this;
	}

	/**
	 * Enable/disable selection mode.
	 * @param obj       pointer to a text area object
	 * @param en        true or false to enable/disable selection mode
	 */
	inline TextArea *setTextSelection(bool en) {
		lv_textarea_set_text_selection(_obj, en);
		return this;
	}

	/**
	 * Set how long show the password before changing it to '*'
	 * @param obj       pointer to a text area object
	 * @param time      show time in milliseconds. 0: hide immediately.
	 */
	inline TextArea *setPasswordShowTime(uint16_t ms) {
		lv_textarea_set_password_show_time(_obj, ms);
		return this;
	}

	/**
	 * Deprecated: use the normal text_align style property instead
	 * Set the label's alignment.
	 * It sets where the label is aligned (in one line mode it can be smaller than the text area)
	 * and how the lines of the area align in case of multiline text area
	 * @param obj       pointer to a text area object
	 * @param align     the align mode from ::lv_text_align_t
	 */
	inline TextArea *setAlign(lv_text_align_t align) {
		lv_textarea_set_align(_obj, align);
		return this;
	}

	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the text of a text area. In password mode it gives the real text (not '*'s).
	 * @param obj       pointer to a text area object
	 * @return          pointer to the text
	 */
	inline const char *getText() {
		return lv_textarea_get_text((const lv_obj_t *)_obj);
	}

	/**
	 * Get the placeholder text of a text area
	 * @param obj       pointer to a text area object
	 * @return          pointer to the text
	 */
	inline const char *getPlaceholderText() {
		return lv_textarea_get_placeholder_text(_obj);
	}

	/**
	 * Get the label of a text area
	 * @param obj       pointer to a text area object
	 * @return          pointer to the label object
	 */
	inline lv_obj_t *getLabel() {
		return lv_textarea_get_label((const lv_obj_t *)_obj);
	}

	/**
	 * Get the current cursor position in character index
	 * @param obj       pointer to a text area object
	 * @return          the cursor position
	 */
	inline uint32_t getCursorPos() {
		return lv_textarea_get_cursor_pos((const lv_obj_t *)_obj);
	}

	/**
	 * Get whether the cursor click positioning is enabled or not.
	 * @param obj       pointer to a text area object
	 * @return          true: enable click positions; false: disable
	 */
	inline bool getCursorClickPos() {
		return lv_textarea_get_cursor_click_pos(_obj);
	}

	/**
	 * Get the password mode attribute
	 * @param obj       pointer to a text area object
	 * @return          true: password mode is enabled, false: disabled
	 */
	inline bool getPasswordMode() {
		return lv_textarea_get_password_mode((const lv_obj_t *)_obj);
	}

	/**
	 * Get the replacement characters to show in password mode
	 * @param obj       pointer to a text area object
	 * @return          pointer to the replacement text
	 */
	inline const char *getPasswordBullet() {
		return lv_textarea_get_password_bullet(_obj);
	}

	/**
	 * Get the one line configuration attribute
	 * @param obj       pointer to a text area object
	 * @return          true: one line configuration is enabled, false: disabled
	 */
	inline bool getOneLine() {
		return lv_textarea_get_one_line((const lv_obj_t *)_obj);
	}

	/**
	 * Get a list of accepted characters.
	 * @param obj       pointer to a text area object
	 * @return          list of accented characters.
	 */
	inline const char *getAcceptedChars() {
		return lv_textarea_get_accepted_chars(_obj);
	}

	/**
	 * Get max length of a Text Area.
	 * @param obj       pointer to a text area object
	 * @return          the maximal number of characters to be add
	 */
	inline uint32_t getMaxLength() {
		return lv_textarea_get_max_length(_obj);
	}

	/**
	 * Find whether text is selected or not.
	 * @param obj       pointer to a text area object
	 * @return          whether text is selected or not
	 */
	inline bool textIsSelected() {
		return lv_textarea_text_is_selected((const lv_obj_t *)_obj);
	}

	/**
	 * Find whether selection mode is enabled.
	 * @param obj       pointer to a text area object
	 * @return          true: selection mode is enabled, false: disabled
	 */
	inline bool getTextSelection() {
		return lv_textarea_get_text_selection(_obj);
	}

	/**
	 * Set how long show the password before changing it to '*'
	 * @param obj       pointer to a text area object
	 * @return          show time in milliseconds. 0: hide immediately.
	 */
	inline uint16_t getPasswordShowTime() {
		return lv_textarea_get_password_show_time(_obj);
	}

	/*=====================
	 * Other functions
	 *====================*/

	/**
	 * Clear the selection on the text area.
	 * @param obj       pointer to a text area object
	 */
	inline TextArea *clearSelection() {
		lv_textarea_clear_selection(_obj);
		return this;
	}

	/**
	 * Move the cursor one character right
	 * @param obj       pointer to a text area object
	 */
	inline TextArea *cursorRight() {
		lv_textarea_cursor_right(_obj);
		return this;
	}

	/**
	 * Move the cursor one character left
	 * @param obj       pointer to a text area object
	 */
	inline TextArea *cursorLeft() {
		lv_textarea_cursor_left(_obj);
		return this;
	}

	/**
	 * Move the cursor one line down
	 * @param obj       pointer to a text area object
	 */
	inline TextArea *cursorDown() {
		lv_textarea_cursor_down(_obj);
		return this;
	}

	/**
	 * Move the cursor one line up
	 * @param obj       pointer to a text area object
	 */
	inline TextArea *cursorUp() {
		lv_textarea_cursor_up(_obj);
		return this;
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_TEXTAREA_H_ */
