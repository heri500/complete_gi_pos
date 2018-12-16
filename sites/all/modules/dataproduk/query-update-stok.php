<?php
$strSQL = "SELECT idproduct,stok FROM product";
$result = db_query($strSQL);
$nilai = 0;
while ($data = db_fetch_object($result)){
    $idProduct = $data->idproduct;
    $stokSebelum = $data->stok;
    $sql_update = "UPDATE product SET stok='%f' WHERE idproduct='%d'";
    $selisihStok = ($nilai*1)-$stokSebelum;
    if ($selisihStok < 0){
        $keluar = abs($selisihStok);
        $keterangan = 'Perubahan Stock-Stok Berkurang';
        db_query("INSERT INTO transaksistock (idproduk, stocksebelum, keluar, stocksetelah, keterangan) VALUES
			('%d', '%f', '%f', '%f', '%s')",$productId,$stokSebelum,$keluar,$nilai,$keterangan);
    }else{
        $masuk = $selisihStok;
        $keterangan = 'Perubahan Stock-Stok Bertambah';
        db_query("INSERT INTO transaksistock (idproduk, stocksebelum, masuk, stocksetelah, keterangan) VALUES
			('%d', '%f', '%f', '%f', '%s')",$productId,$stokSebelum,$masuk,$nilai,$keterangan);
    }
}
