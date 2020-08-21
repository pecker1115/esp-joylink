#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include <fcntl.h>

#ifdef __ESP_PAL__
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#endif

// joylink platform layer header files
#include "joylink_stdio.h"


/**
 * @brief 将数据格式化打印到流
 *
 * @param [in] fmt: @n String that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return None.
 * @see None.
 * @note None.
 */
int32_t   jl_platform_printf(const char *fmt, ...)
{
    int32_t ret = 0;
#ifdef __LINUX_PAL__
    va_list args;
    va_start(args, fmt);
    ret = vprintf(fmt, args);
    va_end(args);
#else
#ifdef __ESP_PAL__
    va_list args;
    va_start(args, fmt);
    ret = vprintf(fmt, args);
    va_end(args);
#endif
#endif
    return ret;
}

#ifdef __ESP_PAL__
static SemaphoreHandle_t   joylink_mutex = NULL;
#define JOYLINK_LOCK()     xSemaphoreTake(joylink_mutex,portMAX_DELAY)
#define JOYLINK_UNLOCK()   xSemaphoreGive(joylink_mutex)

void esp_joylink_mutex_init(void)
{
    if (joylink_mutex == NULL) {
        joylink_mutex = xSemaphoreCreateMutex();
        if (joylink_mutex == NULL) {
            return;
        }

    } else {
        return;
    }
}
#endif

void jl_platform_printf_lock(void)
{
#ifdef __ESP_PAL__
    if (joylink_mutex == NULL) {
        esp_joylink_mutex_init();
    }

    JOYLINK_LOCK();
#endif
}

void jl_platform_printf_unlock(void)
{
#ifdef __ESP_PAL__
    if (joylink_mutex == NULL) {
        esp_joylink_mutex_init();
    }
    
    JOYLINK_UNLOCK();
#endif
}
/**
 * @brief 将数据安格式化写入到字符串
 *
 * @param [out] str: @n String that holds written text.
 * @param [in] fmt: @n Format that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return bytes of character successfully written into string.
 * @see None.
 * @note None.
 */
int32_t jl_platform_sprintf(char *str, const char *fmt, ...)
{
    int32_t ret = 0;
#ifdef __LINUX_PAL__
    va_list args;
    va_start(args, fmt);
    ret = vsprintf(str, fmt, args);
    va_end(args);
#else
#ifdef __ESP_PAL__
    va_list args;
    va_start(args, fmt);
    ret = vsprintf(str, fmt, args);
    va_end(args);
#endif
#endif
    return ret;
}

/**
 * @brief 将数据安指定长度格式化写入到字符串
 *
 * @param [out] str: @n String that holds written text.
 * @param [in] len: @n Maximum length of character will be written
 * @param [in] fmt: @n Format that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return bytes of character successfully written into string.
 * @see None.
 * @note None.
 */
int32_t jl_platform_snprintf(char *str, const int32_t len, const char *fmt, ...)
{
    int32_t ret = 0;
#ifdef __LINUX_PAL__
    va_list args;
    va_start(args, fmt);
    ret = vsnprintf(str, len, fmt, args);
    va_end(args);
#else
#ifdef __ESP_PAL__
    va_list args;
    va_start(args, fmt);
    ret = vsnprintf(str, len, fmt, args);
    va_end(args);
#endif
#endif
    return ret;
}


/** @defgroup group_platform_file_api_manage
 *  @{
 */

/**
 * @brief Opens the file whose name is specified in the parameter filename and associates it
 *  with a stream that can be identified in future operations by the void pointer returned.
 *
 * @param [in] path: @n The file path to open.With reference to fopen
 * @param [in] mode: @n C string containing a file access mode.
 * @return If the file is successfully opened, the function returns a pointer to void object that can be used to
 * identify the stream on future operations.Otherwise, a null pointer is returned.
 * @see None.
 * @note None.
 */
void *jl_platform_fopen(const char *path, const char *mode)
{
#ifdef __LINUX_PAL__
    return fopen(path, mode);
#else
#ifdef __ESP_PAL__
    return fopen(path, mode);
#endif
#endif
}

