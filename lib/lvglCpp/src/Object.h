/*
 * Object.h
 *
 *  Created on: Nov 25, 2023
 *      Author: morgoth
 */

#ifndef LVCPP_LVOBJ_H_
#define LVCPP_LVOBJ_H_

#if !__IN_ECLIPSE__
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif
#include <stdlib.h>


namespace lvgl {

class Object {
protected:
	lv_obj_t *_obj;
	lv_obj_t *_child;
	lv_obj_t **_childs;
public:

	void setEnabled(bool enable) {
	    if (enable) {
	        lv_obj_clear_state(_obj, LV_STATE_DISABLED);
	    }
	    else {
	        lv_obj_add_state(_obj, LV_STATE_DISABLED);
	    }
	}

	inline bool getEnabled() {
	    return lv_obj_has_state(_obj, LV_STATE_DISABLED);
	}

	void setChecked(bool checked) {
	    if (checked) {
	        lv_obj_add_state(_obj, LV_STATE_CHECKED);
	    }
	    else {
	        lv_obj_clear_state(_obj, LV_STATE_CHECKED);
	    }
	}

	inline bool getChecked() {
	    return lv_obj_has_state(_obj, LV_STATE_CHECKED);
	}

	void setVisible(bool visible) {
	    if (visible) {
	    	lv_obj_clear_flag(_obj, LV_OBJ_FLAG_HIDDEN);
	    }
	    else {
	    	lv_obj_add_flag(_obj, LV_OBJ_FLAG_HIDDEN);
	    }
	}

	inline bool getVisible() {
	    return _obj->flags & LV_STATE_CHECKED ? false : true;
	}

	/**
	 * Set the position of an object relative to the set alignment.
	 * @param Object       pointer to an object
	 * @param x         new x coordinate
	 * @param y         new y coordinate
	 * @note            With default alignment it's the distance from the top left corner
	 * @note            E.g. LV_ALIGN_CENTER alignment it's the offset from the center of the parent
	 * @note            The position is interpreted on the content area of the parent
	 * @note            The values can be set in pixel or in percentage of parent size with `lv_pct(v)`
	 */
	inline Object *setPos(lv_coord_t x, lv_coord_t y) {
		lv_obj_set_pos(_obj, x, y);
	return this;
	}

	/**
	 * Set the x coordinate of an object
	 * @param Object       pointer to an object
	 * @param x         new x coordinate
	 * @note            With default alignment it's the distance from the top left corner
	 * @note            E.g. LV_ALIGN_CENTER alignment it's the offset from the center of the parent
	 * @note            The position is interpreted on the content area of the parent
	 * @note            The values can be set in pixel or in percentage of parent size with `lv_pct(v)`
	 */

	inline Object *setPosX(lv_coord_t x) {
		lv_obj_set_x(_obj, x);
	return this;
	}

	/**
	 * Set the y coordinate of an object
	 * @param Object       pointer to an object
	 * @param y         new y coordinate
	 * @note            With default alignment it's the distance from the top left corner
	 * @note            E.g. LV_ALIGN_CENTER alignment it's the offset from the center of the parent
	 * @note            The position is interpreted on the content area of the parent
	 * @note            The values can be set in pixel or in percentage of parent size with `lv_pct(v)`
	 */
	inline Object *setPosY(lv_coord_t y) {
		lv_obj_set_y(_obj, y);
	return this;
	}

	/**
	 * Set the size of an object.
	 * @param Object       pointer to an object
	 * @param w         the new width
	 * @param h         the new height
	 * @note            possible values are:
	 *                  pixel               simple set the size accordingly
	 *                  LV_SIZE_CONTENT     set the size to involve all children in the given direction
	 *                  LV_SIZE_PCT(x)     to set size in percentage of the parent's content area size (the size without paddings).
	 *                                      x should be in [0..1000]% range
	 */
	inline Object *setSize(lv_coord_t w, lv_coord_t h) {
		lv_obj_set_size(_obj, w, h);
	return this;
	}


	/**
	 * Recalculate the size of the object
	 * @param Object       pointer to an object
	 * @return          true: the size has been changed
	 */
	inline Object *setRefrSize() {
		lv_obj_refr_size(_obj);
	return this;
	}


