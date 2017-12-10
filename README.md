
PHP Berbahasa Indonesia
===================

Repositori ini berisi interpreter PHP (Zend Engine) yang telah dimodifikasi sehingga sintaks-sintaks dasarnya menggunakan bahasa Indonesia. Tidak ada tujuan khusus, tapi hanya untuk eksperimen. Selain penerjemahan keyword, ditambahkan juga beberapa perubahan dan tambahan sintaks dari PHP asli. Penjelasan lebih lanjut ada di bagian bawah halaman ini.

Untuk saat ini, baru versi CLI yang sudah dicoba. "SAPI" lainnya (untuk web server) belum dicoba dan belum tentu bisa digunakan, namun web server internal (`php -S host:port`) dapat berfungsi. Basis dari repositori ini adalah PHP versi 7.3.0-dev.


## Kompilasi (Linux)
Instal package yang diperlukan (sesuaikan untuk platform lain)
```
$ sudo apt install build-essential bison re2c libxml2-dev libreadline-dev 
```
Clone repositori ini
```
$ git clone --depth=1 https://github.com/abrari/php-bahasa-indonesia.git
```
Konfigurasi dan build
```
$ cd php-bahasa-indonesia/
$ ./buildconf --force
$ ./configure --with-readline
$ make -j 4
```
Hasil build (PHP CLI) akan berada di folder `sapi/cli`.
```
$ sapi/cli/php --version
Interpreter PHP Bahasa Indonesia
Basis PHP 7.3.0-dev (cli) (dikompilasi: Dec  3 2017 14:16:56) ( NTS )
Hak cipta (c) 1997-2017 The PHP Group
Zend Engine v3.3.0-dev, Copyright (c) 1998-2017 Zend Technologies
```

