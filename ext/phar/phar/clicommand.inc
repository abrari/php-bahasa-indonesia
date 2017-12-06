<?phpid

/** @file clicommand.inc
 * @ingroup Phar
 * @brief class CLICommand
 * @author  Marcus Boerger
 * @date    2007 - 2008
 *
 * Phar Command
 */

/** @ingroup Phar
 * @brief   Abstract base console command implementation
 * @author  Marcus Boerger
 * @version 1.0
 */
kelas abstrak CLICommand
{
	variabel terproteksi $argc;
	variabel terproteksi $argv;
	variabel terproteksi $cmds = larik();
	variabel terproteksi $args = larik();
	variabel terproteksi $typs = larik();

	fungsi publik __konstruktor($argc, larik $argv)
	{
		$ini->argc = $argc;
		$ini->argv = $argv;
		$ini->cmds = diri::getCommands($ini);
		$ini->typs = diri::getArgTyps($ini);

		jika ($argc < 2) {
			diri::error("No command given, check ${argv[0]} help\n");
		} selainnya jika (!diset($ini->cmds[$argv[1]]['run'])) {
			diri::error("Unknown command '${argv[1]}', check ${argv[0]} help\n");
		} selainnya {
			$command = $argv[1];
		}

		jika (diset($ini->cmds[$command]['arg'])) {
			$ini->args = call_user_func(larik($ini, $ini->cmds[$command]['arg']));
			$i = 1;
			$missing = salah;
			selama (++$i < $argc) {
				jika ($argv[$i][0] == '-') {
					jika (panjang($argv[$i]) == 2 && diset($ini->args[$argv[$i][1]])) {
						$arg = $argv[$i][1];
						jika (++$i >= $argc) {
							diri::error("Missing argument to parameter '$arg' of command '$command', check ${argv[0]} help\n");
						} selainnya {
							$ini->args[$arg]['val'] = $ini->checkArgTyp($arg, $i, $argc, $argv);
						}
					}  selainnya {
						diri::error("Unknown parameter '${argv[$i]}' to command $command, check ${argv[0]} help\n");
					}
				} selainnya {
					berhenti;
				}
			}

			jika (diset($ini->args[''])) {
				jika ($i >= $argc) {
					jika (diset($ini->args['']['require']) && $ini->args['']['require']) {
						diri::error("Missing default trailing arguments to command $command, check ${argv[0]} help\n");
					}
				} selainnya {
					$ini->args['']['val'] = larik();
					selama($i < $argc) {
						$ini->args['']['val'][] = $argv[$i++];
					}
				}
			} selainnya jika ($i < $argc) {
				diri::error("Unexpected default arguments to command $command, check ${argv[0]} help\n");
			}

			untuksetiap($ini->args sebagai $arg => $inf) {
				jika (panjang($arg) && !diset($inf['val']) && diset($inf['required']) && $inf['required']) {
					$missing .=  "Missing parameter '-$arg' to command $command, check ${argv[0]} help\n";
				}
			}

			jika (panjang($missing)) {
				diri::error($missing);
			}
		}

		call_user_func(larik($ini, $ini->cmds[$command]['run']), $ini->args);
	}

	fungsi publik statis notice ($msg)
	{
		fprintf(STDERR, $msg);
	}

	fungsi publik statis error ($msg, $exit_code = 1)
	{
		diri::notice($msg);
		keluar($exit_code);
	}

	fungsi publik checkArgTyp($arg, $i, $argc, $argv)
	{
		$typ = $ini->args[$arg]['typ'];

		jika (diset($ini->typs[$typ]['typ'])) {
			kembalikan call_user_func(larik($ini, $ini->typs[$typ]['typ']), $argv[$i], $ini->args[$arg], $arg);
		} selainnya {
			kembalikan $argv[$i];
		}
	}

	fungsi publik statis getSubFuncs(CLICommand $cmdclass, $prefix, larik $subs)
	{
		$a = larik();
		$r = buat ReflectionClass($cmdclass);
		$l = panjang($prefix);

		untuksetiap($r->getMethods() sebagai $m) {
			jika (substr($m->name, 0, $l) == $prefix) {
				untuksetiap($subs sebagai $sub) {
					$what = substr($m->name, $l+panjang($sub)+1);
					$func = $prefix . $sub . '_' . $what;
					$what = str_replace('_', '-', $what);
					jika ($r->hasMethod($func)) {
						jika (!diset($a[$what])) {
							$a[$what] = larik();
						}
						$a[$what][$sub] = /*$m->class . '::' .*/ $func;
					}
				}
			}
		}
		kembalikan $a;
	}

	fungsi publik statis getCommands(CLICommand $cmdclass)
	{
		kembalikan diri::getSubFuncs($cmdclass, 'cli_cmd_', larik('arg','inf','run'));
	}

	fungsi publik statis getArgTyps(CLICommand $cmdclass)
	{
		kembalikan diri::getSubFuncs($cmdclass, 'cli_arg_', larik('typ'));
	}

	fungsi publik statis cli_arg_typ_bool($arg, $cfg, $key)
	{
		kembalikan (bool)$arg;
	}

	fungsi publik statis cli_arg_typ_int($arg, $cfg, $key)
	{
		jika ((int)$arg != $arg) {
			diri::error("Argument to -$key must be an integer.\n");
		}

		kembalikan (int)$arg;
	}

	fungsi publik statis cli_arg_typ_regex($arg, $cfg, $key)
	{
		jika (panjang($arg)) {
			jika (panjang($arg) > 1 && $arg[0] == $arg[panjang($arg)-1] && strpos('/,', $arg) !== salah) {
				kembalikan $arg;
			} selainnya {
				kembalikan '/' . $arg . '/';
			}
		} selainnya {
			kembalikan NULL;
		}
	}

	fungsi publik statis cli_arg_typ_select($arg, $cfg, $key)
	{
		jika (!in_array($arg, array_keys($cfg['select']))) {
			diri::error("Parameter value '$arg' not one of '" . join("', '", array_keys($cfg['select'])) . "'.\n");
		}
		kembalikan $arg;
	}

	fungsi publik statis cli_arg_typ_dir($arg, $cfg, $key)
	{
		$f = realpath($arg);

		jika ($f===salah || !file_exists($f) || !is_dir($f)) {
			diri::error("Requested path '$arg' does not exist.\n");
		}
		kembalikan $f;
	}

	fungsi publik statis cli_arg_typ_file($arg)
	{
		$f = buat SplFileInfo($arg);
		$f = $f->getRealPath();
		jika ($f===salah || !file_exists($f)) {
			tampil "Requested file '$arg' does not exist.\n";
			keluar(1);
		}
		kembalikan $f;
	}

	fungsi publik statis cli_arg_typ_filenew($arg, $cfg, $key)
	{
		$d = dirname($arg);
		$f = realpath($d);

		jika ($f === salah) {
			diri::error("Path for file '$arg' does not exist.\n");
		}
		kembalikan $f . '/' . basename($arg);
	}

	fungsi publik statis cli_arg_typ_filecont($arg, $cfg, $key)
	{
		kembalikan file_get_contents(diri::cli_arg_typ_file($arg, $cfg, $key));
	}

	fungsi publik cli_get_SP2($l1, $arg_inf)
	{
		kembalikan str_repeat(' ', $l1 + 2 + 4 + 8);
	}

	fungsi publik cli_get_SP3($l1, $l2, $arg_inf)
	{
		kembalikan str_repeat(' ', $l1 + 2 + 4 + 8 + 2 + $l2 + 2);
	}

	fungsi publik statis cli_cmd_inf_help()
	{
		kembalikan "This help or help for a selected command.";
	}

	fungsi privat cli_wordwrap($what, $l, $sp)
	{
		$p = max(79 - $l, 40);     // minimum length for paragraph
		$b = substr($what, 0, $l); // strip out initial $l
		$r = substr($what, $l);    // remainder
		$r = str_replace("\n", "\n".$sp, $r); // in remainder replace \n's
		kembalikan $b . wordwrap($r, $p, "\n".$sp);
	}

	fungsi privat cli_help_get_args($func, $l, $sp, $required)
	{
		$inf = "";
		untuksetiap(call_user_func($func, $l, $sp) sebagai $arg => $conf) {
			jika ((diset($conf['required']) && $conf['required']) != $required) {
				lanjut;
			}

			jika (panjang($arg)) {
				$arg = "-$arg  ";
			} selainnya {
				$arg = "... ";
			}

			$sp2 = $ini->cli_get_SP2($l, $inf);
			$l2  = panjang($sp2);
			$inf .= $ini->cli_wordwrap($sp . $arg . $conf['inf'], $l2, $sp2) . "\n";

			jika (diset($conf['select']) && count($conf['select'])) {
				$ls = 0;
				untuksetiap($conf['select'] sebagai $opt => $what) {
					$ls = max($ls, panjang($opt));
				}
				$sp3 = $ini->cli_get_SP3($l, $ls, $inf);
				$l3  = panjang($sp3);
				untuksetiap($conf['select'] sebagai $opt => $what) {
					$inf .= $ini->cli_wordwrap($sp2 . "  " . sprintf("%-${ls}s  ", $opt) . $what, $l3, $sp3) . "\n";
				}
			}
		}
		jika (panjang($inf)) {
			jika ($required) {
				kembalikan $sp . "Required arguments:\n\n" . $inf;
			} selainnya {
				kembalikan $sp . "Optional arguments:\n\n". $inf;
			}
		}
	}

	fungsi publik cli_cmd_arg_help()
	{
		kembalikan larik('' => larik('typ'=>'any','val'=>NULL,'inf'=>'Optional command to retrieve help for.'));
	}

	fungsi publik cli_cmd_run_help()
	{
		$argv  = $ini->argv;
		$which = $ini->args['']['val'];
		jika (diset($which)) {
			jika (count($which) != 1) {
				diri::error("More than one command given.\n");
			}

			$which = $which[0];
			jika (!array_key_exists($which, $ini->cmds)) {
				jika (strtolower($which) == 'commands') {
					diri::cli_cmd_run_help_list();
					keluar(0);
				}
				diri::error("Unknown command, cannot retrieve help.\n");
			}

			$l = panjang($which);
			$cmds = larik($which => $ini->cmds[$which]);
		} selainnya {
			tampil "\n$argv[0] <command> [options]\n\n";
			$l = 0;
			ksort($ini->cmds);
			untuksetiap($ini->cmds sebagai $name => $funcs) {
				$l = max($l, panjang($name));
			}
			$inf = "Commands:";
			$lst = "";
			$ind = panjang($inf) + 1;
			untuksetiap($ini->cmds sebagai $name => $funcs) {
				$lst .= ' ' . $name;
			}
			tampil $ini->cli_wordwrap($inf.$lst, $ind, str_repeat(' ', $ind)) . "\n\n";
			$cmds = $ini->cmds;
		}
		$sp = str_repeat(' ', $l + 2);
		untuksetiap($cmds sebagai $name => $funcs) {
			$inf = $name . substr($sp, panjang($name));
			jika (diset($funcs['inf'])) {
				$inf .= $ini->cli_wordwrap(call_user_func(larik($ini, $funcs['inf'])), $l, $sp) . "\n";
				jika (diset($funcs['arg'])) {
					$inf .= "\n";
					$inf .= $ini->cli_help_get_args(larik($ini, $funcs['arg']), $l, $sp, benar);
					$inf .= "\n";
					$inf .= $ini->cli_help_get_args(larik($ini, $funcs['arg']), $l, $sp, salah);
				}
			}
			tampil "$inf\n\n";
		}
		keluar(0);
	}

	fungsi publik statis cli_cmd_inf_help_list()
	{
		kembalikan "Lists available commands.";
	}

	fungsi publik cli_cmd_run_help_list()
	{
		ksort($ini->cmds);
		tampil join(' ', array_keys($ini->cmds)) . "\n";
	}
}

?>
