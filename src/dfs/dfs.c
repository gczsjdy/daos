/**
 * (C) Copyright 2018 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of the Apache License as
 * provided in Contract No. B609815.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */

#define D_LOGFAC	DD_FAC(dfs)

#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <daos/common.h>
#include <daos/debug.h>
#include "daos_types.h"
#include "daos_api.h"
#include "daos_addons.h"
#include "daos_fs.h"

/** D-key name of SB info in the SB object */
#define SB_DKEY		"DFS_SB_DKEY"
/** A-key name of SB info in the SB object */
#define SB_AKEY		"DFS_SB_AKEY"
/** Magic Value for SB value */
#define SB_MAGIC	0xda05df50da05df50

/** Number of A-keys for attributes in any object entry */
#define INODE_AKEYS	6
/** A-key name of mode_t value */
#define MODE_NAME	"mode"
/** A-key name of object ID value */
#define OID_NAME	"oid"
/** A-key name of last access time */
#define ATIME_NAME	"atime"
/** A-key name of last modify time */
#define MTIME_NAME	"mtime"
/** A-key name of last change time */
#define CTIME_NAME	"ctime"
/** A-key name of symlink value */
#define SYML_NAME	"syml"

/** Array object stripe size for regular files */
/** TODO - make this user configurable */
#define STRIPE_SIZE	1048576

/** Parameters for dkey enumeration */
#define ENUM_KEY_NR     1000
#define ENUM_DESC_NR    10
#define ENUM_DESC_BUF   (ENUM_DESC_NR * DFS_MAX_PATH)

enum {
	DFS_WRITE,
	DFS_READ
};

/** object struct that is instantiated for a DFS open object */
struct dfs_obj {
	/** DAOS object ID */
	daos_obj_id_t		oid;
	/** DAOS object open handle */
	daos_handle_t		oh;
	/** mode_t containing permissions & type */
	mode_t			mode;
	/** DAOS object ID of the parent of the object */
	daos_obj_id_t		parent_oid;
	/** entry name of the object in the parent */
	char			name[DFS_MAX_PATH];
	/** Symlink value if object is a symbolic link */
	char			*value;
};

/** dfs struct that is instantiated for a mounted DFS namespace */
struct dfs {
	/** flag to indicate whether the dfs is mounted */
	bool			mounted;
	/** lock for threadsafety */
	pthread_mutex_t		lock;
	/** uid - inherited from pool. TODO - make this from container. */
	uid_t			uid;
	/** gid - inherited from pool. TODO - make this from container. */
	gid_t			gid;
	/** Open pool handle of the DFS */
	daos_handle_t		poh;
	/** Open container handle of the DFS */
	daos_handle_t		coh;
	/** Epoch being used for access (incremented on each update op) */
	daos_epoch_t		epoch;
	/** Object ID reserved for this DFS (see oid_gen below) */
	daos_obj_id_t		oid;
	/** OID of SB */
	daos_obj_id_t		super_oid;
	/** Open object handle of SB */
	daos_handle_t		super_oh;
	/** Root object info */
	dfs_obj_t		root;
};

struct dfs_entry {
	/** mode (permissions + entry type) */
	mode_t		mode;
	/** Object ID if not a symbolic link */
	daos_obj_id_t	oid;
	/** Sym Link value */
	char		*value;
	/* Time of last access */
	time_t		atime;
	/* Time of last modification */
	time_t		mtime;
	/* Time of last status change */
	time_t		ctime;
};

#if 0
static void
time2str(char *buf, time_t t) {
	struct tm tm;

	gmtime_r(&t, &tm);
	strftime(buf, 26, "%F %T", &tm);
}

static time_t
str2time(char *buf) {
	time_t t;
	struct tm tm;

	strptime(buf, "%F %T", &tm);
	t = mktime(&tm);
	return t;
}

static void
print_mode(mode_t mode)
{
	D_DEBUG(DB_TRACE, "(%o)\t", mode);
	D_DEBUG(DB_TRACE, (mode & S_IRUSR) ? "r" : "-");
	D_DEBUG(DB_TRACE, (mode & S_IWUSR) ? "w" : "-");
	D_DEBUG(DB_TRACE, (mode & S_IXUSR) ? "x" : "-");
	D_DEBUG(DB_TRACE, (mode & S_IRGRP) ? "r" : "-");
	D_DEBUG(DB_TRACE, (mode & S_IWGRP) ? "w" : "-");
	D_DEBUG(DB_TRACE, (mode & S_IXGRP) ? "x" : "-");
	D_DEBUG(DB_TRACE, (mode & S_IROTH) ? "r" : "-");
	D_DEBUG(DB_TRACE, (mode & S_IWOTH) ? "w" : "-");
	D_DEBUG(DB_TRACE, (mode & S_IXOTH) ? "x" : "-");
	D_DEBUG(DB_TRACE, "\n");
}
static void
print_stat(struct stat *stbuf)
{
	char buf[26];

	D_DEBUG(DB_TRACE, "Size = %zu\n", stbuf->st_size);
	D_DEBUG(DB_TRACE, "UID %lu\n", (unsigned long int)stbuf->st_uid);
	D_DEBUG(DB_TRACE, "GID %lu\n", (unsigned long int)stbuf->st_gid);
	print_mode(stbuf->st_mode);
	time2str(buf, stbuf->st_atim.tv_sec);
	D_DEBUG(DB_TRACE, "Access time %s\n", buf);
	time2str(buf, stbuf->st_mtim.tv_sec);
	D_DEBUG(DB_TRACE, "Modify time %s\n", buf);
	time2str(buf, stbuf->st_ctim.tv_sec);
	D_DEBUG(DB_TRACE, "Change time %s\n", buf);
}
#endif

static inline void
oid_cp(daos_obj_id_t *dst, daos_obj_id_t src)
{
	dst->hi = src.hi;
	dst->lo = src.lo;
}

/*
 * Increment the container epoch. Every op that updates the container should
 * bump the epoch to use a new one for it's update to avoid overwrites in the
 * same epoch. If the update operation fails, the epoch is discarded.
 */
static inline void
incr_epoch(dfs_t *dfs)
{
	D_MUTEX_LOCK(&dfs->lock);
	dfs->epoch++;
	D_MUTEX_UNLOCK(&dfs->lock);
}

#define MAX_OID_HI ((1UL << 32) - 1)

/*
 * OID generation for the dfs objects.
 *
 * The oid.lo uint64_t value will be allocated from the DAOS container using the
 * unique oid allocator. 1 oid at a time will be allocated for the dfs mount.
 * The oid.hi value has the high 32 bits reserved for DAOS (obj class, type,
 * etc.). The lower 32 bits will be used locally by the dfs mount point, and
 * hence discarded when the dfs is unmounted.
 */
int
oid_gen(dfs_t *dfs, uint16_t oclass, daos_obj_id_t *oid)
{
	int rc = 0;

	if (oclass == 0)
		oclass = DAOS_OC_REPL_MAX_RW;

	D_MUTEX_LOCK(&dfs->lock);
	/** If we ran out of local OIDs, alloc one from the container */
	if (dfs->oid.hi >= MAX_OID_HI) {
		/** Allocate an OID for the namespace */
		rc = daos_cont_oid_alloc(dfs->coh, 1, &dfs->oid.lo, NULL);
		if (rc) {
			D_ERROR("daos_cont_oid_alloc() Failed (%d)\n", rc);
			D_MUTEX_UNLOCK(&dfs->lock);
			return rc;
		}
		dfs->oid.hi = 0;
	}

	/** set oid and lo, bump the current hi value */
	oid->lo = dfs->oid.lo;
	oid->hi = dfs->oid.hi++;
	D_MUTEX_UNLOCK(&dfs->lock);

	/** generate the daos object ID (set the DAOS owned bits) */
	daos_obj_id_generate(oid, 0, oclass);

	return rc;
}

