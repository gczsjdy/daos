/**
 * (C) Copyright 2019 Intel Corporation.
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
/**
 * This file is part of daos. It implements some miscellaneous functions which
 * not belong to other parts.
 */
#define D_LOGFAC	DD_FAC(common)

#include <daos/common.h>
#include <daos/dtx.h>
#include <daos_security.h>
#include <daos/checksum.h>

daos_prop_t *
daos_prop_alloc(uint32_t entries_nr)
{
	daos_prop_t	*prop;

	if (entries_nr > DAOS_PROP_ENTRIES_MAX_NR) {
		D_ERROR("cannot create daos_prop_t with %d entries(> %d).\n",
			entries_nr, DAOS_PROP_ENTRIES_MAX_NR);
		return NULL;
	}

	D_ALLOC_PTR(prop);
	if (prop == NULL)
		return NULL;

	if (entries_nr > 0) {
		D_ALLOC_ARRAY(prop->dpp_entries, entries_nr);
		if (prop->dpp_entries == NULL) {
			D_FREE_PTR(prop);
			return NULL;
		}
	}
	prop->dpp_nr = entries_nr;
	return prop;
}

void
daos_prop_free(daos_prop_t *prop)
{
	int i;

	if (prop == NULL)
		return;
	if (prop->dpp_nr == 0 || prop->dpp_entries == NULL) {
		D_FREE_PTR(prop);
		return;
	}

	for (i = 0; i < prop->dpp_nr; i++) {
		struct daos_prop_entry *entry;

		entry = &prop->dpp_entries[i];
		switch (entry->dpe_type) {
		case DAOS_PROP_PO_LABEL:
		case DAOS_PROP_CO_LABEL:
		case DAOS_PROP_PO_OWNER:
		case DAOS_PROP_PO_OWNER_GROUP:
			if (entry->dpe_str)
				D_FREE(entry->dpe_str);
			break;
		case DAOS_PROP_PO_ACL:
		case DAOS_PROP_CO_ACL:
			if (entry->dpe_val_ptr)
				D_FREE(entry->dpe_val_ptr);
			break;
		default:
			break;
		};
	}

	D_FREE(prop->dpp_entries);
	D_FREE_PTR(prop);
}

static bool
daos_prop_str_valid(d_string_t str, const char *prop_name, size_t max_len)
{
	size_t len;

	if (str == NULL) {
		D_ERROR("invalid NULL %s\n", prop_name);
		return false;
	}
	/* Detect if it's longer than max_len */
	len = strnlen(str, max_len + 1);
	if (len == 0 || len > max_len) {
		D_ERROR("invalid %s len=%lu, max=%lu\n",
			prop_name, len, max_len);
		return false;
	}
	return true;
}

static bool
daos_prop_owner_valid(d_string_t owner)
{
	/* Max length passed in doesn't include the null terminator */
	return daos_prop_str_valid(owner, "owner",
				   DAOS_ACL_MAX_PRINCIPAL_LEN);
}

static bool
daos_prop_owner_group_valid(d_string_t owner)
{
	/* Max length passed in doesn't include the null terminator */
	return daos_prop_str_valid(owner, "owner-group",
				   DAOS_ACL_MAX_PRINCIPAL_LEN);
}

static bool
daos_prop_label_valid(d_string_t label)
{
	return daos_prop_str_valid(label, "label", DAOS_PROP_LABEL_MAX_LEN);
}

/**
 * Check if the input daos_prop_t parameter is valid
 * \a pool true for pool properties, false for container properties.
 * \a input true for input properties that should with reasonable value,
 *          false for output that need not check the value.
 */
