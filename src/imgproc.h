// C headers
#include "py/runtime.h"

// Function declarations
extern mp_obj_t cv2_imgproc_adaptiveThreshold(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_arcLength(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_arrowedLine(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_approxPolyDP(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_approxPolyN(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_bilateralFilter(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_blur(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_boundingRect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_boxFilter(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_boxPoints(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_Canny(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_connectedComponents(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
// extern mp_obj_t cv2_imgproc_connectedComponentsWithStats(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_contourArea(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_convexHull(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_convexityDefects(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_circle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_cvtColor(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_dilate(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_drawContours(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_drawMarker(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_ellipse(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_erode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_fillConvexPoly(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_fillPoly(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_filter2D(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_findContours(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_fitEllipse(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_fitLine(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_GaussianBlur(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_getStructuringElement(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_HoughCircles(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_HoughCirclesWithAccumulator(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_HoughLines(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_HoughLinesP(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_HoughLinesWithAccumulator(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_isContourConvex(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_Laplacian(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_line(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_matchShapes(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_matchTemplate(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_medianBlur(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_minAreaRect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_minEnclosingCircle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_minEnclosingTriangle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_morphologyEx(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_moments(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_pointPolygonTest(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_putText(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_rectangle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_Scharr(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_Sobel(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_spatialGradient(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t cv2_imgproc_threshold(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

// Python references to the functions
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_adaptiveThreshold_obj, 6, cv2_imgproc_adaptiveThreshold);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_arcLength_obj, 2, cv2_imgproc_arcLength);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_arrowedLine_obj, 4, cv2_imgproc_arrowedLine);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_approxPolyDP_obj, 3, cv2_imgproc_approxPolyDP);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_approxPolyN_obj, 2, cv2_imgproc_approxPolyN);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_bilateralFilter_obj, 4, cv2_imgproc_bilateralFilter);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_blur_obj, 2, cv2_imgproc_blur);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_boundingRect_obj, 1, cv2_imgproc_boundingRect);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_boxFilter_obj, 3, cv2_imgproc_boxFilter);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_boxPoints_obj, 1, cv2_imgproc_boxPoints);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_Canny_obj, 3, cv2_imgproc_Canny);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_connectedComponents_obj, 1, cv2_imgproc_connectedComponents);
// static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_connectedComponentsWithStats_obj, 1, cv2_imgproc_connectedComponentsWithStats);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_contourArea_obj, 1, cv2_imgproc_contourArea);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_convexHull_obj, 1, cv2_imgproc_convexHull);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_convexityDefects_obj, 1, cv2_imgproc_convexityDefects);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_circle_obj, 4, cv2_imgproc_circle);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_cvtColor_obj, 2, cv2_imgproc_cvtColor);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_dilate_obj, 2, cv2_imgproc_dilate);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_drawContours_obj, 3, cv2_imgproc_drawContours);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_drawMarker_obj, 3, cv2_imgproc_drawMarker);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_ellipse_obj, 7, cv2_imgproc_ellipse);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_erode_obj, 2, cv2_imgproc_erode);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_fillConvexPoly_obj, 3, cv2_imgproc_fillConvexPoly);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_fillPoly_obj, 3, cv2_imgproc_fillPoly);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_filter2D_obj, 3, cv2_imgproc_filter2D);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_findContours_obj, 3, cv2_imgproc_findContours);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_fitEllipse_obj, 1, cv2_imgproc_fitEllipse);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_fitLine_obj, 5, cv2_imgproc_fitLine);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_GaussianBlur_obj, 3, cv2_imgproc_GaussianBlur);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_getStructuringElement_obj, 2, cv2_imgproc_getStructuringElement);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_HoughCircles_obj, 4, cv2_imgproc_HoughCircles);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_HoughCirclesWithAccumulator_obj, 4, cv2_imgproc_HoughCirclesWithAccumulator);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_HoughLines_obj, 4, cv2_imgproc_HoughLines);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_HoughLinesP_obj, 4, cv2_imgproc_HoughLinesP);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_HoughLinesWithAccumulator_obj, 4, cv2_imgproc_HoughLinesWithAccumulator);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_isContourConvex_obj, 1, cv2_imgproc_isContourConvex);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_Laplacian_obj, 2, cv2_imgproc_Laplacian);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_line_obj, 4, cv2_imgproc_line);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_matchShapes_obj, 3, cv2_imgproc_matchShapes);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_matchTemplate_obj, 3, cv2_imgproc_matchTemplate);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_medianBlur_obj, 2, cv2_imgproc_medianBlur);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_minAreaRect_obj, 1, cv2_imgproc_minAreaRect);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_minEnclosingCircle_obj, 1, cv2_imgproc_minEnclosingCircle);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_minEnclosingTriangle_obj, 1, cv2_imgproc_minEnclosingTriangle);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_morphologyEx_obj, 3, cv2_imgproc_morphologyEx);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_moments_obj, 1, cv2_imgproc_moments);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_pointPolygonTest_obj, 3, cv2_imgproc_pointPolygonTest);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_putText_obj, 6, cv2_imgproc_putText);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_rectangle_obj, 4, cv2_imgproc_rectangle);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_Scharr_obj, 4, cv2_imgproc_Scharr);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_Sobel_obj, 4, cv2_imgproc_Sobel);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_spatialGradient_obj, 1, cv2_imgproc_spatialGradient);
static MP_DEFINE_CONST_FUN_OBJ_KW(cv2_imgproc_threshold_obj, 4, cv2_imgproc_threshold);