static int
fetch_entry(daos_handle_t oh, daos_epoch_t epoch, const char *name,
	    bool fetch_sym, bool *exists, struct dfs_entry *entry)
{
	daos_sg_list_t	sgls[INODE_AKEYS];
	daos_iov_t	sg_iovs[INODE_AKEYS];
	daos_iod_t	iods[INODE_AKEYS];
	char		value[DFS_MAX_PATH];
	daos_key_t	dkey;
	unsigned int	akeys_nr, i;
	int		rc;

	if (name == NULL)
		return -DER_INVAL;

	D_DEBUG(DB_TRACE, "fetch entry %s\n", name);

	/** TODO - not supported yet */
	if (strcmp(name, ".") == 0)
		D_ASSERT(0);

	daos_iov_set(&dkey, (void *)name, strlen(name));
	i = 0;

	/** Set Akey for MODE */
	daos_iov_set(&sg_iovs[i], &entry->mode, sizeof(mode_t));
	daos_iov_set(&iods[i].iod_name, MODE_NAME, strlen(MODE_NAME));
	i++;

	/** Set Akey for OID; if entry is symlink, this value will be bogus */
	daos_iov_set(&sg_iovs[i], &entry->oid, sizeof(daos_obj_id_t));
	daos_iov_set(&iods[i].iod_name, OID_NAME, strlen(OID_NAME));
	i++;

	/** Set Akey for ATIME */
	daos_iov_set(&sg_iovs[i], &entry->atime, sizeof(time_t));
	daos_iov_set(&iods[i].iod_name, ATIME_NAME, strlen(ATIME_NAME));
	i++;

	/** Set Akey for MTIME */
	daos_iov_set(&sg_iovs[i], &entry->mtime, sizeof(time_t));
	daos_iov_set(&iods[i].iod_name, MTIME_NAME, strlen(MTIME_NAME));
	i++;

	/** Set Akey for CTIME */
	daos_iov_set(&sg_iovs[i], &entry->ctime, sizeof(time_t));
	daos_iov_set(&iods[i].iod_name, CTIME_NAME, strlen(CTIME_NAME));
	i++;

	if (fetch_sym) {
		/** Set Akey for Symlink Value, will be empty if no symlink */
		daos_iov_set(&sg_iovs[i], value, DFS_MAX_PATH);
		daos_iov_set(&iods[i].iod_name, SYML_NAME, strlen(SYML_NAME));
		i++;
	}

	akeys_nr = i;

	for (i = 0; i < akeys_nr; i++) {
		sgls[i].sg_nr		= 1;
		sgls[i].sg_nr_out	= 0;
		sgls[i].sg_iovs		= &sg_iovs[i];

		daos_csum_set(&iods[i].iod_kcsum, NULL, 0);
		iods[i].iod_nr		= 1;
		iods[i].iod_size	= DAOS_REC_ANY;
		iods[i].iod_recxs	= NULL;
		iods[i].iod_eprs	= NULL;
		iods[i].iod_csums	= NULL;
		iods[i].iod_type	= DAOS_IOD_SINGLE;
	}

	rc = daos_obj_fetch(oh, epoch, &dkey, akeys_nr, iods, sgls, NULL, NULL);
	if (rc) {
		D_ERROR("Failed to fetch entry %s (%d)\n", name, rc);
		return rc;
	}

	if (fetch_sym && S_ISLNK(entry->mode)) {
		size_t sym_len = iods[INODE_AKEYS-1].iod_size;

		if (sym_len != 0) {
			entry->value = strdup(value);
			if (entry->value == NULL)
				return -DER_NOMEM;
		}
	}

	if (iods[0].iod_size == 0) {
		D_DEBUG(DB_TRACE, "Entry %s does not exist\n", name);
		*exists = false;
	} else
		*exists = true;

	return rc;
}

static int
remove_entry(dfs_t *dfs, daos_handle_t parent_oh, const char *name,
	     struct dfs_entry entry)
{
	daos_key_t	dkey;
	int		rc;

	if (!S_ISLNK(entry.mode)) {
		daos_handle_t oh;

		rc = daos_obj_open(dfs->coh, entry.oid, dfs->epoch, DAOS_OO_RW,
				   &oh, NULL);
		if (rc)
			return rc;

		rc = daos_obj_punch(oh, dfs->epoch, NULL);
		if (rc) {
			daos_obj_close(oh, NULL);
			return rc;
		}

		rc = daos_obj_close(oh, NULL);
		if (rc)
			return rc;
	}

	daos_iov_set(&dkey, (void *)name, strlen(name));
	return daos_obj_punch_dkeys(parent_oh, dfs->epoch, 1, &dkey, NULL);
}

static int
insert_entry(daos_handle_t oh, daos_epoch_t epoch, const char *name,
	     struct dfs_entry entry)
{
	daos_sg_list_t	sgls[INODE_AKEYS];
	daos_iov_t	sg_iovs[INODE_AKEYS];
	daos_iod_t	iods[INODE_AKEYS];
	daos_key_t	dkey;
	unsigned int	akeys_nr, i;
	int		rc;

	daos_iov_set(&dkey, (void *)name, strlen(name));

	i = 0;

	/** Add the mode */
	daos_iov_set(&sg_iovs[i], &entry.mode, sizeof(mode_t));
	daos_iov_set(&iods[i].iod_name, MODE_NAME, strlen(MODE_NAME));
	iods[i].iod_size = sizeof(mode_t);
	i++;

	/** Add the Obj ID; for sym link this would be bogus */
	daos_iov_set(&sg_iovs[i], &entry.oid, sizeof(daos_obj_id_t));
	daos_iov_set(&iods[i].iod_name, OID_NAME, strlen(OID_NAME));
	iods[i].iod_size = sizeof(daos_obj_id_t);
	i++;

	/** Add the access time */
	daos_iov_set(&sg_iovs[i], &entry.atime, sizeof(time_t));
	daos_iov_set(&iods[i].iod_name, ATIME_NAME, strlen(ATIME_NAME));
	iods[i].iod_size = sizeof(time_t);
	i++;

	/** Add the modify time */
	daos_iov_set(&sg_iovs[i], &entry.mtime, sizeof(time_t));
	daos_iov_set(&iods[i].iod_name, MTIME_NAME, strlen(MTIME_NAME));
	iods[i].iod_size = sizeof(time_t);
	i++;

	/** Add the change time */
	daos_iov_set(&sg_iovs[i], &entry.ctime, sizeof(time_t));
	daos_iov_set(&iods[i].iod_name, CTIME_NAME, strlen(CTIME_NAME));
	iods[i].iod_size = sizeof(time_t);
	i++;

	/** Add the symbolic link value if entry is a symlink */
	if (S_ISLNK(entry.mode)) {
		daos_iov_set(&sg_iovs[i], entry.value, strlen(entry.value) + 1);
		daos_iov_set(&iods[i].iod_name, SYML_NAME, strlen(SYML_NAME));
		iods[i].iod_size = strlen(entry.value) + 1;
		i++;
	}

	akeys_nr = i;

	for (i = 0; i < akeys_nr; i++) {
		sgls[i].sg_nr		= 1;
		sgls[i].sg_nr_out	= 0;
		sgls[i].sg_iovs		= &sg_iovs[i];

		daos_csum_set(&iods[i].iod_kcsum, NULL, 0);
		iods[i].iod_nr		= 1;
		iods[i].iod_recxs	= NULL;
		iods[i].iod_eprs	= NULL;
		iods[i].iod_csums	= NULL;
		iods[i].iod_type	= DAOS_IOD_SINGLE;
	}

	rc = daos_obj_update(oh, epoch, &dkey, akeys_nr, iods, sgls, NULL);
	if (rc) {
		D_ERROR("Failed to insert entry %s (%d)\n", name, rc);
		return rc;
	}

	D_DEBUG(DB_TRACE, "inserted entry %s\n", name);
	return rc;
}

static int
get_nlinks(daos_handle_t oh, daos_epoch_t epoch, uint32_t *nlinks,
	   bool check_empty)
{
	daos_key_desc_t	kds[ENUM_DESC_NR];
	daos_hash_out_t anchor = {0};
	uint32_t	key_nr = 0;
	daos_sg_list_t	sgl;
	daos_iov_t	iov;
	char		enum_buf[ENUM_DESC_BUF] = {0};
	int		rc;

	sgl.sg_nr = 1;
	sgl.sg_nr_out = 0;
	daos_iov_set(&iov, enum_buf, ENUM_DESC_BUF);
	sgl.sg_iovs = &iov;

	/** TODO - Enum of links is expensive. Need to make this faster */
	while (!daos_hash_is_eof(&anchor)) {
		uint32_t number = ENUM_DESC_NR;

		rc = daos_obj_list_dkey(oh, epoch, &number, kds, &sgl, &anchor,
					NULL);
		if (rc)
			return rc;

		if (number == 0)
			continue;

		key_nr += number;

		/** if we just want to check if entries exist, break now */
		if (check_empty)
			break;
	}

	*nlinks = key_nr;
	return rc;
}

static int
entry_stat(dfs_t *dfs, daos_handle_t oh, const char *name, struct stat *stbuf)
{
	struct dfs_entry	entry = {0};
	bool			exists;
	daos_size_t		size;
	uint32_t		nlinks;
	int			rc;

	memset(stbuf, 0, sizeof(struct stat));

	/* Check if parent has the entry */
	rc = fetch_entry(oh, dfs->epoch, name, true, &exists, &entry);
	if (rc)
		return rc;

	if (!exists)
		return -DER_NONEXIST;

	switch (entry.mode & S_IFMT) {
	case S_IFDIR:
	{
		daos_handle_t oh;

		size = sizeof(entry);
		rc = daos_obj_open(dfs->coh, entry.oid, dfs->epoch, DAOS_OO_RO,
				   &oh, NULL);
		if (rc)
			return rc;

		/*
		 * TODO - This makes stat very slow now. Need to figure out a
		 * different way to get/maintain nlinks.
		 */
		rc = get_nlinks(oh, dfs->epoch, &nlinks, false);
		if (rc) {
			daos_obj_close(oh, NULL);
			return rc;
		}

		rc = daos_obj_close(oh, NULL);
		if (rc)
			return rc;
		break;
	}
	case S_IFREG:
	{
		daos_handle_t	oh;
		daos_size_t	elem_size, dkey_size;

		rc = daos_array_open(dfs->coh, entry.oid, dfs->epoch,
				     DAOS_OO_RO, &elem_size, &dkey_size,
				     &oh, NULL);
		if (rc) {
			D_ERROR("daos_array_open() failed (%d)\n", rc);
			return rc;
		}
		if (elem_size != 1) {
			daos_array_close(oh, NULL);
			D_ERROR("Elem size is not 1 in a byte array (%zu)\n",
				 elem_size);
			return rc;
		}

		rc = daos_array_get_size(oh, dfs->epoch, &size, NULL);
		if (rc) {
			daos_array_close(oh, NULL);
			return rc;
		}

		rc = daos_array_close(oh, NULL);
		if (rc)
			return rc;

		nlinks = 1;

		/*
		 * TODO - this is not accurate since it does not account for
		 * sparse files or file metadata or xattributes.
		 */
		stbuf->st_blocks = (size + (1 << 9) - 1) >> 9;
		break;
	}
	case S_IFLNK:
		size = strlen(entry.value);
		free(entry.value);
		entry.value = NULL;
		nlinks = 1;
		break;
	default:
		D_ERROR("Invalid entry type (not a dir, file, symlink).\n");
		return -DER_INVAL;
	}

	stbuf->st_nlink = (nlink_t)nlinks;
	stbuf->st_size = size;
	stbuf->st_mode = entry.mode;
	stbuf->st_uid = dfs->uid;
	stbuf->st_gid = dfs->gid;
	stbuf->st_atim.tv_sec = entry.atime;
	stbuf->st_mtim.tv_sec = entry.mtime;
	stbuf->st_ctim.tv_sec = entry.ctime;

	return rc;
}

