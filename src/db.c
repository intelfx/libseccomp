/**
 * Enhanced Seccomp Filter DB
 *
 * Copyright (c) 2012 Red Hat <pmoore@redhat.com>
 * Author: Paul Moore <pmoore@redhat.com>
 */

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <seccomp.h>

#include "arch.h"
#include "db.h"

/* the priority field is fairly simple - without any user hints, or in the case
 * of a hint "tie", we give higher priority to syscalls with less chain nodes
 * (filter is easier to evaluate) */
#define _DB_PRI_MASK_CHAIN		0x0000FFFF
#define _DB_PRI_MASK_USER		0x00FF0000

static unsigned int _db_tree_free(struct db_arg_chain_tree *tree);

/**
 * Do not call this function directly, use _db_tree_free() instead
 */
static unsigned int __db_tree_free(struct db_arg_chain_tree *tree)
{
	int cnt;

	if (tree == NULL)
		return 0;

	/* we assume the caller has ensured that 'tree->lvl_prv == NULL' */
	cnt = __db_tree_free(tree->lvl_nxt);
	cnt += _db_tree_free(tree->nxt_t);
	cnt += _db_tree_free(tree->nxt_f);

	free(tree);
	return cnt + 1;
}

/**
 * Free a syscall filter argument chain tree
 * @param list the argument chain list
 *
 * This function frees a syscall argument chain list and returns the number of
 * nodes freed.
 *
 */
static unsigned int _db_tree_free(struct db_arg_chain_tree *tree)
{
	struct db_arg_chain_tree *iter;

	if (tree == NULL)
		return 0;

	iter = tree;
	while (iter->lvl_prv != NULL)
		iter = iter->lvl_prv;

	return __db_tree_free(iter);
}

/**
 * Remove a node from an argument chain tree
 * @param tree the pointer to the tree
 * @param node the node to remove
 * @param action the removed node's action
 *
 * This function searches the tree looking for the node and removes it once
 * found.  The function also removes any other nodes that are no longer needed
 * as a result of removing the given node.  Returns the number of nodes freed.
 *
 */
static unsigned int _db_tree_remove(struct db_arg_chain_tree **tree,
				    struct db_arg_chain_tree *node,
				    uint32_t action)
{
	int cnt = 0;
	struct db_arg_chain_tree *c_iter;

	if (tree == NULL || *tree == NULL || node == NULL)
		return 0;

	c_iter = *tree;
	while (c_iter->lvl_prv != NULL)
		c_iter = c_iter->lvl_prv;

	do {
		/* this is only an issue on the first level */
		if (c_iter == node) {
			/* remove from the tree */
			if (c_iter == *tree) {
				if (c_iter->lvl_prv != NULL)
					*tree = c_iter->lvl_prv;
				else
					*tree = c_iter->lvl_nxt;
			}
			if (c_iter->lvl_prv != NULL)
				c_iter->lvl_prv->lvl_nxt = c_iter->lvl_nxt;
			if (c_iter->lvl_nxt != NULL)
				c_iter->lvl_nxt->lvl_prv = c_iter->lvl_prv;

			/* free and return */
			c_iter->lvl_prv = NULL;
			c_iter->lvl_nxt = NULL;
			cnt += _db_tree_free(c_iter);
			return cnt;
		}

		/* check the true sub-tree */
		if (c_iter->nxt_t == node) {
			/* free and return */
			c_iter->act_t_flg = 1;
			c_iter->act_t = action;
			cnt += _db_tree_free(c_iter->nxt_t);
			c_iter->nxt_t = NULL;
			return cnt;
		} else
			cnt += _db_tree_remove(&(c_iter->nxt_t), node, action);

		/* check the false sub-tree */
		if (c_iter->nxt_f == node) {
			/* free and return */
			c_iter->act_f_flg = 1;
			c_iter->act_f = action;
			cnt += _db_tree_free(c_iter->nxt_f);
			c_iter->nxt_f = NULL;
			return cnt;
		} else
			cnt += _db_tree_remove(&(c_iter->nxt_f), node, action);

		c_iter = c_iter->lvl_nxt;
	} while (c_iter != NULL);

	return cnt;
}

