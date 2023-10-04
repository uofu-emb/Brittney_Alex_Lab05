#include <zephyr.h>
#include <device.h>
#include <kernel.h>
#include <devicetree.h>
#include <drivers/gpio.h>

typedef struct k_thread k_thread_t;
typedef struct k_msgq k_msgq_t;

#define DEV_IN DT_NODELABEL(gpioa)
#define DEV_OUT DT_NODELABEL(gpioa)
#define PIN_OUT 0
#define PIN_IN 1
#define STACKSIZE 1000

struct gpio_callback callback;
const struct device *dev_in, *dev_out;

k_thread_t toggle_thread;
K_THREAD_STACK_DEFINE(toggle_stack, STACKSIZE);

K_MSGQ_DEFINE(request, sizeof(int), 32, 4);


void pin_interrupt(const struct device *port,
                   struct gpio_callback *cb,
                   gpio_port_pins_t pins_)
{
    k_msgq_put(&request, 1, K_FOREVER);
    // gpio_pin_toggle(dev_out, PIN_OUT);
}

void message_handler(k_msgq_t *requests)
{
    while (1)
    {
        int i;
        k_msgq_get(requests, i, K_FOREVER);
        gpio_pin_toggle(dev_out, PIN_OUT);
    }
}

void interrupt_main(void)
{
    k_thread_create(&toggle_thread,
                    toggle_stack,
                    STACKSIZE,
                    (k_thread_entry_t) message_handler,
                    &request,
                    NULL,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);
    
    dev_in = device_get_binding(DT_LABEL(DEV_IN));
    dev_out = device_get_binding(DT_LABEL(DEV_OUT));

    gpio_pin_configure(dev_out, PIN_OUT, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure(dev_in, PIN_IN, GPIO_INPUT);
    gpio_pin_interrupt_configure(dev_in, PIN_IN, GPIO_INT_EDGE_RISING);
    gpio_init_callback(&callback, pin_interrupt, 1 << PIN_IN);
    gpio_add_callback(dev_in, &callback);
    k_sleep(K_FOREVER);
}


