#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/filter.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>
#include <linux/device-mapper.h>
#include <linux/dm-dirty-log.h>

#define DM_TABLE_MAX_DEPTH 16

struct dm_table {
	struct mapped_device *md;
	enum dm_queue_mode type;

	/* btree table */
	unsigned int depth;
	unsigned int counts[DM_TABLE_MAX_DEPTH]; /* in nodes */
	sector_t *index[DM_TABLE_MAX_DEPTH];

	unsigned int num_targets;
	unsigned int num_allocated;
	sector_t *highs;
	struct dm_target *targets;

	struct target_type *immutable_target_type;

	bool integrity_supported:1;
	bool singleton:1;
	unsigned integrity_added:1;

	/*
	 * Indicates the rw permissions for the new logical
	 * device.  This should be a combination of FMODE_READ
	 * and FMODE_WRITE.
	 */
	fmode_t mode;

	/* a list of devices used by this table */
	struct list_head devices;

	/* events get handed up using this callback */
	void (*event_fn)(void *);
	void *event_context;

	struct dm_md_mempools *mempools;

#ifdef CONFIG_BLK_INLINE_ENCRYPTION
	struct blk_keyslot_manager *ksm;
#endif
};
int flush_callback_fn(struct dm_target* t){return 0;}

struct dm_target * get_dm_target(struct dm_table * tbl, size_t target_num){
 	if (target_num >= tbl->num_targets) {
                return NULL;
        }
        return tbl->targets + target_num;
}
SYSCALL_DEFINE5(dm_dirty_log, void*, p, const char *, path, u32, target_num, const char *, log_type, char *, region_size){
	int srcu_idx;
	dev_t d_t = dm_get_dev_t(path);
	printk("dev_t: %d", d_t);
	struct mapped_device* md = dm_get_md(d_t);
	if (md)
		printk("Houston we have a mapped device %d", md);

	struct dm_table *tbl = dm_get_live_table(md, &srcu_idx);
	if (tbl)
		printk("Houston, we have a dm_table\n");
	struct dm_target *target = get_dm_target(tbl, target_num);

	if (target)
		printk("Houston, we have a dm_target\n");

	char * args[1];
	args[0] = region_size;

	struct dm_dirty_log *r = dm_dirty_log_create(log_type, target, flush_callback_fn, 1, args);

	if (r)
		printk("Houston, we have a dm_dirty_log\n");
	copy_to_user(p, r, sizeof(struct dm_dirty_log));
	return 0;
}

