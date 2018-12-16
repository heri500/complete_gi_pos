<?php

function delete_penjualan($idpenjualan){
	if ($idpenjualan > 0){
		$no_nota = db_result(db_query("SELECT nonota FROM penjualan WHERE idpenjualan=%d",$idpenjualan));
		$tglpenjualan = db_result(db_query("SELECT tglpenjualan FROM penjualan WHERE idpenjualan=%d",$idpenjualan));
		$idtitipanlaundry = db_result(db_query("SELECT idtitipanlaundry FROM penjualan WHERE idpenjualan=%d",$idpenjualan));
		$detailTitipanLaundry = null;
		if (!empty($idtitipanlaundry)){
			$result = db_query("SELECT iddetail, idproduct, jumlah, sisa, diambil FROM 
			detaillaundry WHERE idtitipanlaundry='%d'",$idtitipanlaundry);
			while ($data = db_fetch_object($result)){
				$detailTitipanLaundry[$data->idproduct] = $data;
			}
		}
		$detailpenjualan_rs = db_query("SELECT a.idproduct, a.jumlah, b.type_product FROM detailpenjualan a LEFT JOIN product b
		ON a.idproduct = b.idproduct WHERE a.idpenjualan=%d",$idpenjualan);
		while($detailpenjualan_dt = db_fetch_object($detailpenjualan_rs)){
			$idproduk = $detailpenjualan_dt->idproduct;
			$jumlah = $detailpenjualan_dt->jumlah;
			if ($detailpenjualan_dt->type_product == 0){
				$stoksebelum = db_result(db_query("SELECT stok FROM product WHERE idproduct=%d",$idproduk));
				$stoksesudah = $stoksebelum + $jumlah;
				$keterangan = 'Menghapus penjualan '.$no_nota.' tanggal: '.$tglpenjualan;
				db_query("UPDATE product SET stok=stok+%d WHERE idproduct=%d",$jumlah,$idproduk);
				db_query("INSERT INTO transaksistock (idproduk, idpenjualan, stocksebelum, keluar, stocksetelah, keterangan) VALUES 
				('%d', '%d', '%f', '%f', '%f', '%s')",$idproduk,$idpenjualan,$stoksebelum,$jumlah,$stoksesudah,$keterangan);
			}
			if (!empty($detailTitipanLaundry) && isset($detailTitipanLaundry[$idproduk])){
				if ($detailTitipanLaundry[$idproduk]->sisa + $jumlah < $detailTitipanLaundry[$idproduk]->jumlah){
					$statusDetail = 1;
				}else{
					$statusDetail = 0;
				}
				$strSQL = "UPDATE detaillaundry SET sisa = sisa + %d, diambil = 0, status_laundry = %d WHERE iddetail = %d";	
				db_query($strSQL, $jumlah, $statusDetail, $detailTitipanLaundry[$idproduk]->iddetail);
			}
		}
		if (isset($idtitipanlaundry) && !empty($idtitipanlaundry)){
			$totalLaundry = db_result(db_query("SELECT COUNT(*) FROM detaillaundry WHERE idtitipanlaundry='%d'",$idtitipanlaundry));
			$totalSelesai = db_result(db_query("SELECT COUNT(*) FROM detaillaundry WHERE idtitipanlaundry='%d' AND status_laundry=2",$idtitipanlaundry));
			if ($totalSelesai > 0 && $totalSelesai == $totalLaundry){
				$statusLaundry = 2;	
			}else if($totalSelesai > 0 && $totalSelesai < $totalLaundry){
				$statusLaundry = 1;	
			}else{
				$statusLaundry = 0;	
			}
			$strSQL = "UPDATE titipanlaundry SET status_laundry=%d WHERE idtitipanlaundry=%d";
			db_query($strSQL, $statusLaundry, $idtitipanlaundry);
		}
		db_query("DELETE FROM detailpenjualan WHERE idpenjualan='%d'", $idpenjualan);
		$nilaihutang = db_result(db_query("SELECT kembali FROM penjualan WHERE idpenjualan='%d'",$idpenjualan));
		if ($nilaihutang < 0){
			$idpelanggan = db_result(db_query("SELECT idpelanggan FROM detailpiutang WHERE idpenjualan=%d", $idpenjualan));
			$nilaihutang = abs($nilaihutang);
			$besarhutangterakhir = db_result(db_query("SELECT SUM(besarhutang) FROM piutang WHERE idpelanggan=%d GROUP BY idpelanggan", $idpelanggan));
			$variables['idpelanggan'] = $idpelanggan;
			$variables['pembayaran'] = $nilaihutang;
			$variables['hutang'] = $besarhutangterakhir;
			$variables['tglbayar'] = date('Y-m-d');
			$variables['keterangan'] = 'Data penjualan nota: '.$no_nota.' tanggal: '.$tglpenjualan.' dihapus';
			$newvariables = pembayaranhutang($variables);
		}
		db_query("DELETE FROM detailpiutang WHERE idpenjualan='%d'", $idpenjualan);
		db_query("DELETE FROM historypenggunaandeposit WHERE idpenjualan='%d'",$idpenjualan);
		db_query("DELETE FROM penjualan WHERE idpenjualan='%d'", $idpenjualan);
		db_query("DELETE FROM historypembayaran WHERE idpenjualan='%d'",$idpenjualan);
	}
	header('Location: '.base_path().$_GET['destination']);
}
function delete_laundry($idtitipanlaundry){
	if ($idtitipanlaundry > 0){
		$no_nota = db_result(db_query("SELECT nonota FROM titipanlaundry WHERE idtitipanlaundry=%d",$idtitipanlaundry));
		$tgllaundry = db_result(db_query("SELECT tglpenjualan FROM titipanlaundry WHERE idtitipanlaundry=%d",$idtitipanlaundry));
		$detaillaundry_rs = db_query("SELECT idproduct, jumlah FROM detaillaundry WHERE idtitipanlaundry=%d",$idtitipanlaundry);
		db_query("DELETE FROM detaillaundry WHERE idtitipanlaundry='%d'", $idtitipanlaundry);
		db_query("DELETE FROM titipanlaundry WHERE idtitipanlaundry='%d'", $idtitipanlaundry);
		$result = db_query("SELECT idpenjualan FROM penjualan WHERE idtitipanlaundry='%d'",$idtitipanlaundry);
		while ($data = db_fetch_object($result)){
			$result2 = db_query("SELECT a.idproduct, a.jumlah, b.stok, b.type_product FROM detailpenjualan a 
			LEFT JOIN product b ON a.idproduct = b.idproduct WHERE idpenjualan=%d", $data->idpenjualan);
			while ($data2 = db_fetch_object($result2)){
				if ($data2->type_product == 0){
					db_query("UPDATE product SET stok = stok + %f WHERE idproduct=%d", $data2->jumlah, $data2->idproduct);
				}
			}
			db_query("DELETE FROM detailpenjualan WHERE idpenjualan='%d'",$data->idpenjualan);
			db_query("DELETE FROM historypenggunaandeposit WHERE idpenjualan='%d'",$data->idpenjualan);
		}
		db_query("DELETE FROM penjualan WHERE idtitipanlaundry='%d'",$idtitipanlaundry);
		db_query("DELETE FROM historypembayaran WHERE idtitipanlaundry='%d'",$idtitipanlaundry);
	}
	header('Location: '.base_path().$_GET['destination']);
}
function delete_customer_order($idcustomerorder = null){
	if (!empty($idcustomerorder)){
		$strSQL = 'SELECT COUNT(idpenjualan) FROM penjualan WHERE idcustomerorder=%d';
		$penjualanTerkait = db_result(db_query($strSQL, $idcustomerorder));
		if ($penjualanTerkait > 0){
			$strError = t('Ada penjualan terkait dengan customer order ini, delete data gagal');
			header('Location: '.base_path().$_GET['destination'].'?message='.$strError.'&status=error');
		}else{
			$strSQLDeleteDetail = 'DELETE FROM detailcustomerorder WHERE idcustomerorder=%d';
			$strSQLDelete = 'DELETE FROM customer_order WHERE id=%d';
			db_query($strSQLDeleteDetail,$idcustomerorder);
			db_query($strSQLDelete,$idcustomerorder);
			db_query("DELETE FROM historypembayaran WHERE idcustomerorder='%d'",$idcustomerorder);
			$strSuccess = t('Customer order berhasil dihapus...!!');
			header('Location: '.base_path().$_GET['destination'].'?message='.$strSuccess.'&status=status');
		}
	}
}
function delete_detail_customer_order($iddetail = null, $idcustomerorder = null){
	$deleteStatus[] = array('message' => 'Penghapusan detail order gagal...!!!');
	if (!empty($iddetail) && !empty($idcustomerorder)){
		$deleteStatus = array();
		$strSQL = 'DELETE FROM detailcustomerorder WHERE id=%d';
		db_query($strSQL,$iddetail);
		$strSQL = 'SELECT SUM(hargajual*jumlah) AS total_order FROM detailcustomerorder WHERE idcustomerorder=%d';
		$totalOrder = db_result(db_query($strSQL,$idcustomerorder));
		$strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailcustomerorder WHERE idcustomerorder=%d';
		$totalModal = db_result(db_query($strSQL,$idcustomerorder));
		$strSQL = 'UPDATE customer_order SET total=\'%f\', totalmodal=\'%f\' WHERE id=%d';
		db_query($strSQL,$totalOrder,$totalModal,$idcustomerorder);
		$deleteStatus[] = array('message' => 'Penghapusan detail order berhasil...!!!');
	}
	return $deleteStatus;
}
function delete_detail_customer_order_ajax(){
	print json_encode(delete_detail_customer_order($_POST['iddetailorder'],$_POST['idcustomerorder']));
	exit();
}
function delete_detail_penjualan($iddetail = null, $idPenjualan = null){
	$deleteStatus[] = array('message' => 'Penghapusan detail penjualan gagal...!!!');
	if (!empty($iddetail) && !empty($idPenjualan)){
		$deleteStatus = array();
		/* Update Stok Product */
		$qtyJual = db_result(db_query('SELECT jumlah FROM detailpenjualan WHERE iddetail=%d', $iddetail));
		$idproduct = db_result(db_query('SELECT idproduct FROM detailpenjualan WHERE iddetail=%d', $iddetail));
		$stokproduct = db_result(db_query('SELECT stok FROM product WHERE idproduct=%d',$idproduct));
		$stoksekarang = $stockproduct + $qtyJual;
		if (!empty($idproduct)){
			db_query("UPDATE product SET stok = stok + '%f', uploaded=0, changed=1 WHERE idproduct='%d'",$qtyJual,$idproduct);
			$keterangan = 'Revisi Penjualan - Hapus Detail Penjualan';
			db_query("INSERT INTO transaksistock (idproduk, idpenjualan, stocksebelum, keluar, stocksetelah, keterangan) VALUES
				('%d', '%d', '%f', '%f', '%f', '%s')",$idproduct,$idPenjualan,$stokproduct,$qtyJual,$stoksekarang,$keterangan);
		}
		/* End Update Stok Product */
		/* Delete Detail Penjualan */
		$strSQL = 'DELETE FROM detailpenjualan WHERE iddetail=%d';
		db_query($strSQL,$iddetail);
		/* End Delete Detail Penjualan */
		/* Update Penjualan */
		$strSQL = 'SELECT SUM(hargajual*jumlah) AS total_order FROM detailpenjualan WHERE idpenjualan=%d';
		$totalOrder = db_result(db_query($strSQL,$idPenjualan));
		$strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpenjualan WHERE idpenjualan=%d';
		$totalModal = db_result(db_query($strSQL,$idPenjualan));
		$strSQL = 'UPDATE penjualan SET total=\'%f\', totalmodal=\'%f\', uploaded=0, changed=1 WHERE idpenjualan=%d';
		db_query($strSQL,$totalOrder,$totalModal,$idPenjualan);
		/* End Update Penjualan */
		/* Update Hutang Penjualan Jika Ada */
		$caraJual = db_result(db_query('SELECT carabayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
		$kembali = db_result(db_query('SELECT kembali FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
		$idPelanggan = db_result(db_query('SELECT idpelanggan FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
		$bayar = db_result(db_query('SELECT bayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
		if (!empty($idPelanggan) && ($caraJual == 'HUTANG' || $kembali < 0)){
			$besarHutang = $totalOrder - $bayar;
			$sqlUpdatePiutang = 'UPDATE detailpiutang SET besarhutang=\'%f\' ';
			$sqlUpdatePiutang .= 'WHERE idpelanggan=%d AND idpenjualan=%d';
			db_query($sqlUpdatePiutang, $besarHutang, $idPelanggan, $idPenjualan);
			syncHutangPelanggan($idPelanggan);
		}
		/* End Update Hutang Penjualan Jika Ada */
		$deleteStatus[] = array('message' => 'Penghapusan detail penjualan berhasil...!!!');
	}
	return $deleteStatus;
}
function delete_detail_penjualan_ajax(){
	print json_encode(delete_detail_penjualan($_POST['iddetail'],$_POST['idpenjualan']));
	exit();
}