/**************************************************************************/
/*!
    @file     common_header.h
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#ifndef _COMMON_HEADER_H_
#define _COMMON_HEADER_H_

#include <stdint.h>
#include <stdbool.h>

//--------------------------------------------------------------------+
// COMPILER
//--------------------------------------------------------------------+
#define STRING_(x)                  #x                   // stringify without expand
#define XSTRING_(x)                 STRING_(x)           // expand then stringify
#define STRING_CONCAT_(a, b)        a##b                 // concat without expand
#define XSTRING_CONCAT_(a, b)       STRING_CONCAT_(a, b) // expand then concat

#define ATTR_PACKED               __attribute__ ((packed))

//--------------------------------------------------------------------+
// ASSERT & VERIFY
//--------------------------------------------------------------------+
//#define ASSERT(condition, err)    if ( !(condition) ) return err;

//------------- Compile-time Assert -------------//
#if defined __COUNTER__ && __COUNTER__ != __COUNTER__
  #define _ASSERT_COUNTER __COUNTER__
#else
  #define _ASSERT_COUNTER __LINE__
#endif

#define ASSERT_STATIC_(const_expr) enum { XSTRING_CONCAT_(static_assert_, _ASSERT_COUNTER) = 1/(!!(const_expr)) }


#define VERIFY_(condition)                if ( !(condition) ) return false;
#define VERIFY_RETURN_(condition, error)  if ( !(condition) ) return error;

//--------------------------------------------------------------------+
// INLINE FUNCTION
//--------------------------------------------------------------------+
static inline bool is_within(uint32_t lower, uint32_t value, uint32_t upper)
{
  return (lower <= value) && (value <= upper);
}

#endif /* _COMMON_HEADER_H_ */
