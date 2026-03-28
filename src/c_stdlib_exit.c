/* @file: c_stdlib_exit.c
 * #desc:
 *    The implementations of standard library.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not,
 *    see <https://www.gnu.org/licenses/>.
 */

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_atomic.h>
#include <conch/c_stdlib.h>


#define ATEXIT_MAX 32

struct atexit_head {
	void (*func[ATEXIT_MAX])(void);
	struct atexit_head *next;
};

static struct atexit_head _at_head, *_at_curr = NULL;
static int32_t _at_count = 0;
static spinlock_t _lock = 0;


/* @func: conch_atexit
 * #desc:
 *    register a exit callback function.
 *
 * #1: func [in]  callback function
 * #r:      [ret] 0: no error, -1: register fail
 */
int32_t conch_atexit(void (*func)(void))
{
	SPIN_LOCK(&_lock);

	if (!_at_count) {
		_at_curr = &_at_head;
	} else if (!(_at_count % ATEXIT_MAX)) {
		_at_curr->next = conch_malloc(sizeof(struct atexit_head));
		if (!_at_curr->next)
			return -1;
		_at_curr = _at_curr->next;
	}
	_at_curr->func[_at_count++ % ATEXIT_MAX] = func;

	SPIN_UNLOCK(&_lock);

	return 0;
}

/* @func: conch_exit
 * #desc:
 *    terminate the calling process.
 *
 * #1: status [in] exit status
 */
void conch_exit(int32_t status)
{
	SPIN_LOCK(&_lock);

	struct atexit_head *p = &_at_head;
	for (int32_t i = 0; i < _at_count; i++) {
		(p->func[i % ATEXIT_MAX])();
		if (!((i + 1) % ATEXIT_MAX))
			p = p->next;
	}

	conch_Exit(status);

	SPIN_UNLOCK(&_lock);
}
