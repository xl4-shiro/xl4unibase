/*
 * Excelfore Universal Base Library
 * Copyright (C) 2019 Excelfore Corporation (https://excelfore.com)
 *
 * This file is part of Excelfore-unibase.
 *
 * Excelfore-unibase is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Excelfore-unibase is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Excelfore-unibase.  If not, see
 * <https://www.gnu.org/licenses/old-licenses/gpl-2.0.html>.
 */
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <cmocka.h>
#include "unibase.h"

static void test_smac2bmac(void **state)
{
	uint8_t bdata[32];
	uint8_t *bdb;

	bdata[6]=0xff;
	bdb=ub_smac2bmac("a1:b2:c3:D4:e5:F6", bdata);
	assert_int_equal(bdb[0],0xa1);
	assert_int_equal(bdb[1],0xb2);
	assert_int_equal(bdb[2],0xc3);
	assert_int_equal(bdb[3],0xd4);
	assert_int_equal(bdb[4],0xe5);
	assert_int_equal(bdb[5],0xf6);
	assert_int_equal(bdb[6],0xff);
}

static void test_ssid2bsid(void **state)
{
	uint8_t bdata[32];
	uint8_t *bdb;

	bdata[8]=0xff;
	bdb=ub_ssid2bsid("11:a1:b2:c3:D4:e5:F6:aA", bdata);
	assert_int_equal(bdb[0],0x11);
	assert_int_equal(bdb[1],0xa1);
	assert_int_equal(bdb[2],0xb2);
	assert_int_equal(bdb[3],0xc3);
	assert_int_equal(bdb[4],0xd4);
	assert_int_equal(bdb[5],0xe5);
	assert_int_equal(bdb[6],0xf6);
	assert_int_equal(bdb[7],0xaa);
	assert_int_equal(bdb[8],0xff);
}

static int setup(void **state)
{
	return 0;
}

static int teardown(void **state)
{
	return 0;
}

int main(int argc, char *argv[])
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_smac2bmac),
		cmocka_unit_test(test_ssid2bsid),
	};

	return cmocka_run_group_tests(tests, setup, teardown);
}
