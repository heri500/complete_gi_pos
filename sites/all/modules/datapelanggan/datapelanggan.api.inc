<?php

/**
 * @param null       $idPelanggan
 * @param bool|false $realData
 *
 * @return bool|int
 */
function getTotalHutangLatest($idPelanggan = null, $realData = false, $periode = null){
    $totalHutangLatest = 0;
    if (!empty($idPelanggan)){
        if ($realData){
            sync_penjualan_hutang($idPelanggan, $periode);
            sync_penggunaan_deposit($idPelanggan, $periode);
            $strSqlTotalHutang = "SELECT SUM(besarhutang) AS total_hutang FROM detailpiutang piutang ";
            $strSqlTotalHutang .= "LEFT JOIN penjualan penj ON piutang.idpenjualan=penj.idpenjualan ";
            $strSqlTotalHutang .= "WHERE piutang.idpelanggan=%d ";
            if (!empty($periode) && isset($periode['tglawal']) && isset($periode['tglakhir'])){
                $strSqlTotalHutang = "AND (penj.tglpenjualan BETWEEN '%s' AND '%s')";
            }
            $strSqlTotalBayar = "SELECT SUM(pembayaran) AS total_bayar FROM historypembayaran WHERE idpelanggan=%d";
            $strSqlTotalPemakaianDeposit = "SELECT SUM(penggunaan) AS total_guna FROM historypenggunaandeposit WHERE idpelanggan=%d";
            if (!empty($periode) && isset($periode['tglawal']) && isset($periode['tglakhir'])) {
                $totalHutang = db_result(db_query($strSqlTotalHutang, $idPelanggan, $periode['tglawal'], $periode['tglakhir']));
                $totalBayar = db_result(db_query($strSqlTotalBayar, $idPelanggan, $periode['tglawal'], $periode['tglakhir']));
                $totalGuna = db_result(db_query($strSqlTotalPemakaianDeposit, $idPelanggan, $periode['tglawal'], $periode['tglakhir']));
            }else{
                $totalHutang = db_result(db_query($strSqlTotalHutang, $idPelanggan));
                $totalBayar = db_result(db_query($strSqlTotalBayar, $idPelanggan));
                $totalGuna = db_result(db_query($strSqlTotalPemakaianDeposit, $idPelanggan));
            }
        }else{
            $strSqlTotalHutang = "SELECT besarhutang FROM piutang WHERE idpelanggan=%d";
            $totalHutang = db_result(db_query($strSqlTotalHutang, $idPelanggan));
            $totalBayar = 0;
            $totalGuna = 0;
        }
        $totalHutangLatest = $totalHutang - $totalBayar + $totalGuna;
    }
    return $totalHutangLatest;
}
function getTotalHutangPelanggan(){
    $strSql = 'SELECT idpelanggan FROM pelanggan';
    $result = db_query($strSql);
    $totalHutang = 0;
    while ($data = db_fetch_object($result)){
        if (!empty($data->idpelanggan)){
            $totalHutangLatest = getTotalHutangLatest($data->idpelanggan);
        }
        $totalHutang = $totalHutang + $totalHutangLatest;
    }
    return $totalHutang;
}

function getTotalHutang($idpelanggan = null){
    $totalhutang = 0;
    if (!empty($idpelanggan)){
        $result = db_result(db_query("SELECT SUM(a.besarhutang) FROM 
		detailpiutang a, penjualan b WHERE a.idpenjualan = b.idpenjualan AND a.idpelanggan='%d'",
            $idpelanggan));
        $totalhutang = $result;
    }
    return $totalhutang;
}
function getTotalPembayaran($idpelanggan = null, $periode = null){
    $totalpembayaran = 0;
    if (!empty($idpelanggan)){
        if (empty($periode)) {
            $result = db_result(
                db_query("SELECT SUM(pembayaran) FROM historypembayaran WHERE idpelanggan='%d'", $idpelanggan)
            );
        }else if (!empty($periode) && isset($periode['awal']) && isset($periode['akhir'])){
            $result = db_result(
                db_query("SELECT SUM(pembayaran) FROM historypembayaran WHERE idpelanggan='%d' AND tglbayar BETWEEN '%s' AND '%s' ", $idpelanggan, $periode['awal'], $periode['akhir'])
            );
        }
        $totalpembayaran = $result;
    }else{
        if (empty($periode)) {
            $result = db_result(
                db_query("SELECT SUM(pembayaran) FROM historypembayaran")
            );
        }else if (!empty($periode) && isset($periode['awal']) && isset($periode['akhir'])){
            $result = db_result(
                db_query("SELECT SUM(pembayaran) FROM historypembayaran WHERE tglbayar BETWEEN '%s' AND '%s' ", $periode['awal'], $periode['akhir'])
            );
        }
        $totalpembayaran = $result;
    }
    return $totalpembayaran;
}
function getTotalPenggunaanDeposit($idpelanggan = null){
    $totalpenggunaan = 0;
    if (!empty($idpelanggan)){
        $result = db_result(db_query("SELECT SUM(penggunaan) FROM historypenggunaandeposit WHERE idpelanggan='%d'",$idpelanggan));
        $totalpenggunaan = $result;
    }
    return $totalpenggunaan;
}
function cekPiutangExists($idpelanggan = null){
    $piutangExist = 0;
    if (!empty($idpelanggan)){
        $piutangExist = db_result(
            db_query(
                "SELECT COUNT(idpelanggan) FROM piutang WHERE idpelanggan=%d",
                $idpelanggan
            )
        );
    }
    return $piutangExist;
}
/**
 * @param null $idPelanggan
 *
 * @return int
 */