	/**
	 * Set the width of an object
	 * @param Object       pointer to an object
	 * @param w         the new width
	 * @note            possible values are:
	 *                  pixel               simple set the size accordingly
	 *                  LV_SIZE_CONTENT     set the size to involve all children in the given direction
	 *                  lv_pct(x)           to set size in percentage of the parent's content area size (the size without paddings).
	 *                                      x should be in [0..1000]% range
	 */
	inline Object *setWidth(lv_coord_t w) {
		lv_obj_set_width(_obj, w);
	return this;
	}


	/**
	 * Set the height of an object
	 * @param Object       pointer to an object
	 * @param h         the new height
	 * @note            possible values are:
	 *                  pixel               simple set the size accordingly
	 *                  LV_SIZE_CONTENT     set the size to involve all children in the given direction
	 *                  lv_pct(x)           to set size in percentage of the parent's content area size (the size without paddings).
	 *                                      x should be in [0..1000]% range
	 */
	inline Object *setHeight(lv_coord_t h) {
		lv_obj_set_height(_obj, h);
	return this;
	}


	/**
	 * Set the width reduced by the left and right padding and the border width.
	 * @param Object       pointer to an object
	 * @param w         the width without paddings in pixels
	 */
	inline Object *setContentWidth(lv_coord_t w) {
		lv_obj_set_content_width(_obj, w);
	return this;
	}


	/**
	 * Set the height reduced by the top and bottom padding and the border width.
	 * @param Object       pointer to an object
	 * @param h         the height without paddings in pixels
	 */
	inline Object *setContentHeight(lv_coord_t h) {
		lv_obj_set_content_height(_obj, h);
	return this;
	}


	/**
	 * Set a layout for an object
	 * @param Object       pointer to an object
	 * @param layout    pointer to a layout descriptor to set
	 */
	inline Object *setLayout(uint32_t layout) {
		lv_obj_set_layout(_obj, layout);
		return this;
	}

	/**
	 * Test whether the and object is positioned by a layout or not
	 * @param Object       pointer to an object to test
	 * @return true:    positioned by a layout; false: not positioned by a layout
	 */
	inline bool isLayoutPositioned() {
		return lv_obj_is_layout_positioned(_obj);
	}

	/**
	 * Mark the object for layout update.
	 * @param Object      pointer to an object whose children needs to be updated
	 */
	inline Object *markLayoutAsDirty() {
		lv_obj_mark_layout_as_dirty(_obj);
		return this;
	}

	/**
	 * Update the layout of an object.
	 * @param Object      pointer to an object whose children needs to be updated
	 */
	inline Object *updateLayout() {
		lv_obj_update_layout(_obj);
		return this;
	}

	/**
	 * Register a new layout
	 * @param cb        the layout update callback
	 * @param user_data custom data that will be passed to `cb`
	 * @return          the ID of the new layout
	 */
	inline uint32_t layoutRegister(lv_layout_update_cb_t cb, void * user_data) {
		return lv_layout_register(cb, user_data);
	}

	/**
	 * Change the alignment of an object.
	 * @param Object       pointer to an object to align
	 * @param align     type of alignment (see 'lv_align_t' enum) `LV_ALIGN_OUT_...` can't be used.
	 */
	inline Object *setAlign(lv_align_t align) {
		lv_obj_set_align(_obj,  align);
		return this;
	}

	/**
	 * Change the alignment of an object and set new coordinates.
	 * Equivalent to:
	 * lv_obj_set_align(Object, align);
	 * lv_obj_set_pos(Object, x_ofs, y_ofs);
	 * @param Object       pointer to an object to align
	 * @param align     type of alignment (see 'lv_align_t' enum) `LV_ALIGN_OUT_...` can't be used.
	 * @param x_ofs     x coordinate offset after alignment
	 * @param y_ofs     y coordinate offset after alignment
	 */
	inline Object *objAlign(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) {
		lv_obj_align(_obj,  align,  x_ofs,  y_ofs);
		return this;
	}

	/**
	 * Align an object to an other object.
	 * @param Object       pointer to an object to align
	 * @param base      pointer to an other object (if NULL `Object`s parent is used). 'Object' will be aligned to it.
	 * @param align     type of alignment (see 'lv_align_t' enum)
	 * @param x_ofs     x coordinate offset after alignment
	 * @param y_ofs     y coordinate offset after alignment
	 * @note            if the position or size of `base` changes `Object` needs to be aligned manually again
	 */
	inline Object *objAlignTo(const lv_obj_t * _base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) {
		lv_obj_align_to(_obj, _base, align, x_ofs, y_ofs);
		return this;
	}


