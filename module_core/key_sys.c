#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>

//Работоспособность проверена для версии ядра 4.9.0-8-amd64(Debian)

MODULE_DESCRIPTION("Example module illustrating the use of Keyboard LEDs.");
MODULE_LICENSE("GPL");
static struct kobject *kobject_key_sys;
static int foo;
struct timer_list my_timer;
struct tty_driver *my_driver;
char kbledstatus = 0;
int *pstatus_now = 0x07;
#define BLINK_DELAY   HZ/5
#define RESTORE_LEDS  0xFF
/*
 * Function my_timer_func blinks the keyboard LEDs periodically by invoking
 * command KDSETLED of ioctl() on the keyboard driver. To learn more on virtual
 * terminal ioctl operations, please see file:
 *     /usr/src/linux/drivers/char/vt_ioctl.c, function vt_ioctl().
 *
 * The argument to KDSETLED is alternatively set to 7 (thus causing the led
 * mode to be set to LED_SHOW_IOCTL, and all the leds are lit) and to 0xFF
 * (any value above 7 switches back the led mode to LED_SHOW_FLAGS, thus
 * the LEDs reflect the actual keyboard status). To learn more on this,
 * please see file:
 *     /usr/src/linux/drivers/char/keyboard.c, function setledstate().
 *
 */
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
		return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
		char *led_num = 0x00;
		sscanf(buf, "%du", &foo);
		if(foo > 7 || foo < 1)
		{
			printk(KERN_INFO "foo out of range, foo = 7\n");
			foo = 7;
		}
		led_num += foo;
		pstatus_now = (int *)led_num;
		return count;
}

static struct kobj_attribute foo_attribute =__ATTR(foo, 0660, foo_show, foo_store);

static void my_timer_func(unsigned long ptr)
{
        int *pstatus = (int *)ptr;
        if (*pstatus == pstatus_now)
                *pstatus = RESTORE_LEDS;
        else
                *pstatus = pstatus_now;
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            *pstatus);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
}

static int __init kbleds_init(void)
{
		int error = 0;
		pr_debug("Module initia;ized successfully \n");
		
		kobject_key_sys = kobject_create_and_add("key_sys", kernel_kobj);
		
		if(!kobject_key_sys)
			return -ENOMEM;

		error = sysfs_create_file(kobject_key_sys, &foo_attribute.attr);
		if(error)
		{
			pr_debug("failed to create the foo file in /sys/kernel/kobject_key_sys\n");
		}
        int i;
        printk(KERN_INFO "kbleds: loading\n");
        printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);
        for (i = 0; i < MAX_NR_CONSOLES; i++) {
                if (!vc_cons[i].d)
                        break;
                printk(KERN_INFO "poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
                       MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
                       (unsigned long)vc_cons[i].d->port.tty);
        }
        printk(KERN_INFO "kbleds: finished scanning consoles\n");
        my_driver = vc_cons[fg_console].d->port.tty->driver;
        printk(KERN_INFO "kbleds: tty driver magic %x\n", my_driver->magic);
        /*
         * Set up the LED blink timer the first time
         */
        init_timer(&my_timer);
        my_timer.function = my_timer_func;
        my_timer.data = (unsigned long)&kbledstatus;
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
        return error;
}
static void __exit kbleds_cleanup(void)
{
		pr_debug("Module un initialized successfully \n");
		kobject_put(kobject_key_sys);
        printk(KERN_INFO "kbleds: unloading...\n");
        del_timer(&my_timer);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            RESTORE_LEDS);
}
module_init(kbleds_init);
module_exit(kbleds_cleanup);
