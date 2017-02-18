# PocketMine-C ChunkUtils extension

This extension provides a C implementation of the [ChunkUtils class](https://github.com/pmmp/PocketMine-MP/blob/master/src/pocketmine/level/format/io/ChunkUtils.php) in PocketMine-MP. 
It is intended as a drop-in performance enhancement if the user is using Anvil-formatted worlds (Minecraft PC worlds).

### Why?
The PHP implementations of the chunk data conversion methods are (relatively) extremely slow. This extension provides chunk load time enhancements of up to 10-200x faster per chunk loaded or unloaded.
### Installation
__Requirements__
- PHP >=7.0
- PocketMine-MP 1.6.2dev-3.0.0-ALPHA3 or later

This extension has been tested and developed on Windows only, support is **not yet** available for other platforms. 

To compile for Windows, you'll need to add the source to your ext source folder in your PHP source and compile PHP with the `--enable-pocketmine_chunkutils` option.
No core changes are needed to PocketMine-MP, simply enable the extension and run the server. With Anvil worlds, you should see a vast performance improvement.
