<?phpid

// for
untuk ($i = 1; $i <= 3; $i++) {
    tampil $i . "\n";
}

// while, break
$i = 1;
selama ($i <= 100) {
    tampil $i . "\n";
    jika ($i == 5) berhenti;
    $i++;
}

// do while
$i = 10;
kerjakan {
    cetak $i . "\n";
    $i += 10;
} selama ($i <= 50);

// foreach, as, continue
$l = ['satu' => 1, 'dua' => 2, 'tiga' => 3, 'empat' => 4];
untuksetiap ($l sebagai $a => $b) {
    jika ($b == 2) lanjutkan;
    cetak $a . "\n";
}