	/**
	 * Align an object to the center on its parent.
	 * @param Object       pointer to an object to align
	 * @note            if the parent size changes `Object` needs to be aligned manually again
	 */
	static inline void objCenter(struct _lv_obj_t * Object) {
	    lv_obj_align(Object, LV_ALIGN_CENTER, 0, 0);
	}


	/**
	 * Copy the coordinates of an object to an area
	 * @param Object       pointer to an object
	 * @param coords    pointer to an area to store the coordinates
	 */
	inline Object *getCoords(lv_area_t * coords) {
		lv_obj_get_coords(_obj, coords);
		return this;
	}

	/**
	 * Get the x coordinate of object.
	 * @param Object       pointer to an object
	 * @return          distance of `Object` from the left side of its parent plus the parent's left padding
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @note            Zero return value means the object is on the left padding of the parent, and not on the left edge.
	 * @note            Scrolling of the parent doesn't change the returned value.
	 * @note            The returned value is always the distance from the parent even if `Object` is positioned by a layout.
	 */
	inline lv_coord_t getX() {
		return lv_obj_get_x(_obj);
	}

	/**
	 * Get the x2 coordinate of object.
	 * @param Object       pointer to an object
	 * @return          distance of `Object` from the right side of its parent plus the parent's right padding
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @note            Zero return value means the object is on the right padding of the parent, and not on the right edge.
	 * @note            Scrolling of the parent doesn't change the returned value.
	 * @note            The returned value is always the distance from the parent even if `Object` is positioned by a layout.
	 */
	inline lv_coord_t getX2() {
		return lv_obj_get_x2(_obj);
	}

	/**
	 * Get the y coordinate of object.
	 * @param Object       pointer to an object
	 * @return          distance of `Object` from the top side of its parent plus the parent's top padding
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @note            Zero return value means the object is on the top padding of the parent, and not on the top edge.
	 * @note            Scrolling of the parent doesn't change the returned value.
	 * @note            The returned value is always the distance from the parent even if `Object` is positioned by a layout.
	 */
	inline lv_coord_t getY() {
		return lv_obj_get_y(_obj);
	}

	/**
	 * Get the y2 coordinate of object.
	 * @param Object       pointer to an object
	 * @return          distance of `Object` from the bottom side of its parent plus the parent's bottom padding
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @note            Zero return value means the object is on the bottom padding of the parent, and not on the bottom edge.
	 * @note            Scrolling of the parent doesn't change the returned value.
	 * @note            The returned value is always the distance from the parent even if `Object` is positioned by a layout.
	 */
	inline lv_coord_t getY2() {
		return lv_obj_get_y2(_obj);
	}

	/**
	 * Get the actually set x coordinate of object, i.e. the offset form the set alignment
	 * @param Object       pointer to an object
	 * @return          the set x coordinate
	 */
	inline lv_coord_t getXAligned() {
		return lv_obj_get_x_aligned(_obj);
	}

	/**
	 * Get the actually set y coordinate of object, i.e. the offset form the set alignment
	 * @param Object       pointer to an object
	 * @return          the set y coordinate
	 */
	inline lv_coord_t getYAligned() {
		return lv_obj_get_y_aligned(_obj);
	}

	/**
	 * Get the width of an object
	 * @param Object       pointer to an object
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @return          the width in pixels
	 */
	inline lv_coord_t getWidth() {
		return lv_obj_get_width(_obj);
	}

	/**
	 * Get the height of an object
	 * @param Object       pointer to an object
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @return          the height in pixels
	 */
	inline lv_coord_t getHeight() {
		return lv_obj_get_height(_obj);
	}

	/**
	 * Get the width reduced by the left and right padding and the border width.
	 * @param Object       pointer to an object
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @return          the width which still fits into its parent without causing overflow (making the parent scrollable)
	 */
	inline lv_coord_t getConstantWidth() {
		return lv_obj_get_content_width(_obj);
	}

	/**
	 * Get the height reduced by the top and bottom padding and the border width.
	 * @param Object       pointer to an object
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @return          the height which still fits into the parent without causing overflow (making the parent scrollable)
	 */
	inline lv_coord_t getConstantHeight() {
		return lv_obj_get_content_height(_obj);
	}

