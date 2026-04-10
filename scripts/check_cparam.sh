#!/bin/sh
#
# check the parameters header in the C files.
#


grep -nH '#[0-9rN]\+:' "$@" \
|grep -v '\(#[0-9]\+:\s*\(\(([^)]*)\)\|[A-za-z0-9_]\)\+\s*\[\(in/out\|out\|in\)\]\)\|\(#[rN]\s*:\s*\(([^)]*)\)\?\s*\[\(ret\|in/out\|out\|in\)\]\)'

out_d() {
	cat <<EOF
$d
EOF
}

out_s() {
	cat <<EOF
$s
EOF
}

out_ss() {
	cat <<EOF
$ss
EOF
}

out_n() {
	cat <<EOF
$n
EOF
}

for f in "$@"; do
	echo "$f"
	d=$(sed -n '/^\/\* @func:/,/{$/p' $f)
	n=$(out_d |grep -o '^/\* @func:\s*[A-Za-z0-9_]\+' \
		|sed 's/^\/\* @func:\s*//')
	a=$(out_n |sort |uniq -d)
	if test "x$a" != "x"; then
		echo "  repates function: $a"
		continue
	fi

	for i in $(out_n); do
		s=$(out_d |sed -n '/^\/\* @func:\s*\b'$i'\b/,/{$/p')
		n=$(out_s |grep -o '#[0-9]\+:\s*[A-za-z0-9_]\+' \
			|sed 's/#[0-9]\+:\s*//')
		if test "x$n" != "x"; then
			a=$(out_n |sort |uniq -d)
			if test "x$a" != "x"; then
				echo "  repates: $i() $a"
				continue
			fi

			ss=$(out_s |sed '/^\/\* @func:/,/^ \*\//d')
			g=$(echo $n |sed 's/^/\\b/;s/$/\\b/;s/ /\\b\\|\\b/g')
			n=$(out_n |sort && out_ss |grep -o "$g" |sort)
			a=$(out_n |sort |uniq -u)
			if test "x$a" != "x"; then
				echo "  not matched: $i() $a"
				continue
			fi

			out_ss |grep -q '\b'$i'\b'
			test "x$?" != "x0" && echo "  not matched: $i() $a"
		fi
	done
done
