<?php

function delete_pembelian($idpembelian){
	if ($idpembelian > 0){
		$no_nota = db_result(db_query("SELECT nonota FROM pembelian WHERE idpembelian=%d",$idpembelian));
		$tglpembelian = db_result(db_query("SELECT tglpembelian FROM pembelian WHERE idpembelian=%d",$idpembelian));
		$detailpembelian_rs = db_query("SELECT idproduct, jumlah FROM detailpembelian WHERE idpembelian=%d",$idpembelian);
		while($detailpembelian_dt = db_fetch_object($detailpembelian_rs)){
			$idproduk = $detailpembelian_dt->idproduct;
			$jumlah = $detailpembelian_dt->jumlah;
			$stoksebelum = db_result(db_query("SELECT stok FROM product WHERE idproduct=%d",$idproduk));
			$stoksesudah = $stoksebelum - $jumlah;
			$keterangan = 'Menghapus pembelian '.$no_nota.' tanggal: '.$tglpembelian;
			db_query("UPDATE product SET stok=stok-%d WHERE idproduct=%d",$jumlah,$idproduk);
			db_query("INSERT INTO transaksistock (idproduk, idpembelian, stocksebelum, keluar, stocksetelah, keterangan) VALUES 
			('%d', '%d', '%f', '%f', '%f', '%s')",$idproduk,$idpembelian,$stoksebelum,$jumlah,$stoksesudah,$keterangan);
		}
		db_query("DELETE FROM detailpembelian WHERE idpembelian='%d'", $idpembelian);
		$nilaihutang = db_result(db_query("SELECT kembali FROM pembelian WHERE idpembelian='%d'",$idpembelian));
		if ($nilaihutang < 0){
			$idsupplier = db_result(db_query("SELECT idsupplier FROM detailhutang WHERE idpembelian=%d", $idpembelian));
			$nilaihutang = abs($nilaihutang);
			$besarhutangterakhir = db_result(db_query("SELECT SUM(besarhutang) FROM hutang WHERE idsupplier=%d GROUP BY idsupplier", $idsupplier));
			$variables['idsupplier'] = $idsupplier;
			$variables['pembayaran'] = $nilaihutang;
			$variables['hutang'] = $besarhutangterakhir;
			$variables['tglbayar'] = date('Y-m-d');
			$variables['keterangan'] = 'Data pembelian nota: '.$no_nota.' tanggal: '.$tglpembelian.' dihapus';
			$variables['simpanpembayaran'] = false;
			$newvariables = do_pembayaran_hutang($variables);
		}
		db_query("DELETE FROM detailhutang WHERE idpembelian='%d'", $idpembelian);
		db_query("DELETE FROM pembelian WHERE idpembelian='%d'", $idpembelian);
	}
	header('Location: '.base_path().$_GET['destination']);
}

function delete_detail_pembelian($iddetail = null, $idPembelian = null){
    $deleteStatus[] = array('message' => 'Penghapusan detail pembelian gagal...!!!');
    if (!empty($iddetail) && !empty($idPembelian)){
        $deleteStatus = array();
        /* Update Stok Product */
        $qtyBeli = db_result(db_query('SELECT jumlah FROM detailpembelian WHERE iddetail=%d', $iddetail));
        $idproduct = db_result(db_query('SELECT idproduct FROM detailpembelian WHERE iddetail=%d', $iddetail));
        $stokproduct = db_result(db_query('SELECT stok FROM product WHERE idproduct=%d',$idproduct));
        $stoksekarang = $stockproduct + $qtyBeli;
        if (!empty($idproduct)){
            db_query("UPDATE product SET stok = stok + '%f', uploaded=0, changed=1 WHERE idproduct='%d'",$qtyBeli,$idproduct);
            $keterangan = 'Revisi Pembelian - Hapus Detail Pembelian';
            db_query("INSERT INTO transaksistock (idproduk, idpembelian, stocksebelum, keluar, stocksetelah, keterangan) VALUES
				('%d', '%d', '%f', '%f', '%f', '%s')",$idproduct,$idPembelian,$stokproduct,$qtyBeli,$stoksekarang,$keterangan);
        }
        /* End Update Stok Product */
        /* Delete Detail Pembelian */
        $strSQL = 'DELETE FROM detailpembelian WHERE iddetail=%d';
        db_query($strSQL,$iddetail);
        /* End Delete Detail Pembelian */
        /* Update Pembelian */
        $strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpembelian WHERE idpembelian=%d';
        $totalModal = db_result(db_query($strSQL,$idPembelian));
        $strSQL = 'UPDATE pembelian SET total=\'%f\' WHERE idpembelian=%d';
        db_query($strSQL,$totalModal,$idPembelian);
        /* End Update Pembelian */
        /* Update Hutang Pembelian Jika Ada */
        $caraBeli = db_result(db_query('SELECT carabayar FROM pembelian WHERE idpembelian=%d',$idPembelian));
        $kembali = db_result(db_query('SELECT kembali FROM pembelian WHERE idpembelian=%d',$idPembelian));
        $idSupplier = db_result(db_query('SELECT idsupplier FROM pembelian WHERE idpembelian=%d',$idPembelian));
        $bayar = db_result(db_query('SELECT bayar FROM pembelian WHERE idpembelian=%d',$idPembelian));
        if (!empty($idSupplier) && ($caraBeli == 'HUTANG' || $kembali < 0)){
            $besarHutang = $totalModal - $bayar;
            $sqlUpdatePiutang = 'UPDATE detailhutang SET besarhutang=\'%f\' ';
            $sqlUpdatePiutang .= 'WHERE idsupplier=%d AND idpembelian=%d';
            db_query($sqlUpdatePiutang, $besarHutang, $idSupplier, $idPembelian);
            syncHutangSupplier($idSupplier);
        }
        /* End Update Hutang Pembelian Jika Ada */
        $kembali = $bayar - $totalModal;
        db_query('UPDATE pembelian SET kembali=\'%f\' WHERE idpembelian=%d',$kembali,$_POST['idpembelian']);
        $deleteStatus[] = array('message' => 'Penghapusan detail pembelian berhasil...!!!');
    }
    return $deleteStatus;
}
function delete_detail_pembelian_ajax(){
    print json_encode(delete_detail_pembelian($_POST['iddetail'],$_POST['idpembelian']));
    exit();
}
function delete_return_supplier($IdReturn = null){
    if (!empty($IdReturn)){
        $IdCaraReturn = db_result(db_query('SELECT carareturn FROM returnsupplier WHERE idreturn=%d', $IdReturn));
        $TotalReturn = db_result(db_query('SELECT total FROM returnsupplier WHERE idreturn=%d', $IdReturn));
        $IdSupplier = db_result(db_query('SELECT idsupplier FROM returnsupplier WHERE idreturn=%d', $IdReturn));
        $WaktuReturn = db_result(db_query('SELECT tglreturn FROM returnsupplier WHERE idreturn=%d', $IdReturn));
        if ($IdCaraReturn == 2){
            $variables = array();
            $variables['hutang'] = db_result(db_query("SELECT besarhutang FROM hutang WHERE idsupplier='%d'",$IdSupplier));
            $variables['pembayaran'] = -1 * $TotalReturn;
            $variables['idsupplier'] = $IdSupplier;
            $variables['tglbayar'] = $WaktuReturn;
            $variables['keterangan'] = 'Penghapusan Return supplier potong hutang';
            $variables['simpanpembayaran'] = true;
            do_pembayaran_hutang($variables);
        }
        $StrSql = 'SELECT idreturn, idproduct, jumlah, hargapokok, hargapokoksebelumnya, ';
        $StrSql .= 'perubahan FROM detailreturn WHERE idreturn=%d';
        $RecDetail = db_query($StrSql, $IdReturn);
        while ($DetailRet = db_fetch_object($RecDetail)){
            $StokSebelum = db_query('SELECT stok FROM product WHERE idproduct=%d', $DetailRet->idproduct);
            $StockSekarang = $StokSebelum + $DetailRet->jumlah;
            db_query("UPDATE product SET stok='%f' WHERE idproduct='%d'",$StockSekarang,$DetailRet->idproduct);
            $Keterangan = 'Penghapusan Return ke supplier';
            db_query("INSERT INTO transaksistock (idproduk, idreturn, stocksebelum, masuk, stocksetelah, 
            keterangan) VALUES ('%d', '%d', '%f', '%f', '%f', '%s')",$DetailRet->idproduct,
            $IdReturn,$StokSebelum, $DetailRet->jumlah,$StockSekarang,$Keterangan);
        }
        $StrSql = 'DELETE FROM detailreturn WHERE idreturn=%d';
        db_query($StrSql, $IdReturn);
        $StrSql = 'DELETE FROM returnsupplier WHERE idreturn=%d';
        db_query($StrSql, $IdReturn);
    }
    drupal_goto($_GET['destination']);
}