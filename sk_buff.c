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

#include <linux/skbuff.h>


struct sk_buff;
struct sk_buff *ldv_sk_buff;
unsigned int ldv_sk_buff_data_len;
unsigned char *ldv_sk_buff_data;

/* MODEL_FUNC Allocates memory for  request. */
struct sk_buff *ldv_net_alloc_sk_buff(unsigned int size)
{
	/* NOTE Choose an arbitrary memory location. */
	struct sk_buff *skb = ldv_undef_ptr();
	/* NOTE If memory is not available. */
	if (!skb) {
		/* NOTE Failed to allocate memory. */
		return skb;
	}
	/* NOTE The memory is successfully allocated. */
	if (ldv_undef_int()) {
		ldv_sk_buff = skb;
		skb->data = ldv_undef_ptr();
		ldv_assume(skb->data);

		skb->len = size;
		ldv_sk_buff_data_len = size;
		ldv_sk_buff_data = skb->data;
	}

	return arbitrary_memory;
}

int ldv_sk_buff_put = 0;

void ldv_net_sk_buff_put(struct sk_buff *sk)
{
	if (sk == ldv_sk_buff) {
		ldv_sk_buff_put += 1;
	}	
}

int ldv_copy_from_user = 0;

void ldv_copy_from_user(void *to, void *from, unsigned long n)
{
	if (ldv_sk_buff_data <= to && to < ldv_sk_buff_data + ldv_sk_buff_data_len) {
		ldv_copy_from_user += 1;
		/* ASSERT The number of call to copy_from_user calls should be in a relation with skb_put calls. */
		ldv_assert("linux:net::skb_put flag", ldv_copy_from_user <= ldv_sk_buff_put);
	}
}