/**
 * @brief Reads an array of count elements, each one with a size of size bytes, from the stream and
 * stores them in the block of memory specified by ptr.
 *
 * @param [in] buff: @n Pointer to a block of memory with a size of at least (size*count) bytes, converted to a void*.
 * @param [in] size: @n size in bytes, of each element to be read.
 * @param [in] count: @n Number of elements, each one with a size of size bytes.
 * @param [in] stream: @n Pointer to void that specifies an input stream.
 * @return The total number of elements successfully read is returned.If either size or count is zero, the function returns zero
 * @see None.
 * @note None.
 */
uint32_t jl_platform_fread(void *buff, uint32_t size, uint32_t count, void *stream)
{
#ifdef __LINUX_PAL__
    return fread(buff, size, count, stream);
#else
#ifdef __ESP_PAL__
    return fread(buff, size, count, stream);
#endif
#endif
}

/**
 * @brief Writes an array of count elements, each one with a size of size bytes, from the block of memory pointed
 * by ptr to the current position in the stream.
 *
 * @param [in] ptr: @n Pointer to the array of elements to be written, converted to a const void*.
 * @param [in] size: @n Size in bytes of each element to be written.
 * @param [in] count: @n Number of elements, each one with a size of size bytes.
 * @param [in] stream: @n Pointer to void that specifies an output stream.
 * @return The total number of elements successfully written is returned.If either size or count is zero, the function returns zero.
 * @see None.
 * @note None.
 */
uint32_t jl_platform_fwrite(const void *ptr, uint32_t size, uint32_t count, void *stream)
{
#ifdef __LINUX_PAL__
    return fwrite(ptr, size, count, stream);
#else
#ifdef __ESP_PAL__
    return fwrite(ptr, size, count, stream);
#endif
#endif
}

/**
 * @brief Sets the position indicator associated with the stream to a new position.
 *
 * @param [in] stream: @n Pointer to void that identifies the stream.
 * @param [in] offset: @n Binary files: Number of bytes to offset from origin.
 * @param [in] origin: @n Position used as reference for the offset. It is specified by one of value enum in hal_fs_seek_type_t.
 *
 * @return If successful, the function returns zero.Otherwise, it returns non-zero value.
 * @see None.
 * @note None.
 */
int32_t jl_platform_fseek(void *stream, long offset, int32_t origin)
{
#ifdef __LINUX_PAL__
    return fseek(stream, offset, origin);
#else
#ifdef __ESP_PAL__
    return fseek(stream, offset, origin);
#endif
#endif
}

/**
 * @brief Closes the file associated with the stream and disassociates it.
 *
 * @param [in] stream: @n Pointer to void that identifies the stream.
 *
 * @return If the stream is successfully closed, a zero value is returned.On failure, non-zero is returned.
 * @see None.
 * @note None.
 */
int32_t jl_platform_fclose(void *stream)
{
#ifdef __LINUX_PAL__
    return fclose(stream);
#else
#ifdef __ESP_PAL__
    return fclose(stream);
#endif
#endif
}

/**
 * @brief Closes the file associated with the stream and disassociates it.
 *
 * @param [in] stream: @n Pointer to void that identifies the stream.
 *
 * @return If the stream is successfully flushed, a zero value is returned.On failure, non-zero is returned.
 * @see None.
 * @note None.
 */
int32_t jl_platform_fflush(void *stream)
{
#ifdef __LINUX_PAL__
    fflush((FILE*)stream);
#else
#ifdef __ESP_PAL__
    return fflush((FILE*)stream);
#endif
#endif
}


/**
 * brief: 
 *
 * @Param: msg
 * @Param: buff
 * @Param: len
 */
void jl_print_buffer(const char *msg, const uint8_t *buff, int len)
{
    if(NULL == msg || NULL == buff){
        return;
    }
    int i = 0;
    jl_platform_printf("len = %d, %s\n", len, msg);
    for(i =0; i < len; i++){
        jl_platform_printf("%02x ", (int)buff[i]);
        if(!((i + 1)%4)){
            jl_platform_printf("| ");
        }

        if(!((i + 1)%16)){
            jl_platform_printf("\n");
        }
    }
    jl_platform_printf("\n");
}

/*
* @brief get a random
* 
* @param none
* @return the random value
*/
int32_t jl_get_random(void)
{
#ifdef __LINUX_PAL__
    return rand();
#else
#ifdef __ESP_PAL__
    return rand();
#endif
#endif
}
