#!/bin/sh

if [ "$1" = "480p-10bit-compress" -o "$1" = "1" ];then
    echo "480p 10bit compress"
    videotunnel-native -i /vendor/test/480p-10bit-1-frame-compress.yuv \
        -f HAL_PIXEL_FORMAT_AW_NV21_10bit \
        --afbc \
        -s 720x480 -w 1920,1080
fi

if [ "$1" = "576p-10bit-compress" -o "$1" = "2" ];then
    echo "576p 10bit compress"
    videotunnel-native -i /vendor/test/576p-yuv420-10bit-compress-1frame.yuv \
        -f HAL_PIXEL_FORMAT_AW_NV21_10bit \
        --afbc \
        -s 720x576 -w 1920,1080
fi

if [ "$1" = "720p-10bit-compress" -o "$1" = "3" ];then
    echo "720p 10bit compress"
    videotunnel-native -i /vendor/test/720p-yuv420-10bit-compress-1frame.yuv \
        -f HAL_PIXEL_FORMAT_AW_NV21_10bit \
        --afbc \
        -s 1280x720 -w 1920,1080
fi

if [ "$1" = "480p-8bit-uncompress" -o "$1" = "4" ];then
    echo "480p 8bit uncompress"
    videotunnel-native -i /vendor/test/480p-NV12-720x480-uncompress-1frame.yuv \
        -f HAL_PIXEL_FORMAT_AW_NV12 \
        -s 720x480 -w 1920,1080
fi

if [ "$1" = "480p-8bit-uncompress 2" -o "$1" = "5" ];then
    echo "480p 8bit uncompress"
    videotunnel-native -i /vendor/test/i2.yuv \
        -f HAL_PIXEL_FORMAT_AW_NV12 \
        -s 720x480 -w 1920,1080
fi

if [ "$1" = "1080p-yuv420p010-uncompress" -o "$1" = "6" ];then
    echo "1080p yuv420p010 uncompress"
    videotunnel-native -i /vendor/test/1080p-YUV420P010-1frame.yuv \
        -f HAL_PIXEL_FORMAT_YCBCR_P010 \
        -s 1920x1080 -w 1920,1080
fi

if [ "$1" = "480p-yuv420p010-uncompress" -o "$1" = "7" ];then
    echo "480p yuv420p010 uncompress"
    videotunnel-native -i /vendor/test/480p_yuv420p010.yuv \
        -f HAL_PIXEL_FORMAT_YCBCR_P010 \
        -s 720x480 -w 1920,1080
fi