static int
open_file(dfs_t *dfs, dfs_obj_t *parent, int flags, dfs_obj_t *file)
{
	struct dfs_entry	entry = {0};
	bool			exists;
	daos_size_t		elem_size, dkey_size;
	int			daos_mode;
	int			rc;

	/* Check if parent has the filename entry */
	rc = fetch_entry(parent->oh, dfs->epoch, file->name, false,
			 &exists, &entry);
	if (rc)
		return rc;

	if (flags & O_CREAT) {
		/** Create a DAOS byte array for the file */
		if (exists && (flags & O_EXCL))
			return -DER_EXIST;

		if (exists && S_ISDIR(entry.mode)) {
			D_ERROR("can't overwrite dir %s with non-directory\n",
				file->name);
			return -DER_INVAL;
		}

		/** Bump the epoch for this op. */
		incr_epoch(dfs);

		if (exists) {
			/** Punch entry and object */
			rc = remove_entry(dfs, parent->oh, file->name, entry);
			if (rc)
				D_GOTO(err, rc);
			if (entry.value) {
				D_ASSERT(S_ISLNK(entry.mode));
				free(entry.value);
				entry.value = NULL;
			}
		}

		/** Get new OID for the file */
		rc = oid_gen(dfs, 0, &file->oid);
		if (rc != 0)
			D_GOTO(err, rc);
		oid_cp(&entry.oid, file->oid);

		/** Create array object for the file */
		rc = daos_array_create(dfs->coh, file->oid, dfs->epoch, 1,
				       STRIPE_SIZE, &file->oh, NULL);
		if (rc != 0) {
			D_ERROR("daos_array_create() failed (%d)\n", rc);
			D_GOTO(err, rc);
		}

		/** Create and insert entry in parent dir object. */
		entry.mode = file->mode;
		entry.atime = entry.mtime = entry.ctime = time(NULL);

		rc = insert_entry(parent->oh, dfs->epoch, file->name, entry);
		if (rc != 0) {
			/** Close and discard epoch if entry insert fails */
			daos_obj_close(file->oh, NULL);
			D_ERROR("Inserting file entry %s failed (%d)\n",
				file->name, rc);
			D_GOTO(err, rc);
		}

		return rc;
	}

	/** Open the byte array */
	if (!exists)
		return -DER_NONEXIST;

	if (!S_ISREG(entry.mode)) {
		if (entry.value) {
			D_ASSERT(S_ISLNK(entry.mode));
			free(entry.value);
			entry.value = NULL;
		}
		return -DER_INVAL;
	}

	if (flags & O_RDONLY)
		daos_mode = DAOS_OO_RO;
	else
		daos_mode = DAOS_OO_RW;

	rc = daos_array_open(dfs->coh, entry.oid, dfs->epoch, daos_mode,
			     &elem_size, &dkey_size, &file->oh, NULL);
	if (rc != 0) {
		D_ERROR("daos_array_open() failed (%d)\n", rc);
		return rc;
	}
	if (elem_size != 1) {
		daos_array_close(file->oh, NULL);
		D_ERROR("Elem size is not 1 in a byte array (%zu)\n",
			elem_size);
		return -DER_INVAL;
	}
	oid_cp(&file->oid, entry.oid);

	return rc;
err:
	daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
	return rc;
}

/*
 * create a dir object. If caller passes parent obj, we check for existence of
 * object first.
 */
static int
create_dir(dfs_t *dfs, daos_handle_t parent_oh, dfs_obj_t *dir)
{
	struct dfs_entry	entry;
	bool			exists;
	int			rc;

	if (!daos_handle_is_inval(parent_oh)) {
		/* Check if parent has the dirname entry */
		rc = fetch_entry(parent_oh, dfs->epoch, dir->name, false,
				 &exists, &entry);
		if (rc)
			return rc;

		if (exists)
			return -DER_EXIST;
	}

	rc = oid_gen(dfs, 0, &dir->oid);
	if (rc != 0)
		return rc;
	rc = daos_obj_open(dfs->coh, dir->oid, dfs->epoch, DAOS_OO_RW, &dir->oh,
			   NULL);
	if (rc) {
		D_ERROR("daos_obj_open() Failed (%d)\n", rc);
		return rc;
	}

	return rc;
}

static int
open_dir(dfs_t *dfs, daos_handle_t parent_oh, int flags, dfs_obj_t *dir)
{
	struct dfs_entry	entry;
	bool			exists;
	unsigned int		daos_mode;
	int			rc;

	if (flags & O_CREAT) {
		incr_epoch(dfs);

		rc = create_dir(dfs, parent_oh, dir);
		if (rc) {
			daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
			return rc;
		}

		entry.oid = dir->oid;
		entry.mode = dir->mode;
		entry.atime = entry.mtime = entry.ctime = time(NULL);

		rc = insert_entry(parent_oh, dfs->epoch, dir->name, entry);
		if (rc != 0) {
			daos_obj_close(dir->oh, NULL);
			daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
			D_ERROR("Inserting dir entry %s failed (%d)\n",
				dir->name, rc);
		}

		return rc;
	}

	if (flags & O_RDONLY)
		daos_mode = DAOS_OO_RO;
	else
		daos_mode = DAOS_OO_RW;

	/* Check if parent has the dirname entry */
	rc = fetch_entry(parent_oh, dfs->epoch, dir->name, false, &exists,
			 &entry);
	if (rc)
		return rc;

	if (!exists)
		return -DER_NONEXIST;

	if (!S_ISDIR(entry.mode))
		return -DER_NOTDIR;

	rc = daos_obj_open(dfs->coh, entry.oid, dfs->epoch, daos_mode, &dir->oh,
			   NULL);
	if (rc) {
		D_ERROR("daos_obj_open() Failed (%d)\n", rc);
		return rc;
	}
	dir->mode = entry.mode;
	oid_cp(&dir->oid, entry.oid);

	return rc;
}

static int
open_symlink(dfs_t *dfs, dfs_obj_t *parent, int flags, const char *value,
	     dfs_obj_t *sym)
{
	struct dfs_entry	entry;
	bool			exists;
	int			rc;

	/* Check if parent has the symlink entry */
	rc = fetch_entry(parent->oh, dfs->epoch, sym->name, false,
			 &exists, &entry);
	if (rc)
		return rc;

	if (flags & O_CREAT) {
		if (exists)
			return -DER_EXIST;

		incr_epoch(dfs);
		entry.value = (char *)value;
		entry.oid.hi = 0;
		entry.oid.lo = 0;
		entry.mode = sym->mode;
		entry.atime = entry.mtime = entry.ctime = time(NULL);

		rc = insert_entry(parent->oh, dfs->epoch, sym->name, entry);
		if (rc != 0) {
			daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
			D_ERROR("Inserting sym link  entry %s failed (%d)\n",
				sym->name, rc);
		}
		return rc;
	}

	D_ASSERT(0);
	if (!exists)
		return -DER_NONEXIST;
	return rc;
}

