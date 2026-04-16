MAKE = make
SHELL = sh
RM = rm
AR = ar
AR_FLAGS = rc

CC = cc
CFLAGS = -O1 -g -std=c99 -Wall -Wextra -Wpedantic -Wshadow -Wno-format-pedantic -Wno-trigraphs -fno-builtin -fno-inline-functions -finline-hint-functions #-Wsign-compare -Wconversion
CPPFLAGS =
HEADERS = -I../include

static_library_liba_C_SRCS = aes_modes.c aes_sbox.c base16.c base32.c base64.c bits_add.c bits_get.c blake2b.c blake2s.c blowfish.c bn4352.c bn6400.c bn8448.c cfg_ini_parse.c cfg_json_parse.c cfg_json_tree.c chacha20.c crc16.c crc32.c crc64.c curve25519.c curve448.c des.c ds_avltree.c ds_list.c ds_rbtree.c ds_swissmap.c ghash.c hkdf_blake2b.c hkdf_blake2s.c hkdf_md5.c hkdf_sha1.c hkdf_sha256.c hkdf_sha3.c hkdf_sha512.c hmac_blake2b.c hmac_blake2s.c hmac_md5.c hmac_sha1.c hmac_sha256.c hmac_sha3.c hmac_sha512.c md5.c mlkem_1024.c mlkem_512.c mlkem_768.c nist-p256.c nist-p384.c nist-p521.c pbkdf2_blake2b.c pbkdf2_blake2s.c pbkdf2_sha1.c pbkdf2_sha256.c pbkdf2_sha3.c pbkdf2_sha512.c poly1305.c pw_bcrypt.c pw_scrypt.c rc4.c ringbuf.c rsa2048.c rsa3072.c rsa4096.c salsa20.c sha1.c sha256.c sha3.c sha512.c siphash24.c totp_sha1.c totp_sha256.c totp_sha512.c xchacha20.c xsalsa20.c xxhash32.c xxhash64.c z_deflate.c z_inflate.c
static_library_liba_OBJS = $(static_library_liba_C_SRCS:.c=.o) $(static_library_liba_ASM_SRCS:.S=.o)
static_library_liba = liba.a

static_library_libb_C_SRCS = c_ctype.c c_getopt.c c_math_double.c c_stdio__printf.c c_stdio__scanf.c c_stdio_snprintf.c c_stdio_sscanf.c c_stdlib_abs.c c_stdlib_atof.c c_stdlib_atoi.c c_stdlib_qsort.c c_stdlib_rand.c c_string.c c_string_strdup.c
static_library_libb_ASM_SRCS = c_atomic_cas.S c_setjmp.S c_thread_tls.S
static_library_libb_OBJS = $(static_library_libb_C_SRCS:.c=.o) $(static_library_libb_ASM_SRCS:.S=.o)
static_library_libb = libb.a

static_library_libc_C_SRCS = c_errno.c c_fcntl.c c_signal.c c_stdio_io.c c_stdio_printf.c c_stdio_scanf.c c_stdlib_Exit.c c_stdlib_abort.c c_stdlib_exit.c c_stdlib_malloc.c c_sys_mman.c c_time_clock.c c_time_sleep.c c_umalloc.c c_unistd_fork.c c_unistd_fs.c c_unistd_pid.c c_unistd_sleep.c
static_library_libc_ASM_SRCS = c_syscall_linux.S
static_library_libc_OBJS = $(static_library_libc_C_SRCS:.c=.o) $(static_library_libc_ASM_SRCS:.S=.o)
static_library_libc = libc.a

static_library = $(static_library_liba) $(static_library_libb) $(static_library_libc)

all: $(static_library)

liba.a: $(static_library_liba_OBJS)
	$(AR) $(AR_FLAGS) $@ $^
libb.a: $(static_library_libb_OBJS)
	$(AR) $(AR_FLAGS) $@ $^
libc.a: $(static_library_libc_OBJS)
	$(AR) $(AR_FLAGS) $@ $^

clean_liba_obj:
	$(RM) -f $(static_library_liba_OBJS)
clean_libb_obj:
	$(RM) -f $(static_library_libb_OBJS)
clean_libc_obj:
	$(RM) -f $(static_library_libc_OBJS)

clean_liba_static:
	$(RM) -f $(static_library_liba)
clean_libb_static:
	$(RM) -f $(static_library_libb)
clean_libc_static:
	$(RM) -f $(static_library_libc)

clean_liba: clean_liba_obj clean_liba_static
clean_libb: clean_libb_obj clean_libb_static
clean_libc: clean_libc_obj clean_libc_static

clean_obj: clean_liba_obj clean_libb_obj clean_libc_obj

clean_static: clean_liba_static clean_libb_static clean_libc_static

clean: clean_liba clean_libb clean_libc

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(HEADERS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) $(CPPFLAGS) $(HEADERS) -c -o $@ $<
