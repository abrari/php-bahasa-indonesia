<?phpid

$web = '000';

jika (in_array('phar', stream_get_wrappers()) && class_exists('Phar', 0)) {
    Phar::interceptFileFuncs();
    set_include_path('phar://' . __FILE__ . PATH_SEPARATOR . get_include_path());
    Phar::webPhar(null, $web);
    sertakan 'phar://' . __FILE__ . '/' . Extract_Phar::START;
    kembalikan;
}

jika (@(diset($_SERVER['REQUEST_URI']) && diset($_SERVER['REQUEST_METHOD']) && ($_SERVER['REQUEST_METHOD'] == 'GET' || $_SERVER['REQUEST_METHOD'] == 'POST'))) {
    Extract_Phar::go(benar);
    $mimes = larik(
        'phps' => 2,
        'c' => 'text/plain',
        'cc' => 'text/plain',
        'cpp' => 'text/plain',
        'c++' => 'text/plain',
        'dtd' => 'text/plain',
        'h' => 'text/plain',
        'log' => 'text/plain',
        'rng' => 'text/plain',
        'txt' => 'text/plain',
        'xsd' => 'text/plain',
        'php' => 1,
        'inc' => 1,
        'avi' => 'video/avi',
        'bmp' => 'image/bmp',
        'css' => 'text/css',
        'gif' => 'image/gif',
        'htm' => 'text/html',
        'html' => 'text/html',
        'htmls' => 'text/html',
        'ico' => 'image/x-ico',
        'jpe' => 'image/jpeg',
        'jpg' => 'image/jpeg',
        'jpeg' => 'image/jpeg',
        'js' => 'application/x-javascript',
        'midi' => 'audio/midi',
        'mid' => 'audio/midi',
        'mod' => 'audio/mod',
        'mov' => 'movie/quicktime',
        'mp3' => 'audio/mp3',
        'mpg' => 'video/mpeg',
        'mpeg' => 'video/mpeg',
        'pdf' => 'application/pdf',
        'png' => 'image/png',
        'swf' => 'application/shockwave-flash',
        'tif' => 'image/tiff',
        'tiff' => 'image/tiff',
        'wav' => 'audio/wav',
        'xbm' => 'image/xbm',
        'xml' => 'text/xml',
       );

    header("Cache-Control: no-cache, must-revalidate");
    header("Pragma: no-cache");

    $basename = basename(__FILE__);
    jika (!strpos($_SERVER['REQUEST_URI'], $basename)) {
        chdir(Extract_Phar::$temp);
        sertakan $web;
        kembalikan;
    }
    $pt = substr($_SERVER['REQUEST_URI'], strpos($_SERVER['REQUEST_URI'], $basename) + strlen($basename));
    jika (!$pt || $pt == '/') {
        $pt = $web;
        header('HTTP/1.1 301 Moved Permanently');
        header('Location: ' . $_SERVER['REQUEST_URI'] . '/' . $pt);
        keluar;
    }
    $a = realpath(Extract_Phar::$temp . DIRECTORY_SEPARATOR . $pt);
    jika (!$a || strlen(dirname($a)) < strlen(Extract_Phar::$temp)) {
        header('HTTP/1.0 404 Not Found');
        tampil "<html>\n <head>\n  <title>File Not Found<title>\n </head>\n <body>\n  <h1>404 - File ", $pt, " Not Found</h1>\n </body>\n</html>";
        keluar;
    }
    $b = pathinfo($a);
    jika (!diset($b['extension'])) {
        header('Content-Type: text/plain');
        header('Content-Length: ' . filesize($a));
        readfile($a);
        keluar;
    }
    jika (diset($mimes[$b['extension']])) {
        jika ($mimes[$b['extension']] === 1) {
            sertakan $a;
            keluar;
        }
        jika ($mimes[$b['extension']] === 2) {
            highlight_file($a);
            keluar;
        }
        header('Content-Type: ' .$mimes[$b['extension']]);
        header('Content-Length: ' . filesize($a));
        readfile($a);
        keluar;
    }
}

