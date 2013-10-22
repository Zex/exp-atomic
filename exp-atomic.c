/*
 * Author: Zex <top_zlynch@yahoo.com>
 *
 * experimental code of spinlock & atomic operations
 *
 * Copyright (c) by Zex Li <top_zlynch@yahoo.com>, Hytera Communications Co., Ltd.
 *
 * Sample command to load the module
 * 	insmod exp-atomic.ko
 */
#include <linux/types.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

atomic_t ato1;
DEFINE_SPINLOCK(spinlk1);

#define print_atomic_i(s,v) \
	printk(s, atomic_read(v));

struct stone
{
	struct list_head l;
	char acc;
	int num;
};

LIST_HEAD(stonies);

int atomic_enter(void)
{
	atomic_t ato1;
	struct list_head *pos, *next;
	struct stone *stone1, *buf;


	stone1 = (struct stone*)kmalloc(sizeof(struct stone), GFP_KERNEL);
	stone1->acc = 'A';
	stone1->num = 199;

	atomic_set(&ato1, 100);
	print_atomic_i("atomic_set(&ato1, 100), ato1:%d\n", &ato1);

	printk(KERN_INFO "initializing spinlk1\n");
	spin_lock_init(&spinlk1);

	list_add_tail(&stone1->l, &stonies);

	spin_lock(&spinlk1);
	list_for_each_safe(pos, next, &stonies) {
		buf = list_entry(pos, struct stone, l);
		printk(KERN_WARNING "acc:%c, num:%d\n", buf->acc, buf->num);
	}
	spin_unlock(&spinlk1);
	kfree(stone1);
	return 0;
}

void atomic_leave(void)
{
	atomic_add(17, &ato1);
	print_atomic_i("atomic_add(&ato1, 17), ato1:%d\n", &ato1);
	atomic_sub(31, &ato1);
	print_atomic_i("atomic_sub(&ato1, 31), ato1:%d\n", &ato1);
	printk("atomic_sub_and_test(&ato1, 77) ato1:%d\n", atomic_sub_and_test(77, &ato1));
	atomic_inc(&ato1);
	print_atomic_i("atomic_inc(&ato1), ato1:%d\n", &ato1);
	atomic_dec(&ato1);
	print_atomic_i("atomic_dec(&ato1), ato1:%d\n", &ato1);
	printk("atomic_add_return(100, &ato1), ato1:%d\n", atomic_add_return(100, &ato1));
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zex <top_zlynch@yahoo.com>");
MODULE_DESCRIPTION("Experimental code of spinlock & atomic operations");
MODULE_VERSION("0.0.1");

module_init(atomic_enter);
module_exit(atomic_leave);
