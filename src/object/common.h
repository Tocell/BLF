#ifndef BLF_H
#define BLF_H

#ifdef _WIN32
#ifdef TWBLF_EXPORTS
#define TWBLF_API __declspec(dllexport)
#else
#define TWBLF_API __declspec(dllimport)
#endif
#else
#define TWBLF_API __attribute__((visibility("default")))
#endif

#include "object_type.h"

#define BLFSTART    namespace BLF {
#define BLFEND      }

#define DISALLOW_COPY_AND_ASSIGN(TypeName)  TypeName(const TypeName&) = delete; \
    void operator=(const TypeName&) = delete; \
    TypeName(const TypeName &&) = delete; \
    void operator=(const TypeName &&) = delete;

#define UNCOMPRESS_FILE_SIZE                ( 4 * 1024 * 1024 )     // 存 blf 时分配的最大压缩缓存
#define COMPRESS_FILE_SIZE                  ( 4 * 1024 * 1024 )     // 读 blf 时分配的最大解压缓存

#define MAX_FRAME_CNT                       ( 30 * 10000 )          // 存 blf 时最大容纳暂存帧数


#endif //BLF_H
