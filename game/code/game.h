#if !defined(GAME_H)
/* ================================================================
   by Jonathan Nunez


   
   ================================================================ */

/*
  NOTE(JN):

  GAME_INTERNAL:
    0 - build for public
    1 - build for development

  GAME_SLOW:
    0 - No slow code.
    1 - Let it slow. Let it slow.
    
*/

#include <stdint.h>

#define internal        static
#define local_persist   static
#define global_variable static

#define Pi32 3.14159265359f

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;

typedef s32      b32;

#if GAME_SLOW
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

// NOTE(JN): Truncate u64 to u32
inline u32
SafeTruncateU64(u64 Value)
{
    Assert(Value <= 0xFFFFFFFF);
    u32 Result = (u32)Value;
    return(Result);
}

struct thread_context
{
    int Placeholder;
};

/*
  NOTE(JN): Services the platform layer provides to the game.
*/

#if GAME_INTERNAL

// IMPORTANT DO NOT USE IN CODE THAT WILL SHIP
struct debug_read_file_result
{
    u32 ContentSize;
    void *Content;
};

#define DEBUG_PLATFORM_FREE_FILE_MEMORY(name) void name(thread_context *Thread, void *Memory)
typedef DEBUG_PLATFORM_FREE_FILE_MEMORY(debug_platform_free_file_memory);

#define DEBUG_PLATFORM_READ_ENTIRE_FILE(name) debug_read_file_result name(thread_context *Thread, char *FileName)
typedef DEBUG_PLATFORM_READ_ENTIRE_FILE(debug_platform_read_entire_file);

#define DEBUG_PLATFORM_WRITE_ENTIRE_FILE(name) b32 name(thread_context *Thread, char *FileName, u32 MemorySize, void *Memory)
typedef DEBUG_PLATFORM_WRITE_ENTIRE_FILE(debug_platform_write_entire_file);

#endif

/*
  NOTE(JN): Services the game provides to the platform layer.
*/

struct rect
{
    f32 Left;
    f32 Top;
    f32 Right;
    f32 Bottom;
};

struct v2
{
    f32 X;
    f32 Y;
};

struct v3
{
    f32 X;
    f32 Y;
    f32 Z;
};

struct game_offscreen_buffer
{
    // NOTE(JN): Pixels are 4 bytes (32-bits) wide. BB GG RR XX (seen
    // in memory as XX RR GG BB due to endianness)
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

struct game_sound_buffer
{
    int SamplesPerSecond;
    int SampleCount;
    s16 *Samples;
};

struct game_button_state
{
    int HalfTransitionCount;
    b32 EndedDown;
};

struct game_controller_input
{
    b32 IsConnected;
    b32 IsAnalog;
    f32 StickAverageX;
    f32 StickAverageY;
    
    union
    {
        game_button_state Buttons[12];
        struct
        {
            game_button_state MoveUp;
            game_button_state MoveDown;
            game_button_state MoveLeft;
            game_button_state MoveRight;
            
            game_button_state Action1;
            game_button_state Action2;
            game_button_state Action3;
            game_button_state Action4;
            game_button_state Action5;
            game_button_state Action6;
            
            game_button_state Start;
            game_button_state Select;
        };
    };
};

struct game_input
{
    f32 SecondsToAdvance;
    game_button_state MouseButtons[5];
    s32 MouseX, MouseY, MouseZ;
    // NOTE(JN): The keyboard is controller 0.
    game_controller_input Controllers[5];
};

inline game_controller_input *GetController(game_input *Input, int ControllerIndex)
{
    Assert(ControllerIndex < ArrayCount(Input->Controllers));
    game_controller_input *Result = &Input->Controllers[ControllerIndex];
    return(Result);
}

struct game_memory
{
    b32 IsInitialized;
    
    u64 PermanentStorageSize;
    void *PermanentStorage; // NOTE(JN): NEEDS to be cleared to zero at startup
    u64 TransientStorageSize;
    void *TransientStorage; // NOTE(JN): NEEDS to be cleared to zero at startup
    
    debug_platform_free_file_memory  *DEBUGPlatformFreeFileMemory;
    debug_platform_read_entire_file  *DEBUGPlatformReadEntireFile; 
    debug_platform_write_entire_file *DEBUGPlatformWriteEntireFile;
};

#define GAME_UPDATE_AND_RENDER(name) void name(thread_context *Thread, game_memory *Memory, game_input *Input, game_offscreen_buffer *Buffer)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

// IMPORTANT(JN): Be fast!
#define GAME_GET_SOUND_SAMPLES(name) void name(thread_context *Thread, game_memory *Memory, game_sound_buffer *SoundBuffer)
typedef GAME_GET_SOUND_SAMPLES(game_get_sound_samples);

struct game_state
{
    f32 PlayerX;
    f32 PlayerY;
};

#define GAME_H
#endif
