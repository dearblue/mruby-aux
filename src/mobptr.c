#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby-aux/mobptr.h>
#include <string.h>

static void *
aux_calloc_simple(mrb_state *mrb, size_t num, size_t size)
{
    if (num < 1 || size < 1 || num > SIZE_MAX / size) { return NULL; }

    size *= num;
    void *p = mrb_malloc_simple(mrb, size);

    if (p) { memset(p, 0, size); }

    return p;
}

#ifdef MRUBY_AUX_MOB_ENTRIES
# if MRUBY_AUX_MOB_ENTRIES < 3
#  error ``MRUBY_AUX_MOB_ENTRIES'' is not defined under ``3''.
# endif
# if MRUBY_AUX_MOB_ENTRIES > 255
#  error ``MRUBY_AUX_MOB_ENTRIES'' is not defined upper ``255''.
# endif
#else
# define MRUBY_AUX_MOB_ENTRIES    7
#endif

#define MOB_ENTRIES ((int)MRUBY_AUX_MOB_ENTRIES)

/*
 * データ構造は、固定長配列の双方向リンクになっている。
 * block1 の entry1 が最初に追加された要素で、次の要素は連続して追加される。
 * 要素が削除された場合、より新しい要素をずらして穴埋めする。
 *
 *                    (blockN)               (block2)      (block1)
 *  (RData->data) --> [ link ] <--> ... <--> [ link ] <--> [ link ]
 *                    [entry1]               [entry1]      [entry1]
 *                    [entry2]               [entry2]      [entry2]
 *                        :                      :             :
 *                    [entryN]               [entryN]      [entryN]
 */

struct mob_holder;
struct mob_entry;

struct mob_entry
{
    void *data;
    mrbx_mob_free_f *dfree;
};

struct mob_holder
{
    struct mob_holder *next;
    struct mob_holder *prev;
    struct mob_entry entries[MOB_ENTRIES];
};

static void
free_mob(mrb_state *mrb, void *data)
{
    struct mob_holder *p = (struct mob_holder *)data;

    while (p) {
        struct mob_holder *next = p->next;
        struct mob_entry *e = p->entries + MOB_ENTRIES - 1;

        for (int i = MOB_ENTRIES; i > 0; i --, e --) {
            if (e->data) e->dfree(mrb, e->data);
            e->data = NULL;
            e->dfree = NULL;
        }

        mrb_free(mrb, p);
        p = next;
    }
}

static const mrb_data_type mob_type = { "mob allocator", free_mob };

MRB_API mrb_value
mrbx_mob_create(mrb_state *mrb)
{
    struct RData *d = mrb_data_object_alloc(mrb, NULL, NULL, &mob_type);

    d->data = mrb_calloc(mrb, 1, sizeof(struct mob_holder));

    return mrb_obj_value(d);
}

static int
mob_order(mrb_state *mrb, mrb_value mob, int order,
        void *(*getdata)(mrb_state *, mrb_value, const mrb_data_type *),
        void *(*callocator)(mrb_state *, size_t num, size_t len))
{
    struct mob_holder *s = (struct mob_holder *)getdata(mrb, mob, &mob_type);
    struct mob_holder *p = s;

    if (s == NULL) return 1;
    if (order < 1) return 0;

    while (p) {
        struct mob_entry *e = p->entries;

        for (int i = MOB_ENTRIES; i > 0; i --, e ++) {
            if (e->data == NULL) {
                order --;
                if (order < 1) return 0;
            }
        }

        p = p->next;
    }

    for (; order > 0; order -= MOB_ENTRIES) {
        p = (struct mob_holder *)callocator(mrb, 1, sizeof(struct mob_holder));

        if (p == NULL) return 1;

        p->next = s;
        s->prev = p;
        DATA_PTR(mob) = s = p;
    }

    return 0;
}

MRB_API int
mrbx_mob_order(mrb_state *mrb, mrb_value mob, int order)
{
    mob_order(mrb, mob, order, mrb_data_get_ptr, mrb_calloc);

    return 0;
}

