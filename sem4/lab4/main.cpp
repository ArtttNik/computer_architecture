#include <iostream>
#include <cmath>
#include <fmod.h>
#include <fmod_errors.h>

#define EFFECT_NONE        0
#define EFFECT_ECHO        1
#define EFFECT_TREMOLO     2
#define EFFECT_HIGHPASS    3

#define EFFECT_MODE 0
#define ENABLE_3D 1

int main() {
    FMOD_SYSTEM* system = nullptr;
    FMOD_SOUND* sound = nullptr;
    FMOD_CHANNEL* channel = nullptr;
    FMOD_DSP* dsp = nullptr;
    FMOD_RESULT result;

    result = FMOD_System_Create(&system);
    if (result != FMOD_OK) {
        std::cout << FMOD_ErrorString(result) << '\n';
        return 1;
    }

    result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        std::cout << FMOD_ErrorString(result) << '\n';
        return 1;
    }

#if ENABLE_3D
    FMOD_System_Set3DSettings(system, 1.0f, 1.0f, 1.0f);
#endif

    result = FMOD_System_CreateSound(
        system,
        "music.mp3",
#if ENABLE_3D
        FMOD_3D | FMOD_LOOP_NORMAL | FMOD_SOFTWARE,
#else
        FMOD_2D | FMOD_LOOP_NORMAL | FMOD_SOFTWARE,
#endif
        nullptr,
        &sound
    );
    if (result != FMOD_OK) {
        std::cout << FMOD_ErrorString(result) << '\n';
        return 1;
    }

    result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, 1, &channel);
    if (result != FMOD_OK) {
        std::cout << FMOD_ErrorString(result) << '\n';
        return 1;
    }

#if ENABLE_3D
    FMOD_VECTOR listener_pos = {0.0f, 0.0f, 0.0f};
    FMOD_VECTOR listener_vel = {0.0f, 0.0f, 0.0f};
    FMOD_VECTOR listener_fwd = {0.0f, 0.0f, 1.0f};
    FMOD_VECTOR listener_up  = {0.0f, 1.0f, 0.0f};

    FMOD_System_Set3DListenerAttributes(system, 0, &listener_pos, &listener_vel, &listener_fwd, &listener_up);
#endif

#if EFFECT_MODE == EFFECT_ECHO
    FMOD_System_CreateDSPByType(system, FMOD_DSP_TYPE_ECHO, &dsp);
#elif EFFECT_MODE == EFFECT_TREMOLO
    FMOD_System_CreateDSPByType(system, FMOD_DSP_TYPE_TREMOLO, &dsp);
#elif EFFECT_MODE == EFFECT_HIGHPASS
    FMOD_System_CreateDSPByType(system, FMOD_DSP_TYPE_HIGHPASS, &dsp);
#endif

    if (dsp)
        FMOD_Channel_AddDSP(channel, dsp, nullptr);

    FMOD_Channel_SetPaused(channel, 0);

    float t = 0.0f;
    FMOD_BOOL playing = 1;

    while (playing) {

#if ENABLE_3D
        FMOD_VECTOR pos;
        pos.x = sin(t) * 5.0f;
        pos.y = 0.0f;
        pos.z = cos(t) * 5.0f;

        FMOD_VECTOR vel;
        vel.x = cos(t);
        vel.y = 0.0f;
        vel.z = -sin(t);

        FMOD_Channel_Set3DAttributes(channel, &pos, &vel);
#endif

        FMOD_Channel_IsPlaying(channel, &playing);
        FMOD_System_Update(system);

        t += 0.0000005f;
    }

    if (dsp) FMOD_DSP_Release(dsp);
    if (sound) FMOD_Sound_Release(sound);
    if (system) FMOD_System_Release(system);

    return 0;
}