kelas Extract_Phar
{
    variabel publik statis $temp;
    variabel publik statis $origdir;
    konstanta publik GZ = 0x1000;
    konstanta publik BZ2 = 0x2000;
    konstanta publik MASK = 0x3000;
    konstanta publik START = 'index.php';
    konstanta publik LEN = XXXX;

    fungsi publik statis go($return  = salah)
    {
        $fp = fopen(__FILE__, 'rb');
        fseek($fp, diri::LEN);
        $L = unpack('V', $a = (binary)fread($fp, 4));
        $m = (binary)'';

        kerjakan {
            $read = 8192;
            jika ($L[1] - strlen($m) < 8192) {
                $read = $L[1] - strlen($m);
            }
            $last = (binary)fread($fp, $read);
            $m .= $last;
        } selama (strlen($last) && strlen($m) < $L[1]);

        jika (strlen($m) < $L[1]) {
            mati('ERROR: manifest length read was "' . 
                strlen($m) .'" should be "' .
                $L[1] . '"');
        }

        $info = diri::_unpack($m);
        $f = $info['c'];

        jika ($f & diri::GZ) {
            jika (!function_exists('gzinflate')) {
                mati('Error: zlib extension is not enabled -' .
                    ' gzinflate() function needed for zlib-compressed .phars');
            }
        }

        jika ($f & diri::BZ2) {
            jika (!function_exists('bzdecompress')) {
                mati('Error: bzip2 extension is not enabled -' .
                    ' bzdecompress() function needed for bz2-compressed .phars');
            }
        }

        $temp = diri::tmpdir();

        jika (!$temp || !is_writable($temp)) {
            $sessionpath = session_save_path();
            jika (strpos ($sessionpath, ";") !== salah)
                $sessionpath = substr ($sessionpath, strpos ($sessionpath, ";")+1);
            jika (!file_exists($sessionpath) || !is_dir($sessionpath)) {
                mati('Could not locate temporary directory to extract phar');
            }
            $temp = $sessionpath;
        }

        $temp .= '/pharextract/'.basename(__FILE__, '.phar');
        diri::$temp = $temp;
        diri::$origdir = getcwd();
        @mkdir($temp, 0777, benar);
        $temp = realpath($temp);

        jika (!file_exists($temp . DIRECTORY_SEPARATOR . md5_file(__FILE__))) {
            diri::_removeTmpFiles($temp, getcwd());
            @mkdir($temp, 0777, benar);
            @file_put_contents($temp . '/' . md5_file(__FILE__), '');

            untuksetiap ($info['m'] sebagai $path => $file) {
                $a = !file_exists(dirname($temp . '/' . $path));
                @mkdir(dirname($temp . '/' . $path), 0777, benar);
                clearstatcache();

                jika ($path[strlen($path) - 1] == '/') {
                    @mkdir($temp . '/' . $path, 0777);
                } selainnya {
                    file_put_contents($temp . '/' . $path, diri::extractFile($path, $file, $fp));
                    @chmod($temp . '/' . $path, 0666);
                }
            }
        }

        chdir($temp);

        jika (!$return) {
            sertakan diri::START;
        }
    }

    fungsi publik statis tmpdir()
    {
        jika (strpos(PHP_OS, 'WIN') !== salah) {
            jika ($var = getenv('TMP') ? getenv('TMP') : getenv('TEMP')) {
                kembalikan $var;
            }
            jika (is_dir('/temp') || mkdir('/temp')) {
                kembalikan realpath('/temp');
            }
            kembalikan salah;
        }
        jika ($var = getenv('TMPDIR')) {
            kembalikan $var;
        }
        kembalikan realpath('/tmp');
    }

    fungsi publik statis _unpack($m)
    {
        $info = unpack('V', substr($m, 0, 4));
        // skip API version, phar flags, alias, metadata
        $l = unpack('V', substr($m, 10, 4));
        $m = substr($m, 14 + $l[1]);
        $s = unpack('V', substr($m, 0, 4));
        $o = 0;
        $start = 4 + $s[1];
        $ret['c'] = 0;

        untuk ($i = 0; $i < $info[1]; $i++) {
            // length of the file name
            $len = unpack('V', substr($m, $start, 4));
            $start += 4;
            // file name
            $savepath = substr($m, $start, $len[1]);
            $start += $len[1];
            // retrieve manifest data:
            // 0 = size, 1 = timestamp, 2 = compressed size, 3 = crc32, 4 = flags
            // 5 = metadata length
            $ret['m'][$savepath] = array_values(unpack('Va/Vb/Vc/Vd/Ve/Vf', substr($m, $start, 24)));
            $ret['m'][$savepath][3] = sprintf('%u', $ret['m'][$savepath][3]
                & 0xffffffff);
            $ret['m'][$savepath][7] = $o;
            $o += $ret['m'][$savepath][2];
            $start += 24 + $ret['m'][$savepath][5];
            $ret['c'] |= $ret['m'][$savepath][4] & diri::MASK;
        }
        kembalikan $ret;
    }

    fungsi publik statis extractFile($path, $entry, $fp)
    {
        $data = '';
        $c = $entry[2];

        selama ($c) {
            jika ($c < 8192) {
                $data .= @fread($fp, $c);
                $c = 0;
            } selainnya {
                $c -= 8192;
                $data .= @fread($fp, 8192);
            }
        }

        jika ($entry[4] & diri::GZ) {
            $data = gzinflate($data);
        } selainnya jika ($entry[4] & diri::BZ2) {
            $data = bzdecompress($data);
        }

        jika (strlen($data) != $entry[0]) {
            mati("Invalid internal .phar file (size error " . strlen($data) . " != " .
                $stat[7] . ")");
        }

        jika ($entry[3] != sprintf("%u", crc32((binary)$data) & 0xffffffff)) {
            mati("Invalid internal .phar file (checksum error)");
        }

        kembalikan $data;
    }

    fungsi publik statis _removeTmpFiles($temp, $origdir)
    {
        chdir($temp);

        untuksetiap (glob('*') sebagai $f) {
            jika (file_exists($f)) {
                is_dir($f) ? @rmdir($f) : @unlink($f);
                jika (file_exists($f) && is_dir($f)) {
                    diri::_removeTmpFiles($f, getcwd());
                }
            }
        }

        @rmdir($temp);
        clearstatcache();
        chdir($origdir);
    }
}
