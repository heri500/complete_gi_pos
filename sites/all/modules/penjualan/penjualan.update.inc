<?php

function update_penjualan(){
	if (isset($_POST['id'])){
		if (isset($_POST['tanggaljual'])){
			$idpenjualan = explode('-', $_POST['id']);
			$jampenjualan = $_POST['jampenjualan'];
			$tgljualbaru = date('Y-m-d',strtotime($_POST['tanggaljual'])).' '.$jampenjualan;
			db_query("UPDATE penjualan SET tglpenjualan='%s',uploaded=0, changed=1  WHERE idpenjualan=%d", $tgljualbaru, $idpenjualan[1]);
			if ($_POST['ubah'] == 'tanggal'){
				print $_POST['tanggaljual'];
			}else{
				print $jampenjualan;
			}
		}else if(isset($_POST['bayar'])){
		    if (!empty($_POST['bayar'])){
                $idpenjualan = explode('-', $_POST['id']);
                $TotalPenjualan = db_result(db_query('SELECT total_round FROM penjualan WHERE idpenjualan = %d', $idpenjualan[1]));
                if ($TotalPenjualan <= $_POST['bayar']){
                    $TotalBayar = $_POST['bayar'] * 1;
                    $TotalBaki = $TotalBayar - $TotalPenjualan;
                    db_query("UPDATE penjualan SET bayar='%f', kembali='%f' WHERE idpenjualan=%d", $_POST['bayar'], $TotalBaki,$idpenjualan[1]);
                    $CaraBayar = db_result(db_query('SELECT bayar_multiple FROM penjualan WHERE idpenjualan = %d', $idpenjualan[1]));
                    $SplitCb = explode(';',$CaraBayar);
                    $BayarMultiple = '';
                    if (count($SplitCb) == 1){
                        $SplitBayar = explode(':', $SplitCb[0]);
                        if ($TotalBayar >= $TotalPenjualan){
                            $BayarMultiple = $SplitBayar[0].':'.$TotalBayar;
                            $StrSqlCheck = "SELECT COUNT(*) FROM penjualan_bayar WHERE idpenjualan=%d AND carabayar='%s'";
                            $PenjualanBayarExist = db_result(db_query($StrSqlCheck,$idpenjualan[1],$SplitBayar[0]));
                            if ($PenjualanBayarExist){
                                $StrSqlUpdate = "UPDATE penjualan_bayar SET total_bayar='%f' WHERE idpenjualan=%d AND carabayar='%s'";
                                db_query($StrSqlUpdate, $TotalBayar, $idpenjualan[1],$SplitBayar[0]);
                            }
                            db_query("UPDATE penjualan SET bayar_multiple='%s' WHERE idpenjualan=%d",$BayarMultiple,$idpenjualan[1]);
                        }
                    }
                }
                print $TotalBayar;
            }
        }
	}else{
		$updateddata = new stdClass();
		$updateddata->error = 'ID Penjualan Empty';
		print $updateddata->error;
	}
	exit();	
}
function update_laundry(){
	if (isset($_POST['id'])){
		if (isset($_POST['tanggaljual'])){
			$idtitipanlaundry = explode('-', $_POST['id']);
			$jampenjualan = $_POST['jampenjualan'];
			$tgljualbaru = date('Y-m-d',strtotime($_POST['tanggaljual'])).' '.$jampenjualan;
			db_query("UPDATE titipanlaundry SET tglpenjualan='%s' WHERE idtitipanlaundry=%d", $tgljualbaru, $idtitipanlaundry[1]);
			if ($_POST['ubah'] == 'tanggal'){
				print $_POST['tanggaljual'];
			}else{
				print $jampenjualan;
			}
		}
	}else{
		$updateddata = new stdClass();
		$updateddata->error = 'ID titipanlaundry Empty';
		print $updateddata->error;
	}
	exit();	
}
function update_detail_customer_order(){
	if (isset($_POST['id'])){
		if (isset($_POST['qty'])){
			$iddetailorder = explode('-', $_POST['id']);
			$strSQL = 'SELECT jumlah FROM detailcustomerorder WHERE id=%d';
			$qtyLama = db_result(db_query($strSQL, $iddetailorder[1]));
			$selisih = $qtyLama - $_POST['qty'];
			$strSQL = 'SELECT idcustomerorder FROM detailcustomerorder WHERE id=%d';
			$idCustomerOrder = db_result(db_query($strSQL, $iddetailorder[1]));
			$strSQL = 'UPDATE detailcustomerorder SET jumlah=\'%f\', sisa = sisa-\'%f\', outstanding = outstanding-\'%f\' ';
			$strSQL .= 'WHERE id=%d';
			db_query($strSQL,$_POST['qty'],$selisih,$selisih,$iddetailorder[1]);
			$strSQL = 'SELECT SUM(hargajual*jumlah) AS total_order FROM detailcustomerorder WHERE idcustomerorder=%d';
			$totalOrder = db_result(db_query($strSQL,$idCustomerOrder));
			$strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailcustomerorder WHERE idcustomerorder=%d';
			$totalModal = db_result(db_query($strSQL,$idCustomerOrder));
			$strSQL = 'UPDATE customer_order SET total=\'%f\', totalmodal=\'%f\' WHERE id=%d';
			db_query($strSQL,$totalOrder,$totalModal,$idCustomerOrder);
			print $_POST['qty'];
		}
	}
	exit();
}
function insert_detail_customer_order(){
	$executedSQL = array();
	if (isset($_POST['idcustomerorder']) && isset($_POST['idproduk']) && isset($_POST['hargajual']) && isset($_POST['qty'])){
		print $_POST['idcustomerorder'].' '.$_POST['idproduk'].' '.$_POST['hargajual'].' '.$_POST['qty'];
		if ($_POST['idcustomerorder'] > 0 && $_POST['idproduk'] > 0 && $_POST['hargajual'] > 0 && $_POST['qty'] > 0){
			$strSQL = 'SELECT id FROM detailcustomerorder WHERE idproduct=%d AND idcustomerorder=%d LIMIT 1';
			$idDetailExist = db_result(db_query($strSQL, $_POST['idproduk'],$_POST['idcustomerorder']));
			if ($idDetailExist > 0){
				$strSQL = 'UPDATE detailcustomerorder SET jumlah = jumlah + \'%f\', hargajual=\'%f\', ';
				$strSQL .= 'hargapokok=\'%f\', sisa = sisa + \'%f\', outstanding = outstanding + \'%f\', ';
				$strSQL .= 'masuk=%d,diskon=\'%f\' WHERE id=%d';
				$executedSQL[] = $strSQL;
				db_query(
					$strSQL,$_POST['qty'],$_POST['hargajual'],$_POST['hargapokok'],
					$_POST['qty'],$_POST['qty'],time(),$_POST['diskon'],$idDetailExist
				);
			}else{
				$strSQL = 'SELECT lead_time FROM product WHERE idproduct=%d';
				$leadTime = db_result(db_query($strSQL,$_POST['idproduk']));
				$perkiraanAmbil = mktime(date('H'),date('i') + $leadTime,0,date('n'),date('d'),date('Y'));
				$detailBarcode = createEAN13Code(getRandomString(9));
				$strSQL = 'INSERT INTO detailcustomerorder (idcustomerorder, idproduct, detailbarcode, jumlah, ';
				$strSQL .= 'hargapokok, hargajual, sisa, masuk, outstanding, perkiraan_ambil,diskon) VALUES ';
				$strSQL .= '(%d, %d, \'%s\', \'%f\', \'%f\', \'%f\', \'%f\', %d, \'%f\', %d,\'%f\')';
				$executedSQL[] = $strSQL;
				db_query(
					$strSQL,$_POST['idcustomerorder'],$_POST['idproduk'],$detailBarcode,$_POST['qty'],
					$_POST['hargapokok'],$_POST['hargajual'],$_POST['qty'],time(),$_POST['qty'],
					$perkiraanAmbil,$_POST['diskon']
				);
			}
			$strSQL = 'SELECT SUM(hargajual*jumlah) AS total_order FROM detailcustomerorder WHERE idcustomerorder=%d';
			$totalOrder = db_result(db_query($strSQL,$_POST['idcustomerorder']));
			$strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailcustomerorder WHERE idcustomerorder=%d';
			$totalModal = db_result(db_query($strSQL,$_POST['idcustomerorder']));
            $strSQL = 'SELECT ppn FROM customer_order WHERE id=%d';
            $PpnValue = db_result(db_query($strSQL));
            $TotalPlusPpn = $totalOrder + ($totalOrder * $PpnValue/100);
			$strSQL = 'UPDATE customer_order SET total=\'%f\', totalmodal=\'%f\', total_plus_ppn=\'%f\' WHERE id=%d';
			db_query($strSQL,$totalOrder,$totalModal,$TotalPlusPpn,$_POST['idcustomerorder']);
		}
		print json_encode($executedSQL);
	}
	exit();
}
function update_detail_penjualan(){
	if (isset($_POST['id'])){
        $iddetailpenjualan = explode('-', $_POST['id']);
		if (isset($_POST['qty'])){
			/* Update Detail Penjualan */
			$strSQL = 'SELECT jumlah FROM detailpenjualan WHERE iddetail=%d';
			$qtyLama = db_result(db_query($strSQL, $iddetailpenjualan[1]));
			$selisih = $qtyLama - $_POST['qty'];
			$strSQL = 'SELECT idpenjualan FROM detailpenjualan WHERE iddetail=%d';
			$idPenjualan = db_result(db_query($strSQL, $iddetailpenjualan[1]));
			$strSQL = 'UPDATE detailpenjualan SET jumlah=\'%f\' ';
			$strSQL .= 'WHERE iddetail=%d';
			db_query($strSQL,$_POST['qty'],$iddetailpenjualan[1]);
			/* End Update Detail Penjualan */
			/* Update Penjualan */
			$strSQL = 'SELECT SUM(hargajual*jumlah) AS total_jual FROM detailpenjualan WHERE idpenjualan=%d';
			$totalOrder = db_result(db_query($strSQL,$idPenjualan));
			$strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpenjualan WHERE idpenjualan=%d';
			$totalModal = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT ppn FROM customer_order WHERE id=%d';
            $PpnValue = db_result(db_query($strSQL));
            $TotalPlusPpn = $totalOrder + ($totalOrder * $PpnValue/100);
            $TotalAfterVoucher = $TotalPlusPpn;
            $TotalRound = round5($TotalPlusPpn);
			$strSQL = 'UPDATE penjualan SET total=\'%f\', totalmodal=\'%f\', total_plus_ppn=\'%f\', ';
            $strSQL .= 'total_after_voucher = \'%f\', total_round = \'%f\', ';
            $strSQL .= 'uploaded=0, changed=1  WHERE idpenjualan=%d';
			db_query($strSQL, $totalOrder, $totalModal, $TotalPlusPpn, $TotalAfterVoucher, $TotalRound, $idPenjualan);
			/* End Update Penjualan */
			/* Update Stok Product */
			$idproduct = db_result(db_query('SELECT idproduct FROM detailpenjualan WHERE iddetail=%d', $iddetailpenjualan[1]));
			$stokproduct = db_result(db_query('SELECT stok FROM product WHERE idproduct=%d',$idproduct));
			$stoksekarang = $stockproduct + $selisih;
			if (!empty($idproduct)){
				db_query("UPDATE product SET stok = stok + '%f',uploaded=0, changed=1  WHERE idproduct='%d'",$selisih,$idproduct);
				$keterangan = 'Revisi Penjualan';
				db_query("INSERT INTO transaksistock (idproduk, idpenjualan, stocksebelum, keluar, stocksetelah, keterangan) VALUES
				('%d', '%d', '%f', '%f', '%f', '%s')",$idproduct,$idPenjualan,$stokproduct,$selisih,$stoksekarang,$keterangan);
			}
			/* End Update Stok Product */
			/* Update Hutang Penjualan */
			$caraJual = db_result(db_query('SELECT carabayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
			$kembali = db_result(db_query('SELECT kembali FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
			$idPelanggan = db_result(db_query('SELECT idpelanggan FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
			$bayar = db_result(db_query('SELECT bayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
			if (!empty($idPelanggan) && ($caraJual == 'HUTANG' || $kembali < 0)){
				$besarHutang = $TotalPlusPpn - $bayar;
				$sqlUpdatePiutang = 'UPDATE detailpiutang SET besarhutang=\'%f\' ';
				$sqlUpdatePiutang .= 'WHERE idpelanggan=%d AND idpenjualan=%d';
				db_query($sqlUpdatePiutang, $besarHutang, $idPelanggan, $idPenjualan);
				syncHutangPelanggan($idPelanggan);
			}
			/* End Update Hutang Penjualan */
			$kembali = $bayar - $TotalRound;
			db_query('UPDATE penjualan SET kembali=\'%f\',uploaded=0, changed=1  WHERE idpenjualan=%d',$kembali,$idPenjualan);
			print $_POST['qty'];
		}else if (isset($_POST['diskon'])){
            /* Update Detail Penjualan */
            $strSQL = 'SELECT idpenjualan FROM detailpenjualan WHERE iddetail=%d';
            $idPenjualan = db_result(db_query($strSQL, $iddetailpenjualan[1]));
            $strSQL = 'UPDATE detailpenjualan SET diskon=\'%f\' ';
            $strSQL .= 'WHERE iddetail=%d';
            db_query($strSQL,$_POST['diskon'],$iddetailpenjualan[1]);
            /* End Update Detail Penjualan */
            /* Update Penjualan */
            $strSQL = 'SELECT SUM(((hargajual - (hargajual*diskon/100))*jumlah) + (((hargajual - (hargajual*diskon/100))*jumlah)*ppn/100)) AS total_jual FROM detailpenjualan WHERE idpenjualan=%d';
            $totalOrder = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpenjualan WHERE idpenjualan=%d';
            $totalModal = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT ppn FROM customer_order WHERE id=%d';
            $PpnValue = db_result(db_query($strSQL));
            $TotalPlusPpn = $totalOrder + ($totalOrder * $PpnValue/100);
            $TotalAfterVoucher = $TotalPlusPpn;
            $TotalRound = round5($TotalPlusPpn);
            $strSQL = 'UPDATE penjualan SET total=\'%f\', totalmodal=\'%f\', total_plus_ppn=\'%f\', ';
            $strSQL .= 'total_after_voucher = \'%f\', total_round = \'%f\', ';
            $strSQL .= 'uploaded=0, changed=1  WHERE idpenjualan=%d';
            db_query($strSQL, $totalOrder, $totalModal, $TotalPlusPpn, $TotalAfterVoucher, $TotalRound, $idPenjualan);
            /* End Update Penjualan */
            /* Update Hutang Penjualan */
            $caraJual = db_result(db_query('SELECT carabayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $kembali = db_result(db_query('SELECT kembali FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $idPelanggan = db_result(db_query('SELECT idpelanggan FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $bayar = db_result(db_query('SELECT bayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            if (!empty($idPelanggan)){
                $besarHutang = $totalOrder - $bayar;
                $sqlUpdatePiutang = 'UPDATE detailpiutang SET besarhutang=\'%f\' ';
                $sqlUpdatePiutang .= 'WHERE idpelanggan=%d AND idpenjualan=%d';
                db_query($sqlUpdatePiutang, $besarHutang, $idPelanggan, $idPenjualan);
                syncHutangPelanggan($idPelanggan);
            }
            /* End Update Hutang Penjualan */
            $kembali = $bayar - $TotalRound;
            db_query('UPDATE penjualan SET kembali=\'%f\', uploaded=0, changed=1 WHERE idpenjualan=%d',$kembali,$idPenjualan);
            print $_POST['diskon'];
        }else if (isset($_POST['ppn'])){
            /* Update Detail Penjualan */
            $strSQL = 'SELECT idpenjualan FROM detailpenjualan WHERE iddetail=%d';
            $idPenjualan = db_result(db_query($strSQL, $iddetailpenjualan[1]));
            $strSQL = 'UPDATE detailpenjualan SET ppn=\'%f\' ';
            $strSQL .= 'WHERE iddetail=%d';
            db_query($strSQL,$_POST['ppn'],$iddetailpenjualan[1]);
            /* End Update Detail Penjualan */
            /* Update Penjualan */
            $strSQL = 'SELECT SUM(((hargajual - (hargajual*diskon/100))*jumlah) + (((hargajual - (hargajual*diskon/100))*jumlah)*ppn/100)) AS total_jual FROM detailpenjualan WHERE idpenjualan=%d';
            $totalOrder = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpenjualan WHERE idpenjualan=%d';
            $totalModal = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT ppn FROM customer_order WHERE id=%d';
            $PpnValue = db_result(db_query($strSQL));
            $TotalPlusPpn = $totalOrder + ($totalOrder * $PpnValue/100);
            $TotalAfterVoucher = $TotalPlusPpn;
            $TotalRound = round5($TotalPlusPpn);
            $strSQL = 'UPDATE penjualan SET total=\'%f\', totalmodal=\'%f\', total_plus_ppn=\'%f\', ';
            $strSQL .= 'total_after_voucher = \'%f\', total_round = \'%f\', ';
            $strSQL .= 'uploaded=0, changed=1  WHERE idpenjualan=%d';
            db_query($strSQL, $totalOrder, $totalModal, $TotalPlusPpn, $TotalAfterVoucher, $TotalRound, $idPenjualan);
            /* End Update Penjualan */
            /* Update Hutang Penjualan */
            $caraJual = db_result(db_query('SELECT carabayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $kembali = db_result(db_query('SELECT kembali FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $idPelanggan = db_result(db_query('SELECT idpelanggan FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $bayar = db_result(db_query('SELECT bayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            if (!empty($idPelanggan)){
                $besarHutang = $totalOrder - $bayar;
                $sqlUpdatePiutang = 'UPDATE detailpiutang SET besarhutang=\'%f\' ';
                $sqlUpdatePiutang .= 'WHERE idpelanggan=%d AND idpenjualan=%d';
                db_query($sqlUpdatePiutang, $besarHutang, $idPelanggan, $idPenjualan);
                syncHutangPelanggan($idPelanggan);
            }
            /* End Update Hutang Penjualan */
            $kembali = $bayar - $TotalRound;
            db_query('UPDATE penjualan SET kembali=\'%f\', uploaded=0, changed=1 WHERE idpenjualan=%d',$kembali,$idPenjualan);
            print $_POST['ppn'];
        }else if (isset($_POST['hargajual'])){
            /* Update Detail Penjualan */
            $strSQL = 'SELECT idpenjualan FROM detailpenjualan WHERE iddetail=%d';
            $idPenjualan = db_result(db_query($strSQL, $iddetailpenjualan[1]));
            $strSQL = 'UPDATE detailpenjualan SET hargajual=\'%f\' ';
            $strSQL .= 'WHERE iddetail=%d';
            db_query($strSQL,$_POST['hargajual'],$iddetailpenjualan[1]);
            /* End Update Detail Penjualan */
            /* Update Penjualan */
            $strSQL = 'SELECT SUM(((hargajual - (hargajual*diskon/100))*jumlah) + (((hargajual - (hargajual*diskon/100))*jumlah)*ppn/100)) AS total_jual FROM detailpenjualan WHERE idpenjualan=%d';
            $totalOrder = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpenjualan WHERE idpenjualan=%d';
            $totalModal = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT ppn FROM customer_order WHERE id=%d';
            $PpnValue = db_result(db_query($strSQL));
            $TotalPlusPpn = $totalOrder + ($totalOrder * $PpnValue/100);
            $TotalAfterVoucher = $TotalPlusPpn;
            $TotalRound = round5($TotalPlusPpn);
            $strSQL = 'UPDATE penjualan SET total=\'%f\', totalmodal=\'%f\', total_plus_ppn=\'%f\', ';
            $strSQL .= 'total_after_voucher = \'%f\', total_round = \'%f\', ';
            $strSQL .= 'uploaded=0, changed=1  WHERE idpenjualan=%d';
            db_query($strSQL, $totalOrder, $totalModal, $TotalPlusPpn, $TotalAfterVoucher, $TotalRound, $idPenjualan);
            /* End Update Penjualan */
            /* Update Hutang Penjualan */
            $caraJual = db_result(db_query('SELECT carabayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $kembali = db_result(db_query('SELECT kembali FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $idPelanggan = db_result(db_query('SELECT idpelanggan FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $bayar = db_result(db_query('SELECT bayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            if (!empty($idPelanggan)){
                $besarHutang = $totalOrder - $bayar;
                $sqlUpdatePiutang = 'UPDATE detailpiutang SET besarhutang=\'%f\' ';
                $sqlUpdatePiutang .= 'WHERE idpelanggan=%d AND idpenjualan=%d';
                db_query($sqlUpdatePiutang, $besarHutang, $idPelanggan, $idPenjualan);
                syncHutangPelanggan($idPelanggan);
            }
            /* End Update Hutang Penjualan */
            $kembali = $bayar - $TotalRound;
            db_query('UPDATE penjualan SET kembali=\'%f\', uploaded=0, changed=1 WHERE idpenjualan=%d',$kembali,$idPenjualan);
            print $_POST['hargajual'];
        }else if (isset($_POST['hargapokok'])){
            /* Update Detail Penjualan */
            $strSQL = 'SELECT idpenjualan FROM detailpenjualan WHERE iddetail=%d';
            $idPenjualan = db_result(db_query($strSQL, $iddetailpenjualan[1]));
            $strSQL = 'UPDATE detailpenjualan SET hargapokok=\'%f\' ';
            $strSQL .= 'WHERE iddetail=%d';
            db_query($strSQL,$_POST['hargapokok'],$iddetailpenjualan[1]);
            /* End Update Detail Penjualan */
            /* Update Penjualan */
            $strSQL = 'SELECT SUM(((hargajual - (hargajual*diskon/100))*jumlah) + (((hargajual - (hargajual*diskon/100))*jumlah)*ppn/100)) AS total_jual FROM detailpenjualan WHERE idpenjualan=%d';
            $totalOrder = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpenjualan WHERE idpenjualan=%d';
            $totalModal = db_result(db_query($strSQL,$idPenjualan));
            $strSQL = 'SELECT ppn FROM customer_order WHERE id=%d';
            $PpnValue = db_result(db_query($strSQL));
            $TotalPlusPpn = $totalOrder + ($totalOrder * $PpnValue/100);
            $TotalAfterVoucher = $TotalPlusPpn;
            $TotalRound = round5($TotalPlusPpn);
            $strSQL = 'UPDATE penjualan SET total=\'%f\', totalmodal=\'%f\', total_plus_ppn=\'%f\', ';
            $strSQL .= 'total_after_voucher = \'%f\', total_round = \'%f\', ';
            $strSQL .= 'uploaded=0, changed=1  WHERE idpenjualan=%d';
            db_query($strSQL, $totalOrder, $totalModal, $TotalPlusPpn, $TotalAfterVoucher, $TotalRound, $idPenjualan);
            /* End Update Penjualan */
            /* Update Hutang Penjualan */
            $caraJual = db_result(db_query('SELECT carabayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $kembali = db_result(db_query('SELECT kembali FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $idPelanggan = db_result(db_query('SELECT idpelanggan FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            $bayar = db_result(db_query('SELECT bayar FROM penjualan WHERE idpenjualan=%d',$idPenjualan));
            if (!empty($idPelanggan)){
                $besarHutang = $totalOrder - $bayar;
                $sqlUpdatePiutang = 'UPDATE detailpiutang SET besarhutang=\'%f\' ';
                $sqlUpdatePiutang .= 'WHERE idpelanggan=%d AND idpenjualan=%d';
                db_query($sqlUpdatePiutang, $besarHutang, $idPelanggan, $idPenjualan);
                syncHutangPelanggan($idPelanggan);
            }
            /* End Update Hutang Penjualan */
            $kembali = $bayar - $TotalRound;
            db_query('UPDATE penjualan SET kembali=\'%f\', uploaded=0, changed=1 WHERE idpenjualan=%d',$kembali,$idPenjualan);
            print $_POST['hargapokok'];
        }
	}
	exit();
}
function insert_detail_penjualan(){
	$executedSQL = array();
	if (isset($_POST['idpenjualan']) && isset($_POST['idproduk']) && isset($_POST['hargajual']) && isset($_POST['qty'])){
		if ($_POST['idpenjualan'] > 0 && $_POST['idproduk'] > 0 && $_POST['hargajual'] > 0 && $_POST['qty'] > 0){
			$strSQL = 'SELECT iddetail FROM detailpenjualan WHERE idproduct=%d AND idpenjualan=%d LIMIT 1';
			$idDetailExist = db_result(db_query($strSQL, $_POST['idproduk'],$_POST['idpenjualan']));
			if ($idDetailExist > 0){
				$strSQL = 'UPDATE detailpenjualan SET jumlah = jumlah + \'%f\', hargajual=\'%f\', ';
				$strSQL .= 'hargapokok=\'%f\', ';
				$strSQL .= 'diskon=\'%f\' WHERE iddetail=%d';
				$executedSQL[] = $strSQL;
				db_query(
					$strSQL,$_POST['qty'],$_POST['hargajual'],$_POST['hargapokok'],
					$_POST['diskon'],$idDetailExist
				);
			}else{
				$strSQL = 'INSERT INTO detailpenjualan (idpenjualan, idproduct, jumlah, ';
				$strSQL .= 'hargapokok, hargajual, diskon) VALUES ';
				$strSQL .= '(%d, %d, \'%f\', \'%f\', \'%f\', \'%f\')';
				$executedSQL[] = $strSQL;
				db_query(
					$strSQL,$_POST['idpenjualan'],$_POST['idproduk'],$_POST['qty'],
					$_POST['hargapokok'],$_POST['hargajual'],$_POST['diskon']
				);
			}
			$strSQL = 'SELECT SUM(hargajual*jumlah) AS total_order FROM detailpenjualan WHERE idpenjualan=%d';
			$totalOrder = db_result(db_query($strSQL,$_POST['idpenjualan']));
			$strSQL = 'SELECT SUM(hargapokok*jumlah) AS total_modal FROM detailpenjualan WHERE idpenjualan=%d';
			$totalModal = db_result(db_query($strSQL,$_POST['idpenjualan']));
            $strSQL = 'SELECT ppn FROM customer_order WHERE id=%d';
            $PpnValue = db_result(db_query($strSQL));
            $TotalPlusPpn = $totalOrder + ($totalOrder * $PpnValue/100);
			$strSQL = 'UPDATE penjualan SET total=\'%f\', totalmodal=\'%f\', total_plus_ppn=\'%f\', uploaded=0, changed=1  WHERE idpenjualan=%d';
			db_query($strSQL,$totalOrder,$totalModal,$TotalPlusPpn,$_POST['idpenjualan']);
			$stokproduct = db_result(db_query('SELECT stok FROM product WHERE idproduct=%d',$_POST['idproduk']));
			$stoksekarang = $stokproduct - $_POST['qty'];
			if (!empty($idproduct)){
				db_query("UPDATE product SET stok = stok - '%f',uploaded=0, changed=1  WHERE idproduct='%d'",$_POST['qty'],$_POST['idproduk']);
				$keterangan = 'Revisi Penjualan - Tambah Detail Penjualan';
				db_query("INSERT INTO transaksistock (idproduk, idpenjualan, stocksebelum, keluar, stocksetelah, keterangan) VALUES
				('%d', '%d', '%f', '%f', '%f', '%s')",$_POST['idproduk'],$_POST['idpenjualan'],
				$stokproduct,$_POST['qty'],$stoksekarang,$keterangan
				);
			}
			/* Update Hutang Penjualan */
			$caraJual = db_result(db_query('SELECT carabayar FROM penjualan WHERE idpenjualan=%d',$_POST['idpenjualan']));
			$kembali = db_result(db_query('SELECT kembali FROM penjualan WHERE idpenjualan=%d',$_POST['idpenjualan']));
			$idPelanggan = db_result(db_query('SELECT idpelanggan FROM penjualan WHERE idpenjualan=%d',$_POST['idpenjualan']));
			$bayar = db_result(db_query('SELECT bayar FROM penjualan WHERE idpenjualan=%d',$_POST['idpenjualan']));
			if (!empty($idPelanggan) && ($caraJual == 'HUTANG' || $kembali < 0)){
				$besarHutang = $TotalPlusPpn - $bayar;
				$sqlUpdatePiutang = 'UPDATE detailpiutang SET besarhutang=\'%f\' ';
				$sqlUpdatePiutang .= 'WHERE idpelanggan=%d AND idpenjualan=%d';
				db_query($sqlUpdatePiutang, $besarHutang, $idPelanggan, $_POST['idpenjualan']);
				syncHutangPelanggan($idPelanggan);
			}
			/* End Update Hutang Penjualan */
			$kembali = $bayar - $TotalPlusPpn;
			db_query('UPDATE penjualan SET kembali=\'%f\',uploaded=0, changed=1  WHERE idpenjualan=%d',$kembali,$_POST['idpenjualan']);
		}
		print json_encode($executedSQL);
	}
	exit();
}