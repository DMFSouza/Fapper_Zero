/*
 * Canvas.h
 *
 *  Created on: Nov 27, 2023
 *      Author: morgoth
 */

#ifndef LVGLCPP_SRC_CANVAS_H_
#define LVGLCPP_SRC_CANVAS_H_

#include "Object.h"

namespace lvgl {
class Canvas : public Object {
public:
	Canvas(lv_obj_t *parent = NULL) {
		if(parent) {
			_obj = lv_canvas_create(parent);
		} else {
			_obj = lv_canvas_create(lv_scr_act());
		}
		_child = NULL;
		_childs = NULL;
	}
	~Canvas() {

	}

	inline lv_obj_t *getObj() {
		return _obj;
	}

	/*=====================
	 * Setter functions
	 *====================*/

	/**
	 * Set a buffer for the canvas.
	 * @param buf a buffer where the content of the canvas will be.
	 * The required size is (lv_img_color_format_get_px_size(cf) * w) / 8 * h)
	 * It can be allocated with `lv_mem_alloc()` or
	 * it can be statically allocated array (e.g. static lv_color_t buf[100*50]) or
	 * it can be an address in RAM or external SRAM
	 * @param canvas pointer to a canvas object
	 * @param w width of the canvas
	 * @param h height of the canvas
	 * @param cf color format. `LV_IMG_CF_...`
	 */
	inline Canvas *setBuffer(void * buf, lv_coord_t w, lv_coord_t h, lv_img_cf_t cf) {
		lv_canvas_set_buffer(_obj, buf, w, h, cf);
		return this;
	}

	/**
	 * Set the color of a pixel on the canvas
	 * @param canvas
	 * @param x x coordinate of the point to set
	 * @param y x coordinate of the point to set
	 * @param c color of the pixel
	 */
	inline Canvas *setPixelColor(lv_coord_t x, lv_coord_t y, lv_color_t c) {
		lv_canvas_set_px_color(_obj, x, y, c);
		return this;
	}

	/**
	 * DEPRECATED: added only for backward compatibility
	 */
	static inline void setPixel(Canvas *inst, lv_coord_t x, lv_coord_t y, lv_color_t c) {
	    lv_canvas_set_px_color(inst->_obj, x, y, c);
	}

	/**
	 * Set the opacity of a pixel on the canvas
	 * @param canvas
	 * @param x x coordinate of the point to set
	 * @param y x coordinate of the point to set
	 * @param opa opacity of the pixel (0..255)
	 */
	inline Canvas *setPixelOpacity(lv_coord_t x, lv_coord_t y, lv_opa_t opa) {
		lv_canvas_set_px_opa(_obj, x, y, opa);
		return this;
	}


	/**
	 * Set the palette color of a canvas with index format. Valid only for `LV_IMG_CF_INDEXED1/2/4/8`
	 * @param canvas pointer to canvas object
	 * @param id the palette color to set:
	 *   - for `LV_IMG_CF_INDEXED1`: 0..1
	 *   - for `LV_IMG_CF_INDEXED2`: 0..3
	 *   - for `LV_IMG_CF_INDEXED4`: 0..15
	 *   - for `LV_IMG_CF_INDEXED8`: 0..255
	 * @param c the color to set
	 */
	inline Canvas *setPallette(uint8_t id, lv_color_t c) {
		lv_canvas_set_palette(_obj, id, c);
		return this;
	}

	/*=====================
	 * Getter functions
	 *====================*/

	/**
	 * Get the color of a pixel on the canvas
	 * @param canvas
	 * @param x x coordinate of the point to set
	 * @param y x coordinate of the point to set
	 * @return color of the point
	 */
	inline lv_color_t getPixel(lv_coord_t x, lv_coord_t y) {
		return lv_canvas_get_px(_obj, x, y);
	}

	/**
	 * Get the image of the canvas as a pointer to an `lv_img_dsc_t` variable.
	 * @param canvas pointer to a canvas object
	 * @return pointer to the image descriptor.
	 */
	inline lv_img_dsc_t * getImage() {
		return lv_canvas_get_img(_obj);
	}

	/*=====================
	 * Other functions
	 *====================*/

	/**
	 * Copy a buffer to the canvas
	 * @param canvas pointer to a canvas object
	 * @param to_copy buffer to copy. The color format has to match with the canvas's buffer color
	 * format
	 * @param x left side of the destination position
	 * @param y top side of the destination position
	 * @param w width of the buffer to copy
	 * @param h height of the buffer to copy
	 */
	inline Canvas *copyBuffer(const void * to_copy, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
		lv_canvas_copy_buf(_obj, to_copy, x, y, w, h);
		return this;
	}

	/**
	 * Transform and image and store the result on a canvas.
	 * @param canvas pointer to a canvas object to store the result of the transformation.
	 * @param img pointer to an image descriptor to transform.
	 *             Can be the image descriptor of an other canvas too (`lv_canvas_get_img()`).
	 * @param angle the angle of rotation (0..3600), 0.1 deg resolution
	 * @param zoom zoom factor (256 no zoom);
	 * @param offset_x offset X to tell where to put the result data on destination canvas
	 * @param offset_y offset X to tell where to put the result data on destination canvas
	 * @param pivot_x pivot X of rotation. Relative to the source canvas
	 *                Set to `source width / 2` to rotate around the center
	 * @param pivot_y pivot Y of rotation. Relative to the source canvas
	 *                Set to `source height / 2` to rotate around the center
	 * @param antialias apply anti-aliasing during the transformation. Looks better but slower.
	 */
	inline Canvas *transform(lv_img_dsc_t * img, int16_t angle, uint16_t zoom, lv_coord_t offset_x,
            lv_coord_t offset_y,
            int32_t pivot_x, int32_t pivot_y, bool antialias) {
		lv_canvas_transform(_obj, img, angle, zoom, offset_x, offset_y, pivot_x, pivot_y, antialias);
		return this;
	}

