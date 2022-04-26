/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_list.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/04 2022 14:59
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/04 2022      create the file
 * 
 *     last modified: 26/04 2022 14:59
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_LIST_H_
#define __LIBHY_HAL_INCLUDE_HY_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

// import from include/linux/types.h
struct hy_list_head {
    struct hy_list_head *next, *prev;
};

struct hy_hlist_head {
    struct hy_hlist_node *first;
};

struct hy_hlist_node {
    struct hy_hlist_node *next, **pprev;
};

// import from include/linux/poison.h

/*
 * Architectures might want to move the poison pointer offset
 * into some well-recognized area such as 0xdead000000000000,
 * that is also not mappable by user-space exploits:
 */
#ifdef CONFIG_ILLEGAL_POINTER_VALUE
# define HY_POISON_POINTER_DELTA _AC(CONFIG_ILLEGAL_POINTER_VALUE, UL)
#else
# define HY_POISON_POINTER_DELTA (0)
#endif

/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define HY_LIST_POISON1  (((void *) (0x00100100 + HY_POISON_POINTER_DELTA)))
#define HY_LIST_POISON2  (((void *) (0x00200200 + HY_POISON_POINTER_DELTA)))

// import from include/linux/stddef.h
#undef hy_offsetof
#ifdef __compiler_offsetof
#define hy_offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define hy_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

// import from include/linux/kernel.h
/**
* hy_container_of - cast a member of a structure out to the containing structure
* @ptr:        the pointer to the member.
* @type:       the type of the container struct this is embedded in.
* @member:     the name of the member within the struct.
*
*/
#define hy_container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - hy_offsetof(type,member) );})

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#define HY_LIST_HEAD_INIT(name) { &(name), &(name) }

#define HY_LIST_HEAD(name) \
    struct hy_list_head name = HY_LIST_HEAD_INIT(name)

