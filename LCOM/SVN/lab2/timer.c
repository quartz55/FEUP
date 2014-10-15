#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <time.h>
#include "i8254.h"

static int hook_id;
static int timer_id = 0;
int timer_counter;


int timer_set_square(unsigned long timer, unsigned long freq) {

  unsigned short div;
  unsigned char msb, lsb, selection = 0x00;

  div = TIMER_FREQ/freq;
  lsb = div & 0x0F;
  msb = (div >> 8) & 0x0F;

  switch(timer){
    case 0:
      selection |= TIMER_SEL0;
      break;
    case 1:
      selection |= TIMER_SEL1;
      break;
    case 2:
      selection |= TIMER_SEL2;
      break;
    default:
      return 1;
      break;
  }

  int res;

  //NOT NEEDED (I think)

  /*unsigned char st;*/
  /*res = timer_get_conf(timer, &st);*/
  /*if (res != 0) return 1;*/
  /*st |= 0x01; //00000001b (00001111b?!)*/
  /*selection |= st; [> Maintain the original BSD/binary option (counting mode?!)<]*/

  res = sys_outb(TIMER_CTRL, selection | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN);
  if (res != 0) return 1;

  res = sys_outb(timer + TIMER_0, lsb);
  if (res != 0) return 1;

  res = sys_outb(timer + TIMER_0, msb);
  if (res != 0) return 1;

  return 0;
}

int timer_subscribe_int(void ) {

	int hook_id = 1;
	int res = sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE, &hook_id);
	if (res != 0) return 1;
	res = sys_irqenable(&hook_id);
	if (res != 0) return 2;
	return 0;
}

int timer_unsubscribe_int() {
    hook_id = timer_id;
    if (sys_irqrmpolicy(&hook_id) != OK)
        return 1;
    if (sys_irqdisable(&hook_id) != OK)
        return 1;
    return 0;

	int hook_id = 1;
	int res = sys_irqrmpolicy(&hook_id);
	if (res != 0) return 1;
	return 0;
}

void timer_int_handler() {
	printf("TIMER 0 interrupt\n");
}


int timer_get_conf(unsigned long timer, unsigned char *st) {

  int res = sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (res != 0) return 1;
  unsigned long stl;
  res = sys_inb(timer + TIMER_0, &stl);
  *st = stl;
  if (res != 0) return 1;
  return 0;
}

int timer_display_conf(unsigned char conf) {

  unsigned char mask;
  printf("\n-------------------------------------------\n");
  unsigned int decNum = conf;
  unsigned int bit;
  for( bit = 1u << 8; bit; bit >>= 1 ) {
    printf( "%c", decNum & bit ? '1' : '0' );
  }
  printf("\n\n");
  printf("Counter Initialization: ");
  mask = 0x20; // 00100000b
  if ( conf & mask ){
    mask = 0x10;  // 00010000b
    if ( conf & mask ) printf("LSB followed by MSB\n");
    else printf("MSB\n");
  }else{
    printf("LSB\n");
  }
  printf("Programmed Mode: ");
  mask = 0x04; // 00000100b
  if ( conf & mask ){
    mask = 0x02;  // 00000010b
    if ( conf & mask ) printf("3\n");
    else printf("2\n");
  }else{
    mask = 0x08;  // 00001000b
    if ( conf & mask ){
      mask = 0x02;  // 00000010b
      if ( conf & mask ) printf("5\n");
      else printf("4\n");
    }
    else{
      mask = 0x02;  // 00000010b
      if ( conf & mask ) printf("1\n");
      else printf("0\n");
    }
  }
  printf("BCD: ");
  mask = 0x01;  // 00000001b
  if ( conf & mask ) printf("BCD (4 digits)\n");
  else printf("Binary (16 bits)\n");

  printf("-------------------------------------------\n\n");

  return 0;
}

int timer_test_square(unsigned long freq) {

  int res = timer_set_square(0, freq);
  return res;
}

int timer_test_int(unsigned long time) {

	printf("start\n");

	int ipc_status, r, res, x=1000000000;
	message msg;
	//time_t start = time(NULL); //do_time()?

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
                            printf("Interrupt\n");
                    }
                    break;
                default:
                    break;
            }
        } else {

	while( x ) { /* Tests if time's up */ /*(time(NULL) - start) <= time*/
		/* Get a request message. */
		if (r = driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)){
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & 0x1) { /* subscribed interrupt */
					timer_int_handler();
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		x--;
		//printf("one iteration\n");
	}
	res = timer_unsubscribe_int();
	if (res != 0) return 2;

	return 0;
}

int timer_test_config(unsigned long timer) {

  if (timer != 0 && timer != 1 && timer != 2) return 1;

  unsigned char st;
  int res;
  res = timer_get_conf(timer, &st);
  if (res != 0) return 2;

  res = timer_display_conf(st);
  if (res != 0) return 3;

  return 0;
}
