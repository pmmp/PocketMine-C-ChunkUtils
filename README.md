# PocketMine-C ChunkUtils extension
[![Travis CI build status](https://travis-ci.org/dktapps/PocketMine-C-ChunkUtils.svg?branch=master)](https://travis-ci.org/dktapps/PocketMine-C-ChunkUtils)

This extension provides a C implementation of the [ChunkUtils class](https://github.com/pmmp/PocketMine-MP/blob/master/src/pocketmine/level/format/io/ChunkUtils.php) in PocketMine-MP. 
It is intended as a drop-in performance enhancement if the user is using Anvil-formatted worlds (Minecraft PC worlds).

### Why?
The PHP implementations of the chunk data conversion methods are (relatively) extremely slow. This extension provides chunk load time enhancements of up to 10-200x faster per chunk loaded or unloaded.
### Installation
__Requirements__
- PHP >=7.0
- PocketMine-MP 1.6.2dev-3.0.0-ALPHA3 or later 

To compile, you'll need to add the source to your ext source folder in your PHP source and compile PHP with the `--enable-pocketmine-chunkutils` option.
No core changes are needed to PocketMine-MP, simply enable the extension and run the server. With Anvil worlds, you should see a vast performance improvement.

## FAQ
- I'm using PMAnvil and/or McRegion worlds, will this extension provide a performance boost to those?

No. This extension provides a fast C replacement for a class which is used primarily by the Anvil chunk I/O provider. This extension simply accelerates Anvil chunk conversion to make PC worlds usable.