MRB_API int
mrbx_mob_order_noraise(mrb_state *mrb, mrb_value mob, int order)
{
    return mob_order(mrb, mob, order, mrb_data_check_get_ptr, aux_calloc_simple);
}

static void
setentry(mrb_value mob, void *data, mrbx_mob_free_f *dfree)
{
    if (data == NULL) { return; }

    if (dfree == NULL) { dfree = mrb_free; }

    struct mob_holder *p = (struct mob_holder *)DATA_PTR(mob);

    for (; p != NULL; p = p->next) {
        if (p->entries[0].data == NULL &&
                p->next &&
                p->next->entries[MOB_ENTRIES - 1].data == NULL) {
            ;
        } else {
            struct mob_entry *e = p->entries;

            for (int i = MOB_ENTRIES; i > 0; i --, e ++) {
                if (e->data == NULL) {
                    e->data = data;
                    e->dfree = dfree;
                    return;
                }
            }
        }
    }
}

static struct mob_entry *
findentry_noraise(mrb_state *mrb, mrb_value mob, void *data, struct mob_holder **holder)
{
    struct mob_holder *p = (struct mob_holder *)mrb_data_check_get_ptr(mrb, mob, &mob_type);

    if (holder) { *holder = NULL; }

    if (p == NULL) { return NULL; }
    if (data == NULL) { return NULL; }

    for (; p != NULL; p = p->next) {
        struct mob_entry *e = p->entries;

        for (int i = MOB_ENTRIES; i > 0; i --, e ++) {
            if (e->data == data) {
                if (holder) { *holder = p; }

                return e;
            }
        }
    }

    return NULL;
}

static struct mob_entry *
findentry(mrb_state *mrb, mrb_value mob, void *data, struct mob_holder **holder)
{
    struct mob_entry *e = findentry_noraise(mrb, mob, data, holder);

    if (e == NULL) { mrb_data_check_type(mrb, mob, &mob_type); }

    return e;
}

static int
mob_push(mrb_state *mrb, mrb_value mob, void *data, mrbx_mob_free_f *dfree,
        struct mob_entry *(*findentry)(mrb_state *, mrb_value, void *, struct mob_holder **),
        int (*mob_order)(mrb_state *, mrb_value, int))
{
    if (data == NULL || dfree == NULL) { return 1; }

    struct mob_entry *e = findentry(mrb, mob, data, NULL);

    if (e) {
        e->data = data;
        e->dfree = dfree;
    } else {
        if (mob_order(mrb, mob, 1) != 0) { return 1; }

        setentry(mob, data, dfree);
    }

    return 0;
}

MRB_API int
mrbx_mob_push(mrb_state *mrb, mrb_value mob, void *data, mrbx_mob_free_f *dfree)
{
    return mob_push(mrb, mob, data, dfree, findentry, mrbx_mob_order);
}

MRB_API int
mrbx_mob_push_noraise(mrb_state *mrb, mrb_value mob, void *data, mrbx_mob_free_f *dfree)
{
    return mob_push(mrb, mob, data, dfree, findentry_noraise, mrbx_mob_order_noraise);
}

static void
slide_entries(struct mob_holder *holder, struct mob_entry *e)
{
    struct mob_entry *f; // ループの始めで e を代入するため、初期化は省略

    while (e->data) {
        f = e ++;
        if (e == NULL || e >= holder->entries + MOB_ENTRIES) {
            holder = holder->prev;
            if (holder == NULL) {
                memset(f, 0, sizeof(struct mob_entry));
                return;
            }
            e = holder->entries;
        }
        *f = *e;
    }
}

MRB_API mrbx_mob_free_f *
mrbx_mob_pop(mrb_state *mrb, mrb_value mob, void *data)
{
    struct mob_holder *holder;
    struct mob_entry *e = findentry_noraise(mrb, mob, data, &holder);

    if (e) {
        mrbx_mob_free_f *dfree = e->dfree;
        slide_entries(holder, e);
        return dfree;
    } else {
        return NULL;
    }
}