/**
 * Traverse a tree checking the action values
 * @param tree the pointer to the tree
 * @param action the action
 *
 * Traverse the tree inspecting each action to see if it matches the given
 * action.  Returns zero if all actions match the given action, negative values
 * on failure.
 *
 */
static int _db_tree_act_check(struct db_arg_chain_tree *tree, uint32_t action)
{
	int rc;
	struct db_arg_chain_tree *c_iter;

	if (tree == NULL)
		return 0;

	c_iter = tree;
	while (c_iter->lvl_prv != NULL)
		c_iter = c_iter->lvl_prv;

	do {
		if (c_iter->act_t_flg && c_iter->act_t != action)
			return -EEXIST;
		if (c_iter->act_f_flg && c_iter->act_f != action)
			return -EEXIST;

		rc = _db_tree_act_check(c_iter->nxt_t, action);
		if (rc < 0)
			return rc;
		rc = _db_tree_act_check(c_iter->nxt_f, action);
		if (rc < 0)
			return rc;

		c_iter = c_iter->lvl_nxt;
	} while (c_iter != NULL);

	return 0;
}

/**
 * Intitalize a seccomp filter DB
 * @param arch the architecture definition
 * @param def_action the default filter action
 *
 * This function initializes a seccomp filter DB and readies it for use.
 * Returns a pointer to the DB on success, NULL on failure.
 *
 */
struct db_filter *db_new(const struct arch_def *arch, uint32_t def_action)
{
	struct db_filter *db;

	db = malloc(sizeof(*db));
	if (db) {
		memset(db, 0, sizeof(*db));
		db->arch = arch;
		db->def_action = def_action;
	}

	return db;
}

/**
 * Destroy a seccomp filter DB
 * @param db the seccomp filter DB
 *
 * This function destroys a seccomp filter DB.  After calling this function,
 * the filter should no longer be referenced.
 *
 */
void db_destroy(struct db_filter *db)
{
	struct db_sys_list *s_iter;

	if (db == NULL)
		return;

	s_iter = db->syscalls;
	while (s_iter != NULL) {
		db->syscalls = s_iter->next;
		_db_tree_free(s_iter->chains);
		free(s_iter);
		s_iter = db->syscalls;
	}
	free(db);
}

/**
 * Add a syscall filter with an optional argument chain
 * @param db the seccomp filter db
 * @param action the filter action
 * @param syscall the syscall number
 * @param chain argument filter chain
 *
 * This function adds a new syscall filter to the seccomp filter DB, adding to
 * the existing filters for the syscall, unless no argument specific filters
 * are present (filtering only on the syscall).  When adding new chains, the
 * shortest chain, or most inclusive filter match, will be entered into the
 * filter DB. Returns zero on success, negative values on failure.
 *
 */
int db_add_syscall(struct db_filter *db, uint32_t action, unsigned int syscall,
		   struct db_api_arg *chain)
{
	int rc = -ENOMEM;
	unsigned int iter;
	int chain_len_max;
	struct db_sys_list *s_new, *s_iter, *s_prev = NULL;
	struct db_arg_chain_tree *c_iter = NULL, *c_prev = NULL;
	struct db_arg_chain_tree *ec_iter;
	unsigned int tf_flag;
	unsigned int n_cnt;

	assert(db != NULL);

