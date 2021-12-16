#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <asm-generic/bug.h>

MODULE_AUTHOR("Kondratiuk Pavlo <pasha.kondratiuk32@gmail.com>");
MODULE_DESCRIPTION("Lab 6");
MODULE_LICENSE("Dual BSD/GPL");

static uint printTimes = 1;

module_param(printTimes, uint, 0444);
MODULE_PARM_DESC(printTimes, "Number of prints");

struct my_struct {
	struct list_head tlist;
	ktime_t time;
};

static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{

	if (printTimes == 0) {
		pr_warn("The parameter is 0");
	} else if (printTimes >= 5 && printTimes <= 10) {
		pr_warn("The parameter is in [5; 10]");
	}

	BUG_ON((printTimes > 10));

	int i;
	for (i = 0; i < printTimes; i++) {

		struct my_struct *ms = NULL;

		if (i != printTimes - 1)
		{
			ms = kmalloc(sizeof(struct my_struct), GFP_KERNEL);
		}
		ms->time = ktime_get();
		list_add_tail(&ms->tlist, &my_list_head);

		pr_info("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	int count = 0;
	struct my_struct *t, *tmp;


	list_for_each_entry_safe(t, tmp, &my_list_head, tlist) {
		pr_info("i: %d; ktime = %lld\n", count++, ktime_to_ns(t->time));
		list_del(&t->tlist);
		kfree(t);
	}
	pr_info("Module unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
