<?phpid

// trait
sifat BisaLari {
    fungsi publik lari() {
        cetak "saya bisa lari\n";
    }
}

kelas Hewan {
    // use trait
    gunakan BisaLari;

    fungsi publik mati() {
        cetak "mati\n";
    }
}

$hewan = Hewan baru;
$hewan->lari();
$hewan->mati();
