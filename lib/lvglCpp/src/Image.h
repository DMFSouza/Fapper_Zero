/*
 * Image.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_IMAGE_H_
#define LVGLCPP_SRC_IMAGE_H_

#include "Object.h"

namespace lvgl {
class Image : public Object {
public:
	Image(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_img_create(parent);
		} else {
			_obj = lv_img_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	~Image() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set the image data to display on the object
	 * @param obj       pointer to an image object
	 * @param src_img   1) pointer to an ::lv_img_dsc_t descriptor (converted by LVGL's image converter) (e.g. &my_img) or
	 *                  2) path to an image file (e.g. "S:/dir/img.bin")or
	 *                  3) a SYMBOL (e.g. LV_SYMBOL_OK)
	 */
	inline Image *setSource(const void *src) {
		lv_img_set_src(_obj, src);
		return this;
	}

	/**
	 * Set an offset for the source of an image so the image will be displayed from the new origin.
	 * @param obj       pointer to an image
	 * @param x         the new offset along x axis.
	 */
	inline Image *setOffset(lv_coord_t x, lv_coord_t y) {
		lv_img_set_offset_x(_obj, x);
		lv_img_set_offset_y(_obj, y);
		return this;
	}

	/**
	 * Set an offset for the source of an image so the image will be displayed from the new origin.
	 * @param obj       pointer to an image
	 * @param x         the new offset along x axis.
	 */
	inline Image *setOffsetX(lv_coord_t x) {
		lv_img_set_offset_x(_obj, x);
		return this;
	}

	/**
	 * Set an offset for the source of an image.
	 * so the image will be displayed from the new origin.
	 * @param obj       pointer to an image
	 * @param y         the new offset along y axis.
	 */
	inline Image *setOffsetY(lv_coord_t y) {
		lv_img_set_offset_y(_obj, y);
		return this;
	}


	/**
	 * Set the rotation angle of the image.
	 * The image will be rotated around the set pivot set by `lv_img_set_pivot()`
	 * Note that indexed and alpha only images can't be transformed.
	 * @param obj       pointer to an image object
	 * @param angle     rotation angle in degree with 0.1 degree resolution (0..3600: clock wise)
	 */
	inline Image *setAngle(int16_t angle) {
		lv_img_set_angle(_obj, angle);
		return this;
	}

	/**
	 * Set the rotation center of the image.
	 * The image will be rotated around this point.
	 * @param obj       pointer to an image object
	 * @param x         rotation center x of the image
	 * @param y         rotation center y of the image
	 */
	inline Image *setPivot(lv_coord_t x, lv_coord_t y) {
		lv_img_set_pivot(_obj, x, y);
		return this;
	}


	/**
	 * Set the zoom factor of the image.
	 * Note that indexed and alpha only images can't be transformed.
	 * @param img       pointer to an image object
	 * @param zoom      the zoom factor.
	 * @example 256 or LV_ZOOM_IMG_NONE for no zoom
	 * @example <256: scale down
	 * @example >256 scale up
	 * @example 128 half size
	 * @example 512 double size
	 */
	inline Image *setZoom(uint16_t zoom) {
		lv_img_set_zoom(_obj, zoom);
		return this;
	}

	/**
	 * Enable/disable anti-aliasing for the transformations (rotate, zoom) or not.
	 * The quality is better with anti-aliasing looks better but slower.
	 * @param obj       pointer to an image object
	 * @param antialias true: anti-aliased; false: not anti-aliased
	 */
	inline Image *setAntiAlias(bool antialias) {
		lv_img_set_antialias(_obj, antialias);
		return this;
	}

	/**
	 * Set the image object size mode.
	 *
	 * @param obj       pointer to an image object
	 * @param mode      the new size mode.
	 */
	inline Image *setSizeMode(lv_img_size_mode_t mode) {
		lv_img_set_size_mode(_obj, mode);
		return this;
	}
	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the source of the image
	 * @param obj       pointer to an image object
	 * @return          the image source (symbol, file name or ::lv-img_dsc_t for C arrays)
	 */
	inline const void *getSource() {
		return lv_img_get_src(_obj);
	}

	/**
	 * Get the offset's x attribute of the image object.
	 * @param img       pointer to an image
	 * @return          offset X value.
	 */
	inline lv_coord_t getOffsetX() {
		return lv_img_get_offset_x(_obj);
	}

	/**
	 * Get the offset's y attribute of the image object.
	 * @param obj       pointer to an image
	 * @return          offset Y value.
	 */
	inline lv_coord_t getOffsetY() {
		return lv_img_get_offset_y(_obj);
	}

	/**
	 * Get the rotation angle of the image.
	 * @param obj       pointer to an image object
	 * @return      rotation angle in 0.1 degrees (0..3600)
	 */
	inline uint16_t getAngle() {
		return lv_img_get_angle(_obj);
	}

	/**
	 * Get the pivot (rotation center) of the image.
	 * @param img       pointer to an image object
	 * @param pivot     store the rotation center here
	 */
	inline lv_point_t getPivot() {
		lv_point_t pivot;
		lv_img_get_pivot(_obj, &pivot);
		return pivot;
	}

	/**
	 * Get the zoom factor of the image.
	 * @param obj       pointer to an image object
	 * @return          zoom factor (256: no zoom)
	 */
	inline uint16_t getZoom() {
		return lv_img_get_zoom(_obj);
	}

	/**
	 * Get whether the transformations (rotate, zoom) are anti-aliased or not
	 * @param obj       pointer to an image object
	 * @return          true: anti-aliased; false: not anti-aliased
	 */
	inline bool getAntialias() {
		return lv_img_get_antialias(_obj);
	}

	/**
	 * Get the size mode of the image
	 * @param obj       pointer to an image object
	 * @return          element of @ref lv_img_size_mode_t
	 */
	inline lv_img_size_mode_t getSizeMode() {
		return lv_img_get_size_mode(_obj);
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_IMAGE_H_ */