	/* do all our possible memory allocation up front so we don't have to
	 * worry about failure once we get to the point where we start updating
	 * the filter db */
	s_new = malloc(sizeof(*s_new));
	if (s_new == NULL)
		return -ENOMEM;
	memset(s_new, 0, sizeof(*s_new));
	s_new->num = syscall;
	/* run through the argument chain */
	chain_len_max = arch_arg_count_max(db->arch);
	for (iter = 0; iter < chain_len_max; iter++) {
		if (chain[iter].valid == 0)
			continue;

		c_iter = malloc(sizeof(*c_iter));
		if (c_iter == NULL)
			goto add_free;
		memset(c_iter, 0, sizeof(*c_iter));
		c_iter->arg = chain[iter].arg;
		c_iter->op = chain[iter].op;
		c_iter->datum = chain[iter].datum;
		/* XXX - sanity check the c_iter->datum value? */

		/* link in the new node and update the chain */
		if (c_prev != NULL) {
			if (tf_flag)
				c_prev->nxt_t = c_iter;
			else
				c_prev->nxt_f = c_iter;
		} else
			s_new->chains = c_iter;
		s_new->node_cnt++;

		/* rewrite the op to reduce the op/datum combos */
		switch (c_iter->op) {
			case SCMP_CMP_NE:
				c_iter->op = SCMP_CMP_EQ;
				tf_flag = 0;
				break;
			case SCMP_CMP_LT:
				c_iter->op = SCMP_CMP_GE;
				tf_flag = 0;
				break;
			case SCMP_CMP_LE:
				c_iter->op = SCMP_CMP_GT;
				tf_flag = 0;
				break;
			default:
				tf_flag = 1;
		}

		c_prev = c_iter;
	}
	if (c_iter != NULL) {
		/* set the leaf node */
		if (tf_flag) {
			c_iter->act_t_flg = 1;
			c_iter->act_t = action;
		} else {
			c_iter->act_f_flg = 1;
			c_iter->act_f = action;
		}
	} else
		s_new->action = action;
	s_new->priority = _DB_PRI_MASK_CHAIN - s_new->node_cnt;

	/* no more failures allowed after this point that would result in the
	 * stored filter being in an inconsistent state */

