/*
 * Copyright (c) 2014-2016 ISPRAS (http://www.ispras.ru)
 * Institute for System Programming of the Russian Academy of Sciences
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * ee the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <linux/ldv/common.h>
#include <verifier/common.h>
#include <verifier/nondet.h>


struct sk_buff;
struct sk_buff *ldv_sk_buff;

/* MODEL_FUNC Allocates memory for  request. */
struct sk_buff *ldv_net_alloc_sk_buff(unsigned size, gfp_t priority)
{
	/* NOTE Choose an arbitrary memory location. */
	void *arbitrary_memory = ldv_undef_ptr();
	/* NOTE If memory is not available. */
	if (!arbitrary_memory) {
		/* NOTE Failed to allocate memory. */
		return arbitrary_memory;
	}
	/* NOTE The memory is successfully allocated. */
	if (ldv_nondet_int()) {
		ldv_sk_buff = arbitrary_memory;
	}
	return arbitrary_memory;
}

int ldv_sk_buff_put = 0;

void ldv_net_sk_buff_put(struct sk_buff *sk)
{
	if (sk == ldv_sk_buff) {
		ldv_sk_buff_put = 1;
	}	
}

void ldv_copy_from_user(struct sk_buff *sk, void *from, unsigned long n)
{
	/* ASSERT The put should be called before. */
	if (sk == ldv_sk_buff) {
		ldv_assert("linux:net::skb_put flag", ldv_sk_buff_put == 1);
	}
}