static inline void HY_INIT_LIST_HEAD(struct hy_list_head *list) {
    list->next = list;
    list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
#ifndef CONFIG_DEBUG_LIST
static inline void __hy_list_add(struct hy_list_head *new,
    struct hy_list_head *prev,
    struct hy_list_head *next) {
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}
#else
extern void __hy_list_add(struct hy_list_head *new,
			      struct hy_list_head *prev,
			      struct hy_list_head *next);
#endif

/**
 * hy_list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void hy_list_add(struct hy_list_head *new, struct hy_list_head *head) {
    __hy_list_add(new, head, head->next);
}

/**
 * hy_list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void hy_list_add_tail(struct hy_list_head *new, struct hy_list_head *head) {
    __hy_list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __hy_list_del(struct hy_list_head *prev, struct hy_list_head *next) {
    next->prev = prev;
    prev->next = next;
}

/**
 * hy_list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
#ifndef CONFIG_DEBUG_LIST

static inline void __hy_list_del_entry(struct hy_list_head *entry) {
    __hy_list_del(entry->prev, entry->next);
}

static inline void hy_list_del(struct hy_list_head *entry) {
    __hy_list_del(entry->prev, entry->next);
    entry->next = HY_LIST_POISON1;
    entry->prev = HY_LIST_POISON2;
}

#else
extern void __hy_list_del_entry(struct hy_list_head *entry);
extern void hy_list_del(struct hy_list_head *entry);
#endif

/**
 * hy_list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void hy_list_replace(struct hy_list_head *old,
        struct hy_list_head *new) {
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static inline void hy_list_replace_init(struct hy_list_head *old,
        struct hy_list_head *new) {
    hy_list_replace(old, new);
    HY_INIT_LIST_HEAD(old);
}

/**
 * hy_list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void hy_list_del_init(struct hy_list_head *entry) {
    __hy_list_del_entry(entry);
    HY_INIT_LIST_HEAD(entry);
}

/**
 * hy_list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void hy_list_move(struct hy_list_head *list, struct hy_list_head *head) {
    __hy_list_del_entry(list);
    hy_list_add(list, head);
}

/**
 * hy_list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void hy_list_move_tail(struct hy_list_head *list,
        struct hy_list_head *head) {
    __hy_list_del_entry(list);
    hy_list_add_tail(list, head);
}

/**
 * hy_list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int hy_list_is_last(const struct hy_list_head *list,
        const struct hy_list_head *head) {
    return list->next == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int hy_list_empty(const struct hy_list_head *head) {
    return head->next == head;
}

/**
 * list_empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using list_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is list_del_init(). Eg. it cannot be used
 * if another CPU could re-list_add() it.
 */
static inline int hy_list_empty_careful(const struct hy_list_head *head) {
    struct hy_list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

/**
 * list_rotate_left - rotate the list to the left
 * @head: the head of the list
 */
static inline void hy_list_rotate_left(struct hy_list_head *head) {
    struct hy_list_head *first;

    if (!hy_list_empty(head)) {
        first = head->next;
        hy_list_move_tail(first, head);
    }
}

/**
 * list_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static inline int hy_list_is_singular(const struct hy_list_head *head) {
    return !hy_list_empty(head) && (head->next == head->prev);
}

static inline void __hy_list_cut_position(struct hy_list_head *list,
        struct hy_list_head *head, struct hy_list_head *entry) {
    struct hy_list_head *new_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_first;
    new_first->prev = head;
}

/**
 * list_cut_position - cut a list into two
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *	and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
static inline void hy_list_cut_position(struct hy_list_head *list,
        struct hy_list_head *head, struct hy_list_head *entry) {
    if (hy_list_empty(head)) {
        return;
    }
    if (hy_list_is_singular(head) &&
            (head->next != entry && head != entry)) {
        return;
    }
    if (entry == head) {
        HY_INIT_LIST_HEAD(list);
    } else {
        __hy_list_cut_position(list, head, entry);
    }
}

static inline void __hy_list_splice(const struct hy_list_head *list,
        struct hy_list_head *prev,
        struct hy_list_head *next) {
    struct hy_list_head *first = list->next;
    struct hy_list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/**
 * list_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void hy_list_splice(const struct hy_list_head *list,
        struct hy_list_head *head) {
    if (!hy_list_empty(list)) {
        __hy_list_splice(list, head, head->next);
    }
}

/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void hy_list_splice_tail(struct hy_list_head *list,
        struct hy_list_head *head) {
    if (!hy_list_empty(list)) {
        __hy_list_splice(list, head->prev, head);
    }
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void hy_list_splice_init(struct hy_list_head *list,
        struct hy_list_head *head) {
    if (!hy_list_empty(list)) {
        __hy_list_splice(list, head, head->next);
        HY_INIT_LIST_HEAD(list);
    }
}

/**
 * list_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
static inline void hy_list_splice_tail_init(struct hy_list_head *list,
        struct hy_list_head *head) {
    if (!hy_list_empty(list)) {
        __hy_list_splice(list, head->prev, head);
        HY_INIT_LIST_HEAD(list);
    }
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct hy_list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define hy_list_entry(ptr, type, member) \
    hy_container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define hy_list_first_entry(ptr, type, member) \
    hy_list_entry((ptr)->next, type, member)

/**
 * list_last_entry - get the last element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define hy_list_last_entry(ptr, type, member) \
    hy_list_entry((ptr)->prev, type, member)

/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define hy_list_first_entry_or_null(ptr, type, member) \
    (!hy_list_empty(ptr) ? hy_list_first_entry(ptr, type, member) : NULL)

/**
 * list_next_entry - get the next element in list
 * @pos:	the type * to cursor
 * @member:	the name of the list_head within the struct.
 */
#define hy_list_next_entry(pos, member) \
    hy_list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_prev_entry - get the prev element in list
 * @pos:	the type * to cursor
 * @member:	the name of the list_head within the struct.
 */
#define hy_list_prev_entry(pos, member) \
    hy_list_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct hy_list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define hy_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct hy_list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define hy_list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct hy_list_head to use as a loop cursor.
 * @n:		another &struct hy_list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define hy_list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

/**
 * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:	the &struct hy_list_head to use as a loop cursor.
 * @n:		another &struct hy_list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define hy_list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
         pos != (head); \
         pos = n, n = pos->prev)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define hy_list_for_each_entry(pos, head, member)                \
    for (pos = hy_list_first_entry(head, typeof(*pos), member);    \
         &pos->member != (head);                    \
         pos = hy_list_next_entry(pos, member))

/**
 * list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define hy_list_for_each_entry_reverse(pos, head, member)            \
    for (pos = hy_list_last_entry(head, typeof(*pos), member);        \
         &pos->member != (head);                    \
         pos = hy_list_prev_entry(pos, member))

/**
 * list_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
 * @pos:	the type * to use as a start point
 * @head:	the head of the list
 * @member:	the name of the list_head within the struct.
 *
 * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
 */
#define hy_list_prepare_entry(pos, head, member) \
    ((pos) ? : hy_list_entry(head, typeof(*pos), member))

/**
 * list_for_each_entry_continue - continue iteration over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define hy_list_for_each_entry_continue(pos, head, member)        \
    for (pos = hy_list_next_entry(pos, member);            \
         &pos->member != (head);                    \
         pos = hy_list_next_entry(pos, member))

/**
 * list_for_each_entry_continue_reverse - iterate backwards from the given point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after
 * the current position.
 */
#define hy_list_for_each_entry_continue_reverse(pos, head, member)        \
    for (pos = hy_list_prev_entry(pos, member);            \
         &pos->member != (head);                    \
         pos = hy_list_prev_entry(pos, member))

/**
 * list_for_each_entry_from - iterate over list of given type from the current point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define hy_list_for_each_entry_from(pos, head, member)            \
    for (; &pos->member != (head);                    \
         pos = hy_list_next_entry(pos, member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define hy_list_for_each_entry_safe(pos, n, head, member)            \
    for (pos = hy_list_first_entry(head, typeof(*pos), member),    \
        n = hy_list_next_entry(pos, member);            \
         &pos->member != (head);                    \
         pos = n, n = hy_list_next_entry(n, member))

/**
 * list_for_each_entry_safe_continue - continue list iteration safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define hy_list_for_each_entry_safe_continue(pos, n, head, member)        \
    for (pos = hy_list_next_entry(pos, member),                \
        n = hy_list_next_entry(pos, member);                \
         &pos->member != (head);                        \
         pos = n, n = hy_list_next_entry(n, member))

/**
 * list_for_each_entry_safe_from - iterate over list from current point safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define hy_list_for_each_entry_safe_from(pos, n, head, member)            \
    for (n = hy_list_next_entry(pos, member);                    \
         &pos->member != (head);                        \
         pos = n, n = hy_list_next_entry(n, member))

/**
 * list_for_each_entry_safe_reverse - iterate backwards over list safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define hy_list_for_each_entry_safe_reverse(pos, n, head, member)        \
    for (pos = hy_list_last_entry(head, typeof(*pos), member),        \
        n = hy_list_prev_entry(pos, member);            \
         &pos->member != (head);                    \
         pos = n, n = hy_list_prev_entry(n, member))

/**
 * list_safe_reset_next - reset a stale list_for_each_entry_safe loop
 * @pos:	the loop cursor used in the list_for_each_entry_safe loop
 * @n:		temporary storage used in list_for_each_entry_safe
 * @member:	the name of the list_head within the struct.
 *
 * list_safe_reset_next is not safe to use in general if the list may be
 * modified concurrently (eg. the lock is dropped in the loop body). An
 * exception to this is if the cursor element (pos) is pinned in the list,
 * and list_safe_reset_next is called after re-taking the lock and before
 * completing the current iteration of the loop body.
 */
#define hy_list_safe_reset_next(pos, n, member)                \
    n = hy_list_next_entry(pos, member)

/*
 * Double linked lists with a single pointer list head.
 * Mostly useful for hash tables where the two pointer list head is
 * too wasteful.
 * You lose the ability to access the tail in O(1).
 */

#define HY_HLIST_HEAD_INIT { .first = NULL }
#define HY_HLIST_HEAD(name) struct hy_hlist_head name = {  .first = NULL }
#define HY_INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)

static inline void HY_INIT_HLIST_NODE(struct hy_hlist_node *h) {
    h->next = NULL;
    h->pprev = NULL;
}

static inline int hy_hlist_unhashed(const struct hy_hlist_node *h) {
    return !h->pprev;
}

static inline int hy_hlist_empty(const struct hy_hlist_head *h) {
    return !h->first;
}

static inline void __hy_hlist_del(struct hy_hlist_node *n) {
    struct hy_hlist_node *next = n->next;
    struct hy_hlist_node **pprev = n->pprev;
    *pprev = next;
    if (next)
        next->pprev = pprev;
}

static inline void hy_hlist_del(struct hy_hlist_node *n) {
    __hy_hlist_del(n);
    n->next = HY_LIST_POISON1;
    n->pprev = HY_LIST_POISON2;
}

static inline void hy_hlist_del_init(struct hy_hlist_node *n) {
    if (!hy_hlist_unhashed(n)) {
        __hy_hlist_del(n);
        HY_INIT_HLIST_NODE(n);
    }
}

static inline void hy_hlist_add_head(struct hy_hlist_node *n, struct hy_hlist_head *h) {
    struct hy_hlist_node *first = h->first;
    n->next = first;
    if (first) {
        first->pprev = &n->next;
    }
    h->first = n;
    n->pprev = &h->first;
}

/* next must be != NULL */
static inline void hy_hlist_add_before(struct hy_hlist_node *n,
        struct hy_hlist_node *next) {
    n->pprev = next->pprev;
    n->next = next;
    next->pprev = &n->next;
    *(n->pprev) = n;
}

static inline void hy_hlist_add_behind(struct hy_hlist_node *n,
        struct hy_hlist_node *prev) {
    n->next = prev->next;
    prev->next = n;
    n->pprev = &prev->next;

    if (n->next) {
        n->next->pprev = &n->next;
    }
}

/* after that we'll appear to be on some hlist and hlist_del will work */
static inline void hy_hlist_add_fake(struct hy_hlist_node *n) {
    n->pprev = &n->next;
}

/*
 * Move a list from one list head to another. Fixup the pprev
 * reference of the first entry if it exists.
 */
static inline void hy_hlist_move_list(struct hy_hlist_head *old,
        struct hy_hlist_head *new) {
    new->first = old->first;
    if (new->first) {
        new->first->pprev = &new->first;
    }
    old->first = NULL;
}

#define hy_hlist_entry(ptr, type, member) hy_container_of(ptr,type,member)

#define hy_hlist_for_each(pos, head) \
    for (pos = (head)->first; pos ; pos = pos->next)

#define hy_hlist_for_each_safe(pos, n, head) \
    for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
         pos = n)

#define hy_hlist_entry_safe(ptr, type, member) \
    ({ typeof(ptr) ____ptr = (ptr); \
       ____ptr ? hy_hlist_entry(____ptr, type, member) : NULL; \
    })

/**
 * hlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hy_hlist_for_each_entry(pos, head, member)                \
    for (pos = hy_hlist_entry_safe((head)->first, typeof(*(pos)), member);\
         pos;                            \
         pos = hy_hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_continue - iterate over a hlist continuing after current point
 * @pos:	the type * to use as a loop cursor.
 * @member:	the name of the hlist_node within the struct.
 */
#define hy_hlist_for_each_entry_continue(pos, member)            \
    for (pos = hy_hlist_entry_safe((pos)->member.next, typeof(*(pos)), member);\
         pos;                            \
         pos = hy_hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_from - iterate over a hlist continuing from current point
 * @pos:	the type * to use as a loop cursor.
 * @member:	the name of the hlist_node within the struct.
 */
#define hy_hlist_for_each_entry_from(pos, member)                \
    for (; pos;                            \
         pos = hy_hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another &struct hy_hlist_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hy_hlist_for_each_entry_safe(pos, n, head, member)        \
    for (pos = hy_hlist_entry_safe((head)->first, typeof(*pos), member);\
         pos && ({ n = pos->member.next; 1; });            \
         pos = hy_hlist_entry_safe(n, typeof(*pos), member))


//---------------------------------------------------------------
// xia add
#define HY_LIST_GET_NODE_PTR(list, index)       \
    ({                                          \
        struct hy_list_head *list_tmp = list;   \
        for (int i = 0; i <= index; ++i) {      \
            list_tmp = list_tmp->next;          \
        }                                       \
        list_tmp;                               \
     })

#define HY_LIST_GET_NODE_DATA(list, index, type, member) \
    ((hy_list_entry(HY_LIST_GET_NODE_PTR(list, index), type, entry))->member)

#define HY_LIST_SET_NODE_DATA(list, index, type, member, val) \
    (((hy_list_entry(HY_LIST_GET_NODE_PTR(list, index), type, entry))->member) = val)

static inline void list_swap_node_ptr(struct hy_list_head *list, int index_a, int index_b)
{
    struct hy_list_head tmp = {NULL, NULL};
    struct hy_list_head *tmp_a = HY_LIST_GET_NODE_PTR(list, index_a);
    struct hy_list_head *tmp_b = HY_LIST_GET_NODE_PTR(list, index_b);

    hy_list_add_tail(&tmp, tmp_a);
    hy_list_del(tmp_a);
    hy_list_add_tail(tmp_a, tmp_b);
    hy_list_del(tmp_b);
    hy_list_add_tail(tmp_b, &tmp);
    hy_list_del(&tmp);
}

#ifdef __cplusplus
}
#endif

#endif

