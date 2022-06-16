#include <unistd.h>

#include "app_wsbrd/wsbr.h"
#include "app_wsbrd/wsbr_mac.h"
#include "common/bus_uart.h"
#include "common/crc.h"
#include "common/log.h"
#include "common/spinel_buffer.h"
#include "common/spinel_defs.h"
#include "wsbrd_fuzz.h"
#include "capture.h"

void fuzz_capture(struct fuzz_ctxt *ctxt, const void *data, size_t size)
{
    int ret;

    ret = write(ctxt->uart_fd, data, size);
    FATAL_ON(ret < 0, 2, "write: %m");
    FATAL_ON(ret < size, 2, "write: Short write");
}

static void fuzz_capture_spinel(struct fuzz_ctxt *ctxt, struct spinel_buffer *buf)
{
    uint16_t crc = crc16(buf->frame, buf->cnt);
    uint8_t *frame = malloc(buf->cnt * 2 + 3);
    size_t frame_len;

    frame_len = uart_encode_hdlc(frame, buf->frame, buf->cnt, crc);
    fuzz_capture(ctxt, frame, frame_len);
    free(frame);
}

void fuzz_capture_timers(struct fuzz_ctxt *ctxt)
{
    struct spinel_buffer *buf;

    if (!ctxt->timer_counter)
        return;

    buf = ALLOC_STACK_SPINEL_BUF(9);
    spinel_push_u8(buf, wsbr_get_spinel_hdr(&g_ctxt));
    spinel_push_uint(buf, SPINEL_CMD_REPLAY_TIMERS);
    spinel_push_u16(buf, ctxt->timer_counter);
    fuzz_capture_spinel(ctxt, buf);
    ctxt->timer_counter = 0;
}