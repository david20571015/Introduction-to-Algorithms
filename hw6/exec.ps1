$len = 1000, 2000, 5000, 10000, 15000, 20000
foreach ($x_len in $len) {
    foreach ($y_len in $len) {
        .\a.exe $x_len $y_len 10
    }
}