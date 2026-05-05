#!/bin/sh
#
# get the C extern functions, compatible with C++.
#


cat <<__EOF__

#ifdef __cplusplus
extern "C" {
#endif
__EOF__

for i in "$@"; do
	echo
	echo "/* $i */"

	sed -n '/^\/\* @fvar:/,/ = \|;$/p' "$i" \
	|sed 's/ = .*$/;/' \
	|sed '/\*\/$/aextern' \
	|sed '/^\/\*/,/\*\/$/d' \
	|sed 's/;/\n;/'

	sed '/^\/\* @func:.*(.*static.*)/,/\*\/$/d' "$i" \
	|sed -n '/^\/\* @func:/,/^{$\| {$/p' \
	|sed '/^\/\*/,/\*\/$/aextern' \
	|sed '/^\/\*/,/\*\/$/d' \
	|sed 's/^{$\| {$/;/' \
	|sed 's/\bnew\b/_new/g'
done

cat <<__EOF__

#ifdef __cplusplus
}
#endif

__EOF__
