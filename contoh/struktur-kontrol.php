<?phpid

// if, else, empty
$a = 10;
jika (kosong($a)) {
    tampil "kosong\n";
} selainnya jika ($a > 0) {
    tampil "positif\n";
} selainnya {
    tampil "negatif\n";
}

// switch, case, break
$b = 2;
pilihan($b) {
    jikanilai 1: 
        $c = "satu"; 
        berhenti;
    jikanilai 2:
        $c = "dua";
        berhenti;
    default:
        $c = "banyak";
}
cetak $c . "\n";

// goto
lompatke bawah;
    cetak "ini tidak tampil";
bawah:
    cetak "langsung ke sini";

// try, catch, finally
coba {
    lemparkan Exception baru("error!");
} tangkap (Exception $e) {
    cetak_variabel($e);
} akhirnya {
    cetak "akhirnya...";
}

// exit
keluar("selesai");