	/**
	 * Apply horizontal blur on the canvas
	 * @param canvas pointer to a canvas object
	 * @param area the area to blur. If `NULL` the whole canvas will be blurred.
	 * @param r radius of the blur
	 */
	inline Canvas *blurHorizontal(const lv_area_t * area, uint16_t r) {
		lv_canvas_blur_hor(_obj, area, r);
		return this;
	}

	/**
	 * Apply vertical blur on the canvas
	 * @param canvas pointer to a canvas object
	 * @param area the area to blur. If `NULL` the whole canvas will be blurred.
	 * @param r radius of the blur
	 */
	inline Canvas *blurVertical(const lv_area_t * area, uint16_t r) {
		lv_canvas_blur_ver(_obj, area, r);
		return this;
	}

	/**
	 * Fill the canvas with color
	 * @param canvas pointer to a canvas
	 * @param color the background color
	 * @param opa the desired opacity
	 */
	inline Canvas *fillBackground(lv_color_t color, lv_opa_t opa) {
		lv_canvas_fill_bg(_obj, color, opa);
		return this;
	}

	/**
	 * Draw a rectangle on the canvas
	 * @param canvas   pointer to a canvas object
	 * @param x        left coordinate of the rectangle
	 * @param y        top coordinate of the rectangle
	 * @param w        width of the rectangle
	 * @param h        height of the rectangle
	 * @param draw_dsc descriptor of the rectangle
	 */
	inline Canvas *drawRectangle(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,
            const lv_draw_rect_dsc_t * draw_dsc) {
		lv_canvas_draw_rect(_obj, x, y, w, h, draw_dsc);
		return this;
	}

	/**
	 * Draw a text on the canvas.
	 * @param canvas   pointer to a canvas object
	 * @param x        left coordinate of the text
	 * @param y        top coordinate of the text
	 * @param max_w    max width of the text. The text will be wrapped to fit into this size
	 * @param draw_dsc pointer to a valid label descriptor `lv_draw_label_dsc_t`
	 * @param txt      text to display
	 */
	inline Canvas *drawText(lv_coord_t x, lv_coord_t y, lv_coord_t max_w,
            lv_draw_label_dsc_t * draw_dsc, const char * txt) {
		lv_canvas_draw_text(_obj, x, y, max_w, draw_dsc,  txt);
		return this;
	}

	/**
	 * Draw an image on the canvas
	 * @param canvas   pointer to a canvas object
	 * @param x        left coordinate of the image
	 * @param y        top coordinate of the image
	 * @param src      image source. Can be a pointer an `lv_img_dsc_t` variable or a path an image.
	 * @param draw_dsc pointer to a valid label descriptor `lv_draw_img_dsc_t`
	 */
	inline Canvas *drawImage(lv_coord_t x, lv_coord_t y, const void * src,
            const lv_draw_img_dsc_t * draw_dsc) {
		lv_canvas_draw_img(_obj, x, y, src, draw_dsc);
		return this;
	}

	/**
	 * Draw a line on the canvas
	 * @param canvas     pointer to a canvas object
	 * @param points     point of the line
	 * @param point_cnt  number of points
	 * @param draw_dsc   pointer to an initialized `lv_draw_line_dsc_t` variable
	 */
	inline Canvas *drawLine(const lv_point_t points[], uint32_t point_cnt,
            const lv_draw_line_dsc_t * draw_dsc) {
		lv_canvas_draw_line(_obj, points, point_cnt, draw_dsc);
		return this;
	}

	/**
	 * Draw a polygon on the canvas
	 * @param canvas    pointer to a canvas object
	 * @param points    point of the polygon
	 * @param point_cnt number of points
	 * @param draw_dsc  pointer to an initialized `lv_draw_rect_dsc_t` variable
	 */
	inline Canvas *drawPolygon(const lv_point_t points[], uint32_t point_cnt,
            const lv_draw_rect_dsc_t * draw_dsc) {
		lv_canvas_draw_polygon(_obj, points, point_cnt, draw_dsc);
		return this;
	}

	/**
	 * Draw an arc on the canvas
	 * @param canvas pointer to a canvas object
	 * @param x      origo x  of the arc
	 * @param y      origo y of the arc
	 * @param r      radius of the arc
	 * @param start_angle start angle in degrees
	 * @param end_angle   end angle in degrees
	 * @param draw_dsc    pointer to an initialized `lv_draw_line_dsc_t` variable
	 */
	inline Canvas *drawArc(lv_coord_t x, lv_coord_t y, lv_coord_t r, int32_t start_angle,
            int32_t end_angle, const lv_draw_arc_dsc_t * draw_dsc) {
		lv_canvas_draw_arc(_obj, x, y, r, start_angle, end_angle, draw_dsc);
		return this;
	}

};

} /* namespace lvgl */

#endif /* LVGLCPP_SRC_CANVAS_H_ */
