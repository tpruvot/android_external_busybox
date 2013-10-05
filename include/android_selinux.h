#ifndef BB_ANDROID_SELINUX_H
#define BB_ANDROID_SELINUX_H

#include <selinux/selinux.h>
#include <selinux/label.h>
#include <selinux/android.h>

static struct selabel_handle *bb_sehandle = NULL;

/* Set flags controlling operation of matchpathcon_init or matchpathcon. */
#define MATCHPATHCON_BASEONLY 1 /* Only process the base file_contexts file. */
#define MATCHPATHCON_NOTRANS  2 /* Do not perform any context translation. */
#define MATCHPATHCON_VALIDATE 4 /* Validate/canonicalize contexts at init time. */

#define set_matchpathcon_canoncon(canoncon) \
	selinux_set_callback(SELINUX_CB_VALIDATE, (union selinux_callback)canoncon ## _android);

#define matchpathcon(pathname, mode, context) \
	selabel_lookup(bb_sehandle, context, pathname, mode)

#define matchpathcon_index(pathname, mode, context) \
	selabel_lookup(bb_sehandle, context, pathname, mode)

#define lgetfilecon_raw(path, context) \
	lgetfilecon(path, context)

#define security_canonicalize_context_raw(context, newctx) \
	security_canonicalize_context(context, newctx)

#define is_context_customizable(ctx) false

#define selinux_policy_root() "/file_contexts"


/* other functions stubs to fix or implement */
#define matchpathcon_filespec_add(ino, n, path) 0
#define matchpathcon_filespec_destroy() {}
#define matchpathcon_checkmatches(str) {}
#define set_matchpathcon_flags(flags) {}
#define set_matchpathcon_printf(fn) {}
#define matchpathcon_filespec_eval() {}
#define selinux_file_context_verify(path, n) 0

static int selinux_getenforcemode(int *rc)
{
	if (rc) {
		*rc = security_getenforce();
		return 0;
	}
	return -1;
}

/* FIXME: These stubs do not implement real matchpathcon init yet */
static int matchpathcon_init(const char * pathname)
{
	if (!bb_sehandle) {
		bb_sehandle = selinux_android_file_context_handle();
	}
	return 0;
}

static int matchpathcon_init_prefix(const char * path, const char* subset)
{
	return matchpathcon_init(path);
}

static int matchpathcon_fini(void)
{
	bb_sehandle = NULL;
	return 0;
}

#endif /* BB_ANDROID_SELINUX_H */