static int
check_sb(dfs_t *dfs, bool insert, bool *exists)
{
	daos_sg_list_t	sgl;
	daos_iov_t	sg_iov;
	daos_iod_t	iod;
	daos_key_t	dkey;
	uint64_t	sb_magic;
	int		rc;

	D_DEBUG(DB_TRACE, "Checking if SB exists...\n");

	daos_iov_set(&dkey, SB_DKEY, strlen(SB_DKEY));

	daos_iov_set(&sg_iov, &sb_magic, sizeof(uint64_t));
	sgl.sg_nr = 1;
	sgl.sg_nr_out = 0;
	sgl.sg_iovs = &sg_iov;

	daos_iov_set(&iod.iod_name, SB_AKEY, strlen(SB_AKEY));
	daos_csum_set(&iod.iod_kcsum, NULL, 0);
	iod.iod_nr	= 1;
	iod.iod_size	= DAOS_REC_ANY;
	iod.iod_recxs	= NULL;
	iod.iod_eprs	= NULL;
	iod.iod_csums	= NULL;
	iod.iod_type	= DAOS_IOD_SINGLE;

	rc = daos_obj_fetch(dfs->super_oh, dfs->epoch, &dkey, 1, &iod, &sgl,
			    NULL, NULL);
	if (rc) {
		D_ERROR("Failed to fetch SB info (%d)\n", rc);
		return rc;
	}

	/** check if SB info exists */
	if (iod.iod_size == 0) {
		D_DEBUG(DB_TRACE, "SB does not exist.\n");

		*exists = false;

		/** if insert option is set, then insert the SB */
		if (!insert)
			return 0;
		D_DEBUG(DB_TRACE, "Inserting SB magic...\n");
		iod.iod_size = sizeof(uint64_t);
		sb_magic = SB_MAGIC;
		rc = daos_obj_update(dfs->super_oh, dfs->epoch, &dkey, 1, &iod,
				     &sgl, NULL);
		if (rc) {
			D_ERROR("Failed to update SB info (%d)\n", rc);
			return rc;
		}
	} else {
		D_DEBUG(DB_TRACE, "SB exists.\n");
		if (sb_magic != SB_MAGIC) {
			D_ERROR("SB MAGIC verification failed\n");
			return -DER_INVAL;
		}

		*exists = true;
	}

	return rc;
}

int
dfs_mount(daos_handle_t poh, daos_handle_t coh, dfs_t **_dfs)
{
	dfs_t			*dfs;
	daos_pool_info_t	pool_info;
	struct dfs_entry	entry = {0};
	bool			sb_exists;
	int			rc;

	D_ALLOC_PTR(dfs);
	if (dfs == NULL)
		return -DER_NOMEM;

	dfs->poh = poh;
	dfs->coh = coh;
	dfs->epoch = 0;
	rc = D_MUTEX_INIT(&dfs->lock, NULL);
	if (rc != 0)
		return rc;

	rc = daos_pool_query(poh, NULL, &pool_info, NULL);
	if (rc) {
		D_ERROR("daos_pool_query() Failed (%d)\n", rc);
		D_GOTO(err_dfs, rc);
	}

	dfs->uid = pool_info.pi_uid;
	dfs->gid = pool_info.pi_gid;

	rc = daos_epoch_hold(coh, &dfs->epoch, NULL, NULL);
	if (rc) {
		D_ERROR("daos_epoch_hold() Failed (%d)\n", rc);
		D_GOTO(err_dfs, rc);
	}

	dfs->oid.hi = 0;
	dfs->oid.lo = 0;

	/** Create special object on container and insert root */
	dfs->super_oid.lo = 0;
	dfs->super_oid.hi = 0;
	daos_obj_id_generate(&dfs->super_oid, 0, DAOS_OC_REPL_MAX_RW);

	rc = daos_obj_open(dfs->coh, dfs->super_oid, dfs->epoch, DAOS_OO_RW,
			   &dfs->super_oh, NULL);
	if (rc) {
		D_ERROR("daos_obj_open() Failed (%d)\n", rc);
		D_GOTO(err_epoch, rc);
	}

	D_DEBUG(DB_TRACE, "DFS super object %"PRIu64".%"PRIu64"\n",
		dfs->super_oid.hi, dfs->super_oid.lo);

	/** Check if SB object exists already */
	rc = check_sb(dfs, true, &sb_exists);
	if (rc)
		D_GOTO(err_epoch, rc);

	/** Check if super object has the root entry */
	strcpy(dfs->root.name, "/");
	oid_cp(&dfs->root.parent_oid, dfs->super_oid);

	rc = open_dir(dfs, dfs->super_oh, O_RDWR, &dfs->root);
	if (rc == 0) {
		if (!sb_exists) {
			D_ERROR("SB does not exist, but root obj does.\n");
			D_GOTO(err_super, -DER_INVAL);
		}

		/** Allocate an OID for the namespace */
		rc = daos_cont_oid_alloc(dfs->coh, 1, &dfs->oid.lo, NULL);
		if (rc) {
			D_ERROR("daos_cont_oid_alloc() Failed (%d)\n", rc);
			D_GOTO(err_super, rc);
		}
		D_DEBUG(DB_TRACE, "Namespace exists. OID lo = %"PRIu64".\n",
			dfs->oid.lo);
	} else if (rc == -DER_NONEXIST) {
		if (sb_exists) {
			D_ERROR("SB exists, but root does not.\n");
			D_GOTO(err_super, -DER_INVAL);
		}

		/** Allocate three OIDs (should return 0 since new namespace) */
		rc = daos_cont_oid_alloc(dfs->coh, 3, &dfs->oid.lo, NULL);
		if (rc) {
			D_ERROR("daos_cont_oid_alloc() Failed (%d)\n", rc);
			D_GOTO(err_super, rc);
		}
		if (dfs->oid.lo != 0) {
			D_ERROR("Container OIDs allocated is not 0\n");
			D_GOTO(err_super, rc = -DER_INVAL);
		}
		dfs->oid.lo += 2;

		D_DEBUG(DB_TRACE, "New Namespace, creating root object..\n");
		dfs->root.mode = S_IFDIR | 0777;

		dfs->root.oid.lo = 1;
		dfs->root.oid.hi = 0;
		daos_obj_id_generate(&dfs->root.oid, 0, DAOS_OC_REPL_MAX_RW);

		rc = daos_obj_open(dfs->coh, dfs->root.oid, dfs->epoch,
				   DAOS_OO_RW, &dfs->root.oh, NULL);
		if (rc) {
			D_ERROR("Failed to create root dir (%d).", rc);
			D_GOTO(err_super, rc);
		}

		oid_cp(&entry.oid, dfs->root.oid);
		entry.mode = S_IFDIR | 0777;
		entry.atime = entry.mtime = entry.ctime = time(NULL);

		rc = insert_entry(dfs->super_oh, dfs->epoch, dfs->root.name,
				  entry);
		if (rc) {
			D_ERROR("Failed to insert root entry (%d).", rc);
			D_GOTO(err_root, rc);
		}

		D_DEBUG(DB_TRACE, "Created root object %"PRIu64".%"PRIu64"\n",
			dfs->root.oid.hi, dfs->root.oid.lo);
	} else {
		D_ERROR("Failed to create/open root object\n");
		D_GOTO(err_super, rc);
	}

	dfs->mounted = true;
	*_dfs = dfs;

	return rc;
err_root:
	daos_obj_punch(dfs->root.oh, dfs->epoch, NULL);
	daos_obj_close(dfs->root.oh, NULL);
err_super:
	daos_obj_punch(dfs->super_oh, dfs->epoch, NULL);
	daos_obj_close(dfs->super_oh, NULL);
err_epoch:
	daos_epoch_discard(coh, dfs->epoch, NULL, NULL);
err_dfs:
	D_FREE_PTR(dfs);
	return rc;
}

int
dfs_umount(dfs_t *dfs)
{
	int rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;

	D_MUTEX_LOCK(&dfs->lock);
	rc = daos_epoch_commit(dfs->coh, dfs->epoch, NULL, NULL);
	if (rc) {
		D_ERROR("daos_epoch_commit() Failed (%d)\n", rc);
		D_MUTEX_UNLOCK(&dfs->lock);
		return rc;
	}

	daos_obj_close(dfs->root.oh, NULL);
	daos_obj_close(dfs->super_oh, NULL);

	D_MUTEX_UNLOCK(&dfs->lock);
	D_MUTEX_DESTROY(&dfs->lock);
	D_FREE_PTR(dfs);
	return rc;
}

int
dfs_mkdir(dfs_t *dfs, dfs_obj_t *parent, const char *name, mode_t mode)
{
	dfs_obj_t		new_dir;
	struct dfs_entry	entry = {0};
	int			rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (parent == NULL || !S_ISDIR(parent->mode))
		return -DER_NOTDIR;
	if (name == NULL)
		return -DER_INVAL;

	incr_epoch(dfs);

	strcpy(new_dir.name, name);
	rc = create_dir(dfs, (parent ? parent->oh : DAOS_HDL_INVAL), &new_dir);
	if (rc) {
		daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
		return rc;
	}

	entry.oid = new_dir.oid;
	entry.mode = S_IFDIR | mode;
	entry.atime = entry.mtime = entry.ctime = time(NULL);

	rc = insert_entry(parent->oh, dfs->epoch, name, entry);
	if (rc != 0) {
		daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
		D_ERROR("Inserting dir entry %s failed (%d)\n", name, rc);
	}

	daos_obj_close(new_dir.oh, NULL);
	return rc;
}

