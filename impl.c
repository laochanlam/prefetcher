#ifndef TRANSPOSE_IMPL
#define TRANSPOSE_IMPL

void naive_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            *(dst + x * h + y) = *(src + y * w + x);
}

void sse_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            __m128i I0 = _mm_loadu_si128((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128((__m128i *)(src + (y + 3) * w + x));
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);
            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}

void sse_prefetch_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
#define PFDIST  8
            _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T1);

            __m128i I0 = _mm_loadu_si128 ((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128 ((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128 ((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128 ((__m128i *)(src + (y + 3) * w + x));
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);
            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}

void avx_transpose(int *src, int *dst, int w, int h)
{
    __m256i I0,I1,I3,I4,I5,I6,I7,T1,T2,T0,T3,T4,T5,T6,T7,I2;

    for ( int x = 0; x < w; x += 8 ) {
        for ( int y = 0; y < h; y += 8 ) {
            I0 = _mm256_loadu_si256((__m256i *)(src + (y + 0) * w + x));
            I1 = _mm256_loadu_si256((__m256i *)(src + (y + 1) * w + x));
            I2 = _mm256_loadu_si256((__m256i *)(src + (y + 2) * w + x));
            I3 = _mm256_loadu_si256((__m256i *)(src + (y + 3) * w + x));
            I4 = _mm256_loadu_si256((__m256i *)(src + (y + 4) * w + x));
            I5 = _mm256_loadu_si256((__m256i *)(src + (y + 5) * w + x));
            I6 = _mm256_loadu_si256((__m256i *)(src + (y + 6) * w + x));
            I7 = _mm256_loadu_si256((__m256i *)(src + (y + 7) * w + x));

            T0 = _mm256_unpacklo_epi32(I0, I1);
            T1 = _mm256_unpackhi_epi32(I0, I1);
            T2 = _mm256_unpacklo_epi32(I2, I3);
            T3 = _mm256_unpackhi_epi32(I2, I3);
            T4 = _mm256_unpacklo_epi32(I4, I5);
            T5 = _mm256_unpackhi_epi32(I4, I5);
            T6 = _mm256_unpacklo_epi32(I6, I7);
            T7 = _mm256_unpackhi_epi32(I6, I7);

            I0 = _mm256_unpacklo_epi64(T0, T2);
            I1 = _mm256_unpackhi_epi64(T0, T2);
            I2 = _mm256_unpacklo_epi64(T1, T3);
            I3 = _mm256_unpackhi_epi64(T1, T3);
            I4 = _mm256_unpacklo_epi64(T4, T6);
            I5 = _mm256_unpackhi_epi64(T4, T6);
            I6 = _mm256_unpacklo_epi64(T5, T7);
            I7 = _mm256_unpackhi_epi64(T5, T7);

            T0 = _mm256_permute2x128_si256(I0, I4, 0x20);
            T1 = _mm256_permute2x128_si256(I1, I5, 0x20);
            T2 = _mm256_permute2x128_si256(I2, I6, 0x20);
            T3 = _mm256_permute2x128_si256(I3, I7, 0x20);
            T4 = _mm256_permute2x128_si256(I0, I4, 0x31);
            T5 = _mm256_permute2x128_si256(I1, I5, 0x31);
            T6 = _mm256_permute2x128_si256(I2, I6, 0x31);
            T7 = _mm256_permute2x128_si256(I3, I7, 0x31);

            _mm256_storeu_si256((__m256i *)(dst + ((x + 0) * h) + y), T0);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 1) * h) + y), T1);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 2) * h) + y), T2);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 3) * h) + y), T3);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 4) * h) + y), T4);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 5) * h) + y), T5);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 6) * h) + y), T6);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 7) * h) + y), T7);
        }
    }
}

void avx_prefetch_transpose(int *src, int *dst, int w, int h)
{
    __m256i I0,I1,I3,I4,I5,I6,I7,T1,T2,T0,T3,T4,T5,T6,T7,I2;

    for ( int x = 0; x < w; x += 8 ) {
        for ( int y = 0; y < h; y += 8 ) {
#define PFDISTHUGE  16
            _mm_prefetch(src+(y + PFDISTHUGE + 0) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDISTHUGE + 1) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDISTHUGE + 2) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDISTHUGE + 3) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDISTHUGE + 4) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDISTHUGE + 5) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDISTHUGE + 6) *w + x, _MM_HINT_T0);
            _mm_prefetch(src+(y + PFDISTHUGE + 7) *w + x, _MM_HINT_T0);

            I0 = _mm256_loadu_si256((__m256i *)(src + (y + 0) * w + x));
            I1 = _mm256_loadu_si256((__m256i *)(src + (y + 1) * w + x));
            I2 = _mm256_loadu_si256((__m256i *)(src + (y + 2) * w + x));
            I3 = _mm256_loadu_si256((__m256i *)(src + (y + 3) * w + x));
            I4 = _mm256_loadu_si256((__m256i *)(src + (y + 4) * w + x));
            I5 = _mm256_loadu_si256((__m256i *)(src + (y + 5) * w + x));
            I6 = _mm256_loadu_si256((__m256i *)(src + (y + 6) * w + x));
            I7 = _mm256_loadu_si256((__m256i *)(src + (y + 7) * w + x));

            T0 = _mm256_unpacklo_epi32(I0, I1);
            T1 = _mm256_unpackhi_epi32(I0, I1);
            T2 = _mm256_unpacklo_epi32(I2, I3);
            T3 = _mm256_unpackhi_epi32(I2, I3);
            T4 = _mm256_unpacklo_epi32(I4, I5);
            T5 = _mm256_unpackhi_epi32(I4, I5);
            T6 = _mm256_unpacklo_epi32(I6, I7);
            T7 = _mm256_unpackhi_epi32(I6, I7);

            I0 = _mm256_unpacklo_epi64(T0, T2);
            I1 = _mm256_unpackhi_epi64(T0, T2);
            I2 = _mm256_unpacklo_epi64(T1, T3);
            I3 = _mm256_unpackhi_epi64(T1, T3);
            I4 = _mm256_unpacklo_epi64(T4, T6);
            I5 = _mm256_unpackhi_epi64(T4, T6);
            I6 = _mm256_unpacklo_epi64(T5, T7);
            I7 = _mm256_unpackhi_epi64(T5, T7);

            T0 = _mm256_permute2x128_si256(I0, I4, 0x20);
            T1 = _mm256_permute2x128_si256(I1, I5, 0x20);
            T2 = _mm256_permute2x128_si256(I2, I6, 0x20);
            T3 = _mm256_permute2x128_si256(I3, I7, 0x20);
            T4 = _mm256_permute2x128_si256(I0, I4, 0x31);
            T5 = _mm256_permute2x128_si256(I1, I5, 0x31);
            T6 = _mm256_permute2x128_si256(I2, I6, 0x31);
            T7 = _mm256_permute2x128_si256(I3, I7, 0x31);

            _mm256_storeu_si256((__m256i *)(dst + ((x + 0) * h) + y), T0);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 1) * h) + y), T1);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 2) * h) + y), T2);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 3) * h) + y), T3);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 4) * h) + y), T4);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 5) * h) + y), T5);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 6) * h) + y), T6);
            _mm256_storeu_si256((__m256i *)(dst + ((x + 7) * h) + y), T7);
        }
    }
}

#endif /* TRANSPOSE_IMPL */