bool
daos_prop_valid(daos_prop_t *prop, bool pool, bool input)
{
	uint32_t	type;
	uint64_t	val;
	struct daos_acl	*acl_ptr;
	int		i;

	if (prop == NULL) {
		D_ERROR("NULL properties\n");
		return false;
	}
	if (prop->dpp_nr > DAOS_PROP_ENTRIES_MAX_NR) {
		D_ERROR("invalid ddp_nr %d (> %d).\n",
			prop->dpp_nr, DAOS_PROP_ENTRIES_MAX_NR);
		return false;
	}
	if (prop->dpp_nr == 0) {
		if (prop->dpp_entries != NULL)
			D_ERROR("invalid properties, NON-NULL dpp_entries with "
				"zero dpp_nr.\n");
		return prop->dpp_entries == NULL;
	}
	if (prop->dpp_entries == NULL) {
		D_ERROR("invalid properties, NULL dpp_entries with non-zero "
			"dpp_nr.\n");
		return false;
	}
	for (i = 0; i < prop->dpp_nr; i++) {
		type = prop->dpp_entries[i].dpe_type;
		if (pool) {
			if (type <= DAOS_PROP_PO_MIN ||
			    type >= DAOS_PROP_PO_MAX) {
				D_ERROR("invalid type %d for pool.\n", type);
				return false;
			}
		} else {
			if (type <= DAOS_PROP_CO_MIN ||
			    type >= DAOS_PROP_CO_MAX) {
				D_ERROR("invalid type %d for container.\n",
					type);
				return false;
			}
		}
		/* for output parameter need not check entry value */
		if (!input)
			continue;
		switch (type) {
		/* pool properties */
		case DAOS_PROP_PO_LABEL:
			if (!daos_prop_label_valid(
				prop->dpp_entries[i].dpe_str))
				return false;
			break;
		case DAOS_PROP_PO_ACL:
			acl_ptr = prop->dpp_entries[i].dpe_val_ptr;
			if (daos_acl_validate(acl_ptr) != 0)
				return false;
			break;
		case DAOS_PROP_CO_ACL:
			/* TODO: Implement container ACL */
			break;
		case DAOS_PROP_PO_SPACE_RB:
			val = prop->dpp_entries[i].dpe_val;
			if (val > 100) {
				D_ERROR("invalid space_rb "DF_U64".\n", val);
				return false;
			}
			break;
		case DAOS_PROP_PO_SELF_HEAL:
			break;
		case DAOS_PROP_PO_RECLAIM:
			val = prop->dpp_entries[i].dpe_val;
			if (val != DAOS_RECLAIM_SNAPSHOT &&
			    val != DAOS_RECLAIM_BATCH &&
			    val != DAOS_RECLAIM_TIME) {
				D_ERROR("invalid reclaim "DF_U64".\n", val);
				return false;
			}
			break;
		case DAOS_PROP_PO_OWNER:
			if (!daos_prop_owner_valid(
				prop->dpp_entries[i].dpe_str))
				return false;
			break;
		case DAOS_PROP_PO_OWNER_GROUP:
			if (!daos_prop_owner_group_valid(
				prop->dpp_entries[i].dpe_str))
				return false;
			break;
		/* container properties */
		case DAOS_PROP_CO_LABEL:
			if (!daos_prop_label_valid(
				prop->dpp_entries[i].dpe_str))
				return false;
			break;
		case DAOS_PROP_CO_LAYOUT_TYPE:
			val = prop->dpp_entries[i].dpe_val;
			if (val != DAOS_PROP_CO_LAYOUT_UNKOWN &&
			    val != DAOS_PROP_CO_LAYOUT_POSIX &&
			    val != DAOS_PROP_CO_LAYOUT_HDF5) {
				D_ERROR("invalid layout type "DF_U64".\n", val);
				return false;
			}
			break;
		case DAOS_PROP_CO_LAYOUT_VER:
			break;
		case DAOS_PROP_CO_CSUM:
			val = prop->dpp_entries[i].dpe_val;
			if (!daos_cont_csum_prop_is_valid(val)) {
				D_ERROR("invalid checksum type "DF_U64".\n",
					val);
				return false;
			}
			break;
		case DAOS_PROP_CO_CSUM_CHUNK_SIZE:
			/** Accepting anything right now */
			break;
		case DAOS_PROP_CO_CSUM_SERVER_VERIFY:
			val = prop->dpp_entries[i].dpe_val;
			if (val != DAOS_PROP_CO_CSUM_SV_OFF &&
			    val != DAOS_PROP_CO_CSUM_SV_ON) {
				D_ERROR("invalid csum Server Verify Property"
						DF_U64".\n", val);
				return false;
			}
			break;
		case DAOS_PROP_CO_REDUN_FAC:
			val = prop->dpp_entries[i].dpe_val;
			if (val != DAOS_PROP_CO_REDUN_RF1 &&
			    val != DAOS_PROP_CO_REDUN_RF3) {
				D_ERROR("invalid redundancy factor "DF_U64".\n",
					val);
				return false;
			}
			break;
		case DAOS_PROP_CO_REDUN_LVL:
			val = prop->dpp_entries[i].dpe_val;
			if (val != DAOS_PROP_CO_REDUN_RACK &&
			    val != DAOS_PROP_CO_REDUN_NODE) {
				D_ERROR("invalid redundancy level "DF_U64".\n",
					val);
				return false;
			}
			break;
		case DAOS_PROP_CO_SNAPSHOT_MAX:
		case DAOS_PROP_CO_COMPRESS:
		case DAOS_PROP_CO_ENCRYPT:
			break;
		default:
			D_ERROR("invaid dpe_type %d.\n", type);
			return false;
		}
	}
	return true;
}

