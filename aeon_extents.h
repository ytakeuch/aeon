#ifndef __AEON_EXTENTS_H
#define __AEON_EXTENTS_H

#include "aeon_inode.h"

#ifdef CONFIG_AEON_FS_COMPRESSION
#define AEON_E_SHIFT		5
#define AEON_EXTENT_HEADER_SIZE 64
#else
#define AEON_E_SHIFT		4
#define AEON_EXTENT_HEADER_SIZE 32
#endif

#define AEON_EXTENT_SIZE	(1 << AEON_E_SHIFT)
#define AEON_EXTENT_PER_PAGE	(AEON_DEF_BLOCK_SIZE_4K / AEON_EXTENT_SIZE)
#define AEON_EXTENT_MAX_DEPTH	(AEON_DEF_BLOCK_SIZE_4K / \
				 AEON_EXTENT_HEADER_SIZE)

static inline
struct aeon_extent_header *aeon_get_extent_header(struct aeon_inode *pi)
{
	return &pi->aeh;
}

static inline
struct aeon_extent_middle_header *aeon_get_extent_mheader(struct super_block *sb,
							  struct aeon_inode *pi)
{
	struct aeon_extent_header *aeh = aeon_get_extent_header(pi);
	//aeon_dbg("%s: 0x%llx\n", __func__, le64_to_cpu(aeh->eh_cur_block_addr));
	if (!le64_to_cpu(aeh->eh_cur_block_addr))
		return NULL;
	return (struct aeon_extent_middle_header *)
		(AEON_HEAD(sb) + le64_to_cpu(aeh->eh_cur_block_addr));
}

static inline struct aeon_extent_middle_header
*aeon_get_extent_first_mheader(struct super_block *sb, struct aeon_inode *pi)
{
	struct aeon_extent_header *aeh = aeon_get_extent_header(pi);
	//aeon_dbg("%s: 0x%llx\n", __func__, le64_to_cpu(aeh->eh_first_block_addr));
	if (!le64_to_cpu(aeh->eh_first_block_addr))
		return NULL;
	return (struct aeon_extent_middle_header *)
		(AEON_HEAD(sb) + le64_to_cpu(aeh->eh_first_block_addr));
}

static inline
void aeon_init_extent_middle_header(struct aeon_extent_middle_header *aemh)
{
	aemh->em_entries = 1; /* header is included */
	aemh->em_next_addr = 0;
}

static inline
void aeon_init_extent_header(struct aeon_extent_header *aeh)
{
	//aeh->eh_entries = 0;
	//aeh->eh_depth = 0;
	//aeh->eh_blocks = 0;
	//memset(aeh->eh_extent_blocks, 0, sizeof(aeh->eh_extent_blocks));
	memset(aeh, 0, sizeof(struct aeon_extent_header));
}

static inline
struct aeon_extent *aeon_get_prev_extent(struct aeon_extent_header *aeh)
{
	return (struct aeon_extent *)le64_to_cpu(aeh->eh_prev_extent);
}

int aeon_delete_extenttree(struct super_block *sb,
			   struct aeon_inode_info_header *sih);
int aeon_cutoff_extenttree(struct super_block *sb,
			   struct aeon_inode_info_header *sih,
			   struct aeon_inode *pi, int remaining, int index);
struct aeon_extent *aeon_search_extent(struct super_block *sb,
				       struct aeon_inode_info_header *sih,
				       unsigned long iblock);
int aeon_update_extent(struct super_block *sb, struct inode *inode,
		       unsigned long blocknr, unsigned long offset,
		       int num_blocks);
int aeon_rebuild_rb_extenttree(struct super_block *sb,
			       struct inode *inode, int entries);
#ifdef CONFIG_AEON_FS_COMPRESSION
struct aeon_extent *aeon_search_cextent(struct super_block *sb,
					struct aeon_inode_info_header *sih,
					unsigned long iblock);
int aeon_update_cextent(struct super_block *sb, struct inode *inode,
			unsigned long blocknr, unsigned long offset,
			int num_blocks, int compressed_length,
			unsigned long original_len,
			int compressed, int allocated);
#endif
#endif