static int
remove_dir_contents(dfs_t *dfs, struct dfs_entry entry)
{
	daos_handle_t	oh;
	daos_key_desc_t	kds[ENUM_DESC_NR];
	daos_hash_out_t anchor = {0};
	daos_iov_t	iov;
	char		enum_buf[ENUM_DESC_BUF] = {0};
	daos_sg_list_t	sgl;
	int		rc;

	D_ASSERT(S_ISDIR(entry.mode));

	rc = daos_obj_open(dfs->coh, entry.oid, dfs->epoch, DAOS_OO_RW, &oh,
			   NULL);
	if (rc)
		return rc;

	sgl.sg_nr = 1;
	sgl.sg_nr_out = 0;
	daos_iov_set(&iov, enum_buf, ENUM_DESC_BUF);
	sgl.sg_iovs = &iov;

	while (!daos_hash_is_eof(&anchor)) {
		uint32_t	number = ENUM_DESC_NR;
		uint32_t	i;
		char		*ptr;

		rc = daos_obj_list_dkey(oh, dfs->epoch, &number, kds, &sgl,
					&anchor, NULL);
		if (rc)
			D_GOTO(out, rc);

		if (number == 0)
			continue;

		for (ptr = enum_buf, i = 0; i < number; i++) {
			struct dfs_entry child_entry;
			char entry_name[DFS_MAX_PATH];
			bool exists;

			snprintf(entry_name, kds[i].kd_key_len + 1, "%s", ptr);
			D_DEBUG(DB_TRACE, "Removing Entry %s\n", entry_name);
			ptr += kds[i].kd_key_len;

			rc = fetch_entry(oh, dfs->epoch, entry_name, false,
					 &exists, &child_entry);
			if (rc)
				D_GOTO(out, rc);

			D_ASSERT(exists);

			if (S_ISDIR(child_entry.mode)) {
				rc = remove_dir_contents(dfs, child_entry);
				if (rc)
					D_GOTO(out, rc);
			}

			rc = remove_entry(dfs, oh, entry_name, child_entry);
			if (rc)
				D_GOTO(out, rc);
		}
	}

out:
	daos_obj_close(oh, NULL);
	return rc;
}

int
dfs_remove(dfs_t *dfs, dfs_obj_t *parent, const char *name, bool force)
{
	struct dfs_entry	entry = {0};
	bool			exists;
	int			rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (parent == NULL || !S_ISDIR(parent->mode))
		return -DER_NOTDIR;
	if (name == NULL)
		return -DER_INVAL;

	D_DEBUG(DB_TRACE, "Remove entry %s from %s\n", name, parent->name);

	rc = fetch_entry(parent->oh, dfs->epoch, name, false, &exists, &entry);
	if (rc)
		return rc;

	if (!exists)
		return -DER_NONEXIST;

	incr_epoch(dfs);

	if (S_ISDIR(entry.mode)) {
		uint32_t nlinks = 0;
		daos_handle_t oh;

		/** check if dir is empty */
		rc = daos_obj_open(dfs->coh, entry.oid, dfs->epoch, DAOS_OO_RW,
				   &oh, NULL);
		if (rc) {
			D_ERROR("daos_obj_open() Failed (%d)\n", rc);
			return rc;
		}

		rc = get_nlinks(oh, dfs->epoch, &nlinks, true);
		if (rc) {
			daos_obj_close(oh, NULL);
			return rc;
		}

		rc = daos_obj_close(oh, NULL);
		if (rc)
			return rc;

		if (!force && nlinks != 0) {
			D_ERROR("dir is not empty\n");
			return -DER_INVAL;
		}

		if (force && nlinks != 0) {
			rc = remove_dir_contents(dfs, entry);
			if (rc)
				D_GOTO(err, rc);
		}
	}

	rc = remove_entry(dfs, parent->oh, name, entry);
	if (rc)
		D_GOTO(err, rc);

	return rc;
err:
	daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
	return rc;
}

int
dfs_lookup(dfs_t *dfs, const char *path, int flags, dfs_obj_t **_obj,
	   mode_t *mode)
{
	dfs_obj_t		parent;
	dfs_obj_t		*obj = NULL;
	struct dfs_entry	entry = {0};
	char			*token;
	char			*rem, *sptr;
	bool			exists;
	unsigned int		daos_mode;
	int			rc = 0;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (_obj == NULL)
		return -DER_INVAL;
	if (path == NULL)
		return -DER_INVAL;

	if (flags & O_RDONLY)
		daos_mode = DAOS_OO_RO;
	else
		daos_mode = DAOS_OO_RW;

	rem = strdup(path);
	if (rem == NULL)
		return -DER_NOMEM;

	D_ALLOC_PTR(obj);
	if (obj == NULL)
		D_GOTO(out, rc = -DER_NOMEM);

	oid_cp(&obj->oid, dfs->root.oid);
	oid_cp(&obj->parent_oid, dfs->root.parent_oid);
	obj->mode = dfs->root.mode;
	strcpy(obj->name, dfs->root.name);
	rc = daos_obj_open(dfs->coh, obj->oid, dfs->epoch,
			   daos_mode, &obj->oh, NULL);
	if (rc)
		D_GOTO(err_obj, rc);

	parent.oh = obj->oh;
	oid_cp(&parent.oid, obj->oid);
	oid_cp(&parent.parent_oid, obj->parent_oid);

	/** get the obj entry in the path */
	for (token = strtok_r(rem, "/", &sptr);
	     token != NULL;
	     token = strtok_r(NULL, "/", &sptr)) {
dfs_lookup_loop:
		D_DEBUG(DB_TRACE, "looking up %s in %"PRIu64".%"PRIu64"\n",
			token, parent.oid.hi, parent.oid.lo);
		rc = fetch_entry(parent.oh, dfs->epoch, token, true,
				 &exists, &entry);
		if (rc)
			D_GOTO(err_obj, rc);

		rc = daos_obj_close(obj->oh, NULL);
		if (rc) {
			D_ERROR("daos_obj_close() Failed (%d)\n", rc);
			D_GOTO(err_obj, rc);
		}

		if (!exists) {
			D_ERROR("Dir/file %s does not exist\n", token);
			D_GOTO(err_obj, rc = -DER_NONEXIST);
		}

		oid_cp(&obj->oid, entry.oid);
		oid_cp(&obj->parent_oid, parent.oid);
		strcpy(obj->name, token);

		/** if entry is a file, open the array object and return */
		if (S_ISREG(entry.mode)) {
			daos_size_t elem_size, dkey_size;

			/* if there are more entries, then file is not a dir */
			if (strtok_r(NULL, "/", &sptr) != NULL) {
				D_ERROR("%s is not a directory\n", obj->name);
				D_GOTO(err_obj, rc = -ENOENT);
			}

			obj->mode = entry.mode;
			rc = daos_array_open(dfs->coh, entry.oid, dfs->epoch,
					     daos_mode, &elem_size, &dkey_size,
					     &obj->oh, NULL);
			if (rc != 0) {
				D_ERROR("daos_array_open() failed (%d)\n", rc);
				D_GOTO(err_obj, rc);
			}
			if (elem_size != 1) {
				D_ERROR("Invalid Byte array elem size (%zu)\n",
					elem_size);
				daos_array_close(obj->oh, NULL);
				D_GOTO(err_obj, rc);
			}

			break;
		}

		if (S_ISLNK(entry.mode)) {
			obj->mode = entry.mode;
			obj->value = entry.value;

			/*
			 * If there is a token after the sym link entry, treat
			 * the sym link as a directory and look up it's value.
			 */
			token = strtok_r(NULL, "/", &sptr);
			if (token) {
				dfs_obj_t *sym;

				rc = dfs_lookup(dfs, obj->value, flags, &sym,
						NULL);
				if (rc) {
					D_ERROR("Invalid Symlink dir %s\n",
						obj->value);
					D_FREE_PTR(sym);
					D_GOTO(err_obj, rc);
				}

				parent.oh = sym->oh;
				D_FREE_PTR(sym);
				free(entry.value);
				entry.value = NULL;
				obj->value = NULL;
				/*
				 * need to go to to the beginning of loop but we
				 * already did the strtok.
				 */
				goto dfs_lookup_loop;
			}

			/** return the symlink obj if this is the last entry */
			break;
		}

		rc = daos_obj_open(dfs->coh, entry.oid, dfs->epoch, daos_mode,
				   &obj->oh, NULL);
		if (rc) {
			D_ERROR("daos_obj_open() Failed (%d)\n", rc);
			D_GOTO(err_obj, rc);
		}

		oid_cp(&parent.oid, obj->oid);
		oid_cp(&parent.parent_oid, obj->parent_oid);
		parent.oh = obj->oh;
	}

	if (mode)
		*mode = obj->mode;
out:
	free(rem);
	*_obj = obj;
	return rc;
err_obj:
	D_FREE_PTR(obj);
	obj = NULL;
	goto out;
}

int
dfs_nlinks(dfs_t *dfs, dfs_obj_t *obj, uint32_t *nlinks)
{
	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL || !S_ISDIR(obj->mode))
		return -DER_NOTDIR;
	if (nlinks == NULL)
		return -DER_INVAL;

	return get_nlinks(obj->oh, dfs->epoch, nlinks, false);
}