/**
 * duplicate the properties
 * \a pool true for pool properties, false for container properties.
 */
daos_prop_t *
daos_prop_dup(daos_prop_t *prop, bool pool)
{
	daos_prop_t		*prop_dup;
	struct daos_prop_entry	*entry, *entry_dup;
	int			 i;
	struct daos_acl		*acl_ptr;

	if (!daos_prop_valid(prop, pool, true))
		return NULL;

	prop_dup = daos_prop_alloc(prop->dpp_nr);
	if (prop_dup == NULL)
		return NULL;

	for (i = 0; i < prop->dpp_nr; i++) {
		entry = &prop->dpp_entries[i];
		entry_dup = &prop_dup->dpp_entries[i];
		entry_dup->dpe_type = entry->dpe_type;
		switch (entry->dpe_type) {
		case DAOS_PROP_PO_LABEL:
		case DAOS_PROP_CO_LABEL:
			entry_dup->dpe_str = strndup(entry->dpe_str,
						     DAOS_PROP_LABEL_MAX_LEN);
			if (entry_dup->dpe_str == NULL) {
				D_ERROR("failed to dup label.\n");
				daos_prop_free(prop_dup);
				return NULL;
			}
			break;
		case DAOS_PROP_PO_ACL:
			acl_ptr = entry->dpe_val_ptr;
			entry_dup->dpe_val_ptr = daos_acl_dup(acl_ptr);
			if (entry_dup->dpe_val_ptr == NULL) {
				D_ERROR("failed to dup ACL\n");
				daos_prop_free(prop_dup);
				return NULL;
			}
			break;
		case DAOS_PROP_CO_ACL:
			/* TODO: Implement container ACL */
			break;
		case DAOS_PROP_PO_OWNER:
		case DAOS_PROP_PO_OWNER_GROUP:
			D_STRNDUP(entry_dup->dpe_str, entry->dpe_str,
				  DAOS_ACL_MAX_PRINCIPAL_LEN);
			if (entry_dup->dpe_str == NULL) {
				D_ERROR("failed to dup ownership info.\n");
				daos_prop_free(prop_dup);
				return NULL;
			}
			break;
		default:
			entry_dup->dpe_val = entry->dpe_val;
			break;
		}
	}

	return prop_dup;
}

/**
 * Get the property entry of \a type in \a prop
 * return NULL if not found.
 */
struct daos_prop_entry *
daos_prop_entry_get(daos_prop_t *prop, uint32_t type)
{
	int i;

	if (prop == NULL || prop->dpp_nr == 0 || prop->dpp_entries == NULL)
		return NULL;
	for (i = 0; i < prop->dpp_nr; i++) {
		if (prop->dpp_entries[i].dpe_type == type)
			return &prop->dpp_entries[i];
	}
	return NULL;
}

/**
 * Copy properties from \a prop_reply to \a prop_req.
 * Used to copy the properties from pool query or container query to user's
 * properties. If user provided \a prop_req with zero dpp_nr (and NULL
 * dpp_entries), it will allocate needed buffer and assign to user's daos_prop_t
 * struct, the needed buffer to store label will be allocated as well.
 * User can free properties buffer by calling daos_prop_free().
 */