	/**
	 * Get the area reduced by the paddings and the border width.
	 * @param Object       pointer to an object
	 * @note            The position of the object is recalculated only on the next redraw. To force coordinate recalculation
	 *                  call `lv_obj_update_layout(Object)`.
	 * @param area      the area which still fits into the parent without causing overflow (making the parent scrollable)
	 */
	inline Object *getContentCoords(lv_area_t * area) {
		lv_obj_get_content_coords(_obj, area);
		return this;
	}

	/**
	 * Get the width occupied by the "parts" of the widget. E.g. the width of all columns of a table.
	 * @param Object       pointer to an objects
	 * @return          the width of the virtually drawn content
	 * @note            This size independent from the real size of the widget.
	 *                  It just tells how large the internal ("virtual") content is.
	 */
	inline lv_coord_t getSelfWidth() {
		return lv_obj_get_self_width(_obj);
	}

	/**
	 * Get the height occupied by the "parts" of the widget. E.g. the height of all rows of a table.
	 * @param Object       pointer to an objects
	 * @return          the width of the virtually drawn content
	 * @note            This size independent from the real size of the widget.
	 *                  It just tells how large the internal ("virtual") content is.
	 */
	inline lv_coord_t getSelfHeight() {
		return lv_obj_get_self_height(_obj);
	}

	/**
	 * Handle if the size of the internal ("virtual") content of an object has changed.
	 * @param Object       pointer to an object
	 * @return          false: nothing happened; true: refresh happened
	 */
	inline bool refreshSelfSize() {
		return lv_obj_refresh_self_size(_obj);
	}

	inline Object *refrPos() {
		lv_obj_refr_pos(_obj);
		return this;
	}

	inline Object *moveTo(lv_coord_t x, lv_coord_t y) {
		lv_obj_move_to(_obj, x, y);
		return this;
	}


	inline Object *mobeChildrenBy(lv_coord_t x_diff, lv_coord_t y_diff, bool ignore_floating) {
		lv_obj_move_children_by(_obj, x_diff, y_diff, ignore_floating);
		return this;
	}

	/**
	 * Transform a point using the angle and zoom style properties of an object
	 * @param Object           pointer to an object whose style properties should be used
	 * @param p             a point to transform, the result will be written back here too
	 * @param recursive     consider the transformation properties of the parents too
	 * @param inv           do the inverse of the transformation (-angle and 1/zoom)
	 */
	inline Object *transformPoint(lv_point_t * p, bool recursive, bool inv) {
		lv_obj_transform_point(_obj, p, recursive, inv);
		return this;
	}

	/**
	 * Transform an area using the angle and zoom style properties of an object
	 * @param Object           pointer to an object whose style properties should be used
	 * @param area          an area to transform, the result will be written back here too
	 * @param recursive     consider the transformation properties of the parents too
	 * @param inv           do the inverse of the transformation (-angle and 1/zoom)
	 */
	inline Object *getTransformedArea(lv_area_t * area, bool recursive, bool inv) {
		lv_obj_get_transformed_area(_obj, area, recursive, inv);
		return this;
	}

	/**
	 * Mark an area of an object as invalid.
	 * The area will be truncated to the object's area and marked for redraw.
	 * @param Object       pointer to an object
	 * @param           area the area to redraw
	 */
	inline Object *invalidateArea(const lv_area_t * area) {
		lv_obj_invalidate_area(_obj, area);
		return this;
	}

	/**
	 * Mark the object as invalid to redrawn its area
	 * @param Object       pointer to an object
	 */
	inline Object *invalidate() {
		lv_obj_invalidate(_obj);
		return this;
	}

	/**
	 * Tell whether an area of an object is visible (even partially) now or not
	 * @param Object       pointer to an object
	 * @param area      the are to check. The visible part of the area will be written back here.
	 * @return true     visible; false not visible (hidden, out of parent, on other screen, etc)
	 */
	inline bool areaIsVisible(lv_area_t * area) {
		return lv_obj_area_is_visible(_obj, area);
	}

	/**
	 * Tell whether an object is visible (even partially) now or not
	 * @param Object       pointer to an object
	 * @return      true: visible; false not visible (hidden, out of parent, on other screen, etc)
	 */
	inline bool isVisible() {
		return lv_obj_is_visible(_obj);
	}

	/**
	 * Set the size of an extended clickable area
	 * @param Object       pointer to an object
	 * @param size      extended clickable area in all 4 directions [px]
	 */
	inline Object *setExtClickArea(lv_coord_t size) {
		lv_obj_set_ext_click_area(_obj, size);
		return this;
	}