function syncHutangPelanggan($idPelanggan = null){
    global $user;
    $strSql = 'SELECT idpelanggan FROM pelanggan';
    if (!empty($idPelanggan)){
        $strSql .= ' WHERE idpelanggan=%d';
        $result = db_query($strSql, $idPelanggan);
    }else{
        $result = db_query($strSql);
    }
    $totalSync = 0;
    while ($data = db_fetch_object($result)){
        $totalHutang = 0;
        if (!empty($data->idpelanggan)){
            $totalHutang = getTotalHutangLatest($data->idpelanggan, true);
        }
        $piutangExist = cekPiutangExists($data->idpelanggan);
        if ($piutangExist){
            $strSqlUpdate = 'UPDATE piutang SET besarhutang=\'%f\' WHERE idpelanggan=%d';
            db_query($strSqlUpdate, $totalHutang, $data->idpelanggan);
        }else{
            $strSqlInsert = "INSERT INTO piutang (idpelanggan, besarhutang, pembayaranterakhir,uid) ";
            $strSqlInsert .= "VALUES (%d, '%f','%f', %d)";
            db_query(
                $strSqlInsert,
                $data->idpelanggan,$totalHutang,-1*($totalHutang), $user->uid
            );
        }
        $totalSync++;
    }
    return $totalSync;
}
function createPelangganSelection($defaultValue = null, $emptyText = 'UMUM', $inisialUlang = true, $IdSelection = 'idpelanggan'){
    if ($inisialUlang){
        $pilihpelanggan = '<select id="'.$IdSelection.'" name="'.$IdSelection.'" onchange="inisialulang();">';
    }else{
        $pilihpelanggan = '<select id="'.$IdSelection.'" name="'.$IdSelection.'">';
    }
    if (empty($defaultValue)){
        $pilihpelanggan .= '<option selected value="0">'.$emptyText.'</option>';
    }else{
        $pilihpelanggan .= '<option value="0">'.$emptyText.'</option>';
    }
    $resultpelanggan = db_query("SELECT idpelanggan,namapelanggan FROM pelanggan");
    if (!empty($defaultValue)){
        $selectedpelanggan = $defaultValue;
    }
    while ($datapelanggan = db_fetch_object($resultpelanggan)){
        if ($datapelanggan->idpelanggan == $selectedpelanggan){
            $pilihpelanggan .= '<option selected value="'.$datapelanggan->idpelanggan.'">'.$datapelanggan->namapelanggan.'</option>';
        }else{
            $pilihpelanggan .= '<option value="'.$datapelanggan->idpelanggan.'">'.$datapelanggan->namapelanggan.'</option>';
        }
    }
    $pilihpelanggan .= '</select>';
    return $pilihpelanggan;
}
function getTotalPembayaranHutang(){

}