int
daos_prop_copy(daos_prop_t *prop_req, daos_prop_t *prop_reply)
{
	struct daos_prop_entry	*entry_req, *entry_reply;
	bool			 entries_alloc = false;
	bool			 label_alloc = false;
	bool			 acl_alloc = false;
	bool			 owner_alloc = false;
	bool			 group_alloc = false;
	struct daos_acl		*acl;
	uint32_t		 type;
	int			 i;
	int			 rc = 0;

	if (prop_reply == NULL || prop_reply->dpp_nr == 0 ||
	    prop_reply->dpp_entries == NULL) {
		D_ERROR("no prop or empty prop in reply.\n");
		return -DER_PROTO;
	}
	if (prop_req->dpp_nr == 0) {
		prop_req->dpp_nr = prop_reply->dpp_nr;
		D_ALLOC_ARRAY(prop_req->dpp_entries, prop_req->dpp_nr);
		if (prop_req->dpp_entries == NULL)
			return -DER_NOMEM;
		entries_alloc = true;
	}

	for (i = 0; i < prop_req->dpp_nr && i < prop_reply->dpp_nr; i++) {
		entry_req = &prop_req->dpp_entries[i];
		type = entry_req->dpe_type;
		if (type == 0) {
			/* req doesn't have any entry type populated yet */
			type = prop_reply->dpp_entries[i].dpe_type;
			entry_req->dpe_type = type;
		}
		entry_reply = daos_prop_entry_get(prop_reply, type);
		if (entry_reply == NULL) {
			D_ERROR("cannot find prop entry for type %d.\n", type);
			D_GOTO(out, rc = -DER_PROTO);
		}
		if (type == DAOS_PROP_PO_LABEL || type == DAOS_PROP_CO_LABEL) {
			entry_req->dpe_str = strndup(entry_reply->dpe_str,
						     DAOS_PROP_LABEL_MAX_LEN);
			if (entry_req->dpe_str == NULL)
				D_GOTO(out, rc = -DER_NOMEM);
			label_alloc = true;
		} else if (type == DAOS_PROP_PO_ACL) {
			acl = entry_reply->dpe_val_ptr;
			entry_req->dpe_val_ptr = daos_acl_dup(acl);
			if (entry_req->dpe_val_ptr == NULL)
				D_GOTO(out, rc = -DER_NOMEM);
			acl_alloc = true;
		} else if (type == DAOS_PROP_CO_ACL) {
			/* TODO: Implement container ACL */
		} else if (type == DAOS_PROP_PO_OWNER) {
			D_STRNDUP(entry_req->dpe_str,
				  entry_reply->dpe_str,
				  DAOS_ACL_MAX_PRINCIPAL_LEN);
			if (entry_req->dpe_str == NULL)
				D_GOTO(out, rc = -DER_NOMEM);
			owner_alloc = true;
		} else if (type == DAOS_PROP_PO_OWNER_GROUP) {
			D_STRNDUP(entry_req->dpe_str,
				  entry_reply->dpe_str,
				  DAOS_ACL_MAX_PRINCIPAL_LEN);
			if (entry_req->dpe_str == NULL)
				D_GOTO(out, rc = -DER_NOMEM);
			group_alloc = true;
		} else {
			entry_req->dpe_val = entry_reply->dpe_val;
		}
	}

out:
	if (rc) {
		if (label_alloc) {
			entry_req = daos_prop_entry_get(prop_req,
							DAOS_PROP_PO_LABEL);
			D_FREE(entry_req->dpe_str);
		}
		if (acl_alloc) {
			entry_req = daos_prop_entry_get(prop_req,
							DAOS_PROP_PO_ACL);
			D_FREE(entry_req->dpe_val_ptr);
		}
		if (owner_alloc) {
			entry_req = daos_prop_entry_get(prop_req,
							DAOS_PROP_PO_OWNER);
			D_FREE(entry_req->dpe_str);
		}
		if (group_alloc) {
			entry_req = daos_prop_entry_get(prop_req,
						DAOS_PROP_PO_OWNER_GROUP);
			D_FREE(entry_req->dpe_str);
		}
		if (entries_alloc) {
			D_FREE(prop_req->dpp_entries);
		}
	}
	return rc;
}
