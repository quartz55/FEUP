

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

static int hook_id;
static int timer_id = 1;
int timer_counter;

int timer_set_square(unsigned long timer, unsigned long freq) {
    unsigned short n;
    unsigned char lsb, msb, port, control = 0x00;

    n = TIMER_FREQ / freq;

    lsb = n;
    msb = n >> 8;

    switch (timer) {
        case 0:
            control |= TIMER_SEL0;
            port = TIMER_0;
            break;
        case 1:
            control |= TIMER_SEL1;
            port = TIMER_1;
            break;
        case 2:
            control |= TIMER_SEL2;
            port = TIMER_2;
            break;
        default:
            break;
    }

    control |= TIMER_LSB_MSB;
    control |= TIMER_SQR_WAVE;
    control |= TIMER_BIN;

    if (sys_outb(TIMER_CTRL, control) != OK || sys_outb(port, lsb) != OK
            || sys_outb(port, msb) != OK)
        return 1;
    return 0;
}

int timer_subscribe_int(void) {
    hook_id = timer_id;
    timer_counter = 0;
    if (sys_irqsetpolicy(0, IRQ_REENABLE, &hook_id) != OK)
        return -1;
    if (sys_irqenable(&hook_id) != OK)
        return -1;
    return 1 << timer_id;
}

int timer_unsubscribe_int() {
    hook_id = timer_id;
    if (sys_irqrmpolicy(&hook_id) != OK)
        return 1;
    if (sys_irqdisable(&hook_id) != OK)
        return 1;
    return 0;

}

void timer_int_handler() {
    ++timer_counter;
}

int timer_get_config(unsigned long timer, unsigned char *st) {

    unsigned char port, control = 0x00;
    switch (timer) {
        case 0:
            port = TIMER_0;
        case 1:
            port = TIMER_1;
        case 2:
            port = TIMER_2;
        default:
            break;
    }
    control |= TIMER_RB_SEL(timer);
    control |= TIMER_RB_COUNT_;
    control |= TIMER_RB_CMD;
    if (sys_outb(TIMER_CTRL, control) == OK)
        /*if (sys_inb(port, st) == OK)*/
        return 0;

    return 1;

}

int timer_show_config(unsigned long timer) {

    unsigned char st;
    if (timer_get_config(timer, &st))
        return 1;
    if ((st & BIT (0)) == 0x00)
        printf("Binario \n");
    else
        printf("BCD \n");
    printf("Modo operativo: %d\n", st >> 1 & 7);
    printf("Tipo de timer: ");
    if ((st & (BIT(4) | BIT(5)) == 0x30))
        printf("lsb e msb");
    printf("\nCounter: %d\n", st >> 6 & 2);
    return 0;
}

int timer_test_square(unsigned long freq) {
    if (timer_set_square(0, freq))
        return -1; /*return -1 on case of failure*/
    else
        return 0;
}

int timer_test_int(unsigned long time) {

    int irq_set = timer_subscribe_int();
    int ipc_status;
    int r;
    message msg;

    while (timer_counter < 60 * time) {

        if ((r = driver_receive(ANY, &msg, &ipc_status) != 0)) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.NOTIFY_ARG & irq_set) {
                        timer_int_handler();
                        if (timer_counter % 60 == 0)
                            printf("cenas\n");
                    }
                    break;
                default:
                    break;
            }
        } else {

        }
    }

    timer_unsubscribe_int();
    timer_counter = 0;

    return 1;
}

int timer_test_config(unsigned long timer) {
    if (timer_show_config(timer))
        return 1;
    return 0;
}