## Kompilasi (Windows)
Untuk kompilasi di Windows, ikuti langkah-langkah pada [link berikut](https://github.com/Microsoft/php-sdk-binary-tools). Sebelumnya Visual Studio 2015 atau 2017 harus sudah terinstal. Hasil kompilasi akan berada pada folder `Release_TS`.

## Rilis versi Windows
Rilis pertama versi Windows tersedia di halaman "Releases" repositori ini: https://github.com/abrari/php-bahasa-indonesia/releases/tag/phpid-0.1.

## Menjalankan Built-in Web Server
Web server internal bisa menjalankan file berekstensi `.phpid`. Untuk menjalankan web server internal, lakukan langkah-langkah berikut:
1. Buka terminal atau command prompt (Windows).
2. Masuk ke folder tempat `php` berada.
3. Jalankan (sesuaikan port):
    ```
    php -S localhost:8080 -t <alamat folder tempat file-file .phpid berada>
    ```
4. Buka browser pada alamat `http://localhost:8080` (sesuaikan port).
5. Jika langsung muncul "Not Found", perlu ada file `index.phpid` pada folder yang ditentukan di atas.

## Perbedaan dengan PHP
Secara umum, beberapa hal yang berbeda dari PHP asli antara lain:

 - Tag pembuka menggunakan `<?phpid`
 - Function reference di method class (simbol `&`) tidak didukung
 - Sintaks alternatif (`endif`, `endforeach`, dll) tidak didukung
 - Seluruh member class (variabel, konstanta, fungsi) harus punya  modifier (`publik`, `terproteksi`, `privat`)
 - Variabel di dalam class harus diawali `variabel`
 - Definisi member class menggunakan *grammar* a la bahasa Indonesia (misalnya dari `public function` menjadi `fungsi publik`)
 - Cara alternatif instantiasi class dengan keyword `<Class> baru()` (lihat contoh di bawah)
 - Alias operator baru: `modulo` dan `pangkat`
 - Sintaks alternatif looping: `untuksetiap ($item dalam $array)`
 - Sebagian kecil fungsi bawaan PHP telah diterjemahkan

## Contoh-contoh Sintaks
Contoh-contoh ini ada di dalam folder **contoh** di repositori ini. Dapat langsung dicoba dengan interactive mode di PHP CLI (`php -a`) atau disimpan dalam file dan dieksekusi (`php namafile.phpid`).

Halo dunia:
```php
// echo
tampil "Halo", "Dunia\n";

// print
cetak "Halo Dunia";
```

Variabel, operator, konstanta:
```php
// true / false, var_dump
$b = benar;
$s = salah;
cetak_variabel($b, $s);

// operator % dan pangkat
cetak 10 modulo 3 . "\n";
cetak 2 pangkat 10 . "\n";
cetak 2 pangkat 9 modulo 7 . "\n";

// strlen -> panjang
cetak panjang("HELLO WORLD") . "\n";

// unset
hapus($s);

// isset
$ada = diset($s);

// global, static
variabel global $glob;
variabel statis $stat = 20;

// define
definisikan('SESUATU', 123);

// defined, die
terdefinisi('KOSONG') atau mati('definisi tidak ada');
```
Beberapa hal terkait array:
```php
// array sintaks lama, print_r
$x = larik(10, 20, 30);
cetak_rekursif($x);

// list
daftar($a, $b) = [100, 200];
cetak $b . "\n";

// count / sizeof
$c = hitung($x);
$c = ukuran($x); 
cetak $c;
```

Kondisional dan struktur kontrol:
```php
// if, else, empty
$a = 10;
jika (kosong($a)) {
    tampil "kosong\n";
} selainnya jika ($a > 0) {
    tampil "positif\n";
} selainnya {
    tampil "negatif\n";
}

$p = benar;
jika ($p == benar && $a == 10) cetak "yes\n";

// switch, case, break
$b = 2;
pilihan($b) {
    kalau 1: 
        $c = "satu"; 
        berhenti;
    kalau 2:
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
    lempar Exception baru("error!");
} tangkap (Exception $e) {
    cetak_variabel($e);
} akhirnya {
    cetak "akhirnya...";
}

// exit
keluar("selesai");
```

Struktur perulangan:
```php
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
    jika ($b == 2) lanjut;
    cetak $a . "\n";
}

// alternatif foreach
untuksetiap ($a => $b dalam $l) {
    tampil $a, $b . "\n";
}

// range -> rentang
untuksetiap ($i dalam rentang(1, 5)) {
    cetak $i;
}
```

Deklarasi fungsi, closure:
```php
// function, return
fungsi jumlah($a, $b) {
    $c = $a + $b;
    kembalikan $c;
}
cetak jumlah(1, 2) . "\n";

// closure, static
$f = fungsi ($x) {
    kembalikan $x * $x;
};
$g = fungsi statis() {
    kembalikan "ABCD";
};
cetak $f(10) . "\n";
cetak $g() . "\n";

// yield
fungsi generator($n) {
    untuk ($i = 1; $i <= $n; $i++) {
        hasilkan $i;
    }    
}
untuksetiap (generator(5) sebagai $x) cetak $x;

// eval
jalankan("cetak 'ini dievaluasi';");

// include, include_once, require, require_once
sertakan 'hello-world.phpid';
sertakan_satu_kali 'hello-world.phpid';
butuh 'hello-world.phpid';
butuh_satu_kali 'hello-world.phpid';
```

## Contoh-contoh OOP

Deklarasi dan instantiasi class:
```php
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
```
Terkait static dalam class:
```php
kelas Contoh {
    // static variable
    variabel publik statis $c = 0;

    fungsi publik __konstruktor() {
        // self -> diri :)
        diri::$c += 1;
    }

    // static method
    fungsi publik statis test() {
        kembalikan "nilai c = " . diri::$c;
    }
}

cetak Contoh::$c . "\n";
$x = Contoh baru; // atau: buat Contoh;

cetak Contoh::test() . "\n";
```

Pewarisan class dan interface:
```php
// file lain di contoh ini
sertakan 'class.phpid';

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
```

Trait di PHP:
```php
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
```
Namespace:
```php
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
```

## Dukungan Text Editor

Telah tersedia package dukungan bahasa ini (syntax highlighting dan snippet) untuk:
- [Sublime Text 3](https://github.com/abrari/php-bahasa-indonesia/tree/master/dukungan_IDE/sublimetext)
- [Visual Studio Code](https://github.com/abrari/php-bahasa-indonesia/tree/master/dukungan_IDE/vscode)