int
dfs_readdir(dfs_t *dfs, dfs_obj_t *obj, daos_hash_out_t *anchor, uint32_t *nr,
	    struct dirent *dirs)
{
	daos_key_desc_t *kds;
	char *enum_buf;
	uint32_t number, key_nr, i;
	daos_sg_list_t sgl;
	int rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL || !S_ISDIR(obj->mode))
		return -DER_NOTDIR;
	if (*nr == 0)
		return 0;
	if (dirs == NULL || anchor == NULL)
		return -DER_INVAL;

	D_ALLOC_ARRAY(kds, *nr);
	if (kds == NULL)
		return -DER_NOMEM;

	D_ALLOC_ARRAY(enum_buf, *nr * DFS_MAX_PATH);
	if (enum_buf == NULL) {
		D_FREE(kds);
		return -DER_NOMEM;
	}

	key_nr = 0;
	number = *nr;
	while (!daos_hash_is_eof(anchor)) {
		daos_iov_t iov;
		char *ptr;

		memset(enum_buf, 0, (*nr) * DFS_MAX_PATH);

		sgl.sg_nr = 1;
		sgl.sg_nr_out = 0;
		daos_iov_set(&iov, enum_buf, (*nr) * DFS_MAX_PATH);
		sgl.sg_iovs = &iov;

		rc = daos_obj_list_dkey(obj->oh, dfs->epoch, &number, kds, &sgl,
					anchor, NULL);
		if (rc)
			D_GOTO(out, rc);

		if (number == 0)
			continue; /* loop should break for EOF */

		for (ptr = enum_buf, i = 0; i < number; i++) {
			snprintf(dirs[key_nr].d_name, kds[i].kd_key_len + 1,
				 "%s", ptr);
			ptr += kds[i].kd_key_len;
			key_nr++;
		}
		number = *nr - key_nr;
		if (number == 0)
			break;
	}
	*nr = key_nr;

out:
	D_FREE(enum_buf);
	D_FREE(kds);
	return rc;
}

int
dfs_open(dfs_t *dfs, dfs_obj_t *parent, const char *name, mode_t mode,
	 int flags, const char *value, dfs_obj_t **_obj)
{
	dfs_obj_t *obj;
	int rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (parent == NULL || !S_ISDIR(parent->mode))
		return -DER_NOTDIR;
	if (name == NULL || _obj == NULL)
		return -DER_INVAL;
	if (S_ISLNK(mode) && value == NULL)
		return -DER_INVAL;

	D_DEBUG(DB_TRACE, "dfs_open: parent %s obj: %s\n", parent->name, name);

	D_ALLOC_PTR(obj);
	if (obj == NULL)
		return -DER_NOMEM;

	strcpy(obj->name, name);
	obj->mode = mode;
	oid_cp(&obj->parent_oid, parent->oid);

	switch (mode & S_IFMT) {
	case S_IFREG:
		rc = open_file(dfs, parent, flags, obj);
		if (rc) {
			D_ERROR("Failed to open file (%d)", rc);
			D_FREE_PTR(obj);
			return rc;
		}
		break;
	case S_IFDIR:
		rc = open_dir(dfs, parent->oh, flags, obj);
		if (rc) {
			D_ERROR("Failed to open directory (%d)", rc);
			D_FREE_PTR(obj);
			return rc;
		}
		break;
	case S_IFLNK:
		rc = open_symlink(dfs, parent, flags, value, obj);
		if (rc) {
			D_ERROR("Failed to open symlink (%d)", rc);
			D_FREE_PTR(obj);
			return rc;
		}
		break;
	default:
		D_ERROR("Invalid entry type (not a dir, file, symlink).\n");
		return -DER_INVAL;
	}

	*_obj = obj;
	return rc;
}

int
dfs_release(dfs_obj_t *obj)
{
	int rc = 0;

	if (obj == NULL)
		return -DER_INVAL;

	if (S_ISDIR(obj->mode))
		rc = daos_obj_close(obj->oh, NULL);
	else if (S_ISREG(obj->mode))
		rc = daos_array_close(obj->oh, NULL);
	else if (S_ISLNK(obj->mode))
		; /** nothing to do */
	else
		D_ASSERT(0);

	if (rc) {
		D_ERROR("daos_obj_close() Failed (%d)\n", rc);
		return rc;
	}

	D_FREE_PTR(obj);
	return 0;
}

static int
io_internal(dfs_t *dfs, dfs_obj_t *obj, daos_sg_list_t sgl, daos_off_t off,
	    int flag)
{
	daos_array_iod_t	ranges;
	daos_range_t		rg;
	daos_size_t		buf_size;
	int			i;
	int			rc;

	buf_size = 0;
	for (i = 0; i < sgl.sg_nr; i++)
		buf_size += sgl.sg_iovs[i].iov_len;

	/** set array location */
	ranges.arr_nr = 1;
	rg.rg_len = buf_size;
	rg.rg_idx = off;
	ranges.arr_rgs = &rg;

	D_DEBUG(DB_TRACE, "IO Epoch %"PRIu64" OP %d, Off %"PRIu64", Len %zu\n",
		dfs->epoch, flag, off, buf_size);

	if (flag == DFS_WRITE) {
		incr_epoch(dfs);
		rc = daos_array_write(obj->oh, dfs->epoch, &ranges, &sgl,
				      NULL, NULL);
		if (rc) {
			daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
			D_ERROR("daos_array_write() failed (%d)\n", rc);
		}
	} else if (flag == DFS_READ) {
		rc = daos_array_read(obj->oh, dfs->epoch, &ranges, &sgl, NULL,
				     NULL);
		if (rc)
			D_ERROR("daos_array_write() failed (%d)\n", rc);
	} else {
		rc = -DER_INVAL;
	}

	return rc;
}

int
dfs_read(dfs_t *dfs, dfs_obj_t *obj, daos_sg_list_t sgl, daos_off_t off,
	 daos_size_t *read_size)
{
	daos_size_t	array_size, max_read;
	daos_size_t	bytes_to_read, rem;
	int		i;
	int		rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL || !S_ISREG(obj->mode))
		return -DER_INVAL;

	rc = daos_array_get_size(obj->oh, dfs->epoch, &array_size, NULL);
	if (rc) {
		D_ERROR("daos_array_get_size() failed (%d)\n", rc);
		return rc;
	}

	if (off >= array_size) {
		*read_size = 0;
		return 0;
	}

	/* Update SGL in case we try to read beyond eof to not do that */
	max_read = array_size - off;
	bytes_to_read = 0;
	for (i = 0; i < sgl.sg_nr; i++) {
		if (bytes_to_read + sgl.sg_iovs[i].iov_len <= max_read) {
			bytes_to_read += sgl.sg_iovs[i].iov_len;
		} else {
			rem = max_read - bytes_to_read;
			if (rem) {
				bytes_to_read += rem;
				sgl.sg_iovs[i].iov_len = rem;
				i++;
				break;
			}
		}
	}
	sgl.sg_nr = i;

	rc = io_internal(dfs, obj, sgl, off, DFS_READ);
	if (rc) {
		D_ERROR("daos_array_read() failed (%d)\n", rc);
		return rc;
	}

	*read_size = bytes_to_read;
	return 0;
}

int
dfs_write(dfs_t *dfs, dfs_obj_t *obj, daos_sg_list_t sgl, daos_off_t off)
{
	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL || !S_ISREG(obj->mode))
		return -DER_INVAL;

	return io_internal(dfs, obj, sgl, off, DFS_WRITE);
}

int
dfs_stat(dfs_t *dfs, dfs_obj_t *parent, const char *name, struct stat *stbuf)
{
	daos_handle_t		oh;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (parent == NULL || !S_ISDIR(parent->mode))
		return -DER_NOTDIR;

	if (name == NULL) {
		if (strcmp(parent->name, "/") != 0) {
			D_ERROR("Invalid path %s and entry name %s)\n",
				parent->name, name);
			return -DER_INVAL;
		}
		name = parent->name;
		oh = dfs->super_oh;
	} else {
		oh = parent->oh;
	}

	return entry_stat(dfs, oh, name, stbuf);
}

int
dfs_ostat(dfs_t *dfs, dfs_obj_t *obj, struct stat *stbuf)
{
	daos_handle_t           oh;
	int			rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL)
		return -DER_INVAL;

	/* Open parent object and fetch entry of obj from it */
	rc = daos_obj_open(dfs->coh, obj->parent_oid, dfs->epoch, DAOS_OO_RO,
			   &oh, NULL);
	if (rc)
		return rc;

	rc = entry_stat(dfs, oh, obj->name, stbuf);
	if (rc)
		D_GOTO(out, rc);

out:
	daos_obj_close(oh, NULL);
	return rc;
}

int
dfs_get_size(dfs_t *dfs, dfs_obj_t *obj, daos_size_t *size)
{
	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL || !S_ISREG(obj->mode))
		return -DER_INVAL;

	return daos_array_get_size(obj->oh, dfs->epoch, size, NULL);
}

