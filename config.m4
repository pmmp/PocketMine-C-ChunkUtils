

PHP_ARG_ENABLE(pocketmine-chunkutils, whether to enable pocketmine_chunkutils support,
[  --enable-pocketmine-chunkutils           enable PocketMine-C ChunkUtils])

if test "$PHP_POCKETMINE_CHUNKUTILS" != "no"; then
  PHP_NEW_EXTENSION(pocketmine_chunkutils, pocketmine_chunkutils.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