function sync_penjualan_hutang($idPelanggan = null, $periode = null){
    if (!empty($idPelanggan)){
        global $user;
        $strSQL = "SELECT jual.idpenjualan, jual.total, jual.kembali, jual.carabayar, jual.tglpenjualan ";
        $strSQL .= "FROM penjualan jual LEFT JOIN detailpiutang piutang ON piutang.idpenjualan = jual.idpenjualan ";
        $strSQL .= "WHERE jual.idpenjualan NOT IN (SELECT idpenjualan FROM detailpiutang WHERE idpelanggan = %d) ";
        $strSQL .= "AND jual.idpelanggan = %d AND jual.kembali < 0 ";
        if (!empty($periode) && isset($periode['tglawal']) && isset($periode['tglakhir'])){
            $strSQL .= "AND (jual.tglpenjualan BETWEEN '%s' AND '%s')";
        }
        if (!empty($periode) && isset($periode['tglawal']) && isset($periode['tglakhir'])) {
            $syncResult = db_query($strSQL, $idPelanggan, $idPelanggan, $periode['tglawal'], $periode['tglakhir']);
        }else{
            $syncResult = db_query($strSQL, $idPelanggan, $idPelanggan);
        }
        while ($dataPenjualan = db_fetch_object($syncResult)){
            db_query("INSERT INTO detailpiutang (idpelanggan, idpenjualan, besarhutang) VALUES ('%d','%d','%f')",
                $idPelanggan,$dataPenjualan->idpenjualan,abs($dataPenjualan->kembali));
            //$resultpiutang = db_query("SELECT idpelanggan, besarhutang  FROM piutang WHERE idpelanggan='%d'",
            //   $idPelanggan);
            //$datapiutang = db_fetch_object($resultpiutang);
            /*if ($datapiutang->idpelanggan){
                $besarhutangskrg = $datapiutang->besarhutang + abs($dataPenjualan->kembali);
                db_query("UPDATE piutang SET besarhutang='%f',uid='%d' WHERE idpelanggan='%d'",
                    $besarhutangskrg,$user->uid,$idPelanggan);
            }else{
                db_query("INSERT INTO piutang(idpelanggan,besarhutang,uid)VALUES('%d','%f','%d')",
                    $idPelanggan,abs($dataPenjualan->kembali),$user->uid);
            }*/
        }
    }
}
function sync_penggunaan_deposit($idPelanggan = null, $periode = null){
    if (!empty($idPelanggan)){
        global $user;
        $strSQL = "SELECT jual.nonota, jual.idpenjualan, jual.total, jual.kembali, jual.carabayar, jual.tglpenjualan ";
        $strSQL .= "FROM penjualan jual LEFT JOIN historypenggunaandeposit depos ON depos.idpenjualan = jual.idpenjualan ";
        $strSQL .= "WHERE jual.idpenjualan NOT IN (SELECT idpenjualan FROM historypenggunaandeposit WHERE idpelanggan = %d) ";
        $strSQL .= "AND jual.idpelanggan = %d AND jual.carabayar = 'DEPOSIT' ";
        if (!empty($periode) && isset($periode['tglawal']) && isset($periode['tglakhir'])){
            $strSQL .= "AND (jual.tglpenjualan BETWEEN '%s' AND '%s')";
        }
        if (!empty($periode) && isset($periode['tglawal']) && isset($periode['tglakhir'])) {
            $syncResult = db_query($strSQL, $idPelanggan, $idPelanggan, $periode['tglawal'], $periode['tglakhir']);
        }else{
            $syncResult = db_query($strSQL, $idPelanggan, $idPelanggan);
        }
        while ($dataPenjualan = db_fetch_object($syncResult)){
            $sisaDeposit = getTotalPembayaran($idPelanggan) - (getTotalHutang($idPelanggan) + getTotalPenggunaanDeposit($idPelanggan));
            $variables['idpelanggan'] = $idPelanggan;
            $variables['deposit'] = $sisaDeposit;
            $variables['penggunaan'] = abs($dataPenjualan->total);
            $variables['tglbayar'] = $dataPenjualan->tglpenjualan;
            $keterangan = 'Penggunaan deposit untuk pembayaran nota : '.$dataPenjualan->nonota.' Tanggal : '.$dataPenjualan->tglpenjualan;
            $variables['keterangan'] = $keterangan;
            $variables['idpenjualan'] = $dataPenjualan->idpenjualan;
            penggunaandeposit($variables);
        }
    }
}

function get_pelanggan_data($IdPelanggan = null){
    $data = null;
    if (!empty($IdPelanggan)){
        $StrSQL = "SELECT idpelanggan, kodepelanggan, namapelanggan, telp, alamat, email, url_database, username, ";
        $StrSQL .= "password, status_pelanggan FROM pelanggan WHERE idpelanggan='%d'";
        $result = db_query($StrSQL,$IdPelanggan);
        $data = db_fetch_object($result);
    }
    return $data;
}

function update_diskon_kategori($IdDiskon = null, $BesarDiskon = null){
    if (!empty($IdDiskon) && !is_null($BesarDiskon)){
        $StrSql = "UPDATE diskonkategori SET besardiskon='%f' WHERE id=%d";
        db_query($StrSql, $BesarDiskon, $IdDiskon);
    }
    return $BesarDiskon;
}