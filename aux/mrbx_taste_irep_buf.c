#include <mruby-aux/dump.h>
#include <string.h>

MRB_API mrb_bool
mrbx_taste_irep_buf(mrb_state *mrb, const void *buf, size_t bufsize, mrb_value name, mrb_bool raise_if_error)
{
  if (mrb_nil_p(name)) { name = mrb_str_new_lit(mrb, "<unknown>"); }

  const struct rite_binary_header *bin = (const struct rite_binary_header *)buf;
  if (bufsize < sizeof(struct rite_binary_header) ||
      bufsize < bin_to_uint32(bin->binary_size)) {
    if (raise_if_error) {
      mrb_raisef(mrb, E_RUNTIME_ERROR,
                 "wrong binary size - %S",
                 name);
    }
    return FALSE;
  }

  if (memcmp(bin->binary_version, RITE_BINARY_FORMAT_VER, sizeof(bin->binary_version)) != 0) {
    if (raise_if_error) {
      mrb_raisef(mrb, E_RUNTIME_ERROR,
                 "wrong binary version - %S (expected \"%S\", but given \"%S\")",
                 name,
                 mrb_str_new_lit(mrb, RITE_BINARY_FORMAT_VER),
                 mrb_str_new(mrb, (const char *)bin->binary_version, sizeof(bin->binary_version)));
    }
    return FALSE;
  }

  return TRUE;
}

MRB_API void
mrbx_err_irep_buf(mrb_state *mrb, const void *buf, size_t bufsize, mrb_value name)
{
  mrbx_taste_irep_buf(mrb, buf, bufsize, name, TRUE);

  if (mrb_nil_p(name)) { name = mrb_str_new_lit(mrb, "<unknown>"); }

  mrb_raisef(mrb, E_RUNTIME_ERROR, "unknown load error - %S", name);
}