	/**
	 * Get the an area where to object can be clicked.
	 * It's the object's normal area plus the extended click area.
	 * @param Object       pointer to an object
	 * @param area      store the result area here
	 */
	inline Object *getClickArea(lv_area_t * area) {
		lv_obj_get_click_area(_obj, area);
		return this;
	}

	/**
	 * Hit-test an object given a particular point in screen space.
	 * @param Object       object to hit-test
	 * @param point     screen-space point (absolute coordinate)
	 * @return          true: if the object is considered under the point
	 */
	inline bool hitTest(const lv_point_t * point) {
		return lv_obj_hit_test(_obj, point);
	}

	/**
	 * Clamp a width between min and max width. If the min/max width is in percentage value use the ref_width
	 * @param width         width to clamp
	 * @param min_width     the minimal width
	 * @param max_width     the maximal width
	 * @param ref_width     the reference width used when min/max width is in percentage
	 * @return              the clamped width
	 */
	inline lv_coord_t clampWidth(lv_coord_t width, lv_coord_t min_width, lv_coord_t max_width, lv_coord_t ref_width) {
		return lv_clamp_width(width, min_width, max_width, ref_width);
	}

	/**
	 * Clamp a height between min and max height. If the min/max height is in percentage value use the ref_height
	 * @param height         height to clamp
	 * @param min_height     the minimal height
	 * @param max_height     the maximal height
	 * @param ref_height     the reference height used when min/max height is in percentage
	 * @return              the clamped height
	 */
	inline lv_coord_t champHeight(lv_coord_t height, lv_coord_t min_height, lv_coord_t max_height, lv_coord_t ref_height) {
		return lv_clamp_height(height, min_height, max_height, ref_height);
	}




