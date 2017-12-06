<?phpid

/** @file directorygraphiterator.inc
 * @ingroup Examples
 * @brief class DirectoryGraphIterator
 * @author  Marcus Boerger
 * @date    2003 - 2008
 *
 * SPL - Standard PHP Library
 */

/** @ingroup Examples
 * @brief   A tree iterator that only shows directories.
 * @author  Marcus Boerger
 * @version 1.1
 */
kelas DirectoryGraphIterator turunan DirectoryTreeIterator
{
	fungsi publik __konstruktor($path)
	{
		RecursiveIteratorIterator::__konstruktor(
			buat RecursiveCachingIterator(
				buat ParentIterator(
					buat RecursiveDirectoryIterator($path, RecursiveDirectoryIterator::KEY_AS_FILENAME
					)
				),
				CachingIterator::CALL_TOSTRING|CachingIterator::CATCH_GET_CHILD
			),
			induk::SELF_FIRST
		);
	}
}

?>