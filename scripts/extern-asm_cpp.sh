#!/bin/sh
#
# get the Assembly extern functions.
#


cat <<__EOF__

#ifdef __cplusplus
extern "C" {
#endif
__EOF__

for i in "$@"; do
	echo
	echo "/* $i */"

	for j in $(sed '/^\/\* @func:.*(.*static.*)/,/\*\/$/d' "$i" \
			|grep '^/\* @func:\s*[A-Za-z0-9_]\+$' \
			|sed 's,^/\* @func:\s*,,'); do
		n=$(sed -n '/^\/\* @func: '$j'/,/^ \*\//p' $i \
			|grep -o '#[0-9r]\+:\s*([^)]*)')
		a=$(echo "$n" |grep -v '^#r:' \
			|sed 's/^#[0-9]\+:\s*(//;s/)//')
		if test "x$a" = "x"; then
			a="void"
		else
			a=$(echo "$a" |sed 's/$/, /' |tr -d '\n' \
				|sed 's/, $//')
		fi
		b=$(echo "$n" |grep -v '^#[0-9]\+:' \
			|sed 's/^#r:\s*(//;s/)//')
		if test "x$b" = "x"; then
			b="void"
		fi
		echo "extern"
		echo "$b $j($a)"
		echo ";"
	done
done

cat <<__EOF__

#ifdef __cplusplus
}
#endif

__EOF__
