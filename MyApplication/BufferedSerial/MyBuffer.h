/**
 * This code refer to Buffer library at mbed by sam grove
 */
#ifndef MYBUFFER_H
#define MYBUFFER_H

#include <stdint.h>
#include <string.h>

template <typename T>
class MyBuffer
{
private:
    T   *_buf;
    volatile uint32_t   _wloc;
    volatile uint32_t   _rloc;
    uint32_t            _size;

public:
    /** Create a Buffer and allocate memory for it
     *  @param size The size of the buffer
     */
    MyBuffer(uint32_t size = 0x100);
    
    /** Get the size of the ring buffer
     * @return the size of the ring buffer
     */
     uint32_t getSize();
    
    /** Destry a Buffer and release it's allocated memory
     */
    ~MyBuffer();
    
    /** Add a data element into the buffer
     *  @param data Something to add to the buffer
     */
    void put(T data);
    
    /** Remove a data element from the buffer
     *  @return Pull the oldest element from the buffer
     */
    T get(void);
    
    /** Get the address to the head of the buffer
     *  @return The address of element 0 in the buffer
     */
    T *head(void);
    
    /** Reset the buffer to 0. Useful if using head() to parse packeted data
     */
    void clear(void);
    
    /** Determine if anything is readable in the buffer
     *  @return 1 if something can be read, 0 otherwise
     */
    uint32_t available(void);
    
    /** Overloaded operator for writing to the buffer
     *  @param data Something to put in the buffer
     *  @return
     */
    MyBuffer &operator= (T data)
    {
        put(data);
        return *this;
    }
    
    /** Overloaded operator for reading from the buffer
     *  @return Pull the oldest element from the buffer 
     */  
    operator int(void)
    {
        return get();
    }
    
     uint32_t peek(char c);
    
};

template <class T>
inline void MyBuffer<T>::put(T data)
{
    _buf[_wloc++] = data;
    _wloc %= (_size-1);
    
    return;
}

template <class T>
inline T MyBuffer<T>::get(void)
{
    T data_pos = _buf[_rloc++];
    _rloc %= (_size-1);
    
    return data_pos;
}

template <class T>
inline T *MyBuffer<T>::head(void)
{
    T *data_pos = &_buf[0];
    
    return data_pos;
}

template <class T>
inline uint32_t MyBuffer<T>::available(void)
{
    return (_wloc == _rloc) ? 0 : 1;
}

#endif


