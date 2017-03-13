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

static inline void INIT_HLIST_NODE(struct hlist_node *n)
{
    n->next = NULL;
    n->pprev = NULL;
}

static inline void __hlist_del(struct hlist_node *n)
{
    struct hlist_node *next = n->next;
    struct hlist_node **pprev = n->pprev;

    *pprev = next;
    if(next){
        next->pprev = pprev;
    }
}

static inline void hlist_del(struct hlist_node *n)
{
    __hlist_del(n);
    n->next = NULL;
    n->pprev = NULL;
}

static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    struct hlist_head *first = first;
    n->next = first;
    if(first){
        first->pprev = &n->next
    }
    h->first = n;
    n->pprev = &h->first;
}

static inline void hlist_add_before(struct hlist_node *n, struct hlist_node *next)
{
    n->next = next;
    n->pprev = next->pprev;
    next->pprev = &n->next
    *(n->pprev) = n;
}

static inline void hlist_add_after(struct hlist_node *n, struct hlist_node *next)
{
    next->next = n->next;
    n->next = next;
    next->pprev = &n->next;
    if(next->next)
        next->next->pprev = &next->next;
}

static inline void hlist_move_list(struct hlist_head *old, struct hlist_head *new)
{
    new->first = old->first;
    if(new->first)
        new->first->pprev = &new->first;
    old->first = NULL;
}

#define offsetof(type, member)      ((size_t)&(((type*)0)->member))
#define container_of(ptr, type, member)     \     
        ((type*)((char *)ptr - offsetof(type, member)))
        
#define hlist_entry(ptr, type, member)      container_of(ptr, type, member)

#define hlist_for_each(pos, head)   \
        for(pos = (head)->first; pos; pos = pos->next)

#define hlist_for_each_safe(pos, n, head)  \
        for(pos = (head)->first; pos && ({ n = pos->next; 1; }); pos = n)
        

#define hlist_entry_safe

#define hlist_for_each_entry(pos, head, member)
     
#define hlist_for_each_entry_continue

#define hlist_for_each_etnry_from

#define hlist_for_each_entry_safe
#endif
