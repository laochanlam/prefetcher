#ifndef TRANSPOSE_IMPL
#define TRANSPOSE_IMPL

void sse_prefetch_transpose(int *src, int *dst, int w, int h, int Locality_Hint)
{
#define PFDIST 8
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            switch (Locality_Hint) {
                case 0:
                    _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T0);
                    _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T0);
                    _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T0);
                    _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T0);
                    break;
                case 1:
                    _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T1);
                    _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T1);
                    _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T1);
                    _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T1);
                    break;
                case 2:
                    _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T2);
                    _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T2);
                    _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T2);
                    _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T2);
                    break;
                case 3:
                    _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_NTA);
                    _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_NTA);
                    _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_NTA);
                    _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_NTA);
                    break;
            }

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
#endif /* TRANSPOSE_IMPL */
