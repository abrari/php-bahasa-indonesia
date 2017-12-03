<?phpid

// file lain di contoh ini
sertakan 'class.php';

// interface
interface Test {
    fungsi publik test();
}

// class, extends, implements
kelas Pegawai turunan Orang mengimplementasi Test {
    variabel publik $unit;
    fungsi publik __konstruktor($n, $u) {
        // parent -> induk :)
        induk::__konstruktor($n, 0);
        $ini->nama = $n;
        $ini->unit = $u;
    }
    fungsi publik test() {
        cetak "test pegawai";
    }
}

$pegawai = buat Pegawai("Chandra", "HRD");
cetak_variabel($pegawai);

// instanceof
cetak_variabel($pegawai merupakansuatu Orang);

// clone
$pegawai2 = salin $pegawai;
cetak_variabel($pegawai2);
