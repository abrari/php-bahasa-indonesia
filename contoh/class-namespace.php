<?phpid

// namespace
ruangnama PHPID\Contoh {
    kelas Test {
        fungsi publik statis test() {
            cetak "test";
        }
    }
}

ruangnama {
    // use ... as
    gunakan PHPID\Contoh\Test sebagai Benda;

    $contoh = Benda baru;
    $contoh->test();
}





