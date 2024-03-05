#ifndef PCG_BASIC_H_INCLUDED
#define PCG_BASIC_H_INCLUDED 1

// NOTE: PCG_CPP Random number generator. I renamed the functions so they make more sense to me and are a bit more readable.

#include <inttypes.h>

#if __cplusplus
extern "C" {
#endif

///////////////////////////////
// NOTE: Defines
///////////////////////////////

typedef struct pcg_state_setseq_64{
    u64 state;                  // RNG state.  All values are possible.
    u64 inc;                    // Controls which RNG sequence (stream) is selected. Must *always* be odd.
} pcg_state_setseq_64;
typedef struct pcg_state_setseq_64 pcg32_random_t;

// If you *must* statically initialize it, here's one.
#define PCG32_INITIALIZER   { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

// state for global RNGs
static pcg32_random_t pcg32_global = PCG32_INITIALIZER;


///////////////////////////////
// NOTE: Usage
///////////////////////////////

// Generate a uniformly distributed 32-bit random number
// call me once, shame on me. call me twice, u64.
#define random_u32() random_u32_(&pcg32_global)
static u32 random_u32_(pcg32_random_t* rng){
    u64 oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    u32 xorshifted = (u32)((oldstate >> 18u) ^ oldstate) >> 27u;
    u32 rot = oldstate >> 59u;

    return (xorshifted >> rot) | (xorshifted << ((0-rot) & 31));
}

// Generate a uniformly distributed 64-bit random number
#define random_u64() random_u64_(&pcg32_global)
static u64 random_u64_(pcg32_random_t* rng){
    u64 value = random_u32_(rng);
    value <<= 32;
    value |= (u64)random_u32_(rng);
    return(value);
}

// Generate a 32-bit floating point random number
static f32 random_f32(){
    u32 x = random_u32();
    return((f32)(u32)(x >> 8) * 0x1.0p-24f);
}

// Generate a 64-bit floating point random number
// We use random_u64() to create a 64-bit floating point number
static f64 random_f64(){
    u64 x = random_u64();
    return((f64)(u64)(x >> 11) * 0x1.0p-53);
}

// Generate a uniformly distributed number, r, where 0 <= r < bound
// EXAMPLE: ((s32)random_range_u32(201) - 100) / 100.0f;
#define random_range_u32(bound) random_range_u32_r(&pcg32_global, bound)
static u32 random_range_u32_r(pcg32_random_t* rng, u32 bound){
    u32 threshold = (0-bound) % bound;

    for (;;) {
        u32 r = random_u32_(rng);
        if (r >= threshold)
            return r % bound;
    }
}

#define random_range_u64(bound) random_range_u64_(&pcg32_global, bound)
static u64 random_range_u64_(pcg32_random_t* rng, u64 bound){
    u64 threshold = (0-bound) % bound;

    for (;;) {
        u64 r = random_u64_(rng);
        if (r >= threshold)
            return r % bound;
    }
}

static f32 random_range_f32(f32 max){
    u32 precision = 1000000000;
    f32 t = (f32)random_range_u32(precision) / (f32)precision; // this needs to change to something smarter
    f32 result = (1.0f - t)*0 + t*max;
    return(result);
}

static f64 random_range_f64(f64 max){
    u64 precision = 1000000000000000000;
    f64 t = (f64)random_range_u64(precision) / (f64)precision; // this needs to change to something smarter
    f64 result = (1.0 - t)*0 + t*max;
    return(result);
}

// Seed the rng. Specified in two parts, state initializer and a sequence selection constant (a.k.a. stream id)
// EXAMPLE: random_seed(time(0), 0) OR random_seed(1, 0)
#define random_seed(seed, seq) randomr_seed(&pcg32_global, seed, seq)
static void randomr_seed(pcg32_random_t* rng, u64 initstate, u64 initseq){
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    random_u32_(rng);
    rng->state += initstate;
    random_u32_(rng);
}

#if __cplusplus
}
#endif

#endif
