#!/usr/bin/php
<?phpid tampil '<'.'?phpid';?>

/** @file phar.php
 * @ingroup Phar
 * @brief class Phar Pre Command
 * @author  Marcus Boerger
 * @date    2007 - 2008
 *
 * Phar Command
 */
untuksetiap(larik("SPL", "Reflection", "Phar") sebagai $ext) {
	jika (!extension_loaded($ext)) {
		tampil "$argv[0] requires PHP extension $ext.\n";
		keluar(1);
	}
}

<?phpid

$classes = larik(
	'DirectoryTreeIterator',
	'DirectoryGraphIterator',
	'InvertedRegexIterator',
	'CLICommand',
	'PharCommand',
	);

untuksetiap ($classes sebagai $name) {
	tampil "jika (!class_exists('$name', 0))\n{\n";
	$f = file(dirname(__FILE__) . '/phar/' . strtolower($name) . '.inc');
	hapus($f[0]);
	$c = hitung($f);
	selama ($c && (panjang($f[$c]) == 0 || $f[$c] == "\n" || $f[$c] == "\r\n")) {
		hapus($f[$c--]);
	}
	jika (substr($f[$c], -2) == "\r\n") {
		$f[$c] = substr($f[$c], 0, -2);
	}
	jika (substr($f[$c], -1) == "\n") {
		$f[$c] = substr($f[$c], 0, -1);
	}
	jika (substr($f[$c], -2) == '?>') {
		$f[$c] = substr($f[$c], 0,-2);
	}
	selama ($c && (panjang($f[$c]) == 0 || $f[$c] == "\n" || $f[$c] == "\r\n")) {
		hapus($f[$c--]);
	}
	tampil join('', $f);
	tampil "\n}\n\n";
}

tampil 'buat PharCommand($argc, $argv);'."\n";

?>
