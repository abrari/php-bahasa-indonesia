<?phpid

/** @file directorytreeiterator.inc
 * @ingroup Examples
 * @brief class DirectoryTreeIterator
 * @author  Marcus Boerger
 * @date    2003 - 2008
 *
 * SPL - Standard PHP Library
 */

/** @ingroup Examples
 * @brief   DirectoryIterator to generate ASCII graphic directory trees
 * @author  Marcus Boerger
 * @version 1.1
 */
kelas DirectoryTreeIterator turunan RecursiveIteratorIterator
{
	/** Construct from a path.
	 * @param $path directory to iterate
	 */
	fungsi publik __konstruktor($path)
	{
		induk::__konstruktor(
			buat RecursiveCachingIterator(
				buat RecursiveDirectoryIterator($path, RecursiveDirectoryIterator::KEY_AS_FILENAME
				),
				CachingIterator::CALL_TOSTRING|CachingIterator::CATCH_GET_CHILD
			),
			induk::SELF_FIRST
		);
	}

	/** @return the current element prefixed with ASCII graphics
	 */
	fungsi publik current()
	{
		$tree = '';
		untuk ($l=0; $l < $ini->getDepth(); $l++) {
			$tree .= $ini->getSubIterator($l)->hasNext() ? '| ' : '  ';
		}
		kembalikan $tree . ($ini->getSubIterator($l)->hasNext() ? '|-' : '\-')
					 . $ini->getSubIterator($l)->__toString();
	}

	/** Aggregates the inner iterator
	 */
	fungsi publik __call($func, $params)
	{
		kembalikan call_user_func_array(larik($ini->getSubIterator(), $func), $params);
	}
}

?>