// Global definitions for functions and constants
#define OPENCV_IMGPROC_GLOBALS \
    /* Functions */ \
    { MP_ROM_QSTR(MP_QSTR_adaptiveThreshold), MP_ROM_PTR(&cv2_imgproc_adaptiveThreshold_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_arcLength), MP_ROM_PTR(&cv2_imgproc_arcLength_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_arrowedLine), MP_ROM_PTR(&cv2_imgproc_arrowedLine_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_approxPolyDP), MP_ROM_PTR(&cv2_imgproc_approxPolyDP_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_approxPolyN), MP_ROM_PTR(&cv2_imgproc_approxPolyN_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_bilateralFilter), MP_ROM_PTR(&cv2_imgproc_bilateralFilter_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_blur), MP_ROM_PTR(&cv2_imgproc_blur_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_boundingRect), MP_ROM_PTR(&cv2_imgproc_boundingRect_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_boxFilter), MP_ROM_PTR(&cv2_imgproc_boxFilter_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_boxPoints), MP_ROM_PTR(&cv2_imgproc_boxPoints_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_Canny), MP_ROM_PTR(&cv2_imgproc_Canny_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_connectedComponents), MP_ROM_PTR(&cv2_imgproc_connectedComponents_obj) }, \
    /* { MP_ROM_QSTR(MP_QSTR_connectedComponentsWithStats), MP_ROM_PTR(&cv2_imgproc_connectedComponentsWithStats_obj) }, */ \
    { MP_ROM_QSTR(MP_QSTR_contourArea), MP_ROM_PTR(&cv2_imgproc_contourArea_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_convexHull), MP_ROM_PTR(&cv2_imgproc_convexHull_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_convexityDefects), MP_ROM_PTR(&cv2_imgproc_convexityDefects_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_circle), MP_ROM_PTR(&cv2_imgproc_circle_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_cvtColor), MP_ROM_PTR(&cv2_imgproc_cvtColor_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_dilate), MP_ROM_PTR(&cv2_imgproc_dilate_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_drawContours), MP_ROM_PTR(&cv2_imgproc_drawContours_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_drawMarker), MP_ROM_PTR(&cv2_imgproc_drawMarker_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_ellipse), MP_ROM_PTR(&cv2_imgproc_ellipse_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_erode), MP_ROM_PTR(&cv2_imgproc_erode_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_fillConvexPoly), MP_ROM_PTR(&cv2_imgproc_fillConvexPoly_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_fillPoly), MP_ROM_PTR(&cv2_imgproc_fillPoly_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_filter2D), MP_ROM_PTR(&cv2_imgproc_filter2D_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_findContours), MP_ROM_PTR(&cv2_imgproc_findContours_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_fitEllipse), MP_ROM_PTR(&cv2_imgproc_fitEllipse_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_fitLine), MP_ROM_PTR(&cv2_imgproc_fitLine_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_GaussianBlur), MP_ROM_PTR(&cv2_imgproc_GaussianBlur_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_getStructuringElement), MP_ROM_PTR(&cv2_imgproc_getStructuringElement_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_HoughCircles), MP_ROM_PTR(&cv2_imgproc_HoughCircles_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_HoughCirclesWithAccumulator), MP_ROM_PTR(&cv2_imgproc_HoughCirclesWithAccumulator_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_HoughLines), MP_ROM_PTR(&cv2_imgproc_HoughLines_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_HoughLinesP), MP_ROM_PTR(&cv2_imgproc_HoughLinesP_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_HoughLinesWithAccumulator), MP_ROM_PTR(&cv2_imgproc_HoughLinesWithAccumulator_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_isContourConvex), MP_ROM_PTR(&cv2_imgproc_isContourConvex_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_Laplacian), MP_ROM_PTR(&cv2_imgproc_Laplacian_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_line), MP_ROM_PTR(&cv2_imgproc_line_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_matchShapes), MP_ROM_PTR(&cv2_imgproc_matchShapes_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_matchTemplate), MP_ROM_PTR(&cv2_imgproc_matchTemplate_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_medianBlur), MP_ROM_PTR(&cv2_imgproc_medianBlur_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_minAreaRect), MP_ROM_PTR(&cv2_imgproc_minAreaRect_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_minEnclosingCircle), MP_ROM_PTR(&cv2_imgproc_minEnclosingCircle_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_minEnclosingTriangle), MP_ROM_PTR(&cv2_imgproc_minEnclosingTriangle_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_morphologyEx), MP_ROM_PTR(&cv2_imgproc_morphologyEx_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_moments), MP_ROM_PTR(&cv2_imgproc_moments_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_pointPolygonTest), MP_ROM_PTR(&cv2_imgproc_pointPolygonTest_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_putText), MP_ROM_PTR(&cv2_imgproc_putText_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_rectangle), MP_ROM_PTR(&cv2_imgproc_rectangle_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_Scharr), MP_ROM_PTR(&cv2_imgproc_Scharr_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_Sobel), MP_ROM_PTR(&cv2_imgproc_Sobel_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_spatialGradient), MP_ROM_PTR(&cv2_imgproc_spatialGradient_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_threshold), MP_ROM_PTR(&cv2_imgproc_threshold_obj) }, \
    \
    /* Morphology operation types, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_MORPH_ERODE), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_DILATE), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_OPEN), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_CLOSE), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_GRADIENT), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_TOPHAT), MP_ROM_INT(5) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_BLACKHAT), MP_ROM_INT(6) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_HITMISS), MP_ROM_INT(7) }, \
    \
    /* Morphology shapes, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_MORPH_RECT), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_CROSS), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_MORPH_ELLIPSE), MP_ROM_INT(2) }, \
    \
    /* Distance types, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_DIST_USER), MP_ROM_INT(-1) }, \
    { MP_ROM_QSTR(MP_QSTR_DIST_L1), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_DIST_L2), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_DIST_C), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_DIST_L12), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_DIST_FAIR), MP_ROM_INT(5) }, \
    { MP_ROM_QSTR(MP_QSTR_DIST_WELSCH), MP_ROM_INT(6) }, \
    { MP_ROM_QSTR(MP_QSTR_DIST_HUBER), MP_ROM_INT(7) }, \
    \
    /* Threshold types, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_THRESH_BINARY), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_THRESH_BINARY_INV), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_THRESH_TRUNC), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_THRESH_TOZERO), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_THRESH_TOZERO_INV), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_THRESH_MASK), MP_ROM_INT(7) }, \
    { MP_ROM_QSTR(MP_QSTR_THRESH_OTSU), MP_ROM_INT(8) }, \
    { MP_ROM_QSTR(MP_QSTR_THRESH_TRIANGLE), MP_ROM_INT(16) }, \
        \
    /* Adaptive threshold methods, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_ADAPTIVE_THRESH_MEAN_C), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_ADAPTIVE_THRESH_GAUSSIAN_C), MP_ROM_INT(1) }, \
    \
    /* Retrieval modes, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_RETR_EXTERNAL), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_RETR_LIST), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_RETR_CCOMP), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_RETR_TREE), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_RETR_FLOODFILL), MP_ROM_INT(4) }, \
    \
    /* Contour approximation methods, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_CHAIN_CODE), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_CHAIN_APPROX_NONE), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_CHAIN_APPROX_SIMPLE), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_CHAIN_APPROX_TC89_L1), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_CHAIN_APPROX_TC89_KCOS), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_LINK_RUNS), MP_ROM_INT(5) }, \
    \
    /* Shape match modes, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_CONTOURS_MATCH_I1), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_CONTOURS_MATCH_I2), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_CONTOURS_MATCH_I3), MP_ROM_INT(3) }, \
    \
    /* Hough modes, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_HOUGH_STANDARD), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_HOUGH_PROBABILISTIC), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_HOUGH_MULTI_SCALE), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_HOUGH_GRADIENT), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_HOUGH_GRADIENT_ALT), MP_ROM_INT(4) }, \
    \
    /* Color conversion codes, from opencv2/imgproc.hpp */ \
    /* Not all are included, to reduce bloat */ \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2BGRA), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2RGBA), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2BGR), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2RGB), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2RGBA), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2BGRA), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2BGR), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2RGB), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2RGB), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2BGR), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2RGBA), MP_ROM_INT(5) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2BGRA), MP_ROM_INT(5) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2GRAY), MP_ROM_INT(6) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2GRAY), MP_ROM_INT(7) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2BGR), MP_ROM_INT(8) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2RGB), MP_ROM_INT(8) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2BGRA), MP_ROM_INT(9) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2RGBA), MP_ROM_INT(9) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2GRAY), MP_ROM_INT(10) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2GRAY), MP_ROM_INT(11) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2BGR565), MP_ROM_INT(12) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2BGR565), MP_ROM_INT(13) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652BGR), MP_ROM_INT(14) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652RGB), MP_ROM_INT(15) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGRA2BGR565), MP_ROM_INT(16) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGBA2BGR565), MP_ROM_INT(17) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652BGRA), MP_ROM_INT(18) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652RGBA), MP_ROM_INT(19) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_GRAY2BGR565), MP_ROM_INT(20) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR5652GRAY), MP_ROM_INT(21) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2YCrCb), MP_ROM_INT(36) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2YCrCb), MP_ROM_INT(37) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_YCrCb2BGR), MP_ROM_INT(38) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_YCrCb2RGB), MP_ROM_INT(39) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BGR2HSV), MP_ROM_INT(40) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_RGB2HSV), MP_ROM_INT(41) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_HSV2BGR), MP_ROM_INT(54) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_HSV2RGB), MP_ROM_INT(55) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerBG2BGR), MP_ROM_INT(46) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGB2BGR), MP_ROM_INT(47) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerRG2BGR), MP_ROM_INT(48) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGR2BGR), MP_ROM_INT(49) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerRG2RGB), MP_ROM_INT(46) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGR2RGB), MP_ROM_INT(47) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerBG2RGB), MP_ROM_INT(48) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGB2RGB), MP_ROM_INT(49) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerBG2GRAY), MP_ROM_INT(86) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGB2GRAY), MP_ROM_INT(87) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerRG2GRAY), MP_ROM_INT(88) }, \
    { MP_ROM_QSTR(MP_QSTR_COLOR_BayerGR2GRAY), MP_ROM_INT(89) }, \
    \
    /* Line types, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_FILLED), MP_ROM_INT(-1) }, \
    { MP_ROM_QSTR(MP_QSTR_LINE_4), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_LINE_8), MP_ROM_INT(8) }, \
    { MP_ROM_QSTR(MP_QSTR_LINE_AA), MP_ROM_INT(16) }, \
    \
    /* Hershey fonts, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_SIMPLEX), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_PLAIN), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_DUPLEX), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_COMPLEX), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_TRIPLEX), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_COMPLEX_SMALL), MP_ROM_INT(5) }, \
    { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_SCRIPT_SIMPLEX), MP_ROM_INT(6) }, \
    { MP_ROM_QSTR(MP_QSTR_FONT_HERSHEY_SCRIPT_COMPLEX), MP_ROM_INT(7) }, \
    { MP_ROM_QSTR(MP_QSTR_FONT_ITALIC), MP_ROM_INT(16) }, \
    \
    /* Marker types, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_MARKER_CROSS), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_MARKER_TILTED_CROSS), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_MARKER_STAR), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_MARKER_DIAMOND), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_MARKER_SQUARE), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_MARKER_TRIANGLE_UP), MP_ROM_INT(5) }, \
    { MP_ROM_QSTR(MP_QSTR_MARKER_TRIANGLE_DOWN), MP_ROM_INT(6) }, \
    \
    /* Template matching modes, from opencv2/imgproc.hpp */ \
    { MP_ROM_QSTR(MP_QSTR_TM_SQDIFF), MP_ROM_INT(0) }, \
    { MP_ROM_QSTR(MP_QSTR_TM_SQDIFF_NORMED), MP_ROM_INT(1) }, \
    { MP_ROM_QSTR(MP_QSTR_TM_CCORR), MP_ROM_INT(2) }, \
    { MP_ROM_QSTR(MP_QSTR_TM_CCORR_NORMED), MP_ROM_INT(3) }, \
    { MP_ROM_QSTR(MP_QSTR_TM_CCOEFF), MP_ROM_INT(4) }, \
    { MP_ROM_QSTR(MP_QSTR_TM_CCOEFF_NORMED), MP_ROM_INT(5) }