	/* find a matching syscall/chain or insert a new one */
	s_iter = db->syscalls;
	while (s_iter != NULL && s_iter->num < syscall) {
		s_prev = s_iter;
		s_iter = s_iter->next;
	}
	if (s_iter == NULL || s_iter->num != syscall) {
		/* new syscall, add before s_iter */
		if (s_prev != NULL) {
			s_new->next = s_prev->next;
			s_prev->next = s_new;
		} else {
			s_new->next = db->syscalls;
			db->syscalls = s_new;
		}
		return 0;
	} else if (s_iter->chains == NULL) {
		/* syscall exists without any chains - existing filter is at
		 * least as large as the new entry so cleanup and exit */
		goto add_free_ok;
	} else if (s_iter->chains != NULL && s_new->chains == NULL) {
		/* syscall exists with chains but the new filter has no chains
		 * so we need to clear the existing chains and exit */
		_db_tree_free(s_iter->chains);
		s_iter->chains = NULL;
		s_iter->node_cnt = 0;
		s_iter->action = action;
		goto add_free_ok;
	}
	/* syscall exists and has at least one existing chain - start at the
	 * top and walk the two chains */
	c_prev = NULL;
	c_iter = s_new->chains;
	ec_iter = s_iter->chains;
	do {
		if (db_chain_eq(c_iter, ec_iter)) {
			/* found a matching node on this chain level */
			if (db_chain_leaf(c_iter) && db_chain_leaf(ec_iter)) {
				/* both are leaf nodes */
				if (c_iter->act_t_flg && ec_iter->act_t_flg) {
					if (ec_iter->act_t != action)
						goto add_free_exist;
				} else if (c_iter->act_t_flg) {
					ec_iter->act_t_flg = 1;
					ec_iter->act_t = action;
				}
				if (c_iter->act_f_flg && ec_iter->act_f_flg) {
					if (ec_iter->act_f != action)
						goto add_free_exist;
				} else if (c_iter->act_f_flg) {
					ec_iter->act_f_flg = 1;
					ec_iter->act_f = action;
				}
				if (ec_iter->act_t_flg == ec_iter->act_f_flg &&
				    ec_iter->act_t == ec_iter->act_f) {
					n_cnt = _db_tree_remove(
							     &(s_iter->chains),
							     ec_iter,
							     ec_iter->act_t);
					s_iter->node_cnt -= n_cnt;
				}
				goto add_free_ok;
			} else if (db_chain_leaf(c_iter)) {
				/* new is shorter */
				if (c_iter->act_t_flg) {
					rc = _db_tree_act_check(ec_iter->nxt_t,
								action);
					if (rc < 0)
						goto add_free;
					n_cnt = _db_tree_free(ec_iter->nxt_t);
					ec_iter->nxt_t = NULL;
					ec_iter->act_t_flg = 1;
					ec_iter->act_t = action;
				} else {
					rc = _db_tree_act_check(ec_iter->nxt_f,
								action);
					if (rc < 0)
						goto add_free;
					n_cnt = _db_tree_free(ec_iter->nxt_f);
					ec_iter->nxt_f = NULL;
					ec_iter->act_f_flg = 1;
					ec_iter->act_f = action;
				}
				s_iter->node_cnt -= n_cnt;
				return 0;
			} else if (c_iter->nxt_t != NULL) {
				if (ec_iter->nxt_t != NULL) {
					/* jump to the next level */
					c_prev = c_iter;
					c_iter = c_iter->nxt_t;
					ec_iter = ec_iter->nxt_t;
					s_new->node_cnt--;
				} else if (ec_iter->act_t_flg) {
					/* existing is shorter */
					if (ec_iter->act_t == action)
						goto add_free_ok;
					goto add_free_exist;
				} else {
					/* add a new branch */
					ec_iter->nxt_t = c_iter->nxt_t;
					s_iter->node_cnt += (s_new->node_cnt-1);
					goto add_free_match;
				}
			} else if (c_iter->nxt_f != NULL) {
				if (ec_iter->nxt_f != NULL) {
					/* jump to the next level */
					c_prev = c_iter;
					c_iter = c_iter->nxt_f;
					ec_iter = ec_iter->nxt_f;
					s_new->node_cnt--;
				} else if (ec_iter->act_f_flg) {
					/* existing is shorter */
					if (ec_iter->act_f == action)
						goto add_free_ok;
					goto add_free_exist;
				} else {
					/* add a new branch */
					ec_iter->nxt_f = c_iter->nxt_f;
					s_iter->node_cnt += (s_new->node_cnt-1);
					goto add_free_match;
				}
			} else {
				/* we should never be here! */
				rc = -EFAULT;
				goto add_free;
			}
		} else {
			/* need to check other nodes on this level */
			if (db_chain_lt(c_iter, ec_iter)) {
				if (ec_iter->lvl_prv == NULL) {
					ec_iter->lvl_prv = c_iter;
					c_iter->lvl_nxt = ec_iter;
					if (ec_iter == s_iter->chains)
						s_iter->chains = c_iter;
					goto add_free_match;
				} else
					ec_iter = ec_iter->lvl_prv;
			} else {
				if (ec_iter->lvl_nxt == NULL) {
					ec_iter->lvl_nxt = c_iter;
					c_iter->lvl_prv = ec_iter;
					goto add_free_match;
				} else if (db_chain_lt(c_iter,
						       ec_iter->lvl_nxt)) {
					/* add new chain in between */
					c_iter->lvl_nxt = ec_iter->lvl_nxt;
					ec_iter->lvl_nxt->lvl_prv = c_iter;
					ec_iter->lvl_nxt = c_iter;
					c_iter->lvl_prv = ec_iter;
					goto add_free_match;
				} else
					ec_iter = ec_iter->lvl_nxt;
			}
		}
	} while ((c_iter != NULL) && (ec_iter != NULL));

	/* we should never be here! */
	return -EFAULT;

add_free_exist:
	rc = -EEXIST;
	goto add_free;
add_free_ok:
	rc = 0;
add_free:
	/* update the priority */
	if (s_iter != NULL) {
		s_iter->priority &= (~_DB_PRI_MASK_CHAIN);
		s_iter->priority |= (_DB_PRI_MASK_CHAIN - s_iter->node_cnt);
	}
	/* free the new chain and its syscall struct */
	_db_tree_free(s_new->chains);
	free(s_new);
	return rc;
add_free_match:
	/* update the priority */
	if (s_iter != NULL) {
		s_iter->priority &= (~_DB_PRI_MASK_CHAIN);
		s_iter->priority |= (_DB_PRI_MASK_CHAIN - s_iter->node_cnt);
	}
	/* free the matching portion of new chain */
	if (c_prev != NULL) {
		c_prev->nxt_t = NULL;
		c_prev->nxt_f = NULL;
		_db_tree_free(s_new->chains);
	}
	free(s_new);
	return 0;
}
