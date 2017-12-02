<?phpid

// class
kelas Orang {
    // atribut kelas harus diawali "variabel" 
    // dan diikuti access modifier (publik, terproteksi, privat)
    variabel publik $nama;
    variabel privat $usia;

    // konstruktor
    fungsi publik __konstruktor($n, $u) {
        // $this diganti $ini :)
        $ini->nama = $n;
        $ini->usia = $u;
    }

    // fungsi harus diberi paling tidak 1 modifier (yang ini ada 2)
    fungsi publik final informasi() {
        kembalikan $ini->nama . " berusia " . $ini->usia;
    }
}

// abstract / final class, const
kelas abstrak ContohAbstrak {
    variabel terproteksi $x = 10;
    fungsi abstrak sesuatu();
}
kelas final ContohFinal {
    konstanta publik ABC = 100;
}
cetak ContohFinal::ABC . "\n";

// instantiasi class bisa dengan prefix "buat"
$orang1 = buat Orang("Alfa", 30);
cetak $orang1->informasi() . "\n";

// bisa dengan suffix "baru"
$orang2 = Orang baru("Beta", 32);
cetak $orang2->informasi() . "\n";
