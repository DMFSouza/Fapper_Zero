/*
 * Arc.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_ARC_H_
#define LVGLCPP_SRC_ARC_H_

#include "Object.h"

namespace lvgl {
class Arc : public Object {
public:
	Arc(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_arc_create(parent);
		} else {
			_obj = lv_arc_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	/**
	 * Create an empty btnMatrix object, this is useful when used as a child.
	 */
	Arc() {
		_obj = NULL;
		_child = NULL;
		_childs = NULL;
	}

	~Arc() {

	}

	/**
	 * Set the object pointer when used as a child.
	 */
	inline Arc *setObj(lv_obj_t *obj) {
		_obj = obj;
		return this;
	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*======================
	 * Add/remove functions
	 *=====================*/

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set the start angle of an arc. 0 deg: right, 90 bottom, etc.
	 * @param obj   pointer to an arc object
	 * @param start the start angle
	 */
	inline Arc *setStartAngle(uint16_t start) {
		lv_arc_set_start_angle(_obj, start);
		return this;
	}

	/**
	 * Set the end angle of an arc. 0 deg: right, 90 bottom, etc.
	 * @param obj   pointer to an arc object
	 * @param end   the end angle
	 */
	inline Arc *setEndAngle(uint16_t end) {
		lv_arc_set_end_angle(_obj, end);
		return this;
	}

	/**
	 * Set the start and end angles
	 * @param obj   pointer to an arc object
	 * @param start the start angle
	 * @param end   the end angle
	 */
	inline Arc *setAngles(uint16_t start, uint16_t end) {
		lv_arc_set_angles(_obj, start, end);
		return this;
	}

	/**
	 * Set the start angle of an arc background. 0 deg: right, 90 bottom, etc.
	 * @param obj   pointer to an arc object
	 * @param start the start angle
	 */
	inline Arc *setBgStartAngle(uint16_t start) {
		lv_arc_set_bg_start_angle(_obj, start);
		return this;
	}

	/**
	 * Set the start angle of an arc background. 0 deg: right, 90 bottom etc.
	 * @param obj   pointer to an arc object
	 * @param end   the end angle
	 */
	inline Arc *setBgEndAngle(uint16_t end) {
		lv_arc_set_bg_end_angle(_obj, end);
		return this;
	}

	/**
	 * Set the start and end angles of the arc background
	 * @param obj   pointer to an arc object
	 * @param start the start angle
	 * @param end   the end angle
	 */
	inline Arc *setBgAngles(uint16_t start, uint16_t end) {
		lv_arc_set_bg_angles(_obj, start, end);
		return this;
	}

	/**
	 * Set the rotation for the whole arc
	 * @param obj       pointer to an arc object
	 * @param rotation  rotation angle
	 */
	inline Arc *setRotation(uint16_t rotation) {
		lv_arc_set_rotation(_obj, rotation);
		return this;
	}

	/**
	 * Set the type of arc.
	 * @param obj   pointer to arc object
	 * @param mode  arc's mode
	 */
	inline Arc *setMode(lv_arc_mode_t type) {
		lv_arc_set_mode(_obj, type);
		return this;
	}

	/**
	 * Set a new value on the arc
	 * @param obj   pointer to an arc object
	 * @param value new value
	 */
	inline Arc *setValue(int16_t value) {
		lv_arc_set_value(_obj, value);
		return this;
	}

	/**
	 * Set minimum and the maximum values of an arc
	 * @param obj   pointer to the arc object
	 * @param min   minimum value
	 * @param max   maximum value
	 */
	inline Arc *setRange(int16_t min, int16_t max) {
		lv_arc_set_range(_obj, min, max);
		return this;
	}

	/**
	 * Set a change rate to limit the speed how fast the arc should reach the pressed point.
	 * @param obj       pointer to an arc object
	 * @param rate      the change rate
	 */
	inline Arc *setChangeRate(uint16_t rate) {
		lv_arc_set_change_rate(_obj, rate);
		return this;
	}

	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the start angle of an arc.
	 * @param obj   pointer to an arc object
	 * @return      the start angle [0..360]
	 */
	inline uint16_t getAngleStart() {
		return lv_arc_get_angle_start(_obj);
	}

	/**
	 * Get the end angle of an arc.
	 * @param obj   pointer to an arc object
	 * @return      the end angle [0..360]
	 */
	inline uint16_t lv_arc_get_angle_end(lv_obj_t * obj);

	/**
	 * Get the start angle of an arc background.
	 * @param obj   pointer to an arc object
	 * @return      the  start angle [0..360]
	 */
	inline uint16_t getBgAngleStart() {
		return lv_arc_get_bg_angle_start(_obj);
	}

	/**
	 * Get the end angle of an arc background.
	 * @param obj   pointer to an arc object
	 * @return      the end angle [0..360]
	 */
	inline uint16_t getBgAngleEnd() {
		return lv_arc_get_bg_angle_end(_obj);
	}

	/**
	 * Get the value of an arc
	 * @param obj       pointer to an arc object
	 * @return          the value of the arc
	 */
	inline int16_t getValue() {
		return lv_arc_get_value((const lv_obj_t *)_obj);
	}

	/**
	 * Get the minimum value of an arc
	 * @param obj   pointer to an arc object
	 * @return      the minimum value of the arc
	 */
	inline int16_t getMinValue() {
		return lv_arc_get_min_value((const lv_obj_t *)_obj);
	}

	/**
	 * Get the maximum value of an arc
	 * @param obj   pointer to an arc object
	 * @return      the maximum value of the arc
	 */
	inline int16_t getMaxValue() {
		return lv_arc_get_max_value((const lv_obj_t *)_obj);
	}

	/**
	 * Get whether the arc is type or not.
	 * @param obj       pointer to an arc object
	 * @return          arc's mode
	 */
	inline lv_arc_mode_t getMode() {
		return lv_arc_get_mode((const lv_obj_t *)_obj);
	}

	/*=====================
	 * Other functions
	 *====================*/

	/**
	 * Align an object to the current position of the arc (knob)
	 * @param obj           pointer to an arc object
	 * @param obj_to_align  pointer to an object to align
	 * @param r_offset      consider the radius larger with this value (< 0: for smaller radius)
	 */
	inline Arc *alignObjectToAngle(lv_obj_t * obj_to_align, lv_coord_t r_offset) {
		lv_arc_align_obj_to_angle((const lv_obj_t *)_obj, obj_to_align, r_offset);
		return this;
	}

	/**
	 * Rotate an object to the current position of the arc (knob)
	 * @param obj           pointer to an arc object
	 * @param obj_to_align  pointer to an object to rotate
	 * @param r_offset      consider the radius larger with this value (< 0: for smaller radius)
	 */
	inline Arc *rotateObjectToAngle(lv_obj_t * obj_to_rotate, lv_coord_t r_offset) {
		lv_arc_rotate_obj_to_angle((const lv_obj_t *)_obj, obj_to_rotate, r_offset);
		return this;
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_ARC_H_ */