	inline Object *setStyleWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleMinWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_min_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleMaxWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_max_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleHeight(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_height(_obj, value, selector);
		return this;
	}
	inline Object *setStyleMinHeight(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_min_height(_obj, value, selector);
		return this;
	}
	inline Object *setStyleMaxHeight(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_max_height(_obj, value, selector);
		return this;
	}
	inline Object *setStyleX(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_x(_obj, value, selector);
		return this;
	}
	inline Object *setStyleY(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_y(_obj, value, selector);
		return this;
	}
	inline Object *setStyleAlign(lv_align_t value, lv_style_selector_t selector) {
		lv_obj_set_style_align(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_transform_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformHeight(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_transform_height(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformX(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_translate_x(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformY(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_translate_y(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformZoom(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_transform_zoom(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTranbsformAngle(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_transform_angle(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformPivotX(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_transform_pivot_x(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformPivotY(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_transform_pivot_y(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformPadTop(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_pad_top(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformPadBottom(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_pad_bottom(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformPadLeft(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_pad_left(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransformPadRight(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_pad_right(_obj, value, selector);
		return this;
	}
	inline Object *setStylePadRow(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_pad_row(_obj, value, selector);
		return this;
	}
	inline Object *setStylePadCollumn(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_pad_column(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgColor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_color(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgGradColor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_grad_color(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgGradDir(lv_grad_dir_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_grad_dir(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgMainStop(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_main_stop(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgGradStop(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_grad_stop(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgGrad(const lv_grad_dsc_t * value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_grad(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgDitherMode(lv_dither_mode_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_dither_mode(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgImgSrc(const void * value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_img_src(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgImgOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_img_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgImgRecolor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_img_recolor(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgImgRecolorOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_img_recolor_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBgImgTiled(bool value, lv_style_selector_t selector) {
		lv_obj_set_style_bg_img_tiled(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBorderColor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_border_color(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBorderOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_border_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBorderWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_border_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBorderSide(lv_border_side_t value, lv_style_selector_t selector) {
		lv_obj_set_style_border_side(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBorderPost(bool value, lv_style_selector_t selector) {
		lv_obj_set_style_border_post(_obj, value, selector);
		return this;
	}
	inline Object *setStyleOutlineWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_outline_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleOutlineColor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_outline_color(_obj, value, selector);
		return this;
	}
	inline Object *setStyleOutlineOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_outline_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleOutlinePad(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_outline_pad(_obj, value, selector);
		return this;
	}
	inline Object *setStyleShadowWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_shadow_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleShadowOfsX(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_shadow_ofs_x(_obj, value, selector);
		return this;
	}
	inline Object *setStyleShadowOfsY(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_shadow_ofs_y(_obj, value, selector);
		return this;
	}
	inline Object *setStyleShadowSpread(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_shadow_spread(_obj, value, selector);
		return this;
	}
	inline Object *setStyleShadowColor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_shadow_color(_obj, value, selector);
		return this;
	}
	inline Object *setStyleShadowOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_shadow_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleImgOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_img_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleImgRecolor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_img_recolor(_obj, value, selector);
		return this;
	}
	inline Object *setStyleImgRecolorOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_img_recolor_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleLineWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_line_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleLineDashWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_line_dash_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleLineDashGap(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_line_dash_gap(_obj, value, selector);
		return this;
	}
	inline Object *setStyleLineRounded(bool value, lv_style_selector_t selector) {
		lv_obj_set_style_line_rounded(_obj, value, selector);
		return this;
	}
	inline Object *setStyleLineColor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_line_color(_obj, value, selector);
		return this;
	}
	inline Object *setStyleLineOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_line_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleArcWidth(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_arc_width(_obj, value, selector);
		return this;
	}
	inline Object *setStyleArcRounded(bool value, lv_style_selector_t selector) {
		lv_obj_set_style_arc_rounded(_obj, value, selector);
		return this;
	}
	inline Object *setStyleArcColor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_arc_color(_obj, value, selector);
		return this;
	}
	inline Object *setStyleArcOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_arc_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleArcImgSrc(const void * value, lv_style_selector_t selector) {
		lv_obj_set_style_arc_img_src(_obj,value, selector);
		return this;
	}
	inline Object *setStyleTextColor(lv_color_t value, lv_style_selector_t selector) {
		lv_obj_set_style_text_color(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTextOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_text_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTextFont(const lv_font_t * value, lv_style_selector_t selector) {
		lv_obj_set_style_text_font(_obj,value, selector);
		return this;
	}
	inline Object *setStyleTextLetterSpace(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_text_letter_space(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTextLineSpace(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_text_line_space(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTextDecor(lv_text_decor_t value, lv_style_selector_t selector) {
		lv_obj_set_style_text_decor(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTextAlign(lv_text_align_t value, lv_style_selector_t selector) {
		lv_obj_set_style_text_align(_obj, value, selector);
		return this;
	}
	inline Object *setStyleRadius(lv_coord_t value, lv_style_selector_t selector) {
		lv_obj_set_style_radius(_obj, value, selector);
		return this;
	}
	inline Object *setStyleClipCorner(bool value, lv_style_selector_t selector) {
		lv_obj_set_style_clip_corner(_obj, value, selector);
		return this;
	}
	inline Object *setStyleOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleOpaLayered(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_opa_layered(_obj, value, selector);
		return this;
	}
	inline Object *setStyleColorFilterDsc(const lv_color_filter_dsc_t * value, lv_style_selector_t selector) {
		lv_obj_set_style_color_filter_dsc(_obj,value, selector);
		return this;
	}
	inline Object *setStyleColorFilterOpa(lv_opa_t value, lv_style_selector_t selector) {
		lv_obj_set_style_color_filter_opa(_obj, value, selector);
		return this;
	}
	inline Object *setStyleAnim(const lv_anim_t * value, lv_style_selector_t selector) {
		lv_obj_set_style_anim(_obj,value, selector);
		return this;
	}
	inline Object *setStyleAnimTime(uint32_t value, lv_style_selector_t selector) {
		lv_obj_set_style_anim_time(_obj, value, selector);
		return this;
	}
	inline Object *setStyleAnimSpeed(uint32_t value, lv_style_selector_t selector) {
		lv_obj_set_style_anim_speed(_obj, value, selector);
		return this;
	}
	inline Object *setStyleTransition(const lv_style_transition_dsc_t * value, lv_style_selector_t selector) {
		lv_obj_set_style_transition(_obj,value, selector);
		return this;
	}
	inline Object *setStyleBlendMode(lv_blend_mode_t value, lv_style_selector_t selector) {
		lv_obj_set_style_blend_mode(_obj, value, selector);
		return this;
	}
	inline Object *setStyleLayout(uint16_t value, lv_style_selector_t selector) {
		lv_obj_set_style_layout(_obj, value, selector);
		return this;
	}
	inline Object *setStyleBaseDir(lv_base_dir_t value, lv_style_selector_t selector) {
		lv_obj_set_style_base_dir(_obj, value, selector);
		return this;
	}
};

} /* namespace lvgl */

#endif /* LVCPP_LVOBJ_H_ */
