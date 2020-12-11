/*
 * PocketMine-C ChunkUtils extension
 * Copyright (C) 2017 Dylan K. Taylor <https://github.com/dktapps/PocketMine-C-ChunkUtils>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
*/


/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pocketmine_chunkutils.h"
#include "zend_interfaces.h"

/* True global resources - no need for thread safety here */
static int le_pocketmine_chunkutils;

zend_class_entry *chunkutils_ce;


ZEND_BEGIN_ARG_INFO_EX(arginfo_reorder_byte_array, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, byte_array, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_reorder_nibble_array, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, nibble_array, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, common_value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_convert_biome_colors, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, color_array, 0)
ZEND_END_ARG_INFO()

zend_function_entry chunkutils_class_methods[] = {
	PHP_ME(ChunkUtils, reorderByteArray, arginfo_reorder_byte_array, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(ChunkUtils, reorderNibbleArray, arginfo_reorder_nibble_array, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(ChunkUtils, convertBiomeColors, arginfo_convert_biome_colors, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_FE_END
};


/* {{{ proto string ChunkUtils::reorderByteArray(string $byte_array)
		Re-orders a 4096-byte array (YZX -> XZY and vice versa) */
PHP_METHOD(ChunkUtils, reorderByteArray) {
	unsigned char *byte_array;
	size_t len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|z",
		&byte_array, &len) != SUCCESS) {
		return;
	}

	unsigned char result[4096] = { '\0' };

	unsigned short x, y, z, zm, ym;
	unsigned short i = 0;

	for (x = 0; x < 16; x++) {
		zm = x + 256;
		for (z = x; z < zm; z += 16) {
			ym = z + 4096;
			for (y = z; y < ym; y += 256) {
				result[i] = byte_array[y];
				i++;
			}
		}
	}

	RETURN_STRINGL(result, 4096)
} /* }}} */


/* {{{ proto string ChunkUtils::reorderNibbleArray(string $nibble_array[, string $common_value])
		Re-orders a 2048-byte nibble array (YZX -> XZY and vice versa) */
PHP_METHOD(ChunkUtils, reorderNibbleArray) {
	unsigned char *nibble_array;
	size_t len, c_len;
	unsigned char *common_value = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|s!",
		&nibble_array, &len, &common_value, &c_len) != SUCCESS) {
		return;
	}

	unsigned char result[2048] = { '\0' };

	unsigned char x, y, z;
	unsigned short i = 0;

	unsigned short zx;
	unsigned char byte1, byte2;

	for (x = 0; x < 8; x++) {
		for (z = 0; z < 16; z++) {
			zx = ((z << 3) | x);
			for (y = 0; y < 8; y++) {
				byte1 = (unsigned char)nibble_array[(y << 8) | zx];
				byte2 = (unsigned char)nibble_array[(y << 8) | zx | 0x80];

				result[i] = ((byte2 << 4) | (byte1 & 0x0f));
				result[i | 0x80] = ((byte1 >> 4) | (byte2 & 0xf0));
				i++;
			}
		}
		i += 128;
	}

	RETURN_STRINGL(result, 2048)
} /* }}} */


/* {{{ proto string ChunkUtils::convertBiomeColors(array $color_array)
		Converts pre-MCPE-1.0 biome color array to biome ID array. */
PHP_METHOD(ChunkUtils, convertBiomeColors) {

	zval *old_colors_zv = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &old_colors_zv) != SUCCESS) {
		return;
	}

	HashTable *old_colors_ht;
	unsigned char converted_ids[256] = { '\0' };

	old_colors_ht = Z_ARRVAL_P(old_colors_zv);

	unsigned short i = 0;
	
	Bucket *b = old_colors_ht->arData;
	for (i = 0; i < old_colors_ht->nNumUsed; i++) {
		unsigned int color = (unsigned int) Z_LVAL(b[i].val);
		converted_ids[i] = (unsigned char)((color >> 24) & 0xff);
	}

	RETURN_STRINGL(converted_ids, 256);
} /* }}} */


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pocketmine_chunkutils)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "pocketmine\\level\\format\\io\\ChunkUtils", chunkutils_class_methods);

	chunkutils_ce = zend_register_internal_class(&ce);

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pocketmine_chunkutils)
{
	return SUCCESS;
}
/* }}} */


/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pocketmine_chunkutils)
{
#if defined(COMPILE_DL_POCKETMINE_CHUNKUTILS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pocketmine_chunkutils)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pocketmine_chunkutils support", "enabled");
	php_info_print_table_end();
}
/* }}} */


/* {{{ pocketmine_chunkutils_module_entry
 */
zend_module_entry pocketmine_chunkutils_module_entry = {
	STANDARD_MODULE_HEADER,
	"pocketmine_chunkutils",
	NULL,
	PHP_MINIT(pocketmine_chunkutils),
	PHP_MSHUTDOWN(pocketmine_chunkutils),
	NULL,
	NULL,
	PHP_MINFO(pocketmine_chunkutils),
	PHP_POCKETMINE_CHUNKUTILS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */


#ifdef COMPILE_DL_POCKETMINE_CHUNKUTILS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(pocketmine_chunkutils)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
