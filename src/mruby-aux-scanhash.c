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


#if MRUBY_RELEASE_NO <= 10200
#   define id_values mrb_intern_lit(mrb, "values")

    static mrb_value
    mrb_hash_values(mrb_state *mrb, mrb_value hash)
    {
        return mrb_funcall_argv(mrb, hash, id_values, 0, NULL);
    }
#endif


struct mrbx_scanhash_args
{
    struct mrbx_scanhash_arg *args;
    const struct mrbx_scanhash_arg *end;
    mrb_value rest;
};

static void
mrbx_scanhash_error(mrb_state *mrb, mrb_sym given, struct mrbx_scanhash_arg *args, const struct mrbx_scanhash_arg *end)
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
    struct mrbx_scanhash_arg *p = args->args;
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

    if (mrb_test(args->rest)) {
        mrb_hash_set(mrb, args->rest, key, value);
    } else {
        mrbx_scanhash_error(mrb, keyid, args->args, args->end);
    }

    return 0;
}

static mrb_value
mrbx_scanhash_to_hash(mrb_state *mrb, mrb_value hash)
{
    if (mrb_nil_p(hash)) { return mrb_nil_value(); }

    mrb_sym id_to_hash = mrb_intern_lit(mrb, "to_hash");
    mrb_value hash1 = mrb_funcall_argv(mrb, hash, id_to_hash, 0, 0);
    if (!mrb_hash_p(hash1)) {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "converted object is not a hash (<#%S>)",
                   hash);
    }
    return hash1;
}

static inline void
mrbx_scanhash_setdefaults(struct mrbx_scanhash_arg *args, struct mrbx_scanhash_arg *end)
{
    for (; args < end; args ++) {
        if (args->dest) {
            *args->dest = args->initval;
        }
    }
}


static inline void
mrbx_scanhash_check_missingkeys(mrb_state *mrb, struct mrbx_scanhash_arg *args, struct mrbx_scanhash_arg *end)
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
mrbx_hash_foreach(mrb_state *mrb, mrb_value hash, int (*block)(mrb_state *, mrb_value, mrb_value, struct mrbx_scanhash_args *), struct mrbx_scanhash_args *args)
{
    mrb_value keys = mrb_hash_keys(mrb, hash);
    mrb_value values = mrb_hash_values(mrb, hash);
    const mrb_value *k = RARRAY_PTR(keys);
    const mrb_value *const kk = k + RARRAY_LEN(keys);
    const mrb_value *v = RARRAY_PTR(values);
    int arena = mrb_gc_arena_save(mrb);

    for (; k < kk; k ++, v ++) {
        block(mrb, *k, *v, args);
        mrb_gc_arena_restore(mrb, arena);
    }
}

mrb_value
mrbx_scanhash(mrb_state *mrb, mrb_value hash, mrb_value rest, size_t argc, struct mrbx_scanhash_arg *argv)
{
    if (mrb_bool(rest)) {
        if (mrb_type(rest) == MRB_TT_TRUE) {
            rest = mrb_hash_new(mrb);
        } else if (!mrb_obj_is_kind_of(mrb, rest, mrb->hash_class)) {
            mrb_raise(mrb, E_ARGUMENT_ERROR,
                      "`rest' is not a hash");
        }
    } else {
        rest = mrb_nil_value();
    }

    mrbx_scanhash_setdefaults(argv, argv + argc);

    hash = mrbx_scanhash_to_hash(mrb, hash);
    if (!mrb_nil_p(hash) && !mrb_bool(mrb_hash_empty_p(mrb, hash))) {
        struct mrbx_scanhash_args argset = { argv, argv + argc, rest };
        mrbx_hash_foreach(mrb, hash, mrbx_scanhash_foreach, &argset);
    }

    mrbx_scanhash_check_missingkeys(mrb, argv, argv + argc);

    return rest;
}


void
mrb_mruby_aux_scanhash_gem_init(mrb_state *mrb)
{
}

void
mrb_mruby_aux_scanhash_gem_final(mrb_state *mrb)
{
}
