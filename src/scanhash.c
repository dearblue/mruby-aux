/*
 * This code is under public domain (CC0)
 * <https://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * To the extent possible under law, dearblue has waived all copyright
 * and related or neighboring rights to this work.
 *
 *     dearblue <dearblue@users.noreply.github.com>
 */

#include <mruby-aux/scanhash.h>
#include <mruby-aux/compat/hash.h>


struct mrbx_scanhash_args
{
    const struct mrbx_scanhash_arg *args;
    const struct mrbx_scanhash_arg *end;
    struct RHash *receptor;
};

static void
mrbx_scanhash_error(mrb_state *mrb, mrb_sym given, const struct mrbx_scanhash_arg *args, const struct mrbx_scanhash_arg *end)
{
    // 引数の数が㌧でもない数の場合、よくないことが起きそう。

    mrb_value names = mrb_ary_new(mrb);
    for (; args < end; args ++) {
        mrb_ary_push(mrb, names, mrb_symbol_value(args->name));
    }

    size_t namenum = RARRAY_LEN(names);
    if (namenum > 2) {
        mrb_value w = mrb_ary_pop(mrb, names);
        names = mrb_ary_join(mrb, names, mrb_str_new_cstr(mrb, ", "));
        names = mrb_ary_new_from_values(mrb, 1, &names);
        mrb_ary_push(mrb, names, w);
        names = mrb_ary_join(mrb, names, mrb_str_new_cstr(mrb, " or "));
    } else if (namenum > 1) {
        names = mrb_ary_join(mrb, names, mrb_str_new_cstr(mrb, " or "));
    }

    {
        mrb_value key = mrb_symbol_value(given);
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
                   "unknown keyword (%S for %S)",
                   key, names);
    }
}

static int
mrbx_scanhash_foreach(mrb_state *mrb, mrb_value key, mrb_value value, struct mrbx_scanhash_args *args)
{
    const struct mrbx_scanhash_arg *p = args->args;
    const struct mrbx_scanhash_arg *end = args->end;
    mrb_sym keyid = mrb_obj_to_sym(mrb, key);

    for (; p < end; p ++) {
        if (p->name == keyid) {
            if (p->dest) {
                *p->dest = value;
            }
            return 0;
        }
    }

    if (args->receptor) {
        mrb_hash_set(mrb, mrb_obj_value(args->receptor), key, value);
    } else {
        mrbx_scanhash_error(mrb, keyid, args->args, args->end);
    }

    return 0;
}

static struct RHash *
mrbx_scanhash_to_hash(mrb_state *mrb, mrb_value hash)
{
    if (mrb_nil_p(hash)) { return NULL; }
    mrb_check_type(mrb, hash, MRB_TT_HASH);
    return RHASH(hash);
}

static inline void
mrbx_scanhash_setdefaults(const struct mrbx_scanhash_arg *args, const struct mrbx_scanhash_arg *end)
{
    for (; args < end; args ++) {
        if (args->dest) {
            *args->dest = args->initval;
        }
    }
}


static inline void
mrbx_scanhash_check_missingkeys(mrb_state *mrb, const struct mrbx_scanhash_arg *args, const struct mrbx_scanhash_arg *end)
{
    for (; args < end; args ++) {
        if (args->dest && mrb_undef_p(*args->dest)) {
            mrb_value key = mrb_symbol_value(args->name);
            mrb_raisef(mrb, E_ARGUMENT_ERROR,
                       "missing keyword: `%S'",
                       key);
        }
    }
}

static void
mrbx_hash_foreach(mrb_state *mrb, struct RHash *hash, int (*block)(mrb_state *, mrb_value, mrb_value, struct mrbx_scanhash_args *), struct mrbx_scanhash_args *args)
{
    mrb_value keys = mrb_hash_keys(mrb, mrb_obj_value(hash));
    mrb_value values = mrb_hash_values(mrb, mrb_obj_value(hash));
    const mrb_value *k = RARRAY_PTR(keys);
    const mrb_value *const kk = k + RARRAY_LEN(keys);
    const mrb_value *v = RARRAY_PTR(values);
    int arena = mrb_gc_arena_save(mrb);

    for (; k < kk; k ++, v ++) {
        block(mrb, *k, *v, args);
        mrb_gc_arena_restore(mrb, arena);
    }
}

static struct RHash *
make_receptor(mrb_state *mrb, mrb_value rest)
{
    if (mrb_bool(rest)) {
        if (mrb_type(rest) == MRB_TT_TRUE) {
            return RHASH(mrb_hash_new(mrb));
        } else {
            mrb_check_type(mrb, rest, MRB_TT_HASH);
            return RHASH(rest);
        }
    } else {
        return NULL;
    }
}

MRB_API mrb_value
mrbx_scanhash(mrb_state *mrb, mrb_value hash, mrb_value rest, size_t argc, const struct mrbx_scanhash_arg *argv)
{
    struct RHash *receptor = make_receptor(mrb, rest);
    struct RHash *hashp = mrbx_scanhash_to_hash(mrb, hash);

    mrbx_scanhash_setdefaults(argv, argv + argc);

    if (hashp && !mrb_hash_empty_p(mrb, mrb_obj_value(hashp))) {
        struct mrbx_scanhash_args argset = { argv, argv + argc, receptor };
        mrbx_hash_foreach(mrb, hashp, mrbx_scanhash_foreach, &argset);
    }

    mrbx_scanhash_check_missingkeys(mrb, argv, argv + argc);

    if (receptor) {
        return mrb_obj_value(receptor);
    } else {
        return mrb_nil_value();
    }
}