MRB_API void
mrbx_mob_free(mrb_state *mrb, mrb_value mob, void *data)
{
    mrbx_mob_free_f *dfree = mrbx_mob_pop(mrb, mob, data);
    if (dfree) { dfree(mrb, data); }
}

MRB_API void
mrbx_mob_all_free(mrb_state *mrb, mrb_value mob)
{
    struct mob_holder *p = (struct mob_holder *)mrb_data_check_get_ptr(mrb, mob, &mob_type);

    if (p == NULL) { return; }

    for (; p != NULL; p = p->next) {
        struct mob_entry *e = p->entries + MOB_ENTRIES - 1;

        for (int i = MOB_ENTRIES; i > 0; i --, e --) {
            void *data = e->data;
            if (data) {
                mrbx_mob_free_f *dfree = e->dfree;

                e->dfree = NULL;
                e->data = NULL;
                dfree(mrb, data);
            }
        }
    }
}

MRB_API void
mrbx_mob_compact(mrb_state *mrb, mrb_value mob)
{
    struct mob_holder *p = (struct mob_holder *)mrb_data_check_get_ptr(mrb, mob, &mob_type);

    if (p == NULL) { return; }

    while (p != NULL && p->entries[0].dfree == NULL) {
        struct mob_holder *next = p->next;

        if (next) {
            next->prev = NULL;
        }

        DATA_PTR(mob) = next;
        mrb_free(mrb, (void *)p);
        p = next;
    }
}

MRB_API void
mrbx_mob_cleanup(mrb_state *mrb, mrb_value mob)
{
    mrbx_mob_all_free(mrb, mob);
    mrbx_mob_compact(mrb, mob);
}

MRB_API void *
mrbx_mob_malloc(mrb_state *mrb, mrb_value mob, size_t size)
{
    if (size < 1) { return NULL; }

    mrbx_mob_order(mrb, mob, 1);
    void *p = mrb_malloc(mrb, size);
    setentry(mob, p, NULL);

    return p;
}

MRB_API void *
mrbx_mob_calloc(mrb_state *mrb, mrb_value mob, size_t num, size_t size)
{
    if (size < 1) { return NULL; }

    mrbx_mob_order(mrb, mob, 1);
    void *p = mrb_calloc(mrb, num, size);
    setentry(mob, p, NULL);

    return p;
}

MRB_API void *
mrbx_mob_realloc(mrb_state *mrb, mrb_value mob, void *data, size_t size)
{
    struct mob_entry *e = findentry(mrb, mob, data, NULL);

    if (e == NULL) { mrb_raise(mrb, E_RUNTIME_ERROR, "not attached pointer"); }
    if (size < 1) { mrbx_mob_free(mrb, mob, data); return NULL; }

    e->data = mrb_realloc(mrb, data, size);

    return e->data;
}

MRB_API void *
mrbx_mob_malloc_simple(mrb_state *mrb, mrb_value mob, size_t size)
{
    if (size < 1) { return NULL; }
    if (mrbx_mob_order_noraise(mrb, mob, 1) != 0) { return NULL; }

    void *p = mrb_malloc_simple(mrb, size);

    if (p) { setentry(mob, p, NULL); }

    return p;
}

MRB_API void *
mrbx_mob_calloc_simple(mrb_state *mrb, mrb_value mob, size_t num, size_t size)
{
    if (num < 1 || size < 1 || num > SIZE_MAX / size) { return NULL; }

    size *= num;
    void *p = mrbx_mob_malloc_simple(mrb, mob, size);

    if (p) { memset(p, 0, size); }

    return p;
}

MRB_API void *
mrbx_mob_realloc_simple(mrb_state *mrb, mrb_value mob, void *data, size_t size)
{
    struct mob_entry *e = findentry_noraise(mrb, mob, data, NULL);

    if (e == NULL) { return NULL; }
    if (size < 1) { mrbx_mob_free(mrb, mob, data); return NULL; }

    void *p = mrb_realloc_simple(mrb, data, size);

    if (p) { e->data = p; }

    return p;
}
