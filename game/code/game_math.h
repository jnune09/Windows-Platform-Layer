#ifndef GAME_MATH_H
#define GAME_MATH_H

#include <math.h>

inline s32
RoundF32ToS32(f32 Value)
{
    s32 Result = (s32)(Value + 0.5f);
    return(Result);
}

inline u32
RoundF32ToU32(f32 Value)
{
    u32 Result = (u32)(Value + 0.5f);
    return(Result);
}

inline s32
FloorF32ToS32(f32 Angle)
{
    s32 Result = (s32)floorf(Angle);
    return(Result);
}


inline f32
Sin(f32 Angle)
{
    f32 Result = sinf(Angle);
    return(Result);
}

inline f32
Cos(f32 Value)
{
    f32 Result = cosf(Result);
    return(Result);
}


inline f32 
ATan2(f32 Y, f32 X)
{
    f32 Result = atan2f(Y, X);
    return(Result);
}

#endif //GAME_MATH_H
