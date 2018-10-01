<?php

function update_pembelian(){
	if (isset($_POST['id'])){
		if (isset($_POST['tanggalbeli'])){
			$idpembelian = explode('-', $_POST['id']);
			$jampembelian = $_POST['jampembelian'];
			$tglbelibaru = date('Y-m-d',strtotime($_POST['tanggalbeli'])).' '.$jampembelian;
			db_query("UPDATE pembelian SET tglpembelian='%s' WHERE idpembelian=%d", $tglbelibaru, $idpembelian[1]);
			if ($_POST['ubah'] == 'tanggal'){
				print $_POST['tanggalbeli'];
			}else{
				print $jampembelian;
			}
		}
	}else{
		$updateddata = new stdClass();
		$updateddata->error = 'ID Penbelian Empty';
		print $updateddata->error;
	}
	exit();	
}
function update_detail_pembelian(){
    if (isset($_POST['id'])){
        if (isset($_POST['qty'])){
            /* Update Detail Pembelian */
            $iddetailpembelian = explode('-', $_POST['id']);
            $strSQL = 'SELECT jumlah FROM detailpembelian WHERE iddetail=%d';
            $qtyLama = db_result(db_query($strSQL, $iddetailpembelian[1]));
            $selisih = $qtyLama - $_POST['qty'];
            $strSQL = 'SELECT idpembelian FROM detailpembelian WHERE iddetail=%d';
            $idPembelian = db_result(db_query($strSQL, $iddetailpembelian[1]));
            $strSQL = 'UPDATE detailpembelian SET jumlah=\'%f\' ';
            $strSQL .= 'WHERE iddetail=%d';
            db_query($strSQL,$_POST['qty'],$iddetailpembelian[1]);
            /* End Update Detail Pembelian */
            /* Update Pembelian */
            $strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpembelian WHERE idpembelian=%d';
            $totalModal = db_result(db_query($strSQL,$idPembelian));
            $strSQL = 'UPDATE pembelian SET total=\'%f\' WHERE idpembelian=%d';
            db_query($strSQL,$totalModal,$idPembelian);
            /* End Update Pembelian */
            /* Update Stok Product */
            $idproduct = db_result(db_query('SELECT idproduct FROM detailpembelian WHERE iddetail=%d', $iddetailpembelian[1]));
            $stokproduct = db_result(db_query('SELECT stok FROM product WHERE idproduct=%d',$idproduct));
            $stoksekarang = $stockproduct + $selisih;
            if (!empty($idproduct)){
                db_query("UPDATE product SET stok = stok + '%f', uploaded=0, changed=1 WHERE idproduct='%d'",$selisih,$idproduct);
                $keterangan = 'Revisi Pembelian';
                db_query("INSERT INTO transaksistock (idproduk, idpembelian, stocksebelum, keluar, stocksetelah, keterangan) VALUES
				('%d', '%d', '%f', '%f', '%f', '%s')",$idproduct,$idPembelian,$stokproduct,$selisih,$stoksekarang,$keterangan);
            }
            /* End Update Stok Product */
            /* Update Hutang Pembelian */
            $caraJual = db_result(db_query('SELECT carabayar FROM pembelian WHERE idpembelian=%d',$idPembelian));
            $kembali = db_result(db_query('SELECT kembali FROM pembelian WHERE idpembelian=%d',$idPembelian));
            $idSupplier = db_result(db_query('SELECT idsupplier FROM pembelian WHERE idpembelian=%d',$idPembelian));
            $bayar = db_result(db_query('SELECT bayar FROM pembelian WHERE idpembelian=%d',$idPembelian));
            if (!empty($idSupplier) && ($caraJual == 'HUTANG' || $kembali < 0)){
                $besarHutang = $totalModal - $bayar;
                $sqlUpdatePiutang = 'UPDATE detailhutang SET besarhutang=\'%f\' ';
                $sqlUpdatePiutang .= 'WHERE idsupplier=%d AND idpembelian=%d';
                db_query($sqlUpdatePiutang, $besarHutang, $idSupplier, $idPembelian);
                $totalHutang = db_result(db_query("SELECT SUM(besarhutang) FROM detailhutang WHERE idsupplier=%d", $idSupplier));
                $totalBayar = db_result(db_query("SELECT SUM(pembayaran) FROM pembayaranhutang WHERE idsupplier=%d", $idSupplier));
                db_query("UPDATE hutang SET besarhutang='%f' WHERE idsupplier=%d", ($totalHutang - $totalBayar), $idSupplier);
            }
            /* End Update Hutang Pembelian */
            $kembali = $bayar - $totalModal;
            db_query('UPDATE pembelian SET kembali=\'%f\' WHERE idpembelian=%d',$kembali,$idPembelian);
            print $_POST['qty'];
        }else if (isset($_POST['harga'])){
            /* Update Detail Pembelian */
            $iddetailpembelian = explode('-', $_POST['id']);
            $strSQL = 'SELECT hargapokok FROM detailpembelian WHERE iddetail=%d';
            $hargaLama = db_result(db_query($strSQL, $iddetailpembelian[1]));
            $selisih = $hargaLama - $_POST['harga'];
            $strSQL = 'SELECT idpembelian FROM detailpembelian WHERE iddetail=%d';
            $idPembelian = db_result(db_query($strSQL, $iddetailpembelian[1]));
            $strSQL = 'UPDATE detailpembelian SET hargapokok=\'%f\' ';
            $strSQL .= 'WHERE iddetail=%d';
            db_query($strSQL,$_POST['harga'],$iddetailpembelian[1]);
            /* End Update Detail Pembelian */
            /* Update Pembelian */
            $strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpembelian WHERE idpembelian=%d';
            $totalModal = db_result(db_query($strSQL,$idPembelian));
            $strSQL = 'UPDATE pembelian SET total=\'%f\' WHERE idpembelian=%d';
            db_query($strSQL,$totalModal,$idPembelian);
            /* End Update Pembelian */
            /* Update Hutang Pembelian */
            $caraJual = db_result(db_query('SELECT carabayar FROM pembelian WHERE idpembelian=%d',$idPembelian));
            $kembali = db_result(db_query('SELECT kembali FROM pembelian WHERE idpembelian=%d',$idPembelian));
            $idSupplier = db_result(db_query('SELECT idsupplier FROM pembelian WHERE idpembelian=%d',$idPembelian));
            $bayar = db_result(db_query('SELECT bayar FROM pembelian WHERE idpembelian=%d',$idPembelian));
            if (!empty($idSupplier) && ($caraJual == 'HUTANG' || $kembali < 0)){
                $besarHutang = $totalModal - $bayar;
                $sqlUpdatePiutang = 'UPDATE detailhutang SET besarhutang=\'%f\' ';
                $sqlUpdatePiutang .= 'WHERE idsupplier=%d AND idpembelian=%d';
                db_query($sqlUpdatePiutang, $besarHutang, $idSupplier, $idPembelian);
                syncHutangSupplier($idSupplier);
            }
            /* End Update Hutang Pembelian */
            $kembali = $bayar - $totalModal;
            db_query('UPDATE pembelian SET kembali=\'%f\' WHERE idpembelian=%d',$kembali,$idPembelian);
            print $_POST['harga'];
        }
    }
    exit();
}

function insert_detail_pembelian(){
    $executedSQL = array();
    if (isset($_POST['idpembelian']) && isset($_POST['idproduk']) && isset($_POST['hargapokok']) && isset($_POST['qty'])){
        if ($_POST['idpembelian'] > 0 && $_POST['idproduk'] > 0 && $_POST['hargapokok'] > 0 && $_POST['qty'] > 0 && isset($_POST['expired_date'])){
            $strSQL = 'SELECT iddetail FROM detailpembelian WHERE idproduct=%d AND idpembelian=%d LIMIT 1';
            $idDetailExist = db_result(db_query($strSQL, $_POST['idproduk'],$_POST['idpembelian']));
            if ($idDetailExist > 0){
                $strSQL = 'UPDATE detailpembelian SET jumlah = jumlah + \'%f\', hargapokok=\'%f\', expired_date=\'%s\' ';
                $strSQL .= 'WHERE iddetail=%d';
                $executedSQL[] = $strSQL;
                db_query(
                    $strSQL,$_POST['qty'],$_POST['hargapokok'],$_POST['expired_date'],$idDetailExist
                );
            }else{
                $strSQL = 'INSERT INTO detailpembelian (idpembelian, idproduct, jumlah, ';
                $strSQL .= 'hargapokok, expired_date, barcode_batch) VALUES ';
                $strSQL .= '(%d, %d, \'%f\', \'%f\', \'%s\', \'%s\')';
                $executedSQL[] = $strSQL;
                $barcodeBatch = create_barcode_batch();
                db_query(
                    $strSQL,$_POST['idpembelian'],$_POST['idproduk'],$_POST['qty'],
                    $_POST['hargapokok'],$_POST['expired_date'],$barcodeBatch
                );
            }
            $strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpembelian WHERE idpembelian=%d';
            $totalModal = db_result(db_query($strSQL,$_POST['idpembelian']));
            $strSQL = 'UPDATE pembelian SET total=\'%f\' WHERE idpembelian=%d';
            db_query($strSQL,$totalModal,$_POST['idpembelian']);
            $stokproduct = db_result(db_query('SELECT stok FROM product WHERE idproduct=%d',$_POST['idproduk']));
            $stoksekarang = $stockproduct + $_POST['qty'];
            if (!empty($idproduct)){
                db_query("UPDATE product SET stok = stok + '%f', uploaded=0, changed=1 WHERE idproduct='%d'",$_POST['qty'],$_POST['idproduk']);
                $keterangan = 'Revisi Penjualan - Tambah Detail Pembelian';
                db_query("INSERT INTO transaksistock (idproduk, idpembelian, stocksebelum, keluar, stocksetelah, keterangan) VALUES
				('%d', '%d', '%f', '%f', '%f', '%s')",$_POST['idproduk'],$_POST['idpembelian'],
                    $stokproduct,$_POST['qty'],$stoksekarang,$keterangan
                );
            }
            /* Update Hutang Penjualan */
            $caraJual = db_result(db_query('SELECT carabayar FROM pembelian WHERE idpembelian=%d',$_POST['idpembelian']));
            $kembali = db_result(db_query('SELECT kembali FROM pembelian WHERE idpembelian=%d',$_POST['idpembelian']));
            $idSupplier = db_result(db_query('SELECT idsupplier FROM pembelian WHERE idpembelian=%d',$_POST['idpembelian']));
            $bayar = db_result(db_query('SELECT bayar FROM pembelian WHERE idpembelian=%d',$_POST['idpembelian']));
            if (!empty($idSupplier) && ($caraJual == 'HUTANG' || $kembali < 0)){
                $besarHutang = $totalModal + $bayar;
                $sqlUpdatePiutang = 'UPDATE detailhutang SET besarhutang=\'%f\' ';
                $sqlUpdatePiutang .= 'WHERE idsupplier=%d AND idpembelian=%d';
                db_query($sqlUpdatePiutang, $besarHutang, $idSupplier, $_POST['idpembelian']);
                syncHutangSupplier($idSupplier);
            }
            /* End Update Hutang Penjualan */
            $kembali = $bayar - $totalModal;
            db_query('UPDATE pembelian SET kembali=\'%f\' WHERE idpembelian=%d',$kembali,$_POST['idpembelian']);
        }
        print json_encode($executedSQL);
    }
    exit();
}