int
dfs_punch(dfs_t *dfs, dfs_obj_t *obj, daos_off_t offset, daos_size_t len)
{
	daos_size_t size;
	int rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL || !S_ISREG(obj->mode))
		return -DER_INVAL;

	incr_epoch(dfs);

	/** simple truncate */
	if (len == DFS_MAX_FSIZE) {
		rc = daos_array_set_size(obj->oh, dfs->epoch, offset, NULL);
		D_GOTO(out, rc);
	}

	rc = daos_array_get_size(obj->oh, dfs->epoch, &size, NULL);
	if (rc)
		return rc;

	/** nothing to do if offset is the same as the file size */
	if (size == offset)
		return 0;

	/** if file is smaller than the offset, extend the file */
	if (size < offset) {
		rc = daos_array_set_size(obj->oh, dfs->epoch, offset, NULL);
		D_GOTO(out, rc);
	}

	/** if fsize is between the range to punch, just truncate to offset */
	if (offset < size && size <= offset + len) {
		rc = daos_array_set_size(obj->oh, dfs->epoch, offset, NULL);
		D_GOTO(out, rc);
	}

	D_ASSERT(size > offset + len);

	/** Punch offset -> len */
	/** TODO - need to add support for that in array API */
	D_ASSERT(0);
	return -DER_INVAL;

out:
	if (rc)
		daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
	return rc;
}

int
dfs_get_mode(dfs_obj_t *obj, mode_t *mode)
{
	if (obj == NULL || mode == NULL)
		return -DER_INVAL;

	*mode = obj->mode;
	return 0;
}

int
dfs_get_symlink_value(dfs_obj_t *obj, char *buf, daos_size_t *size)
{
	daos_size_t val_size;

	if (obj == NULL || !S_ISLNK(obj->mode))
		return -DER_INVAL;

	val_size = strlen(obj->value);
	if (*size == 0 || buf == NULL) {
		*size = val_size;
		return 0;
	}

	if (*size < val_size)
		strncpy(buf, obj->value, *size);
	else
		strcpy(buf, obj->value);

	*size = val_size;
	return 0;
}

int
dfs_move(dfs_t *dfs, dfs_obj_t *parent, char *name, dfs_obj_t *new_parent,
	 char *new_name)
{
	struct dfs_entry	entry = {0}, new_entry = {0};
	bool			exists;
	daos_key_t		dkey;
	int			rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (parent == NULL || !S_ISDIR(parent->mode))
		return -DER_NOTDIR;
	if (new_parent == NULL || !S_ISDIR(new_parent->mode))
		return -DER_NOTDIR;
	if (name == NULL || new_name == NULL)
		return -DER_INVAL;

	rc = fetch_entry(parent->oh, dfs->epoch, name, true, &exists, &entry);
	if (rc) {
		D_ERROR("Failed to fetch entry %s (%d)\n", name, rc);
		return rc;
	}
	if (exists == false) {
		D_ERROR("Entry %s does not exist\n", name);
		return rc;
	}

	rc = fetch_entry(new_parent->oh, dfs->epoch, new_name, true, &exists,
			 &new_entry);
	if (rc) {
		D_ERROR("Failed to fetch entry %s (%d)\n", new_name, rc);
		return rc;
	}

	incr_epoch(dfs);

	if (exists) {
		if (S_ISDIR(new_entry.mode)) {
			uint32_t nlinks = 0;
			daos_handle_t oh;

			/** if old entry not a dir, return error */
			if (!S_ISDIR(entry.mode)) {
				D_ERROR("Can't rename non dir over a dir\n");
				D_GOTO(out, rc = -DER_INVAL);
			}

			/** make sure new dir is empty */
			rc = daos_obj_open(dfs->coh, new_entry.oid, dfs->epoch,
					   DAOS_OO_RW, &oh, NULL);
			if (rc) {
				D_ERROR("daos_obj_open() Failed (%d)\n", rc);
				D_GOTO(out, rc);
			}

			rc = get_nlinks(oh, dfs->epoch, &nlinks, true);
			if (rc) {
				D_ERROR("failed to check dir %s (%d)\n",
					new_name, rc);
				daos_obj_close(oh, NULL);
				D_GOTO(out, rc);
			}

			rc = daos_obj_close(oh, NULL);
			if (rc) {
				D_ERROR("daos_obj_close() Failed (%d)\n", rc);
				D_GOTO(out, rc);
			}

			if (nlinks != 0) {
				D_ERROR("target dir is not empty\n");
				D_GOTO(out, rc = -DER_INVAL);
			}
		}

		rc = remove_entry(dfs, new_parent->oh, new_name, new_entry);
		if (rc) {
			D_ERROR("Failed to remove entry %s (%d)\n",
				new_name, rc);
			D_GOTO(out, rc);
		}
	}

	entry.atime = entry.mtime = entry.ctime = time(NULL);
	/** insert old entry in new parent object */
	rc = insert_entry(new_parent->oh, dfs->epoch, new_name, entry);
	if (rc) {
		D_ERROR("Inserting entry %s failed (%d)\n", new_name, rc);
		D_GOTO(out, rc);
	}

	/** remove the old entry from the old parent (just the dkey) */
	daos_iov_set(&dkey, (void *)name, strlen(name));
	rc = daos_obj_punch_dkeys(parent->oh, dfs->epoch, 1, &dkey, NULL);
	if (rc) {
		D_ERROR("Punch entry %s failed (%d)\n", name, rc);
		D_GOTO(out, rc);
	}

out:
	if (entry.value) {
		D_ASSERT(S_ISLNK(entry.mode));
		free(entry.value);
		entry.value = NULL;
	}
	if (new_entry.value) {
		D_ASSERT(S_ISLNK(new_entry.mode));
		free(new_entry.value);
		new_entry.value = NULL;
	}
	if (rc)
		daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
	return rc;
}

int
dfs_exchange(dfs_t *dfs, dfs_obj_t *parent1, char *name1, dfs_obj_t *parent2,
	     char *name2)
{
	struct dfs_entry	entry1 = {0}, entry2 = {0};
	bool			exists;
	daos_key_t		dkey;
	int			rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (parent1 == NULL || !S_ISDIR(parent1->mode))
		return -DER_NOTDIR;
	if (parent2 == NULL || !S_ISDIR(parent2->mode))
		return -DER_NOTDIR;
	if (name1 == NULL || name2 == NULL)
		return -DER_INVAL;

	rc = fetch_entry(parent1->oh, dfs->epoch, name1, true,
			 &exists, &entry1);
	if (rc) {
		D_ERROR("Failed to fetch entry %s (%d)\n", name1, rc);
		return rc;
	}
	if (exists == false) {
		D_ERROR("Entry %s does not exist\n", name1);
		return rc;
	}

	rc = fetch_entry(parent2->oh, dfs->epoch, name2, true,
			 &exists, &entry2);
	if (rc) {
		D_ERROR("Failed to fetch entry %s (%d)\n", name2, rc);
		D_GOTO(out, rc);
	}

	if (exists == false) {
		D_ERROR("Exchange: New Entry %s does not exist\n", name2);
		D_GOTO(out, rc = -DER_INVAL);
	}

	incr_epoch(dfs);

	/** remove the first entry from parent1 (just the dkey) */
	daos_iov_set(&dkey, (void *)name1, strlen(name1));
	rc = daos_obj_punch_dkeys(parent1->oh, dfs->epoch, 1, &dkey, NULL);
	if (rc) {
		D_ERROR("Punch entry %s failed (%d)\n", name1, rc);
		D_GOTO(out, rc);
	}

	/** remove the second entry from parent2 (just the dkey) */
	daos_iov_set(&dkey, (void *)name2, strlen(name2));
	rc = daos_obj_punch_dkeys(parent2->oh, dfs->epoch, 1, &dkey, NULL);
	if (rc) {
		D_ERROR("Punch entry %s failed (%d)\n", name2, rc);
		D_GOTO(out, rc);
	}

	entry1.atime = entry1.mtime = entry1.ctime = time(NULL);
	/** insert entry1 in parent2 object */
	rc = insert_entry(parent2->oh, dfs->epoch, name1, entry1);
	if (rc) {
		D_ERROR("Inserting entry %s failed (%d)\n", name1, rc);
		D_GOTO(out, rc);
	}

	entry2.atime = entry2.mtime = entry2.ctime = time(NULL);
	/** insert entry2 in parent1 object */
	rc = insert_entry(parent1->oh, dfs->epoch, name2, entry2);
	if (rc) {
		D_ERROR("Inserting entry %s failed (%d)\n", name2, rc);
		D_GOTO(out, rc);
	}

out:
	if (entry1.value) {
		D_ASSERT(S_ISLNK(entry1.mode));
		free(entry1.value);
		entry1.value = NULL;
	}
	if (entry2.value) {
		D_ASSERT(S_ISLNK(entry2.mode));
		free(entry2.value);
		entry2.value = NULL;
	}
	if (rc)
		daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
	return rc;
}

int
dfs_sync(dfs_t *dfs)
{
	int rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;

	D_MUTEX_LOCK(&dfs->lock);
	rc = daos_epoch_commit(dfs->coh, dfs->epoch, NULL, NULL);
	if (rc) {
		D_ERROR("daos_epoch_commit() Failed (%d)\n", rc);
		D_GOTO(out, rc);
	}

	rc = daos_epoch_hold(dfs->coh, &dfs->epoch, NULL, NULL);
	if (rc) {
		D_ERROR("daos_epoch_hold() Failed (%d)\n", rc);
		D_GOTO(out, rc);
	}

out:
	D_MUTEX_UNLOCK(&dfs->lock);
	return rc;
}
static char *
concat(const char *s1, const char *s2)
{
	char *result = NULL;

	result = malloc(strlen(s1)+strlen(s2)+1);
	if (result == NULL)
		return NULL;

	strcpy(result, s1);
	strcat(result, s2);

	return result;
}

