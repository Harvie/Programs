/* MDRAID Superblock generator
This should create valid mdraid superblock for raid1 with 1 device.
It is still work in progress, but following seems to be recognized:

make mdraid
./mdraid > test.img
mdadm --examine test.img

losetup /dev/loop1 test.img
mdadm --assemble md /dev/loop1

Some docs:
https://raid.wiki.kernel.org/index.php/RAID_superblock_formats#Sub-versions_of_the_version-1_superblock
https://docs.huihoo.com/doxygen/linux/kernel/3.7/md__p_8h_source.html

*/

//#include <cstddef>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/raid/md_p.h>

void random_uuid(__u8 *buf)
{
	__u32 r[4];
	for (int i = 0; i < 4; i++)
		r[i] = random();
	memcpy(buf, r, 16);
}

static unsigned int calc_sb_1_csum(struct mdp_superblock_1 * sb)
{
	unsigned int disk_csum, csum;
	unsigned long long newcsum;
	int size = sizeof(*sb) + __le32_to_cpu(sb->max_dev)*2;
	unsigned int *isuper = (unsigned int*)sb;

/* make sure I can count... (needs include cstddef) */
/*
	if (offsetof(struct mdp_superblock_1,data_offset) != 128 ||
	    offsetof(struct mdp_superblock_1, utime) != 192 ||
	    sizeof(struct mdp_superblock_1) != 256) {
		fprintf(stderr, "WARNING - superblock isn't sized correctly\n");
	}
*/

	disk_csum = sb->sb_csum;
	sb->sb_csum = 0;
	newcsum = 0;
	for (; size>=4; size -= 4 ) {
		newcsum += __le32_to_cpu(*isuper);
		isuper++;
	}

	if (size == 2)
		newcsum += __le16_to_cpu(*(unsigned short*) isuper);

	csum = (newcsum & 0xffffffff) + (newcsum >> 32);
	sb->sb_csum = disk_csum;
	return __cpu_to_le32(csum);
}

int main() {
	//printf("Superblock\n");

	size_t data_size = 8192; //512B sectors (should be divisible by 8 sectors to keep 4kB alignment)

	srand(time(NULL)); //FIXME: Seed UUID properly

	struct mdp_superblock_1 sb = {0};

	/* constant array information - 128 bytes */
	sb.magic = 0xa92b4efc;		/* MD_SB_MAGIC: 0xa92b4efc - little endian */
	sb.major_version = 1;	/* 1 */
	sb.feature_map = 0; //MD_FEATURE_BITMAP_OFFSET;	/* bit 0 set if 'bitmap_offset' is meaningful */ //FIXME: internal bitmap bit is not seen by mdadm????
	sb.pad0 = 0;		/* always set to 0 when writing */

	//TODO: set these
	random_uuid(sb.set_uuid);	/* user-space generated. U8[16]*/
	memcpy(sb.set_name, "localhost:7", 12);	/* set and interpreted by user-space. CHAR[32] */
	sb.ctime=0;		/* lo 40 bits are seconds, top 24 are microseconds or 0*/

	sb.level=1;		/* -4 (multipath), -1 (linear), 0,1,4,5 */
	//sb.layout=2;		/* only for raid5 and raid10 currently */
	sb.size=data_size;	/* used size of component devices, in 512byte sectors */

	sb.chunksize=0;		/* in 512byte sectors - not used in raid 1 */
	sb.raid_disks=1;
	sb.bitmap_offset=8;	/* sectors after start of superblock that bitmap starts
					 * NOTE: signed, so bitmap can be before superblock
					 * only meaningful of feature_map[0] is set.
					 */

	/* constant this-device information - 64 bytes */
	sb.data_offset=2048;	/* sector start of data, often 0 */
	sb.data_size=data_size;	/* sectors in this device that can be used for data */
	sb.super_offset=8;	/* sector start of this superblock */

	sb.dev_number=0;	/* permanent identifier of this  device - not role in raid */
	sb.cnt_corrected_read=0; /* number of read errors that were corrected by re-writing */
	random_uuid(sb.device_uuid); /* user-space setable, ignored by kernel U8[16] */
	sb.devflags=0;	/* per-device flags.  Only two defined...*/
		//#define	WriteMostly1	1	/* mask for writemostly flag in above */
		//#define	FailFast1	2	/* Should avoid retries and fixups and just fail */

		/* Bad block log.  If there are any bad blocks the feature flag is set.
		* If offset and size are non-zero, that space is reserved and available
		*/
	sb.bblog_shift=9;    /* shift from sectors to block size */ //FIXME: not sure with this!
	sb.bblog_size=8; /* number of sectors reserved for list */
	sb.bblog_offset=16;   /* sector offset from superblock to bblog,
			* signed - not unsigned */

	/* array state information - 64 bytes */
	sb.utime=0;		/* 40 bits second, 24 bits microseconds */
	sb.events=0;		/* incremented when superblock updated */
	sb.resync_offset=0;	/* data before this offset (from data_offset) known to be in sync */
	sb.max_dev=sb.raid_disks; /* size of devs[] array to consider */
	//__u8	pad3[64-32];	/* set to 0 when writing */

	/* device state information. Indexed by dev_number.
	 * 2 bytes per device
	 * Note there are no per-device state flags. State information is rolled
	 * into the 'roles' value.  If a device is spare or faulty, then it doesn't
	 * have a meaningful role.
	 */
	//__le16	dev_roles[];	/* role in array, or 0xffff for a spare, or 0xfffe for faulty */


	//Calculate checksum
	sb.sb_csum=calc_sb_1_csum(&sb);

	//Empty space before metadata (sector 0 - 7)
	for(int i=0;i<(sb.super_offset*512);i++) putc(0, stdout);

	//Superblock and padding (sector 8 - 2048)
	fwrite(&sb, sizeof(sb), 1, stdout);
	for(int i=0;i<(((sb.data_offset-sb.super_offset)*512)-sizeof(sb));i++) putc(0, stdout);

	//Data (N sectors)
	for(int i=0;i<(data_size*512);i++) putc(0, stdout);
}
