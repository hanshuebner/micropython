/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Hans Hübner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/runtime.h"
#include "modrp2.h"

#include "hardware/dma.h"

typedef struct _rp2_dma_obj_t {
    mp_obj_base_t base;
} rp2_dma_obj_t;

STATIC const rp2_dma_obj_t rp2_dma = {{&rp2_dma_type}};

STATIC mp_obj_t rp2_dma_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    // check arguments
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    return MP_OBJ_FROM_PTR(&rp2_dma);
}

STATIC mp_obj_t rp2_dma_channel_claim(mp_obj_t self_in, mp_obj_t channel_in) {
    (void)self_in;
    int channel = mp_obj_int_get_checked(channel_in);
    if (channel < 0 || channel >= NUM_DMA_CHANNELS) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid DMA channel number"));
    }
    dma_channel_claim(channel);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(rp2_dma_channel_claim_obj, rp2_dma_channel_claim);

STATIC mp_obj_t rp2_dma_channel_unclaim(mp_obj_t self_in, mp_obj_t channel_in) {
    (void)self_in;
    int channel = mp_obj_int_get_checked(channel_in);
    if (channel < 0 || channel >= NUM_DMA_CHANNELS) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid DMA channel number"));
    }
    dma_channel_unclaim(channel);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(rp2_dma_channel_unclaim_obj, rp2_dma_channel_unclaim);

STATIC mp_obj_t rp2_dma_claim_unused_channel(mp_obj_t self_in, mp_obj_t required_in) {
    (void)self_in;
    int channel = dma_claim_unused_channel(mp_obj_is_true(required_in));
    return mp_obj_new_int(channel);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(rp2_dma_claim_unused_channel_obj, rp2_dma_claim_unused_channel);

STATIC const mp_rom_map_elem_t rp2_dma_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_channel_claim), MP_ROM_PTR(&rp2_dma_channel_claim_obj) },
    { MP_ROM_QSTR(MP_QSTR_channel_unclaim), MP_ROM_PTR(&rp2_dma_channel_unclaim_obj) },
    { MP_ROM_QSTR(MP_QSTR_claim_unused_channel), MP_ROM_PTR(&rp2_dma_claim_unused_channel_obj) },
};
STATIC MP_DEFINE_CONST_DICT(rp2_dma_locals_dict, rp2_dma_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    rp2_dma_type,
    MP_QSTR_DMA,
    MP_TYPE_FLAG_NONE,
    make_new, rp2_dma_make_new,
    locals_dict, &rp2_dma_locals_dict
    );
