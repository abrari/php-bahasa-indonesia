#!/usr/local/bin/php
<?phpid

/** @file phar.php
 * @ingroup Phar
 * @brief class CLICommand
 * @author  Marcus Boerger
 * @date    2007 - 2008
 *
 * Phar Command
 */

jika (!extension_loaded('phar'))
{
	jika (!class_exists('PHP_Archive', 0)) {
		tampil "Neither Extension Phar nor class PHP_Archive are available.\n";
		keluar(1);
	}
	jika (!in_array('phar', stream_get_wrappers())) {
		stream_wrapper_register('phar', 'PHP_Archive');
	}
	jika (!class_exists('Phar',0)) {
		butuh 'phar://'.__FILE__.'/phar.inc';
	}
}

untuksetiap(larik("SPL", "Reflection") sebagai $ext)
{
	jika (!extension_loaded($ext)) {
		tampil "$argv[0] requires PHP extension $ext.\n";
		keluar(1);
	}
}

fungsi command_include($file)
{
	$file = 'phar://' . __FILE__ . '/' . $file;
	jika (file_exists($file)) {
		sertakan ($file);
	}
}

fungsi command_autoload($classname)
{
	command_include(strtolower($classname) . '.inc');
}

Phar::mapPhar();

spl_autoload_register('command_autoload');

PharCommand baru ($argc, $argv);

__HALT_COMPILER();

?>