int
dfs_setxattr(dfs_t *dfs, dfs_obj_t *obj, const char *name,
	     const void *value, daos_size_t size, int flags)
{
	char		*xname = NULL;
	daos_sg_list_t	sgl;
	daos_iov_t	sg_iov;
	daos_iod_t	iod;
	daos_key_t	dkey;
	daos_handle_t	oh;
	int		rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL)
		return -DER_INVAL;

	/** prefix name with x: to avoid collision with internal attrs */
	xname = concat("x:", name);
	if (xname == NULL)
		return -DER_NOMEM;

	/** Open parent object and insert xattr in the entry of the object */
	rc = daos_obj_open(dfs->coh, obj->parent_oid, dfs->epoch, DAOS_OO_RW,
			   &oh, NULL);
	if (rc)
		return rc;

	/** set dkey as the entry name */
	daos_iov_set(&dkey, (void *)obj->name, strlen(obj->name));

	/** set akey as the xattr name */
	daos_iov_set(&iod.iod_name, xname, strlen(xname));
	daos_csum_set(&iod.iod_kcsum, NULL, 0);
	iod.iod_nr	= 1;
	iod.iod_recxs	= NULL;
	iod.iod_eprs	= NULL;
	iod.iod_csums	= NULL;
	iod.iod_type	= DAOS_IOD_SINGLE;

	/** if not default flag, check for xattr existence */
	if (flags != 0) {
		bool exists;

		iod.iod_size	= DAOS_REC_ANY;
		rc = daos_obj_fetch(oh, dfs->epoch, &dkey, 1, &iod, NULL,
				    NULL, NULL);
		if (rc)
			D_GOTO(out, rc);

		if (iod.iod_size == 0)
			exists = false;
		else
			exists = true;

		if (flags == XATTR_CREATE && exists) {
			D_ERROR("Xattribute already exists (XATTR_CREATE)");
			D_GOTO(out, rc = -DER_EXIST);
		} else if (flags == XATTR_REPLACE && !exists) {
			D_ERROR("Xattribute does not exist (XATTR_REPLACE)");
			D_GOTO(out, rc = -DER_NONEXIST);
		}
	}

	incr_epoch(dfs);

	/** set sgl for update */
	daos_iov_set(&sg_iov, (void *)value, size);
	sgl.sg_nr	= 1;
	sgl.sg_nr_out	= 0;
	sgl.sg_iovs	= &sg_iov;

	iod.iod_size	= size;
	rc = daos_obj_update(oh, dfs->epoch, &dkey, 1, &iod, &sgl, NULL);
	if (rc) {
		D_ERROR("Failed to add extended attribute %s\n", name);
		daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
	}

out:
	if (xname)
		free(xname);
	daos_obj_close(oh, NULL);
	return rc;
}

int
dfs_getxattr(dfs_t *dfs, dfs_obj_t *obj, const char *name, void *value,
	     daos_size_t *size)
{
	char            *xname = NULL;
	daos_sg_list_t	sgl;
	daos_iov_t	sg_iov;
	daos_iod_t	iod;
	daos_key_t	dkey;
	daos_handle_t	oh;
	int		rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL)
		return -DER_INVAL;

	xname = concat("x:", name);
	if (xname == NULL)
		return -DER_NOMEM;

	/** Open parent object and insert xattr in the entry of the object */
	rc = daos_obj_open(dfs->coh, obj->parent_oid, dfs->epoch, DAOS_OO_RO,
			   &oh, NULL);
	if (rc)
		return rc;

	/** set dkey as the entry name */
	daos_iov_set(&dkey, (void *)obj->name, strlen(obj->name));

	/** set akey as the xattr name */
	daos_iov_set(&iod.iod_name, xname, strlen(xname));
	daos_csum_set(&iod.iod_kcsum, NULL, 0);
	iod.iod_nr	= 1;
	iod.iod_recxs	= NULL;
	iod.iod_eprs	= NULL;
	iod.iod_csums	= NULL;
	iod.iod_type	= DAOS_IOD_SINGLE;

	if (*size) {
		iod.iod_size	= *size;

		/** set sgl for fetch */
		daos_iov_set(&sg_iov, value, *size);
		sgl.sg_nr	= 1;
		sgl.sg_nr_out	= 0;
		sgl.sg_iovs	= &sg_iov;

		rc = daos_obj_fetch(oh, dfs->epoch, &dkey, 1, &iod, &sgl,
				    NULL, NULL);
	} else {
		iod.iod_size	= DAOS_REC_ANY;

		rc = daos_obj_fetch(oh, dfs->epoch, &dkey, 1, &iod, NULL,
				    NULL, NULL);
	}
	if (rc) {
		D_ERROR("Failed to fetch xattr %s (%d)\n", name, rc);
		D_GOTO(out, rc);
	}

	*size = iod.iod_size;
	if (iod.iod_size == 0)
		D_GOTO(out, rc = -DER_NONEXIST);

out:
	if (xname)
		free(xname);
	daos_obj_close(oh, NULL);
	return rc;
}

int
dfs_removexattr(dfs_t *dfs, dfs_obj_t *obj, const char *name)
{
	char            *xname = NULL;
	daos_iod_t	iod;
	daos_key_t	dkey;
	daos_handle_t	oh;
	int		rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL)
		return -DER_INVAL;

	xname = concat("x:", name);
	if (xname == NULL)
		return -DER_NOMEM;

	/** Open parent object and insert xattr in the entry of the object */
	rc = daos_obj_open(dfs->coh, obj->parent_oid, dfs->epoch, DAOS_OO_RW,
			   &oh, NULL);
	if (rc)
		return rc;

	/** set dkey as the entry name */
	daos_iov_set(&dkey, (void *)obj->name, strlen(obj->name));

	/** set akey as the xattr name */
	daos_iov_set(&iod.iod_name, xname, strlen(xname));
	daos_csum_set(&iod.iod_kcsum, NULL, 0);
	iod.iod_nr	= 1;
	iod.iod_recxs	= NULL;
	iod.iod_eprs	= NULL;
	iod.iod_csums	= NULL;
	iod.iod_type	= DAOS_IOD_SINGLE;
	iod.iod_size	= 0;

	incr_epoch(dfs);

	/** Punch the xattr */
	rc = daos_obj_update(oh, dfs->epoch, &dkey, 1, &iod, NULL, NULL);
	if (rc) {
		D_ERROR("Failed to punch extended attribute %s\n", name);
		daos_epoch_discard(dfs->coh, dfs->epoch, NULL, NULL);
	}

	if (xname)
		free(xname);
	daos_obj_close(oh, NULL);
	return rc;
}

int
dfs_listxattr(dfs_t *dfs, dfs_obj_t *obj, char *list, daos_size_t *size)
{
	daos_key_t	dkey;
	daos_handle_t	oh;
	daos_key_desc_t	kds[ENUM_DESC_NR];
	daos_hash_out_t anchor = {0};
	daos_size_t	list_size, ret_size;
	char		*ptr_list;
	int		rc;

	if (dfs == NULL || !dfs->mounted)
		return -DER_INVAL;
	if (obj == NULL)
		return -DER_INVAL;
	if (*size == 0)
		return 0;
	if (list == NULL)
		return -DER_INVAL;

	/** Open parent object and insert xattr in the entry of the object */
	rc = daos_obj_open(dfs->coh, obj->parent_oid, dfs->epoch, DAOS_OO_RW,
			   &oh, NULL);
	if (rc)
		return rc;

	/** set dkey as the entry name */
	daos_iov_set(&dkey, (void *)obj->name, strlen(obj->name));

	list_size = *size;
	ret_size = 0;
	ptr_list = list;

	while (!daos_hash_is_eof(&anchor)) {
		uint32_t	number = ENUM_DESC_NR;
		uint32_t	i;
		daos_iov_t	iov;
		char		enum_buf[ENUM_DESC_BUF] = {0};
		daos_sg_list_t	sgl;
		char		*ptr;

		sgl.sg_nr = 1;
		sgl.sg_nr_out = 0;
		daos_iov_set(&iov, enum_buf, ENUM_DESC_BUF);
		sgl.sg_iovs = &iov;

		rc = daos_obj_list_akey(oh, dfs->epoch, &dkey, &number, kds,
					&sgl, &anchor, NULL);
		if (rc)
			D_GOTO(out, rc);

		if (number == 0)
			continue;

		for (ptr = enum_buf, i = 0; i < number; i++) {
			if (strncmp("x:", ptr, 2) != 0) {
				ptr += kds[i].kd_key_len;
				continue;
			}

			if (list_size < kds[i].kd_key_len - 2)
				break;

			snprintf(ptr_list, kds[i].kd_key_len - 2 + 1, "%s",
				 ptr + 2);
			list_size -= kds[i].kd_key_len - 2;
			ret_size += kds[i].kd_key_len - 2;
			ptr_list += kds[i].kd_key_len - 2 + 1;
			ptr += kds[i].kd_key_len;
		}
	}

	*size = ret_size;
out:
	daos_obj_close(oh, NULL);
	return rc;
}
