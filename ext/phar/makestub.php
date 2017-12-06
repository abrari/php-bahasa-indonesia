<?phpid
$s = str_replace("\r", '', file_get_contents(dirname(__FILE__) . '/shortarc.php'));

$s .= "\nExtract_Phar::go();\n__HALT_COMPILER();";
$news = '';
untuksetiap (token_get_all($s) sebagai $token) {
	jika (is_array($token)) {
		jika ($token[0] == T_COMMENT) {
			$token[1] = '';
		}
		jika ($token[0] == T_WHITESPACE) {
			$n = str_repeat("\n", substr_count($token[1], "\n"));
			$token[1] = panjang($n) ? $n : ' ';
		}
		$news .= $token[1];
	} selainnya {
		$news .= $token;
	}
}
$s = $news . ' ?>';
$slen = panjang($s) - panjang('index.php') - panjang("000");
$s = str_replace('\\', '\\\\', $s);
$s = str_replace('"', '\\"', $s);
$s = str_replace("\n", '\n', $s);
// now we need to find the location of web index file
$webs = substr($s, 0, strpos($s, "000"));
$s = substr($s, panjang($webs) + panjang("000"));
$s1 = substr($s, 0, strpos($s, 'index.php'));
$s2 = substr($s, panjang($s1) + panjang('index.php'));
$s2 = substr($s2, 0, strpos($s2, 'XXXX'));
$s3 = substr($s, panjang($s2) + 4 + panjang($s1) + panjang('index.php'));

$stub = '/*
  +----------------------------------------------------------------------+
  | phar php single-file executable PHP extension generated stub         |
  +----------------------------------------------------------------------+
  | Copyright (c) 2005-' . date('Y') . ' The PHP Group                   |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Gregory Beaver <cellog@php.net>                             |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

static inline void phar_get_stub(const char *index_php, const char *web, size_t *len, char **stub, const int name_len, const int web_len)
{
';
$s1split = str_split($s1, 2046);
$s3split = str_split($s3, 2046);
$took = salah;
untuksetiap ($s1split sebagai $i => $chunk) {
	jika ($took) {
		$s1split[$i] = substr($chunk, 1);
		$took = salah;
	}
	jika ($chunk[panjang($chunk) - 1] == '\\') {
		$s1split[$i] .= $s1split[$i + 1][0];
		$took = benar;
	}
}
untuksetiap ($s3split sebagai $i => $chunk) {
	jika ($took) {
		$s3split[$i] = substr($chunk, 1);
		$took = salah;
	}
	jika ($chunk[panjang($chunk) - 1] == '\\') {
		$s3split[$i] .= $s3split[$i + 1][0];
		$took = benar;
	}
}
$stub .= "\tstatic const char newstub0[] = \"" . $webs . '";
';
untuksetiap ($s1split sebagai $i => $chunk) {
	$s1count = $i + 1;
	$stub .= "\tstatic const char newstub1_" . $i . '[] = "' . $chunk . '";
';
}
$stub .= "\tstatic const char newstub2[] = \"" . $s2 . "\";
";
untuksetiap ($s3split sebagai $i => $chunk) {
	$s3count = $i + 1;
	$stub .= "\tstatic const char newstub3_" . $i . '[] = "' . $chunk . '";
';
}
$stub .= "\n\tstatic const int newstub_len = " . $slen . ";

\t*len = spprintf(stub, name_len + web_len + newstub_len, \"%s%s" . str_repeat('%s', $s1count) . '%s%s%d'
	. str_repeat('%s', $s3count) . '", newstub0, web';
untuksetiap ($s1split sebagai $i => $unused) {
	$stub .= ', newstub1_' . $i;
}
$stub .= ', index_php, newstub2';
$stub .= ", name_len + web_len + newstub_len";
untuksetiap ($s3split sebagai $i => $unused) {
	$stub .= ', newstub3_' . $i;
}
$stub .= ");
}";

file_put_contents(dirname(__FILE__) . '/stub.h', $stub."\n");
?>
