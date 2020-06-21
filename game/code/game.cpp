/* ================================================================
  by Jonathan Nunez
  


  ================================================================ */

#include "game.h"
#include "game_math.h"

internal void
GameOutputSound(game_state *GameState, game_sound_buffer *SoundBuffer, int ToneHz)
{
    s16 ToneVolume = 3000;
    int WavePeriod = SoundBuffer->SamplesPerSecond/ToneHz;
    
    s16 *SampleOut = SoundBuffer->Samples;
    for(int SampleIndex = 0; SampleIndex < SoundBuffer->SampleCount; ++SampleIndex)
    {
        s16 SampleValue = 0;
        
        *SampleOut++ = SampleValue;
        *SampleOut++ = SampleValue;
    }
}

internal u32
BlendColor(v3 Color)
{
    u32 Result = ((RoundF32ToU32(Color.X * 255.0f) << 16) |
                  (RoundF32ToU32(Color.Y * 255.0f) << 8)  |
                  (RoundF32ToU32(Color.Z * 255.0f) << 0));
    return(Result);
};

internal void
DrawRect(game_offscreen_buffer *Buffer, rect Rect, v3 Color)
{
    s32 Left   = RoundF32ToU32(Rect.Left);
    s32 Top    = RoundF32ToU32(Rect.Top);
    s32 Right  = RoundF32ToU32(Rect.Right);
    s32 Bottom = RoundF32ToU32(Rect.Bottom);
    
    if(Left < 0)
    {
        Left = 0;
    }
    if(Top < 0)
    {
        Top = 0;
    }
    if(Right > Buffer->Width)
    {
        Right = Buffer->Width;
    }
    if(Bottom > Buffer->Height)
    {
        Bottom = Buffer->Height;
    }
    
    u8 *Row = ((u8 *)Buffer->Memory + Left*Buffer->BytesPerPixel + Top*Buffer->Pitch);
    
    for(int Y = Top; Y < Bottom; ++Y)
    {
        u32 *Pixel = (u32 *)Row;
        
        for (int X = Left; X < Right; ++X)
        {
            *Pixel++ = BlendColor(Color);
        }
        
        Row += Buffer->Pitch;
    }
    
}

extern "C" GAME_GET_SOUND_SAMPLES(GameGetSoundSamples)
{
    game_state *GameState = (game_state *)Memory->PermanentStorage;
    GameOutputSound(GameState, SoundBuffer, 400) ;
}

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
    
    f32 PlayerWidth  = 0.75f*50;
    f32 PlayerHeight = 50;
    
    game_state *GameState = (game_state *)Memory->PermanentStorage;
    if(!Memory->IsInitialized)
    {
        GameState->PlayerX = 100;
        GameState->PlayerY = 100;
        
        Memory->IsInitialized = true;
    }
    
    
    for(int ControllerIndex = 0; ControllerIndex < ArrayCount(Input->Controllers); ++ControllerIndex)
    {
        game_controller_input *Controller = GetController(Input, ControllerIndex);
        
        if(Controller->IsAnalog)
        {
            // NOTE(JN): Use analog movement.
            
        }
        else
        {
            f32 DeltaX = 0.0f;
            f32 DeltaY = 0.0f;
            
            // NOTE(JN): Use digital movement.
            if(Controller->MoveUp.EndedDown)
            {
                DeltaY = -1.0f;
            }
            if(Controller->MoveDown.EndedDown)
            {
                DeltaY = 1.0f;
            }
            if(Controller->MoveLeft.EndedDown)
            {
                DeltaX = -1.0f;
            }
            if(Controller->MoveRight.EndedDown)
            {
                DeltaX = 1.0f;
            }
            
            DeltaX *= 64.0f;
            DeltaY *= 64.0f;
            
            f32 NewPlayerX = GameState->PlayerX + Input->SecondsToAdvance*DeltaX;
            f32 NewPlayerY = GameState->PlayerY + Input->SecondsToAdvance*DeltaY;
            
            GameState->PlayerX = NewPlayerX;
            GameState->PlayerY = NewPlayerY;
            
        }
    }
    
    /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    
       Draw
    
       -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
    
    v3 Grey = {};
    Grey.X = 0.5f;
    Grey.Y = 0.5f;
    Grey.Z = 0.5f;
    
    v3 Red = {};
    Red.X = 1.0f;
    
    v3 Green = {};
    Green.Y = 1.0f;
    
    v3 Blue =  {};
    Blue.Z = 1.0f;
    
    v3 Purple = {};
    Purple.X = 1.0f;
    Purple.Z = 1.0f;
    
    rect BackgroundRect = {};
    BackgroundRect.Left   = 0;
    BackgroundRect.Top    = 0;
    BackgroundRect.Right  = (f32)Buffer->Width;
    BackgroundRect.Bottom =  (f32)Buffer->Height;
    
    DrawRect(Buffer, BackgroundRect, Grey);
    
    rect PlayerRect = {};
    PlayerRect.Left   = GameState->PlayerX - (0.5f*PlayerWidth) ;
    PlayerRect.Top    = GameState->PlayerY - PlayerHeight;
    PlayerRect.Right  = PlayerRect.Left    + PlayerWidth;
    PlayerRect.Bottom = PlayerRect.Top     + PlayerHeight;
    
    DrawRect(Buffer, PlayerRect, Red);
}

/*
internal void
RenderGradient(game_offscreen_buffer *Buffer, int XOffset, int YOffset)
{
    // NOTE(JN): Row points to the first pixel in the buffer. Row is 8
    // bits wide because pitch is also measured in bytes.
    u8 *Row = (u8 *)Buffer->Memory;    
    for(int Y = 0; Y < Buffer->Height;++Y)
    {
        u32 *Pixel = (u32 *)Row;
        for(int X = 0; X < Buffer->Width; ++X)
        {
            // NOTE(JN): The 256x256 blocks in the gradient animation are due to
            // the 8 bits colors overflowing.
            u8 Blue  = (u8)(X + XOffset);
            u8 Red   = (u8)(Y + YOffset);

            // NOTE(JN): *Pixel = x; *Pixel = *Pixel + 1*sizeof(u32);
            *Pixel++ = ((Red << 16) | Blue);
        }

        Row += Buffer->Pitch;
    }
}
*/
