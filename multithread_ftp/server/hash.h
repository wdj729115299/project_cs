#ifndef _HASH_H
#define _HASH_H

struct hlist_head
{
    struct hlist_node *first;
}

struct hlist_node
{
    struct hlist_node *next;
    struct hlist_node **pprev;
};

#define HLIST_HEAD_INIT {.first = NULL}
#define HLIST_HEAD(name)    struct hlist_head name = { .first = NULL }
#define INIT_HLIST_HEAD(ptr)    ((ptr)->first = NULL)

static inline void INIT_HLIST_NODE(struct hlist_node *node)
{
    node->next = NULL;
    node->pprev = NULL;
}

static inline void __hlist_del(struct hlist_node *node)
{
}

static inline void hlist_del(struct hlist_node *node)
{
}

static inline void hlist_add_head(struct hlist_node *node, struct hlist_head *head)
{
}

static inline void hlist_add_before(struct hlist_node *node, struct hlist_node *next)
{
}

static inline void hlist_add_after(struct hlist_node *node, struct hlist_node *next)
{
}

#define hlist_entry
#endif
