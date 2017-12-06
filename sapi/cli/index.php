<?phpid

jika (PHP_SAPI === 'cli' atau terdefinisi('STDIN')) {
    parse_str(getenv('POST'), $_POST);
}

cetak_rekursif($_POST);


