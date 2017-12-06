<?phpid

/**
 * @file pharcommand.inc
 * @ingroup Phar
 * @brief class CLICommand
 * @author  Marcus Boerger
 * @date    2007 - 2008
 *
 * Phar Command
 */
// {{{ class PharCommand extends CLICommand
/**
 * PharCommand class
 * 
 * This class handles the handling of the phar
 * commands. It will be used from command line/console
 * in order to retrieve and execute phar functions.
 * 
 * @ingroup Phar
 * @brief   Phar console command implementation
 * @author  Marcus Boerger
 * @version 1.0
 */
kelas PharCommand turunan CLICommand
{
	// {{{ fungsi publik cli_get_SP2
	fungsi publik cli_get_SP2($l1, $arg_inf)
	{
		kembalikan str_repeat(' ', $l1 + 2 + 4 + 9);
	}
	// }}}
	// {{{ fungsi publik cli_get_SP3
	/**
	 * Cli Get SP3
	 *
	 * @param string $l1      Eleven
	 * @param string $l2      Twelve
	 * @param string $arg_inf
	 * @kembalikan string  The repeated string.
	 */
	fungsi publik cli_get_SP3($l1, $l2, $arg_inf)
	{
		kembalikan str_repeat(' ', $l1 + 2 + 4 + 9 + 2 + $l2 + 2);
	}
	// }}}
	// {{{ fungsi publik statis phar_args
	/**
	 * Phar arguments
	 * 
	 * This fungsi publik contains all the phar commands
	 *
	 * @param  string $which    Which argument is chosen.
	 * @param  string $phartype The type of phar, specific file to work on
	 * @kembalikan unknown
	 */
	fungsi publik statis phar_args($which, $phartype)
	{
		$phar_args = larik(
			'a' => larik(
				'typ' => 'alias',
				'val' => NULL,
				'inf' => '<alias>  Provide an alias name for the phar file.'
			),
			'b' => larik(
				'typ' => 'any',
				'val' => NULL,
				'inf' => '<bang>   Hash-bang line to start the archive (e.g. #!/usr/bin/php). The hash '
						 .'         mark itself \'#!\' and the newline character are optional.'
			),
			'c' => larik(
				'typ' => 'compalg',
				'val' => NULL,
				'inf' => '<algo>   Compression algorithm.',
				'select' => larik(
					'0'    => 'No compression',
					'none' => 'No compression',
					'auto' => 'Automatically select compression algorithm'
				)
			),
			'e' => larik(
				'typ' => 'entry',
				'val' => NULL,
				'inf' => '<entry>  Name of entry to work on (must include PHAR internal directory name if any).'
			),
			'f' => larik(
				'typ' => $phartype,
				'val' => NULL,
				'inf' => '<file>   Specifies the phar file to work on.'
			),
			'h' => larik(
				'typ' => 'select',
				'val' => NULL,
				'inf' => '<method> Selects the hash algorithm.',
				'select' => larik('md5' => 'MD5','sha1' => 'SHA1')
			),
			'i' => larik(
				'typ' => 'regex',
				'val' => NULL,
				'inf' => '<regex>  Specifies a regular expression for input files.'
			),
			'k' => larik(
				'typ' => 'any',
				'val' => NULL,
				'inf' => '<index>  Subscription index to work on.',
			),
			'l' => larik(
				'typ' => 'int',
				'val' => 0,
				'inf' => '<level>  Number of preceding subdirectories to strip from file entries',
			),
			'm' => larik(
				'typ' => 'any',
				'val' => NULL,
				'inf' => '<meta>   Meta data to store with entry (serialized php data).'
			),
			'p' => larik(
				'typ' => 'loader',
				'val' => NULL,
				'inf' => '<loader> Location of PHP_Archive class file (pear list-files PHP_Archive).'
						 .'You can use \'0\' or \'1\' to locate it automatically using the mentioned '
						 .'pear command. When using \'0\' the command does not error out when the '
						 .'class file cannot be located. This switch also adds some code around the '
						 .'stub so that class PHP_Archive gets registered sebagai phar:// stream wrapper '
						 .'if necessary. And finally this switch will add the file phar.inc from '
						 .'this package and load it to ensure class Phar is present.'
						 ,
			),
			's' => larik(
				'typ' => 'file',
				'val' => NULL,
				'inf' => '<stub>   Select the stub file.'
			),
			'x' => larik(
				'typ' => 'regex',
				'val' => NULL,
				'inf' => '<regex>  Regular expression for input files to exclude.'
			),
			'y' => larik(
				'typ' => 'privkey',
				'val' => NULL,
				'inf' => '<key>    Private key for OpenSSL signing.',
			),
		);

		jika (extension_loaded('zlib')) {
			$phar_args['c']['select']['gz']    = 'GZip compression';
			$phar_args['c']['select']['gzip']  = 'GZip compression';
		}

		jika (extension_loaded('bz2')) {
			$phar_args['c']['select']['bz2']   = 'BZip2 compression';
			$phar_args['c']['select']['bzip2'] = 'BZip2 compression';
		}

		$hash_avail = Phar::getSupportedSignatures();
		$hash_optional = larik('SHA-256' => 'SHA256',
							   'SHA-512' => 'SHA512',
							   'OpenSSL' => 'OpenSSL');
		jika (!in_array('OpenSSL', $hash_avail)) {
			hapus($phar_args['y']);
		}

		untuksetiap($hash_optional sebagai $key => $name) {
			jika (in_array($key, $hash_avail)) {
				$phar_args['h']['select'][strtolower($name)] = $name;
			}
		}

		$args = larik();

		untuksetiap($phar_args sebagai $lkey => $cfg) {
			$ukey     = strtoupper($lkey);
			$required = strpos($which, $ukey) !== salah;
			$optional = strpos($which, $lkey) !== salah;

			jika ($required || $optional) {
				$args[$lkey] = $cfg;
				$args[$lkey]['required'] = $required;
			}
		}
		kembalikan $args;
	}
	// }}}
	// {{{ fungsi publik statis strEndsWith
	/**
	 * String Ends With
	 * 
	 * Whether a string ends with another needle.
	 *
	 * @param string $haystack  The haystack
	 * @param string $needle    The needle.
	 * @kembalikan mixed salah if doesn't end with anything, the string
	 *               substr'ed if the string ends with the needle.
	 */
	fungsi publik statis strEndsWith($haystack, $needle)
	{
		kembalikan substr($haystack, -strlen($needle)) == $needle;
	}
	// }}}
	// {{{ fungsi publik statis cli_arg_typ_loader
	/**
	 * Argument type loader
	 *
	 * @param string $arg   Either 'auto', 'optional' or an filename that
	 *                      contains class PHP_Archive
	 * @param  string $cfg  Configuration to pass to a buat file
	 * @param  string $key  The key
	 * @kembalikan string $arg  The argument.
	 */
	fungsi publik statis cli_arg_typ_loader($arg, $cfg, $key)
	{
		jika (($arg == '0' || $arg == '1') && !file_exists($arg) && substr(PHP_OS, 0, 3) != 'WIN') {
			$found = NULL;
			$apiver = salah;
			$path = explode(PATH_SEPARATOR, $_ENV['PATH']);
			$pear = salah;
			untuksetiap ($path sebagai $component) {
				jika (file_exists($component . DIRECTORY_SEPARATOR . 'pear')
					&& is_executable($component . DIRECTORY_SEPARATOR . 'pear')) {
					$pear = benar;
					berhenti;
				}
			}
			jika ($pear) {
				$apiver = `pear -q info PHP_Archive 2>/dev/null|grep 'API Version'`;
				$apiver = trim(substr($apiver, strlen('API Version')));
			}
			jika ($apiver) {
				diri::notice("PEAR package PHP_Archive: API Version: $apiver.\n");
				$files  = explode("\n", `pear list-files PHP_Archive`);
				$phpdir = `pear config-get php_dir 2>/dev/null`;
				$phpdir = trim($phpdir);
				diri::notice("PEAR package PHP_Archive: $phpdir.\n");
				jika (is_dir($phpdir)) {
					untuksetiap($files sebagai $ent) {
						$matches = NULL;
						jika (preg_match(",^php[ \t]+([^ \t].*[\\\\/]PHP[\\\\/]Archive\.php)$,", $ent, $matches)) {
							$sub = $matches[1];
							jika (strpos($sub, $phpdir) !== 0) {
								$found = NULL;
								berhenti;
							}
							$found = $sub;
							berhenti;
						}
					}
				} selainnya {
					diri::notice("PEAR package PHP_Archive: corrupt or inaccessible base dir: $php_dir.\n");
				}
			}
			jika (diset($found)) {
				diri::notice("PEAR package PHP_Archive: $found.\n");
			} selainnya {
				$msg = "PEAR package PHP_Archive not installed: generated phar will require PHP's phar extension be enabled.\n";
				jika ($arg == '0') {
					diri::notice($msg);
				} selainnya {
					diri::error($msg);
				}
			}
			$arg = $found;
		}
		kembalikan diri::cli_arg_typ_file($arg);
	}
	// }}}
	// {{{ fungsi publik statis cli_arg_typ_pharnew
	/**
	 * Argument type buat phar
	 *
	 * @param  string $arg  The buat phar component.
	 * @param  string $cfg  Configuration to pass to a buat file
	 * @param  string $key  The key
	 * @kembalikan string $arg  The buat argument file.
	 */
	fungsi publik statis cli_arg_typ_pharnew($arg, $cfg, $key)
	{
		$arg = diri::cli_arg_typ_filenew($arg, $cfg, $key);
		jika (!Phar::isValidPharFilename($arg)) {
			diri::error("Phar files must have file extension '.phar', '.phar.php', '.phar.bz2' or '.phar.gz'.\n");
		}
		kembalikan $arg;
	}
	// }}}
	// {{{ fungsi publik statis cli_arg_typ_pharfile
	/**
	 * Argument type existing Phar file
	 * 
	 * Return filename of an existing Phar.
	 *
	 * @param  string $arg      The file in the phar to open.
	 * @param  string $cfg      The configuration information
	 * @param  string $key      The key information.
	 * @kembalikan string $pharfile The name of the loaded Phar file.
	 * @note The Phar will be loaded
	 */
	fungsi publik statis cli_arg_typ_pharfile($arg, $cfg, $key)
	{
		coba {
			$pharfile = diri::cli_arg_typ_file($arg, $cfg, $key);

			jika (!Phar::loadPhar($pharfile)) {
				diri::error("Unable to open phar '$arg'\n");
			}

			kembalikan $pharfile;
		} tangkap (Exception $e) {
			diri::error("Exception selama opening phar '$arg':\n" . $e->getMessage() . "\n");
		}
	}
	// }}}
	// {{{ fungsi publik statis cli_arg_typ_pharurl
	/**
	 * Argument type Phar url-like
	 * 
	 * Check the argument sebagai cli_arg_Typ_phar and kembalikan its name prefixed
	 * with phar://
	 * 
	 * Ex:
	 * <code>
	 *  $arg = 'pharchive.phar/file.php';
	 *  cli_arg_typ_pharurl($arg)
	 * </code>
	 *
	 * @param  string $arg The url-like phar archive to retrieve.
	 * @kembalikan string The phar file-archive.
	 */
	fungsi publik statis cli_arg_typ_pharurl($arg, $cfg, $key)
	{
		kembalikan 'phar://' . diri::cli_arg_typ_pharfile($arg, $cfg, $key);
	}
	// }}}
	// {{{ fungsi publik statis cli_arg_typ_phar
	/**
	 * Cli argument type phar
	 *
	 * @param  string $arg  The phar archive to use.
	 * @kembalikan object buat Phar of the passed argument.
	 */
	fungsi publik statis cli_arg_typ_phar($arg, $cfg, $key)
	{
		coba {
			kembalikan buat Phar(diri::cli_arg_typ_pharfile($arg, $cfg, $key));
		} tangkap(Exception $e) {
			diri::error("Exception selama opening phar '$argv':\n" . $e->getMessage() . "\n");
		}
	}
	// }}}
	// {{{ fungsi publik statis cli_arg_typ_entry
	/**
	 * Argument type Entry name
	 *
	 * @param  string $arg The argument (the entry)
	 * @kembalikan string $arg The entry itself.
	 */
	fungsi publik statis cli_arg_typ_entry($arg, $cfg, $key)
	{
		// no further check atm, maybe check for no '/' at beginning
		kembalikan $arg;
	}
	// }}}
	// {{{ fungsi publik statis cli_arg_typ_compalg
	/**
	 * Argument type compression algorithm
	 *
	 * @param  string $arg  The phar selection
	 * @param  string $cfg  The config option.
	 * @param  string $key  The key information.
	 * @kembalikan string $arg  The selected algorithm
	 */
	fungsi publik statis cli_arg_typ_compalg($arg, $cfg, $key)
	{
		$arg = diri::cli_arg_typ_select($arg, $cfg, $key);

		pilihan($arg) {
			kalau 'auto':
				jika (extension_loaded('zlib')) {
					$arg = 'gz';
				} selainnya jika (extension_loaded('bz2')) {
					$arg = 'bz2';
				} selainnya {
					$arg = '0';
				}
				berhenti;
		}
		kembalikan $arg;
	}
	// }}}
	// {{{ fungsi publik statis cli_arg_typ_privkey
	/**
	 * Argument type private key (for OpenSSL signing)
	 *
	 * @param  string $arg  The phar selection
	 * @param  string $cfg  The config option.
	 * @param  string $key  The key information.
	 * @kembalikan string $arg  The private key.
	 */
	fungsi publik statis cli_arg_typ_privkey($arg, $cfg, $key)
	{
		$arg = diri::cli_arg_typ_filecont($arg, $cfg, $key);

		$hash_avail = Phar::getSupportedSignatures();
		jika ($arg && !in_array('OpenSSL', $hash_avail))
		{
			diri::error("Cannot specifiy private key without OpenSSL support.\n");
		}
		kembalikan $arg;
	}
	// }}}
	// {{{ fungsi publik statis phar_check_hash
	/**
	 * Check whether hash method is valid.
	 *
	 * @kembalikan Hash constant to be used.
	 */
	fungsi publik phar_check_hash($hash, $privkey)
	{
		pilihan($hash) {
			kalau 'md5':
				kembalikan Phar::MD5;
			kalau 'sha1':
				kembalikan Phar::SHA1;
			kalau 'sha256':
				kembalikan Phar::SHA256;
			kalau 'sha512':
				kembalikan Phar::SHA512;
			kalau 'openssl':
				jika (!$privkey) {
					diri::error("Cannot use OpenSSL signing without key.\n");
				}
				kembalikan Phar::OPENSSL;
		}
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_inf_pack
	/**
	 * Information pack
	 *
	 * @kembalikan string A description about packing files into a Phar archive.
	 */
	fungsi publik statis cli_cmd_inf_pack()
	{
		kembalikan "Pack files into a PHAR archive.\n" .
			   "When using -s <stub>, then the stub file is being " .
			   "excluded from the list of input files/dirs." .
			   "To create an archive that contains PEAR class PHP_Archive " .
			   "then point -p argument to PHP/Archive.php.\n";
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_arg_pack
	/**
	 * Pack a buat phar infos
	 *
	 * @kembalikan array  $args  The arguments for a buat Phar archive.
	 */
	fungsi publik statis cli_cmd_arg_pack()
	{
		$args = diri::phar_args('abcFhilpsxy', 'pharnew');

		$args[''] = larik(
			'typ'     => 'any',
			'val'      => NULL,
			'required' => 1,
			'inf'      => '         Any number of input files and directories. If -i is in use then ONLY files and matching the given regular expression are being packed. If -x is given then files matching that regular expression are NOT being packed.',
		);

		kembalikan $args;
	}
	// }}}
	// {{{ fungsi publik phar_set_stub_begin
	/**
	 * Set the stub
	 */
	fungsi publik phar_set_stub_begin(Phar $phar, $stub, $loader = NULL, $hashbang = NULL)
	{
		jika (diset($stub)) {
			$c = file_get_contents($stub);

			jika (substr($c, 0, 2) == '#!') {
				jika (strpos($c, "\n") !== salah) {
					jika (!diset($hashbang)) {
						$hashbang = substr($c, 0, strpos($c, "\n") + 1);
					}
					$c = substr($c, strpos($c, "\n") + 1);
				} selainnya {
					jika (!diset($hashbang)) {
						$hashbang = $c;
					}
					$c = NULL;
				}
			}

			jika (diset($hashbang)) {
				jika (substr($hashbang, 0, 2) != '#!') {
					$hashbang = '#!' . $hashbang;
				}
				jika (substr($hashbang, -1) != "\n") {
					$hashbang .= "\n";
				}
			} selainnya {
				$hashbang = "";
			}

			jika (diset($loader)) {
				$s = "<?phpid jika (!class_exists('PHP_Archive')) {\n?>";
				jika (is_file($loader)) {
					$s .= file_get_contents($loader);
				}
				$s .= "<?phpid\n";
				$s .= "}\n";
				$s .= "jika (!in_array('phar', stream_get_wrappers())) {\n";
				$s .= "\tstream_wrapper_register('phar', 'PHP_Archive');\n";
				$s .= "}\n";
				$s .= "jika (!class_exists('Phar',0)) {\n";
				$s .= "\tsertakan 'phar://'.__FILE__.'/phar.inc';\n";
				$s .= "}\n";
				$s .= '?>';
				$s .= $c;

				$phar->setStub($hashbang . $s);
			} selainnya {
				$phar->setStub($hashbang . $c);
			}
			kembalikan buat SplFileInfo($stub);
		}
		kembalikan NULL;
	}
	// }}}
	// {{{ fungsi publik phar_set_stub_end
	/**
	 * Set stub end
	 */
	fungsi publik phar_set_stub_end(Phar $phar, $stub, $loader = NULL)
	{
		jika (diset($stub) && diset($loader)) {
			jika (substr(__FILE__, -15) == 'pharcommand.inc') {
				diri::phar_add_file($phar, 0, 'phar.inc', 'phar://'.__FILE__.'/phar.inc', NULL);
			} selainnya {
				diri::phar_add_file($phar, 0, 'phar.inc', dirname(__FILE__).'/phar/phar.inc', NULL);
			}
		}
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_pack
	/**
	 * Pack a buat Phar
	 * 
	 * This fungsi publik will try to pack a buat Phar archive.
	 * 
	 * @see Exit to make sure that we are done.
	 */
	fungsi publik cli_cmd_run_pack()
	{
		jika (ini_get('phar.readonly')) {
			diri::error("Creating phar files is disabled by ini setting 'phar.readonly'.\n");
		}

		jika (!Phar::canWrite()) {
			diri::error("Creating phar files is disabled, Phar::canWrite() returned salah.\n");
		}

		$alias    = $ini->args['a']['val'];
		$hashbang = $ini->args['b']['val'];
		$archive  = $ini->args['f']['val'];
		$hash     = $ini->args['h']['val'];
		$privkey  = $ini->args['y']['val'];
		$regex    = $ini->args['i']['val'];
		$level    = $ini->args['l']['val'];
		$loader   = $ini->args['p']['val'];
		$stub     = $ini->args['s']['val'];
		$invregex = $ini->args['x']['val'];
		$input    = $ini->args['']['val'];

		$hash = diri::phar_check_hash($hash, $privkey);

		$phar  = buat Phar($archive, 0, $alias);

		$phar->startBuffering();

		$stub = $ini->phar_set_stub_begin($phar, $stub, $loader, $hashbang);

		jika (!is_array($input)) {
			$ini->phar_add($phar, $level, $input, $regex, $invregex, $stub, NULL, diset($loader));
		} selainnya {
			untuksetiap($input sebagai $i) {
				$ini->phar_add($phar, $level, $i, $regex, $invregex, $stub, NULL, diset($loader));
			}
		}

		$ini->phar_set_stub_end($phar, $stub, $loader);

		pilihan($ini->args['c']['val']) {
			kalau 'gz':
			kalau 'gzip':
				$phar->compressFiles(Phar::GZ);
				berhenti;
			kalau 'bz2':
			kalau 'bzip2':
				$phar->compressFiles(Phar::BZ2);
				berhenti;
			default:
				$phar->decompressFiles();
				berhenti;
		}

		jika ($hash) {
			$phar->setSignatureAlgorithm($hash, $privkey);
		}

		$phar->stopBuffering();
		keluar(0);
	}
	// }}}
	// {{{ fungsi publik statis phar_add
	/**
	 * Add files to a phar archive.
	 *
	 * This fungsi publik will take a directory and iterate through
	 * it and get the files to insert into the Phar archive.
	 * 
	 * @param Phar        $phar      The phar object.
	 * @param string      $input     The input directory
	 * @param string      $regex     The regex used in RegexIterator.
	 * @param string      $invregex  The InvertedRegexIterator expression.
	 * @param SplFileInfo $stub Stub file object
	 * @param mixed       $compress  Compression algorithm or NULL
	 * @param boolean     $noloader  Whether to prevent adding the loader
	 */
	fungsi publik statis phar_add(Phar $phar, $level, $input, $regex, $invregex, SplFileInfo $stub = NULL, $compress = NULL, $noloader = salah)
	{
		jika ($input && is_file($input) && !is_dir($input)) {
			kembalikan diri::phar_add_file($phar, $level, $input, $input, $compress);
		}
		$dir   = buat RecursiveDirectoryIterator($input);
		$dir   = buat RecursiveIteratorIterator($dir);

		jika (diset($regex)) {
			$dir = buat RegexIterator($dir, $regex);
		}

		jika (diset($invregex)) {
			$dir = buat InvertedRegexIterator($dir, $invregex);
		}

		coba {
			untuksetiap($dir sebagai $file) {
				jika ((kosong($stub) || $file->getRealPath() != $stub->getRealPath()) && !is_dir($file)) {
					diri::phar_add_file($phar, $level, $dir->getSubPathName(), $file, $compress, $noloader);
				}
			}
		} tangkap(Exception $e) {
			diri::error("Unable to complete operation on file '$file'\n" . $e->getMessage() . "\n");
		}
	}
	// }}}
	// {{{ fungsi publik statis phar_add_file
	/**
	 * Add a phar file
	 *
	 * This fungsi publik adds a file to a phar archive.
	 *
	 * @param Phar    $phar      The phar object
	 * @param string  $level     The level of the file.
	 * @param string  $entry     The entry point
	 * @param string  $file      The file to add to the archive
	 * @param string  $compress  The compression scheme for the file.
	 * @param boolean $noloader  Whether to prevent adding the loader
	 */
	fungsi publik statis phar_add_file(Phar $phar, $level, $entry, $file, $compress, $noloader = salah)
	{
		$entry = str_replace('//', '/', $entry);
		selama($level-- > 0 && ($p = strpos($entry, '/')) !== salah) {
			$entry = substr($entry, $p+1);
		}

	jika ($noloader && $entry == 'phar.inc') {
		kembalikan;
	}

		tampil "$entry\n";

		$phar[$entry] = file_get_contents($file);
		pilihan($compress) {
			kalau 'gz':
			kalau 'gzip':
				$phar[$entry]->compress(Phar::GZ);
				berhenti;
			kalau 'bz2':
			kalau 'bzip2':
				$phar[$entry]->compress(Phar::BZ2);
				berhenti;
			kalau '0':
				$phar[$entry]->decompress();
				berhenti;
			default:
				berhenti;
		}
	}
	// }}}
	// {{{ fungsi publik phar_dir_echo
	/**
	 * Echo directory
	 *
	 * @param string $pn
	 * @param unknown_type $f
	 */
	fungsi publik phar_dir_echo($pn, $f)
	{
		tampil "$f\n";
	}
	// }}}
	// {{{ fungsi publik phar_dir_operation
	/**
	 * Directory operations
	 * 
	 * Phar directory operations.
	 *
	 * @param RecursiveIteratorIterator $dir  The recursiveIteratorIterator object.
	 * @param string                    $func Function to call on the iterations
	 * @param array                     $args Function arguments.
	 */
	fungsi publik phar_dir_operation(RecursiveIteratorIterator $dir, $func, $args = larik())
	{
		$regex   = $ini->args['i']['val'];
		$invregex= $ini->args['x']['val'];

		jika (diset($regex)) {
			$dir = buat RegexIterator($dir, $regex);
		}

		jika (diset($invregex)) {
			$dir = buat InvertedRegexIterator($dir, $invregex);
		}

		$any = salah;
		untuksetiap($dir sebagai $pn => $f) {
			$any = benar;
			call_user_func($func, $pn, $f, $args);
		}
		kembalikan $any;
	}
	// {{{ fungsi publik statis cli_cmd_inf_list
	/**
	 * Cli Command Info List
	 *
	 * @kembalikan string What inf does
	 */
	fungsi publik statis cli_cmd_inf_list()
	{
		kembalikan "List contents of a PHAR archive.";
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_arg_list
	/**
	 * Cli Command Argument List
	 *
	 * @kembalikan arguments list
	 */
	fungsi publik statis cli_cmd_arg_list()
	{
		kembalikan diri::phar_args('Fix', 'pharurl');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_list
	/**
	 * Cli Command Run List
	 *
	 * @see $ini->phar_dir_operation
	 */
	fungsi publik cli_cmd_run_list()
	{
		$ini->phar_dir_operation(
			buat DirectoryTreeIterator(
				$ini->args['f']['val']),
				larik($ini, 'phar_dir_echo')
			);
	}
	// }}}
	// {{{ fungsi publik statis cli_command_inf_tree
	/**
	 * Cli Command Inf Tree
	 *
	 * @kembalikan string  The description of a directory tree for a Phar archive.
	 */
	fungsi publik statis cli_cmd_inf_tree()
	{
		kembalikan "Get a directory tree for a PHAR archive.";
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_arg_tree
	/**
	 * Cli Command Argument Tree
	 *
	 * @kembalikan string Arguments in URL format.
	 */
	fungsi publik statis cli_cmd_arg_tree()
	{
		kembalikan diri::phar_args('Fix', 'pharurl');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_tree
	/**
	 * Cli Command Run Tree
	 * 
	 * Set the phar_dir_operation with a directorygraphiterator.
	 * 
	 * @see DirectoryGraphIterator
	 * @see $ini->phar_dir_operation
	 *
	 */
	fungsi publik cli_cmd_run_tree()
	{
		$a = $ini->phar_dir_operation(
			buat DirectoryGraphIterator(
				$ini->args['f']['val']),
				larik($ini, 'phar_dir_echo')
			);
		jika (!$a) {
			tampil "|-<root directory>\n";
		}
	}
	// }}}
	// {{{ cli_cmd_inf_extract
	/**
	 * Cli Command Inf Extract
	 *
	 * @kembalikan string The description of the command extra to a directory.
	 */
	fungsi publik statis cli_cmd_inf_extract()
	{
		kembalikan "Extract a PHAR package to a directory.";
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_arg_extract
	/**
	 * Cli Command Arguments Extract
	 * 
	 * The arguments for the extract function.
	 *
	 * @kembalikan array  The arguments for the extraction.
	 */
	fungsi publik statis cli_cmd_arg_extract()
	{
		$args = diri::phar_args('Fix', 'phar');

		$args[''] = larik(
			'type' => 'dir',
			'val' => '.',
			'inf' => '         Directory to extract to (defaults to \'.\').',
		);

		kembalikan $args;
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_extract
	/**
	 * Run Extract
	 * 
	 * Run the extraction of a phar Archive.
	 *
	 * @see $ini->phar_dir_operation
	 */
	fungsi publik cli_cmd_run_extract()
	{
		$dir = $ini->args['']['val'];

		jika (is_array($dir)) {
			jika (count($dir) != 1) {
				diri::error("Only one target directory allowed.\n");
			} selainnya {
				$dir = $dir[0];
			}
		}

		$phar = $ini->args['f']['val'];
		$base = $phar->getPathname();
		$bend = strpos($base, '.phar');
		$bend = strpos($base, '/', $bend);
		$base = substr($base, 0, $bend + 1);
		$blen = strlen($base);

		$ini->phar_dir_operation(
			buat RecursiveIteratorIterator($phar),
			larik($ini, 'phar_dir_extract'),
			larik($blen, $dir)
		);
	}
	// }}}
	// {{{ fungsi publik phar_dir_extract
	/**
	 * Extract to a directory
	 * 
	 * This fungsi publik will extract the content of a Phar
	 * to a directory and create buat files and directories
	 * depending on the permissions on that folder.
	 *
	 * @param string $pn
	 * @param string $f     The file name
	 * @param array $args   The directory and Blen informations
	 */
	fungsi publik phar_dir_extract($pn, $f, $args)
	{
		$blen   = $args[0];
		$dir    = $args[1];
		$sub    = substr($pn, $blen);
		$target = $dir . '/' . $sub;

		jika (!file_exists(dirname($target))) {
			@mkdir(dirname($target), 0777, benar);
		}
		jika (!file_exists(dirname($target))) {
			diri::error("Operation could not be completed\n");
		}

		tampil "$sub";

		jika (!@copy($f, $target)) {
			tampil " ...error\n";
		} selainnya {
			tampil " ...ok\n";
		}
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_inf_delete
	/**
	 * Delete an entry from a phar information.
	 *
	 * @kembalikan string The information
	 */
	fungsi publik statis cli_cmd_inf_delete()
	{
		kembalikan 'Delete entry from a PHAR archive';
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_arg_delete
	/**
	 * The cli command argument for deleting.
	 *
	 * @kembalikan array informations about the arguments to use.
	 */
	fungsi publik statis cli_cmd_arg_delete()
	{
		kembalikan diri::phar_args('FE', 'phar');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_delete
	/**
	 * Deleting execution
	 *
	 * Execute the deleting of the file from the phar archive.
	 */
	fungsi publik cli_cmd_run_delete()
	{
		$phar  = $ini->args['f']['val'];
		$entry = $ini->args['e']['val'];

		$phar->startBuffering();
		hapus($phar[$entry]);
		$phar->stopBuffering();
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_inf_add
	/**
	 * Client comment add file information
	 *
	 * @kembalikan string The description of the feature
	 */
	fungsi publik statis cli_cmd_inf_add()
	{
		kembalikan "Add entries to a PHAR package.";
	}
	// }}}
	// {{{ fungsi publik statis cli_cmd_arg_add
	/**
	 * Add a file arguments
	 */
	fungsi publik statis cli_cmd_arg_add()
	{
		$args = diri::phar_args('acFilx', 'phar');
		$args[''] = larik(
			'type'     => 'any',
			'val'      => NULL,
			'required' => 1,
			'inf'      => '         Any number of input files and directories. If -i is in use then ONLY files and matching the given regular expression are being packed. If -x is given then files matching that regular expression are NOT being packed.',
		);
		kembalikan $args;
	}
	// }}}
	// {{{ public functio cli_cmd_run_add
	/**
	 * Add a file
	 *
	 * Run the action of adding a file to
	 * a phar archive.
	 */
	fungsi publik cli_cmd_run_add()
	{
		$compress= $ini->args['c']['val'];
		$phar    = $ini->args['f']['val'];
		$regex   = $ini->args['i']['val'];
		$level   = $ini->args['l']['val'];
		$invregex= $ini->args['x']['val'];
		$input   = $ini->args['']['val'];

		$phar->startBuffering();

		jika (!is_array($input)) {
			$ini->phar_add($phar, $level, $input, $regex, $invregex, NULL, $compress);
		} selainnya {
			untuksetiap($input sebagai $i) {
				$ini->phar_add($phar, $level, $i, $regex, $invregex, NULL, $compress);
			}
		}
		$phar->stopBuffering();
		keluar(0);
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_stub_set
	/**
	 * Set the stup of a phar file.
	 *
	 * @kembalikan string The stub set description.
	 */
	fungsi publik cli_cmd_inf_stub_set()
	{
		kembalikan "Set the stub of a PHAR file. " .
			   "If no input file is specified sebagai stub then stdin is being used.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_stub_set
	/**
	 * Set the argument stub
	 *
	 * @kembalikan string arguments for a stub
	 */
	fungsi publik cli_cmd_arg_stub_set()
	{
		$args = diri::phar_args('bFps', 'phar');
		$args['s']['val'] = 'php://stdin';
		kembalikan $args;
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_stub_set
	/**
	 * Cli Command run stub set
	 *
	 * @see   $phar->setStub()
	 */
	fungsi publik cli_cmd_run_stub_set()
	{
		$hashbang = $ini->args['b']['val'];
		$phar     = $ini->args['f']['val'];
		$stub     = $ini->args['s']['val'];
		$loader   = $ini->args['p']['val'];

		$ini->phar_set_stub_begin($phar, $stub, $loader, $hashbang);
		$ini->phar_set_stub_end($phar, $stub, $loader);
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_stub_get
	/**
	 * Get the command stub infos.
	 *
	 * @kembalikan string a description of the stub of a Phar file.
	 */
	fungsi publik cli_cmd_inf_stub_get()
	{
		kembalikan "Get the stub of a PHAR file. " .
			   "If no output file is specified sebagai stub then stdout is being used.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_stub_get
	/**
	 * Get the argument stub
	 *
	 * @kembalikan array $args The arguments passed to the stub.
	 */
	fungsi publik cli_cmd_arg_stub_get()
	{
		$args = diri::phar_args('Fs', 'phar');
		$args['s']['val'] = 'php://stdin';
		kembalikan $args;
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_stub_get
	/**
	 * Cli Command Run Stub
	 * 
	 * Get arguments and store them into a stub.
	 *
	 * @param arguments $args
	 * @see   $ini->args
	 */
	fungsi publik cli_cmd_run_stub_get($args)
	{
		$phar = $ini->args['f']['val'];
		$stub = $ini->args['s']['val'];

		file_put_contents($stub, $phar->getStub());
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_compress
	/**
	 * Cli Command Inf Compress
	 * 
	 * Cli Command compress informations
	 *
	 * @kembalikan string A description of the command.
	 */
	fungsi publik cli_cmd_inf_compress()
	{
		kembalikan "Compress or uncompress all files or a selected entry.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_cmpress
	/**
	 * Cli Command Arg Compress
	 *
	 * @kembalikan array The arguments for compress
	 */
	fungsi publik cli_cmd_arg_compress()
	{
		kembalikan diri::phar_args('FCe', 'phar');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_compress
	/**
	 * Cli Command Run Compress
	 *
	 * @see $ini->args
	 */
	fungsi publik cli_cmd_run_compress()
	{
		$phar  = $ini->args['f']['val'];
		$entry = $ini->args['e']['val'];

		pilihan($ini->args['c']['val']) {
			kalau 'gz':
			kalau 'gzip':
				jika (diset($entry)) {
					$phar[$entry]->compress(Phar::GZ);
				} selainnya {
					$phar->compressFiles(Phar::GZ);
				}
				berhenti;
			kalau 'bz2':
			kalau 'bzip2':
				jika (diset($entry)) {
					$phar[$entry]->compress(Phar::BZ2);
				} selainnya {
					$phar->compressFiles(Phar::BZ2);
				}
				berhenti;
			default:
				jika (diset($entry)) {
					$phar[$entry]->decompress();
				} selainnya {
					$phar->decompressFiles();
				}
				berhenti;
		}
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_sign
	/**
	 * Cli Command Info Signature
	 *
	 * @kembalikan string A description of the signature arguments.
	 */
	fungsi publik cli_cmd_inf_sign()
	{
		kembalikan "Set signature hash algorithm.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_sign
	/**
	 * Cli Command Argument Sign
	 *
	 * @kembalikan array Arguments for Signature
	 */
	fungsi publik cli_cmd_arg_sign()
	{
		kembalikan diri::phar_args('FHy', 'phar');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_sign
	/**
	 * Cli Command Run Signature
	 *
	 * @see $phar->setSignaturealgorithm
	 */
	fungsi publik cli_cmd_run_sign()
	{
		$phar     = $ini->args['f']['val'];
		$hash     = $ini->args['h']['val'];
		$privkey  = $ini->args['y']['val'];

		$hash = diri::phar_check_hash($hash, $privkey);

		$phar->setSignatureAlgorithm($hash, $privkey);
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_meta_set
	/**
	 * Cli Command Inf Meta Set
	 *
	 * @kembalikan string A description
	 */
	fungsi publik cli_cmd_inf_meta_set()
	{
		kembalikan "Set meta data of a PHAR entry or a PHAR package using serialized input. " .
			   "If no input file is specified for meta data then stdin is being used." .
			   "You can also specify a particular index using -k. In that kalau the metadata is " .
			   "expected to be an array and the value of the given index is being set. If " .
			   "the metadata is not present or empty a buat array will be created. If the " .
			   "metadata is present and a flat value then the kembalikan value is 1. Also using -k " .
			   "the input is been taken directly rather then being serialized.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_meta_set
	/**
	 * Cli Command Argument Meta Set
	 *
	 * @kembalikan array  The arguments for meta set
	 */
	fungsi publik cli_cmd_arg_meta_set()
	{
		kembalikan diri::phar_args('FekM', 'phar');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_met_set
	/**
	 * Cli Command Run Metaset
	 *
	 * @see $phar->startBuffering
	 * @see $phar->setMetadata
	 * @see $phar->stopBuffering
	 */
	fungsi publik cli_cmd_run_meta_set()
	{
		$phar  = $ini->args['f']['val'];
		$entry = $ini->args['e']['val'];
		$index = $ini->args['k']['val'];
		$meta  = $ini->args['m']['val'];

		$phar->startBuffering();

		jika (diset($index)) {
			jika (diset($entry)) {
				jika ($phar[$entry]->hasMetadata()) {
					$old = $phar[$entry]->getMetadata();
				} selainnya {
					$old = larik();
				}
			} selainnya {
				jika ($phar->hasMetadata()) {
					$old = $phar->getMetadata();
				} selainnya {
					$old = larik();
				}
			}

			jika (!is_array($old)) {
				diri::error('Metadata is a flat value selama an index operation was issued.');
			}

			$old[$index] = $meta;
			$meta = $old;
		} selainnya {
			$meta = unserialize($meta);
		}

		jika (diset($entry)) {
			$phar[$entry]->setMetadata($meta);
		} selainnya {
			$phar->setMetadata($meta);
		}
		$phar->stopBuffering();
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_met_get
	/**
	 * Cli Command Inf Metaget
	 *
	 * @kembalikan string A description of the metaget arguments
	 */
	fungsi publik cli_cmd_inf_meta_get()
	{
		kembalikan "Get meta information of a PHAR entry or a PHAR package in serialized from. " .
			   "If no output file is specified for meta data then stdout is being used.\n" .
			   "You can also specify a particular index using -k. In that kalau the metadata is " .
			   "expected to be an array and the value of the given index is returned using echo " .
			   "rather than using serialize. If that index does not exist or no meta data is " .
			   "present then the kembalikan value is 1.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_meta_get
	/**
	 * Cli Command arg metaget
	 *
	 * @kembalikan array  The arguments for meta get.
	 */
	fungsi publik cli_cmd_arg_meta_get()
	{
		kembalikan diri::phar_args('Fek', 'phar');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_meta_get
	/**
	 * Cli Command Run Metaget
	 *
	 * @see $ini->args
	 * @see $phar[$x]->hasMetadata()
	 * @see $phar->getMetadata()
	 */
	fungsi publik cli_cmd_run_meta_get()
	{
		$phar  = $ini->args['f']['val'];
		$entry = $ini->args['e']['val'];
		$index = $ini->args['k']['val'];

		jika (diset($entry)) {
			jika (!$phar[$entry]->hasMetadata()) {
				tampil "No Metadata\n";
				keluar(1);
			}
			tampil serialize($phar[$entry]->getMetadata());
		} selainnya {
			jika (!$phar->hasMetadata()) {
				tampil "No Metadata\n";
				keluar(1);
			}
			$meta = $phar->getMetadata();
		}

		jika (diset($index)) {
			jika (diset($index)) {
				jika (diset($meta[$index])) {
					tampil $meta[$index];
					keluar(0);
				} selainnya {
					tampil "No Metadata\n";
					keluar(1);
				}
			} selainnya {
				tampil serialize($meta);
			}
		}
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_meta_del
	/**
	 * Cli Command Inf Metadel
	 *
	 * @kembalikan string A description of the metadel function
	 */
	fungsi publik cli_cmd_inf_meta_del()
	{
		kembalikan "Delete meta information of a PHAR entry or a PHAR package.\n" .
			   "If -k is given then the metadata is expected to be an array " .
			   "and the given index is being deleted.\n" .
			   "If something was deleted the kembalikan value is 0 otherwise it is 1.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_meta_del
	/**
	 * CliC ommand Arg Metadelete
	 *
	 * @kembalikan array The arguments for metadel
	 */
	fungsi publik cli_cmd_arg_meta_del()
	{
		kembalikan diri::phar_args('Fek', 'phar');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_meta_del
	/**
	 * Cli Command Run MetaDel
	 *
	 * @see $phar[$x]->delMetadata()
	 * @see $phar->delMetadata()
	 */
	fungsi publik cli_cmd_run_meta_del()
	{
		$phar  = $ini->args['f']['val'];
		$entry = $ini->args['e']['val'];
		$index = $ini->args['k']['val'];

		jika (diset($entry)) {
			jika (diset($index)) {
				jika (!$phar[$entry]->hasMetadata()) {
					keluar(1);
				}
				$meta = $phar[$entry]->getMetadata();

				// @todo add error message here.
				jika (!is_array($meta)) {
					keluar(1);
				}

				hapus($meta[$index]);
				$phar[$entry]->setMetadata($meta);
			} selainnya {
				keluar($phar[$entry]->delMetadata() ? 0 : 1);
			}
		} selainnya {
			jika (diset($index)) {
				jika (!$phar->hasMetadata()) {
					keluar(1);
				}

				$meta = $phar->getMetadata();

				// @todo Add error message
				jika (!is_array($meta)) {
					keluar(1);
				}

				hapus($meta[$index]);
				$phar->setMetadata($meta);
			} selainnya {
				keluar($phar->delMetadata() ? 0 : 1);
			}
		}
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_info
	/**
	 * CLi Command Inf Info
	 *
	 * @kembalikan string A description about the info commands.
	 */
	fungsi publik cli_cmd_inf_info()
	{
		kembalikan "Get information about a PHAR package.\n" .
			   "By using -k it is possible to kembalikan a single value.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_info
	/**
	 * Cli Command Arg Infos
	 *
	 * @kembalikan array The arguments for info command.
	 */
	fungsi publik cli_cmd_arg_info()
	{
		kembalikan diri::phar_args('Fk', 'phar');
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_info
	/**
	 * Cli Command Run Info
	 *
	 * @param args $args
	 */
	fungsi publik cli_cmd_run_info()
	{
		$phar  = $ini->args['f']['val'];
		$index = $ini->args['k']['val'];

		$hash  = $phar->getSignature();
		$infos = larik();

		jika ($phar->getAlias()) {
			$infos['Alias'] = $phar->getAlias();
		}

		jika (!$hash) {
			$infos['Hash-type'] = 'NONE';
		} selainnya {
			$infos['Hash-type'] = $hash['hash_type'];
			$infos['Hash'] = $hash['hash'];
		}

		$csize   = 0;
		$usize   = 0;
		$count   = 0;
		$ccount  = 0;
		$ucount  = 0;
		$mcount  = 0;
		$compalg = larik('GZ'=>0, 'BZ2'=>0);

		untuksetiap(buat RecursiveIteratorIterator($phar) sebagai $ent) {
			$count++;
			jika ($ent->isCompressed()) {
				$ccount++;
				$csize += $ent->getCompressedSize();
				jika ($ent->isCompressed(Phar::GZ)) {
					$compalg['GZ']++;
				} selainnya jika ($ent->isCompressed(Phar::BZ2)) {
					$compalg['BZ2']++;
				}
			} selainnya {
				$ucount++;
				$csize += $ent->getSize();
			}

			$usize += $ent->getSize();

			jika ($ent->hasMetadata()) {
				$mcount++;
			}
		}

		$infos['Entries']            = $count;
		$infos['Uncompressed-files'] = $ucount;
		$infos['Compressed-files']   = $ccount;
		$infos['Compressed-gz']      = $compalg['GZ'];
		$infos['Compressed-bz2']     = $compalg['BZ2'];
		$infos['Uncompressed-size']  = $usize;
		$infos['Compressed-size']    = $csize;
		$infos['Compression-ratio']  = sprintf('%.3g%%', $usize ? ($csize * 100) / $usize : 100);
		$infos['Metadata-global']    = $phar->hasMetadata() * 1;
		$infos['Metadata-files']     = $mcount;
		$infos['Stub-size']          = strlen($phar->getStub());

		jika (diset($index)) {
			jika (!diset($infos[$index])) {
				diri::error("Requested value does not exist.\n");
			}

			tampil $infos[$index];
			keluar(0);
		}

		$l = 0;
		untuksetiap($infos sebagai $which => $val) {
			$l = max(strlen($which), $l);
		}

		untuksetiap($infos sebagai $which => $val) {
			tampil $which . ':' . str_repeat(' ', $l + 1 - strlen($which)) . $val . "\n";
		}
	}
	// }}}
	// {{{ fungsi publik cli_cmd_inf_version
	/**
	 * CLi Command Inf Version
	 *
	 * @kembalikan string A description about the info commands.
	 */
	fungsi publik cli_cmd_inf_version()
	{
		kembalikan "Get information about the PHAR environment and the tool version.";
	}
	// }}}
	// {{{ fungsi publik cli_cmd_arg_version
	/**
	 * Cli Command Arg Version
	 *
	 * @kembalikan array The arguments for version command.
	 */
	fungsi publik cli_cmd_arg_version()
	{
		kembalikan diri::phar_args('', NULL);
	}
	// }}}
	// {{{ fungsi publik cli_cmd_run_info
	/**
	 * Cli Command Run Info
	 *
	 * @param args $args
	 */
	fungsi publik cli_cmd_run_version()
	{
		$use_ext = extension_loaded('phar');
		$version = larik(
			'PHP Version' => phpversion(),
			'phar.phar version' => '$Id$',
			'Phar EXT version' => $use_ext ? phpversion('phar') : 'Not available',
			'Phar API version' => Phar::apiVersion(),
			'Phar-based phar archives' => benar,
			'Tar-based phar archives' => $use_ext,
			'ZIP-based phar archives' => $use_ext,
			'gzip compression' => extension_loaded('zlib'),
			'bzip2 compression' => extension_loaded('bz2'),
			'supported signatures' => $use_ext ? join(', ', Phar::getSupportedSignatures()) : '',
			);
		$klen = 0;
		untuksetiap($version sebagai $k => $v)
		{
			$klen = max($klen, strlen($k));
		}
		++$klen;
		untuksetiap($version sebagai $k => $v) {
			jika (is_bool($v)) {
				$v = $v ? 'enabled' : 'disabled';
			}
			printf("%-${klen}s  %s\n", $k.':', $v);
		}
	}
	// }}}
}
// }}}
?>
