//
// Created by zyn on 2023/05/22.
// Modified by zhongda on 2024/03/11 ： add aruco detector interface 
//

#ifndef OBARUCO_API_H
#define OBARUCO_API_H

#pragma once

#include <stdint.h>

#define ORBBEC_EXPORTS 1

#if defined(_WIN32) || defined(WIN32)
#if defined(ORBBEC_EXPORTS) || defined(ORBBEC_BUILD_SHARED)
#define ORBBEC_API __declspec(dllexport)
#define ORBBEC_LOCAL
#else
#define ORBBEC_API __declspec(dllimport)
#define ORBBEC_LOCAL
#endif
#else
#if defined(ORBBEC_EXPORTS) || defined(ORBBEC_BUILD_SHARED)
#if __GNUC__ >= 4
#define ORBBEC_API __attribute__((visibility("default")))
#define ORBBEC_LOCAL __attribute__((visibility("hidden")))
#else
#define ORBBEC_API __attribute__((visibility("default")))
#define ORBBEC_LOCAL __attribute__((visibility("default")))
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** define the slam handle */
#define FUSION_HANDLE void *

/** the status of the slam proccessing
 */
typedef enum {
  SILENT = 0,   // 静默状态  = 不接收图片  处理线程1000ms间歇执行               不输出
  RUNNING = 1,  // 运行状态  = 接收图片    处理线程启动           处理图片       输出
  WAITING = 2,  // 等待状态  = 接收图片    处理线程启动5ms间歇     等待图片输入   不输出
  PAUSE = 3,    // 暂停状态  = 接收图片   处理线程启动50ms间歇执行               不输出
} ob_aruco_status;

/** \struct ob_slam_result_t
 * Structure to define the slam result
 */
/// adjust the sequence, and memory align
typedef struct{
    uint64_t timestamp;                           /**< 时间戳 >**/
    
    float camera_pose[7];                         /* sta > 1 该值才具有下列含义
                                                   *< camera pose: quaternion(x,y,z,w),translation(tx,ty,tz) >**/
    
    float rpyxyz[6];                              /* sta > 1 该值才具有下列含义
                                                          roll pitch yaw x y z, mark in robot， rad  m
                                                  */

    double DevAng;                                /* sta > 0 该值(偏离角)才具有下列含义  
                                                          二维码中心偏离主光轴的角度，z轴朝上，即二维码在左为正，rad
                                                  */

    int sta;                                      /* 0 检测不到二维码
                                                     1 检测到二维码，不能计算出位姿
                                                     2 计算出位姿
                                                     3 计算出位姿
                                                  */
} ob_aruco_result;

// chargestation detection output 
// timestamp:        检测帧时间戳.
// is_chargestation: 是否检测到充电桩.
// station_box:      充电桩整体检测框的类别id、置信度和左上右下角的像素坐标(classID, conf, x1, y1, x2, y2), 只检测到充电桩头部时，station_box与head_box相同.
// head_box:         充电桩头部检测框的类别id、置信度和左上右下角的像素坐标(classID, conf, x1, y1, x2, y2).
typedef struct {
    uint64_t timestamp;
    bool is_chargestation;
    bool is_head;
    std::vector<float> station_box;
    std::vector<float> head_box;
} ChargestationInfo;

/** Callback function for get slam result being generated by the SDK.
 *
 * \param result
 * The context of the callback function. This is the context that was supplied by the caller to \p
 *
 * \remarks
 * The callback is called asynchronously when the SDK generates a slam result
 * when calling \ref obslam_rigister_slam_result_callback() to register the callback.
 *
 */
typedef void (*obsystem_aruco_callback)(const ob_aruco_result *status);

/**
 * @brief 配置回调函数
 * @param handle slam对象句柄
 * @param proc 回调函数
 */
ORBBEC_API void ob_register_aruco_result_callback(FUSION_HANDLE handle, obsystem_aruco_callback proc);

/**
 * @brief 创建SLAM系统
 * @param handle slam对象句柄
 * @param config_path 配置文件路径及文件名
 */
ORBBEC_API void ob_create(FUSION_HANDLE *handle, const char *config_path);

/**
 * @brief 释放SLAM系统
 * @param handle slam对象句柄
 */
ORBBEC_API void ob_release(FUSION_HANDLE handle);

/**
 * @brief 启动SLAM系统
 * @param handle slam对象句柄
 * rerurn, 1 for start successfully; 0 for start failure
 */
ORBBEC_API int32_t ob_start(FUSION_HANDLE handle);

/**
 * @brief 停止SLAM系统
 * @param handle slam对象句柄
 */
ORBBEC_API int32_t ob_pause(FUSION_HANDLE handle);

/**
 * @brief 输入图像信息
 * @param handle slam对象句柄
 * @param width 图像宽度
 * @param height 图像高度
 * @param data 图像数据
 * @param size 图像通道数目：1或3
 */
ORBBEC_API int32_t ob_add_image(FUSION_HANDLE handle, uint64_t timestamp, uint32_t width, uint32_t height, uint8_t *data,
                                    uint64_t size = 1);

ORBBEC_API int32_t ob_add_station(FUSION_HANDLE handle, uint64_t timestamp, ChargestationInfo station_info);

ORBBEC_API const ob_aruco_status ob_get_status(FUSION_HANDLE handle);

ORBBEC_API const char *ob_get_version(FUSION_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif // FUSION_OBSLAM_API_H