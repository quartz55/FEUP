	

    int timer_set_square(unsigned long timer, unsigned long freq) {
     
            unsigned long MSB = (freq & 0xF0);
            unsigned long LSB = (freq & 0x0F);
     
            if (timer > 2)
                    return 1;
            if (timer == 0)
            {
                    sys_outb(TIMER_CTRL,TIMER_SEL0|TIMER_LSB_MSB|TIMER_SQR_WAVE);
                    sys_outb(TIMER_0, LSB);
                    sys_outb(TIMER_0, MSB);
                    return 0;
            }
            if (timer == 1)
            {
                    sys_outb(TIMER_CTRL,TIMER_SEL1|TIMER_LSB_MSB|TIMER_SQR_WAVE);
                    sys_outb(TIMER_1, LSB);
                    sys_outb(TIMER_1, MSB);
                    return 0;
            }
            if (timer == 2)
            {
                    sys_outb(TIMER_CTRL,TIMER_SEL2|TIMER_LSB_MSB|TIMER_SQR_WAVE);
                    sys_outb(TIMER_2, LSB);
                    sys_outb(TIMER_2, MSB);
                    return 0;
            }
    }
     
     
    int timer_test_square(unsigned long freq) {
           
            if(timer_set_square(0, freq) == 1)
                    return 1;
            return 0;
    }

