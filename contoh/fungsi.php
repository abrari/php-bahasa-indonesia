<?phpid

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

// include, include_once, require, require_once
sertakan 'hello-world.php';
sertakan_satu_kali 'hello-world.php';
membutuhkan 'hello-world.php';
membutuhkan_satu_kali 'hello-world.php';
