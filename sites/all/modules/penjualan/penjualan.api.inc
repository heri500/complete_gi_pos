<?php
function JAM_KERJA_LAUNDRY(){
	return array(
		'normal' => array(
			'jam_masuk' => 7, 
			'jam_keluar' => 20, 
			'total_jam' => 13
		), 
		'minggu' => array(
			'jam_masuk' => 8, 
			'jam_keluar' => 16, 
			'total_jam' => 8
		)
	);	
}
function PERLAKUAN_KEMBALIAN(){
	return array('DIKEMBALIKAN','DISUMBANGKAN','DIMASUKAN DEPOSIT');
}

function getDataLaundry($nidtitipanlaundry = null){
	$dataLaundry = null;
	if (!empty($nidtitipanlaundry)){
		$dataLaundry = array();
		$strSQL = "SELECT a.idtitipanlaundry, a.nonota, a.tglpenjualan, a.idpemakai, b.name, a.idpelanggan, a.total, 
		a.totalmodal, a.carabayar, a.bayar, a.kembali, a.nokartu, a.keterangan, a.insert_date,a.status_laundry, 
		c.namapelanggan,c.telp,c.alamat, c.email
		FROM titipanlaundry a LEFT JOIN cms_users b ON a.idpemakai = b.uid 
		LEFT JOIN pelanggan c ON a.idpelanggan = c.idpelanggan WHERE a.idtitipanlaundry=%d";
		$result = db_query($strSQL,$nidtitipanlaundry);
		$dataLaundry['info'] = db_fetch_object($result);
		$dataLaundry['detail'] = getDetailLaundry($nidtitipanlaundry);
	}
	return $dataLaundry;
}
function getDetailLaundry($nidtitipanlaundry = null){
	$detailLaundry = null;
	if (!empty($nidtitipanlaundry)){
		$detailLaundry = array();
		$strSQL = "SELECT iddetail, idtitipanlaundry, a.idproduct, jumlah, a.hargapokok, a.hargajual, 
		a.diskon, status_laundry, sisa, diambil, b.namaproduct, b.satuan, masuk, perkiraan_ambil FROM detaillaundry a LEFT JOIN product b
		ON a.idproduct = b.idproduct WHERE a.idtitipanlaundry=%d ORDER BY perkiraan_ambil DESC";
		$result = db_query($strSQL,$nidtitipanlaundry);
		while ($data = db_fetch_object($result)){
			$data->masuk = date('Y-m-d H:i', $data->masuk);
			$data->perkiraan_ambil = date('Y-m-d H:i', $data->perkiraan_ambil);
			$detailLaundry[] = $data;
		}
	}
	return $detailLaundry;
}
function getDataPenjualan($nidpenjualan = null){
	$dataPenjualan = null;
	if (!empty($nidpenjualan)){
		$dataPenjualan = array();
		$strSQL = "SELECT idpenjualan, name, nonota, tglpenjualan, a.idpemakai, a.idpelanggan, total,
		totalmodal, carabayar, bayar, kembali, nokartu, a.keterangan, insert_date,idtitipanlaundry,
		perlakuankembalian,namapelanggan, ppn, (total*(ppn/100)) AS ppn_value, total_plus_ppn, a.idmeja, d.meja 
		FROM penjualan a 
		LEFT JOIN cms_users b ON a.idpemakai = b.uid
		LEFT JOIN pelanggan c ON a.idpelanggan = c.idpelanggan 
		LEFT JOIN meja d ON a.idmeja = d.id
		WHERE idpenjualan=%d";
		$result = db_query($strSQL,$nidpenjualan);
		$dataPenjualan['info'] = db_fetch_object($result);
        $dataPenjualan['info']->tglorder = $dataPenjualan['info']->tglpenjualan;

        $args = array($dataPenjualan['info']->idpemakai);
        $UserData = views_get_view('user_data');
        $UserData->set_display('default');
        $UserData->set_arguments($args);
        $UserData->pre_execute();
        $UserData->execute();
        if (!empty($UserData->result)) {
            $UserProfile = $UserData->result[0];
            $dataPenjualan['info']->kasir = $UserProfile->node_title;
        }else{
            $dataPenjualan['info']->kasir = $dataPenjualan['info']->idpemakai;
        }
		if (!empty($dataPenjualan['info']->idtitipanlaundry)){
			$dataPenjualan['laundry'] = getDataLaundry($dataPenjualan['info']->idtitipanlaundry);
		}
		$dataDetail = getDetailPenjualan($nidpenjualan);
        $dataDetailNew = array();
		for ($i = 0;$i < count($dataDetail);$i++){
            $dataDetailNew[] = array(
               'iddetail' => $dataDetail[$i]->iddetail,
               'idpenjualan' => $dataDetail[$i]->idpenjualan,
               'idproduct' => $dataDetail[$i]->idproduct,
               'jumlah' => $dataDetail[$i]->jumlah,
               'hargapokok' => $dataDetail[$i]->hargapokok,
               'hargajual' => $dataDetail[$i]->hargajual,
               'diskon' => $dataDetail[$i]->diskon,
               'namaproduct' => $dataDetail[$i]->namaproduct,
               'satuan' => $dataDetail[$i]->satuan,
               'harga_diskon' => $dataDetail[$i]->harga_diskon,
               'idkategori' => $dataDetail[$i]->idkategori,
               'kategori' => $dataDetail[$i]->kategori,
            );
        }
		$dataPenjualan['detail'] = $dataDetailNew;
        $dataPenjualan['detailbykategori'] = createDetailPenjualanByKategori($dataPenjualan['detail']);
	}
	return $dataPenjualan;
}
function getDetailPenjualan($nidpenjualan = null)
{
    $detailPenjualan = null;
    if (!empty($nidpenjualan)) {
        $detailPenjualan = array();
        $strSQL = "SELECT iddetail, idpenjualan, a.idproduct, jumlah, a.hargapokok, a.hargajual, 
		a.diskon, b.namaproduct, b.satuan, (a.hargajual - (a.hargajual * a.diskon/100)) AS harga_diskon,
		b.idkategori, c.kategori 
		FROM detailpenjualan a 
		LEFT JOIN product b ON a.idproduct = b.idproduct
		LEFT JOIN kategori c ON b.idkategori = c.idkategori
		WHERE a.idpenjualan=%d";
        $result = db_query($strSQL, $nidpenjualan);
        while ($data = db_fetch_object($result)) {
            $detailPenjualan[] = $data;
        }
    }
    return $detailPenjualan;
}
function createDetailPenjualanByKategori($detailPenjualan = null){
    $penjualanByKategori = array();
    if (count($detailPenjualan)){
        $kategoriInfo = array();
        for ($i = 0;$i < count($detailPenjualan);$i++){
            $idkategori = $detailPenjualan[$i]->idkategori;
            $kategoriInfo[$idkategori] = $detailPenjualan[$i]->kategori;
            $penjualanByKategori[$idkategori][] = $detailPenjualan[$i];
        }
    }
    return array($kategoriInfo,$penjualanByKategori);
}
function printPenjualanDetail(){
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
    _addBarcodePlugins();
    drupal_add_js($js_path);
    $js_path = drupal_get_path('module','penjualan').'/js/ghorder.barcode.js';
    drupal_add_js($js_path);
    $idPenjualan = isset($_GET['idpenjualangh']) ? $_GET['idpenjualangh'] : null;
    $dataOrder = getDataPenjualan($idPenjualan);
    $tablePrint = '';
    if (count($dataOrder)){
        $tablePrint = printKasirOrder($dataOrder);
        $tablePrint .= '<div>&nbsp;</div>';
        //$tablePrint .= '<div class="pagebreak">&nbsp;</div>';
        //$tablePrint .= printKasirOrder($dataOrder);
        //$tablePrint .= printOrderByCategory($dataOrder['detailbykategori'],$dataOrder['info']);
    }
    db_query('UPDATE customer_order SET printed=1 WHERE id=%d', $idGhOrder);
    return $tablePrint;
}
function printOutLaundry(){
	$dataPremis = getPremisInformation();
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
	drupal_add_js($js_path);
	global $user;
	$tablePrint = '';
	$nidtitipanlaundry = isset($_GET['nidlaundry']) ? $_GET['nidlaundry'] : null;
	$printHeader = false;
	if (!empty($nidtitipanlaundry)){
		global $user;
		$dataLaundry = getDataLaundry($nidtitipanlaundry);
		$tablePrint = '<table style="width: 240px;border-collapse: collapse;" >';
		$tablePrint .= '<tbody>';
		if ($printHeader){
			$tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-bottom: 1px dotted #444;border-top: 1px dotted #444;">'.$dataPremis->nama.'</td></tr>';
			$tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-top: 1px dotted #444;"><span style="float: left"><img src="'.base_path().'misc/media/images/bbm.png" width="12" style="vertical-align:middle;">&nbsp;'.$dataPremis->bbm.'</span><span style="float: right"><img src="'.base_path().'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">&nbsp;'.$dataPremis->whatsapp.'&nbsp;&nbsp;</span></td></tr>';
		}
		$tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 13px;">No. <strong>'.$dataLaundry['info']->nonota.'&nbsp;&nbsp;</strong></td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 13px;"><div style="width: 30%;float: left;">Nama</div><div style="width: 70%;float: left;">: <strong>'.$dataLaundry['info']->namapelanggan.'&nbsp;&nbsp;</strong></div></td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 13px;"><div style="width: 30%;float: left;">Telp/ HP</div><div style="width: 70%;float: left;">: '.$dataLaundry['info']->telp.'&nbsp;&nbsp;</div></td></tr>';
		$waktuPejualan = explode(' ', $dataLaundry['info']->tglpenjualan);
		$tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 13px;"><div style="width: 30%;float: left;">Terima</div><div style="width: 70%;float: left;"><span style="float: left;">: '.$waktuPejualan[0].'</span><span style="float: right;">'.substr($waktuPejualan[1],0,5).'&nbsp;</span></div></td></tr>';
		$waktuSelesai = explode(' ', $dataLaundry['detail'][0]->perkiraan_ambil);
		$tablePrint .= '<tr><td colspan="3" style="font-size: 14px;border-bottom: 1px dotted #444;line-height: 13px;"><div style="width: 30%;float: left;">Selesai</div><div style="width: 70%;float: left;"><span style="float: left;">: '.$waktuSelesai[0].'</span><span style="float: right;">'.$waktuSelesai[1].'&nbsp;</span></div></td></tr>';
		$totalLaundry = 0;
		$totalDiskon = 0;
		$totalItem = array();
		for ($i = 0;$i < count($dataLaundry['detail']);$i++){
			$dataDetail = $dataLaundry['detail'][$i];
			$tablePrint .= '<tr>';
			$tablePrint .= '<td colspan="3" style="font-size: 13px;line-height: 13px;"><strong>'.$dataDetail->namaproduct.'</strong></td>';
			$tablePrint .= '</tr>';
			if ($i == (count($dataLaundry['detail']) - 1)){
				$tablePrint .= '<tr style="border-bottom: 1px dotted #444;">';
			}else{
				$tablePrint .= '<tr>';
			}
			$tablePrint .= '<td style="font-size: 13px;width:30%;line-height: 13px;">'.$dataDetail->jumlah.' '.$dataDetail->satuan.'</td>';
			if (isset($totalItem[$dataDetail->satuan])){
				$totalItem[$dataDetail->satuan] = $totalItem[$dataDetail->satuan] + $dataDetail->jumlah;
			}else{
				$totalItem[$dataDetail->satuan] = $dataDetail->jumlah;
			}
			$tablePrint .= '<td style="font-size: 13px;width:30%;line-height: 13px;">Rp. '.number_format($dataDetail->hargajual,0,",",".").'</td>';
			$tablePrint .= '<td style="font-size: 13px;line-height: 13px;line-height: 13px;text-align: right;"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($dataDetail->hargajual*$dataDetail->jumlah,0,",",".").'&nbsp;</span></td>';
			$tablePrint .= '</tr>';
			$totalLaundry = $totalLaundry + ($dataDetail->hargajual*$dataDetail->jumlah);
			$totalDiskon = $totalDiskon + round(($dataDetail->diskon*$dataDetail->hargajual*$dataDetail->jumlah)/100,0);
		}
		$nilaiBayar = 0;
		if ($dataLaundry['info']->carabayar != 'KEMUDIAN'){
			$nilaiBayar = $dataLaundry['info']->bayar;
		}
		$sisaYangHarusDibayar = $totalLaundry - $nilaiBayar;
		$tablePrint .= '<tr>';
		$tablePrint .= '<td colspan="2" style="font-size: 13px;text-align: right;border-top: 1px dotted #444;">TOTAL :</td>';
		$tablePrint .= '<td style="font-size: 13px;text-align: right;border-top: 1px dotted #444;text-align: right"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($totalLaundry,0,",",".").'&nbsp;</span></td>';
		$tablePrint .= '</tr>';
		if ($totalDiskon > 0){
			$tablePrint .= '<tr>';
			$tablePrint .= '<td colspan="2" style="font-size: 13px;text-align: right;">DISKON :</td>';
			$tablePrint .= '<td style="font-size: 13px;text-align: right;text-align: right"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($totalDiskon,0,",",".").'&nbsp;</span></td>';
			$tablePrint .= '</tr>';
		}
		$tablePrint .= '<tr>';
		$tablePrint .= '<td colspan="2" style="font-size: 13px;text-align: right;">BAYAR :</td>';
		$tablePrint .= '<td style="font-size: 13px;text-align: right;text-align: right"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($nilaiBayar,0,",",".").'&nbsp;</span></td>';
		$tablePrint .= '</tr>';
		$tablePrint .= '<tr>';
		$tablePrint .= '<td colspan="2" style="font-size: 13px;text-align: right;border-bottom: 1px dotted #444;">SISA :</td>';
		$tablePrint .= '<td style="font-size: 13px;text-align: right;border-bottom: 1px dotted #444;text-align: right"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($sisaYangHarusDibayar,0,",",".").'&nbsp;</span></td>';
		$tablePrint .= '</tr>';
		if ($printHeader){
			$tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;">'.$dataPremis->alamat.'&nbsp;&nbsp;</td></tr>';
			//$tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-bottom: 1px dotted #444;">Blok C No.22, Sentul City - Bogor&nbsp;&nbsp;</td></tr>';
		}
		$totalItemView = '';
		if (count($totalItem)){
			$itemCount = 0;
			foreach($totalItem as $key => $value){
				if ($itemCount == 0){
					$totalItemView = $value.' '.$key;
				}else{
					$totalItemView .= '+'.$value.' '.$key;
				}
				$itemCount++;
			}
		}
		$statusPembayaran  = '<b>LUNAS</b>';
		if ($sisaYangHarusDibayar > 0){
			$statusPembayaran = '<b>BELUM LUNAS</b>';
		}
		$keterangan = !empty($dataLaundry['info']->keterangan) ? $dataLaundry['info']->keterangan : '-';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;line-height: 13px;">Total Qty Item : '.$totalItemView.'</td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;line-height: 13px;">Status : '.$statusPembayaran.'</td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;line-height: 13px;">Ket : '.$keterangan.'</td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;line-height: 13px;">Kasir : '.$user->name.'</td></tr>';
		$tablePrint .= '</tbody>';
		$tablePrint .= '</table>';
	}
	return $tablePrint;
}
function printOutPenjualan(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
	$dataPremis = getPremisInformation();
	$perlakuanKembalian = PERLAKUAN_KEMBALIAN();
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
	drupal_add_js($js_path);
	$tablePrint = '';
	$nidpenjualan = isset($_GET['idpenjualan']) ? $_GET['idpenjualan'] : null;
	$printHeader = true;
	$pathLogo = createLogoImagePath();
	if (!empty($nidpenjualan)){
		global $user;
		$dataPenjualan = getDataPenjualan($nidpenjualan);
		$tablePrint = '<table style="width: 240px;border-collapse: collapse;" >';
		$tablePrint .= '<tbody>';
		if ($printHeader){
			$tablePrint .= '<tr><td rowspan="3" style="border-bottom: 1px solid #000;border-top: 1px solid #000;width:50px;text-align: center;"><img height="48" src="'.$pathLogo.'" /></td><td colspan="2" style="font-size: 14px;font-weight: bold;text-align: center;border-bottom: 1px solid #000;border-top: 1px solid #000;">'.$dataPremis->nama.'</td></tr>';
			$tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;border-bottom: 1px solid #000;">'.$dataPremis->alamat.'&nbsp;&nbsp;</td></tr>';
			//$tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;border-bottom: 1px solid #000;">SENTUL CITY - BOGOR&nbsp;&nbsp;</td></tr>';
		}
		$tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;"><span style="float: left">No.</span><span style="float: right">'.$dataPenjualan['info']->nonota.'&nbsp;&nbsp;</span></td></tr>';
		if (!empty($dataPenjualan['info']->namapelanggan)){
			$tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;"><span style="float: left">Member</span><span style="float: right">'.$dataPenjualan['info']->namapelanggan.'&nbsp;&nbsp;</span></td></tr>';
		}
		$tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">Tanggal</span><span style="float: right">'.$dataPenjualan['info']->tglpenjualan.'&nbsp;&nbsp;</span></td></tr>';
		if (isset($dataPenjualan['laundry'])){
			$tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">MASUK : '.$dataPenjualan['laundry']['info']->tglpenjualan.'</span><span style="float: right">KASIR : '.$dataPenjualan['laundry']['info']->name.'&nbsp;&nbsp;</span></td></tr>';
			$tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">Status : AMBIL</span><span style="float: right">Bayar : '.$dataPenjualan['info']->carabayar.' '.$nilaiBayar.'&nbsp;&nbsp;</span></td></tr>';
		}
		$totalPenjualan = 0;
		for ($i = 0;$i < count($dataPenjualan['detail']);$i++){
			$dataDetail = $dataPenjualan['detail'][$i];
			$tablePrint .= '<tr>';
			$tablePrint .= '<td colspan="3" style="font-size: 13px;font-weight: bold;">'.$dataDetail->namaproduct.'</td>';
			$tablePrint .= '</tr>';
			$tablePrint .= '<tr>';
			$tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;">'.number_format($dataDetail->hargajual,2,$decimalSep,$thousandSep).'</td>';
			$tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;">x '.number_format($dataDetail->jumlah,2,$decimalSep,$thousandSep).'</td>';
			$tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;">'.number_format($dataDetail->hargajual*$dataDetail->jumlah,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</td>';
			$tablePrint .= '</tr>';
			$totalPenjualan = $totalPenjualan + ($dataDetail->hargajual*$dataDetail->jumlah);
		}
		$besarPpn = $dataPenjualan['info']->ppn;
		$totalPpn = $dataPenjualan['info']->ppn_value;
		$totalPlusPpn = $dataPenjualan['info']->total_plus_ppn;
		$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;border-top: 1px solid #000;"><span style="float: left">TOTAL</span><span style="float: right">'.number_format($totalPenjualan,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</span></td></tr>';
		if ($besarPpn > 0) {
			$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">PPN(' . $besarPpn . '%)</span><span style="float: right">' . number_format($totalPpn,2,$decimalSep,$thousandSep) . '&nbsp;&nbsp;</span></td></tr>';
			$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL+PPN</span><span style="float: right">' . number_format($totalPlusPpn,2,$decimalSep,$thousandSep) . '&nbsp;&nbsp;</span></td></tr>';
		}
		$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.strtoupper($dataPenjualan['info']->carabayar).'</span><span style="float: right">'.number_format($dataPenjualan['info']->bayar,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</span></td></tr>';
		if (($dataPenjualan['info']->carabayar == 'HUTANG' || $dataPenjualan['info']->kembali < 0) && !empty($dataPenjualan['info']->idpelanggan)){
			$totalHutang = getTotalHutangLatest($dataPenjualan['info']->idpelanggan);
			$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL HUTANG</span><span style="float: right">'.number_format($totalHutang,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</span></td></tr>';
		}
		$viewperlakuankembalian = '';
		if ($dataPenjualan['info']->perlakuankembalian != 0){
			$viewperlakuankembalian = '<span style="font-size: 13px;font-weight: bold;">'.$perlakuanKembalian[$dataPenjualan['info']->perlakuankembalian].'</span>';
		}
		if ($dataPenjualan['info']->kembali >= 0){
			$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">KEMBALI '.$viewperlakuankembalian.'</span><span style="float: right">'.number_format($dataPenjualan['info']->kembali,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</span></td></tr>';
		}
		$tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-top: 1px solid #000;">TERIMAKASIH</td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;">SILAHKAN DATANG KEMBALI</td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-top: 1px solid #000;">KRITIK/SARAN DAN PESAN ANTAR HUBUNGI</td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;font-weight: bold;"><span style="float: left"><img src="'.base_path().'misc/media/images/bbm.png" width="12" style="vertical-align:middle;">&nbsp;'.$dataPremis->bbm.'</span><span style="float: right"><img src="'.base_path().'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">&nbsp;'.$dataPremis->whatsapp.'&nbsp;&nbsp;</span></td></tr>';
		$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;text-align: center;border-bottom: 1px solid #000;">"ISLAM ITU INDAH"</td></tr>';
		$tablePrint .= '</tbody>';
		$tablePrint .= '</table>';
	}
	return $tablePrint;
}
function createDateRangeArray($strDateFrom,$strDateTo)
{
    $aryRange=array();
		$iDateFrom=mktime(1,0,0,substr($strDateFrom,5,2),     substr($strDateFrom,8,2),substr($strDateFrom,0,4));
    $iDateTo=mktime(1,0,0,substr($strDateTo,5,2),     substr($strDateTo,8,2),substr($strDateTo,0,4));

    if ($iDateTo>=$iDateFrom)
    {
        array_push($aryRange,date('Y-m-d',$iDateFrom)); // first entry
        while ($iDateFrom<$iDateTo)
        {
            $iDateFrom+=86400; // add 24 hours
            array_push($aryRange,date('Y-m-d',$iDateFrom));
        }
    }
    return $aryRange;
}
function createLogoImagePath(){
	$logoPath = base_path().file_directory_path().'/cti_flex_logo.png';
	drupal_add_js(
		array(
			'logo' => $logoPath,
		),
	'setting');
	return $logoPath;
}
function getTotalPenjualan($field_name = null, $tglAwal = null, $tglAkhir = null, $carabayar = 0, $idPelanggan = 0){
	if (!empty($field_name) && !empty($tglAwal) && !empty($tglAkhir)){
		$tglAwal = $tglAwal.' 00:00';
		$tglAkhir = $tglAkhir.' 23:59';
		if ($carabayar == 0){
			if (!empty($idPelanggan)){
				$totalPenjualan = db_result(db_query("SELECT SUM($field_name) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s' AND idpelanggan=%d",$tglAwal,$tglAkhir, $idPelanggan));
			}else{
				$totalPenjualan = db_result(db_query("SELECT SUM($field_name) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s'",$tglAwal,$tglAkhir));
			}
		}else if($carabayar == 1){
			//Untuk mendapatkan total cash saja
			if (empty($idPelanggan)){
				$strSQL = "SELECT SUM($field_name) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s' AND carabayar='TUNAI' AND kembali >= 0";
				$totalPenjualanCash = db_result(db_query($strSQL,$tglAwal,$tglAkhir));
			}else{
				$strSQL = "SELECT SUM($field_name) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s' AND carabayar='TUNAI' AND kembali >= 0 AND idpelanggan=%d";
				$totalPenjualanCash = db_result(db_query($strSQL,$tglAwal,$tglAkhir,$idPelanggan));
			}
			if (empty($idPelanggan)) {
				$strSQL = "SELECT SUM(bayar) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND ";
				$strSQL .= "'%s' AND ((carabayar='TUNAI' AND kembali < 0) OR (carabayar='HUTANG'))";
				$totalPenjualanDP = db_result(db_query($strSQL,$tglAwal,$tglAkhir));
			}else{
				$strSQL = "SELECT SUM(bayar) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND ";
				$strSQL .= "'%s' AND ((carabayar='TUNAI' AND kembali < 0) OR (carabayar='HUTANG')) AND idpelanggan = %d";
				$totalPenjualanDP = db_result(db_query($strSQL,$tglAwal,$tglAkhir,$idPelanggan));
			}
			$totalPenjualan = $totalPenjualanCash + $totalPenjualanDP;
		}else if($carabayar == 2){
			//Untuk mendapatkan total hutang dari penjualan
			if (empty($idPelanggan)) {
				$strSQL = "SELECT SUM(kembali) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND ";
				$strSQL .= "'%s' AND ((carabayar='TUNAI' AND kembali < 0) OR (carabayar='HUTANG'))";
				$totalPenjualan = db_result(db_query($strSQL,$tglAwal,$tglAkhir));
			}else{
				$strSQL = "SELECT SUM(kembali) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND ";
				$strSQL .= "'%s' AND ((carabayar='TUNAI' AND kembali < 0) OR (carabayar='HUTANG')) AND idpelanggan = %d";
				$totalPenjualan = db_result(db_query($strSQL,$tglAwal,$tglAkhir,$idPelanggan));
			}
		}else if($carabayar == 3){
			//Untuk mendapatkan total debit/giro saja
			if (empty($idPelanggan)) {
				$totalPenjualan = db_result(
					db_query(
						"SELECT SUM($field_name) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s' AND carabayar IN ('DEBIT','GIRO')",
						$tglAwal,
						$tglAkhir
					)
				);
			}else{
				$totalPenjualan = db_result(
					db_query(
						"SELECT SUM($field_name) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s' AND carabayar IN ('DEBIT','GIRO') AND idpelanggan=%d",
						$tglAwal,
						$tglAkhir,
						$idPelanggan
					)
				);
			}
		}else if($carabayar == 4){
			//Untuk mendapatkan total pembayaran menggunakan deposit
			if (empty($idPelanggan)) {
				$totalPenjualan = db_result(
					db_query(
						"SELECT SUM($field_name) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s' AND carabayar = 'DEPOSIT'",
						$tglAwal,
						$tglAkhir
					)
				);
			}else{
				$totalPenjualan = db_result(
					db_query(
						"SELECT SUM($field_name) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s' AND carabayar = 'DEPOSIT' AND idpelanggan=%d",
						$tglAwal,
						$tglAkhir,
						$idPelanggan
					)
				);
			}
		}
	}
	return $totalPenjualan;
}
function getTotalPenjualan2($field_name1 = null, $field_name2 = null, $tglAwal = null, $tglAkhir = null, $idSupplier = 0){
	if (!empty($field_name1) && !empty($field_name2) && !empty($tglAwal) && !empty($tglAkhir)){
		$tglAwal = $tglAwal.' 00:00';
		$tglAkhir = $tglAkhir.' 23:59';
		if (empty($idSupplier)) {
			$totalPenjualan = db_result(
				db_query(
					"SELECT SUM($field_name1*$field_name2) FROM detailpenjualan AS detail
					LEFT JOIN penjualan AS penj ON detail.idpenjualan = penj.idpenjualan
					WHERE penj.tglpenjualan BETWEEN '%s' AND '%s'",
					$tglAwal,
					$tglAkhir
				)
			);
		}else{
			$totalPenjualan = db_result(
				db_query(
					"SELECT SUM($field_name1*$field_name2) FROM detailpenjualan AS detail
					LEFT JOIN penjualan AS penj ON detail.idpenjualan = penj.idpenjualan
					LEFT JOIN product AS prod ON detail.idproduct = prod.idproduct
					WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' AND prod.idsupplier=%d ",
					$tglAwal,
					$tglAkhir,
					$idSupplier
				)
			);
		}
	}
	return $totalPenjualan;
}
function getTotalCustomerOrder($field_name = null, $tglAwal = null, $tglAkhir = null, $carabayar = 0, $idPelanggan = 0){
	if (!empty($field_name) && !empty($tglAwal) && !empty($tglAkhir)){
		$tglAwal = $tglAwal.' 00:00';
		$tglAkhir = $tglAkhir.' 23:59';
		if ($carabayar == 0){
			if (!empty($idPelanggan)){
				$totalCustomerOrder = db_result(db_query("SELECT SUM($field_name) FROM customer_order WHERE tglorder BETWEEN '%s' AND '%s' AND idpelanggan=%d",$tglAwal,$tglAkhir, $idPelanggan));
			}else{
				$totalCustomerOrder = db_result(db_query("SELECT SUM($field_name) FROM customer_order WHERE tglorder BETWEEN '%s' AND '%s'",$tglAwal,$tglAkhir));
			}
		}else if($carabayar == 1){
			//Untuk mendapatkan total cash saja
			if (empty($idPelanggan)){
				$strSQL = "SELECT SUM($field_name) FROM customer_order WHERE tglorder BETWEEN '%s' AND '%s' AND carabayar='TUNAI' AND kembali >= 0";
				$totalCustomerOrderCash = db_result(db_query($strSQL,$tglAwal,$tglAkhir));
			}else{
				$strSQL = "SELECT SUM($field_name) FROM customer_order WHERE tglorder BETWEEN '%s' AND '%s' AND carabayar='TUNAI' AND kembali >= 0 AND idpelanggan=%d";
				$totalCustomerOrderCash = db_result(db_query($strSQL,$tglAwal,$tglAkhir,$idPelanggan));
			}
			if (empty($idPelanggan)) {
				$strSQL = "SELECT SUM(bayar) FROM customer_order WHERE tglorder BETWEEN '%s' AND ";
				$strSQL .= "'%s' AND ((carabayar='TUNAI' AND kembali < 0) OR (carabayar='KEMUDIAN'))";
				$totalCustomerOrderDP = db_result(db_query($strSQL,$tglAwal,$tglAkhir));
			}else{
				$strSQL = "SELECT SUM(bayar) FROM customer_order WHERE tglorder BETWEEN '%s' AND ";
				$strSQL .= "'%s' AND ((carabayar='TUNAI' AND kembali < 0) OR (carabayar='KEMUDIAN')) AND idpelanggan = %d";
				$totalCustomerOrderDP = db_result(db_query($strSQL,$tglAwal,$tglAkhir,$idPelanggan));
			}
			$totalCustomerOrder = $totalCustomerOrderCash + $totalCustomerOrderDP;
		}else if($carabayar == 2){
			//Untuk mendapatkan total hutang dari customer_order
			if (empty($idPelanggan)) {
				$strSQL = "SELECT SUM(kembali) FROM customer_order WHERE tglorder BETWEEN '%s' AND ";
				$strSQL .= "'%s' AND ((carabayar='TUNAI' AND kembali < 0) OR (carabayar='KEMUDIAN'))";
				$totalCustomerOrder = db_result(db_query($strSQL,$tglAwal,$tglAkhir));
			}else{
				$strSQL = "SELECT SUM(kembali) FROM customer_order WHERE tglorder BETWEEN '%s' AND ";
				$strSQL .= "'%s' AND ((carabayar='TUNAI' AND kembali < 0) OR (carabayar='KEMUDIAN')) AND idpelanggan = %d";
				$totalCustomerOrder = db_result(db_query($strSQL,$tglAwal,$tglAkhir,$idPelanggan));
			}
		}else if($carabayar == 3){
			//Untuk mendapatkan total debit/giro saja
			if (empty($idPelanggan)) {
				$totalCustomerOrder = db_result(
					db_query(
						"SELECT SUM($field_name) FROM customer_order WHERE tglorder BETWEEN '%s' AND '%s' AND carabayar IN ('DEBIT','GIRO')",
						$tglAwal,
						$tglAkhir
					)
				);
			}else{
				$totalCustomerOrder = db_result(
					db_query(
						"SELECT SUM($field_name) FROM customer_order WHERE tglorder BETWEEN '%s' AND '%s' AND carabayar IN ('DEBIT','GIRO') AND idpelanggan=%d",
						$tglAwal,
						$tglAkhir,
						$idPelanggan
					)
				);
			}
		}
	}
	return $totalCustomerOrder;
}
function getRandomString($length=22)
{
	$key = '';
	$keys = array_merge(range(0, 9));
	for ($i = 0; $i < $length; $i++) {
		mt_srand((double)microtime() * 10000000);
		$key .= $keys[array_rand($keys)];
	}
	return $key;
}
function createEAN13Code($number){
	$code = '899' . str_pad($number, 9, '0');
	$weightflag = true;
	$sum = 0;
	// Weight for a digit in the checksum is 3, 1, 3.. starting from the last digit.
	// loop backwards to make the loop length-agnostic. The same basic functionality
	// will work for codes of different lengths.
	for ($i = strlen($code) - 1; $i >= 0; $i--)
	{
		$sum += (int)$code[$i] * ($weightflag?3:1);
		$weightflag = !$weightflag;
	}
	$code .= (10 - ($sum % 10)) % 10;
	return $code;
}
function printCustomerOrderBarcode(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
	_addBarcodePlugins();
	drupal_add_js($js_path);
	$js_path = drupal_get_path('module','penjualan').'/js/order.barcode.js';
	drupal_add_js($js_path);
	$tablePrint = '';
	$nidorder = isset($_GET['idcustomerorder']) ? $_GET['idcustomerorder'] : null;
	if (!empty($nidorder)){
		$dataOrder = getCustomerOrder($nidorder);
		if (count($dataOrder)){
			$tablePrint = '<table style="width: 100px;border-collapse: collapse;" >';
			$tablePrint .= '<tbody>';
			$tablePrint .= '<tr><td id="'.$dataOrder['info']->nonota.'" class="barcode-place"></td></tr>';
			$tablePrint .= '<tr><td style="text-align: center;">'.$dataOrder['info']->namapelanggan.'</td></tr>';
			$tablePrint .= '</tbody>';
			$tablePrint .= '</table>';
		}
	}
	return $tablePrint;
}
function printProductBarcode(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
	_addBarcodePlugins();
	drupal_add_js($js_path);
	$js_path = drupal_get_path('module','penjualan').'/js/product.barcode.js';
	drupal_add_js($js_path);
	$tablePrint = '';
	$nidProduct = isset($_GET['idproduct']) ? $_GET['idproduct'] : null;
	$explodeNid = explode('__',$nidProduct);
	if (!empty($explodeNid) && count($explodeNid)){
		for ($i = 0;$i < count($explodeNid);$i++){
			$nidProduct = $explodeNid[$i];
			$dataProduct = getDataProduct($nidProduct);
			if (count($dataProduct)){
				if ($i == 0){
					$tablePrint .= '<table style="width: 186px;border-collapse: collapse;" >';
					$tablePrint .= '<tbody>';
				}else{
					$tablePrint .= '<table style="width: 186px;border-collapse: collapse;" class="pagebreak">';
					$tablePrint .= '<tbody>';
					$tablePrint .= '<tr><td style="font-size: 10px;">&nbsp;</td></tr>';
				}
				$tablePrint .= '<tr><td>';
				$tablePrint .= '<div id="'.$dataProduct['info']->barcode.'" class="barcode-place" align="center" style="width: 166px;"></div>';
				$tablePrint .= '<div class="small-font" align="center" style="width: 100%;">'.$dataProduct['info']->barcode.'</div>';
				$tablePrint .= '</td></tr>';
				$tablePrint .= '<tr class="standard-font"><td style="text-align: center;">'.$dataProduct['info']->namaproduct.'</td></tr>';
				$tablePrint .= '<tr class="small-font"><td style="text-align: center;"><span style="float: left;">&nbsp;&nbsp;&nbsp;'.$dataProduct['info']->alt_code.'</span><span style="float: right;">'.$currencySym.' '.number_format($dataProduct['info']->hargajual,2,$decimalSep,$thousandSep).',-&nbsp;&nbsp;&nbsp;</span></td></tr>';
				$tablePrint .= '</tbody>';
				$tablePrint .= '</table>';
				$tablePrint .= '</div>';
			}
		}
	}
	return $tablePrint;
}
function printProductBarcodeWithLogo(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
	_addBarcodePlugins();
	drupal_add_js($js_path);
	$js_path = drupal_get_path('module','penjualan').'/js/productlogo.barcode.js';
	drupal_add_js($js_path);
	$tablePrint = '';
	$nidProduct = isset($_GET['idproductlogo']) ? $_GET['idproductlogo'] : null;
	$explodeNid = explode('__',$nidProduct);
	$pathLogo = createLogoImagePath();
	if (!empty($explodeNid) && count($explodeNid)){
		for ($i = 0;$i < count($explodeNid);$i++) {
			$nidProduct = $explodeNid[$i];
			$dataProduct = getDataProduct($nidProduct);
			if (count($dataProduct)) {
				if ($i == 0){
					$tablePrint .= '<table style="width: 190px;border-collapse: collapse;" >';
					$tablePrint .= '<tbody>';
				}else{
					$tablePrint .= '<table style="width: 190px;border-collapse: collapse;" class="pagebreak">';
					$tablePrint .= '<tbody>';
					$tablePrint .= '<tr><td colspan="2" style="font-size: 8px;">&nbsp;</td></tr>';
				}
				$tablePrint .= '<tr>';
				$tablePrint .= '<td rowspan="3" class="logo-place">';
				$tablePrint .= '<img height="68" src="' . $pathLogo . '" />';
				$tablePrint .= '</td>';
				$tablePrint .= '<td>';
				$tablePrint .= '<div id="' . $dataProduct['info']->barcode . '" class="barcode-place" align="center"></div>';
				$tablePrint .= '<div class="small-font" align="center" style="width: 100%;">' . $dataProduct['info']->barcode . '</div>';
				$tablePrint .= '</td></tr>';
				$tablePrint .= '<tr class="standard-font"><td align="center">' . $dataProduct['info']->namaproduct . '</td></tr>';
				$tablePrint .= '<tr class="small-font"><td align="center"><span>'.$currencySym.' ' . number_format(
						$dataProduct['info']->hargajual,
						2,
						$decimalSep,
						$thousandSep
					) . '</span></td></tr>';
				$tablePrint .= '</tbody>';
				$tablePrint .= '</table>';
			}
		}
	}
	return $tablePrint;
}
function printCustomerOrderLogo(){
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
	_addBarcodePlugins();
	drupal_add_js($js_path);
	$js_path = drupal_get_path('module','penjualan').'/js/orderlogo.barcode.js';
	drupal_add_js($js_path);
	$tablePrint = '';
	$nidOrder = isset($_GET['idorderlogo']) ? $_GET['idorderlogo'] : null;
	$pathLogo = createLogoImagePath();
	if ($nidOrder > 0){
		$detailOrders = getDetailOrder($nidOrder);
		if (count($detailOrders)) {
			$counterData = 0;
			for ($i = 0;$i < count($detailOrders);$i++){
				$dataOrder = $detailOrders[$i];
				for ($j = 0;$j < (int)$dataOrder['outstanding'];$j++){
					if ($counterData == 0){
						$tablePrint .= '<table style="width: 190px;border-collapse: collapse;" >';
						$tablePrint .= '<tbody>';
					}else{
						$tablePrint .= '<table style="width: 190px;border-collapse: collapse;" class="pagebreak">';
						$tablePrint .= '<tbody>';
						$tablePrint .= '<tr><td colspan="2" style="font-size: 8px;">&nbsp;</td></tr>';
					}
					$tablePrint .= '<tr>';
					$tablePrint .= '<td rowspan="3" class="logo-place">';
					$tablePrint .= '<img height="68" src="' . $pathLogo . '" />';
					$tablePrint .= '</td>';
					$tablePrint .= '<td>';
					$tablePrint .= '<div id="' . $dataOrder['detailbarcode'] . '" class="barcode-place" align="center"></div>';
					$tablePrint .= '<div class="small-font" align="center" style="width: 100%;">' . $dataOrder['detailbarcode'] . '</div>';
					$tablePrint .= '</td></tr>';
					$tablePrint .= '<tr class="standard-font"><td align="center">' . $dataOrder['namapelanggan'] . '</td></tr>';
					$tablePrint .= '<tr class="small-font"><td align="center">' . $dataOrder['namaproduct'] . ' ['.($j + 1).']</td></tr>';
					$tablePrint .= '</tbody>';
					$tablePrint .= '</table>';
					$counterData++;
				}
			}
		}
	}
	return $tablePrint;
}
function printCustomerOrderBarcodeNew($totalPrint = 1){
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
	_addBarcodePlugins();
	drupal_add_js($js_path);
	$js_path = drupal_get_path('module','penjualan').'/js/orderlogo.barcode.js';
	drupal_add_js($js_path);
	$tablePrint = '';
	$nidOrder = isset($_GET['idorder']) ? $_GET['idorder'] : null;
	$pathLogo = createLogoImagePath();
	if ($nidOrder > 0){
		$detailOrders = getCustomerOrder($nidOrder);
		if (isset($detailOrders['info']) && count($detailOrders['info'])) {
			$dataOrder = $detailOrders['info'];
            $counterData = 0;
			for ($i = 0;$i < $totalPrint;$i++){
				if ($counterData == 0){
					$tablePrint .= '<table style="width: 190px;border-collapse: collapse;" >';
					$tablePrint .= '<tbody>';
				}else{
					$tablePrint .= '<table style="width: 190px;border-collapse: collapse;" class="pagebreak">';
					$tablePrint .= '<tbody>';
					$tablePrint .= '<tr><td colspan="2" style="font-size: 8px;">&nbsp;</td></tr>';
				}
				$tablePrint .= '<tr>';
				$tablePrint .= '<td rowspan="3" class="logo-place">';
				$tablePrint .= '<img height="68" src="' . $pathLogo . '" />';
				$tablePrint .= '</td>';
				$tablePrint .= '<td>';
				$tablePrint .= '<div id="' . $dataOrder->nonota . '" class="barcode-place" align="center"></div>';
				$tablePrint .= '<div class="small-font" align="center" style="width: 100%;">' . $dataOrder->nonota . '</div>';
				$tablePrint .= '</td></tr>';
				$tablePrint .= '<tr class="standard-font"><td align="center">' . $dataOrder->namapelanggan . '</td></tr>';
				$splitTgl = explode(' ',$dataOrder->tglorder);
				$tablePrint .= '<tr class="small-font"><td align="center"><span class="pull-left">'.$splitTgl[0].'</span><span class="pull-right">Rp. ' . number_format($dataOrder->total,0,',','.') . '</span></td></tr>';
				$tablePrint .= '</tbody>';
				$tablePrint .= '</table>';
                $counterData++;
			}
		}
	}
	return $tablePrint;
}
function printMejaBarcodeWithLogo(){
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
	_addBarcodePlugins();
	drupal_add_js($js_path);
	$js_path = drupal_get_path('module','penjualan').'/js/productlogo.barcode.js';
	drupal_add_js($js_path);
	$tablePrint = '';
	$nidMeja = isset($_GET['idmeja']) ? $_GET['idmeja'] : null;
	$explodeNid = explode('__',$nidMeja);
	$pathLogo = createLogoImagePath();
	if (!empty($explodeNid) && count($explodeNid)){
		for ($i = 0;$i < count($explodeNid);$i++) {
			$nidMeja = $explodeNid[$i];
			$dataMeja = getDataMeja($nidMeja);
            if (count($dataMeja)) {
				if ($i == 0){
					$tablePrint .= '<table style="width: 190px;border-collapse: collapse;" >';
					$tablePrint .= '<tbody>';
				}else{
					$tablePrint .= '<table style="width: 190px;border-collapse: collapse;" class="pagebreak">';
					$tablePrint .= '<tbody>';
					$tablePrint .= '<tr><td colspan="2" style="font-size: 8px;">&nbsp;</td></tr>';
				}
				$tablePrint .= '<tr>';
				$tablePrint .= '<td rowspan="3" class="logo-place">';
				$tablePrint .= '<img height="68" src="' . $pathLogo . '" />';
				$tablePrint .= '</td>';
				$tablePrint .= '<td>';
				$tablePrint .= '<div id="' . $dataMeja['info']->barcodemeja . '" class="barcode-place" align="center"></div>';
				$tablePrint .= '<div class="small-font" align="center" style="width: 100%;">' . $dataMeja['info']->barcodemeja . '</div>';
				$tablePrint .= '</td></tr>';
				$tablePrint .= '<tr class="standard-font"><td align="center">' . $dataMeja['info']->kodemeja . '</td></tr>';
                $tablePrint .= '<tr class="standard-font"><td align="center">' . $dataMeja['info']->meja . '</td></tr>';
				$tablePrint .= '</tbody>';
				$tablePrint .= '</table>';
			}
		}
	}
	return $tablePrint;
}
//Print customer order GHC
function printGHCOrder_old(){
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
    _addBarcodePlugins();
    drupal_add_js($js_path);
    $js_path = drupal_get_path('module','penjualan').'/js/ghorder.barcode.js';
    drupal_add_js($js_path);
    $idGhOrder = isset($_GET['idghorder']) ? $_GET['idghorder'] : null;
    $dataOrder = getCustomerOrder($idGhOrder);
    $tablePrint = '';
    if (count($dataOrder)){
        $tablePrint = printKasirOrder($dataOrder);
        $tablePrint .= '<div>&nbsp;</div>';
        $tablePrint .= '<div class="pagebreak">&nbsp;</div>';
        $tablePrint .= printKasirOrder($dataOrder);
        $tablePrint .= printOrderByCategory($dataOrder['detailbykategori'],$dataOrder['info']);
    }
	db_query('UPDATE customer_order SET printed=1 WHERE id=%d', $idGhOrder);
    return $tablePrint;
}
function printGHCOrder(){
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
    _addBarcodePlugins();
    drupal_add_js($js_path);
    $js_path = drupal_get_path('module','penjualan').'/js/ghorder.barcode.js';
    drupal_add_js($js_path);
    $idGhOrder = isset($_GET['idghorder']) ? $_GET['idghorder'] : null;
    $dataOrder = getCustomerOrder($idGhOrder);
    $tablePrint = '';
    if (count($dataOrder)){
        $tablePrint = printKasirOrder($dataOrder);
        $tablePrint .= '<div>&nbsp;</div>';
        $tablePrint .= '<div class="pagebreak">&nbsp;</div>';
        $tablePrint .= printKasirOrder($dataOrder);
        $tablePrint .= printOrderByCategoryNew($dataOrder['detailbykategori'],$dataOrder['info']);
    }
    db_query('UPDATE customer_order SET printed=1 WHERE id=%d', $idGhOrder);
    return $tablePrint;
}
//Print customer order only GHC
function printGHCOrderOnly($TotalPrint = 1, $PrintCategory = 0){
    $idGhOrder = isset($_GET['idghorderonly']) ? $_GET['idghorderonly'] : null;
    $tablePrint = '';
    if (!empty($idGhOrder)) {
        drupal_add_js('misc/media/jquery-1.4.4.min.js');
        $js_path = drupal_get_path('module', 'penjualan') . '/js/direct.print.js';
        _addBarcodePlugins();
        drupal_add_js($js_path);
        drupal_add_js(
            array(
                'print_category' => $PrintCategory,
                'id_order' => $idGhOrder,
            ),
            'setting'
        );
        $js_path = drupal_get_path('module', 'penjualan') . '/js/ghorder.barcode.js';
        drupal_add_js($js_path);
        $dataOrder = getCustomerOrder($idGhOrder);
        if (count($dataOrder)) {
            for ($i = 0; $i < $TotalPrint; $i++) {
                $tablePrint .= printKasirOrder($dataOrder);
                $tablePrint .= '<div>&nbsp;</div>';
            }
        }
        db_query('UPDATE customer_order SET printed=1 WHERE id=%d', $idGhOrder);
    }
	return $tablePrint;
}
function printKasirOrder($dataOrder = null){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    if (count($dataOrder)){
		$dataPremis = getPremisInformation();
        $DecDigit = $dataPremis->decimal_digit;
        $printHeader = true;
        $pathLogo = createLogoImagePath();
        if (count($dataOrder)){
            $orderInfo = $dataOrder['info'];
            $orderDetail = $dataOrder['detail'];
            $tablePrint = '<table style="width: 305px;border-collapse: collapse;" >';
            $tablePrint .= '<tbody>';
            if ($printHeader){
                $tablePrint .= '<tr><td rowspan="3" style="border-bottom: 1px solid #000;border-top: 1px solid #000;width:50px;text-align: center;"><img height="48" src="'.$pathLogo.'" /></td><td colspan="2" style="font-size: 14px;font-weight: bold;text-align: center;border-bottom: 1px solid #000;border-top: 1px solid #000;">'.$dataPremis->nama.'</td></tr>';
                $tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;border-bottom: 1px solid #000;">'.$dataPremis->alamat.'&nbsp;&nbsp;</td></tr>';
                //$tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;">SENTUL CITY - BOGOR&nbsp;&nbsp;</td></tr>';
            }
            $tablePrint .= '<tr><td colspan="3" style="text-align: center;padding: 4px;"><div align="center"><div id="'.$orderInfo->nonota.'" class="barcode-place"></div></div></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;">'.$orderInfo->nonota.'</td></tr>';
            if (!empty($orderInfo->namapelanggan)){
                $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;"><span style="float: left">Member</span><span style="float: right">'.$orderInfo->namapelanggan.'&nbsp;&nbsp;</span></td></tr>';
            }
            if (!empty($orderInfo->meja)) {
                $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">Meja</span><span style="float: right">' . strtoupper($orderInfo->meja) . '</span></td></tr>';
            }
            //Informasi Tanggal
            $tablePrint .= '<tr>';
            if (!empty($orderInfo->kasir)) {
                $tablePrint .= '<td colspan="3" style="font-size: 14px;text-align: center;">';
            }else{
                $tablePrint .= '<td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;">';
            }
            $tablePrint .= '<span style="float: left">'.ucfirst(trans('tanggal', $dataPremis->bahasa)).'</span>';
            $tablePrint .= '<span style="float: right">'.substr($orderInfo->tglorder,0,16).'</span>';
            $tablePrint .= '</td>';
            $tablePrint .= '</tr>';
            //Informasi Kasir
            if (!empty($orderInfo->kasir)) {
                $tablePrint .= '<tr>';
                $tablePrint .= '<td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;">';
                $tablePrint .= '<span style="float: left">' . ucfirst(trans('kasir', $dataPremis->bahasa)) . '</span>';
                $tablePrint .= '<span style="float: right">' . $orderInfo->kasir . '</span>';
                $tablePrint .= '</td>';
                $tablePrint .= '</tr>';
            }
			$totalOrder = 0;
            for ($i = 0;$i < count($orderDetail);$i++){
                $dataDetail = $orderDetail[$i];
                $tablePrint .= '<tr>';
                $tablePrint .= '<td colspan="2" style="font-size: 13px;">'.strtoupper($dataDetail['namaproduct']).'</td>';
                $HargaDiskon = $dataDetail['hargajual'] - ($dataDetail['hargajual'] * ($dataDetail['diskon']/100));
                $tablePrint .= '<td style="font-size: 12px;text-align: right;width: 100px;">'.number_format($HargaDiskon,$DecDigit,$decimalSep,$thousandSep).' x '.$dataDetail['jumlah'].'</td>';
                $tablePrint .= '</tr>';
				$totalOrder = $totalOrder + ($HargaDiskon*$dataDetail['jumlah']);
            }
			$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;border-top: 1px solid #000;"><span style="float: left">TOTAL</span><span style="float: right">'.number_format($totalOrder,$DecDigit,$decimalSep,$thousandSep).'</span></td></tr>';
            $besarPpn = $dataOrder['info']->ppn;
            $totalPpn = $dataOrder['info']->ppn_value;
            $totalPlusPpn = $totalOrder + ($totalOrder * $besarPpn/100);
            if ($besarPpn > 0) {
                $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.$gstSym.'(' . $besarPpn . '%)</span><span style="float: right">' . number_format($totalPpn,$DecDigit,$decimalSep,$thousandSep) . '</span></td></tr>';
                $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL+'.$gstSym.'</span><span style="float: right">' . number_format($totalPlusPpn,$DecDigit,$decimalSep,$thousandSep) . '</span></td></tr>';
            }
            $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">BAYAR ('.$dataOrder['info']->carabayar.')</span><span style="float: right">' . number_format($dataOrder['info']->bayar,$DecDigit,$decimalSep,$thousandSep) . '</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.strtoupper(trans('kembali', $dataPremis->bahasa)).'</span><span style="float: right">' . number_format($dataOrder['info']->kembali,$DecDigit,$decimalSep,$thousandSep) . '</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-top: 1px solid #000;">'.strtoupper(trans('terimakasih', $dataPremis->bahasa)).'</td></tr>';
            $Madah = variable_get('madah_slogan','INGAT MAKAN INGAT TUHAN');
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;">'.strtoupper($Madah).'</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-top: 1px solid #000;">'.$dataPremis->kritik_saran.'</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;font-weight: bold;">';
            if (!empty($dataPremis->bbm)) {
                $tablePrint .= '<span style="float: left">';
                $tablePrint .= '<img src="' . base_path() . 'misc/media/images/bbm.png" width="12" style="vertical-align:middle;">';
                $tablePrint .= '&nbsp;' . $dataPremis->bbm . '</span>';
            }
            if (!empty($dataPremis->bbm) && !empty($dataPremis->whatsapp)) {
                $tablePrint .= '<span style="float: right">';
                $tablePrint .= '<img src="' . base_path() . 'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">';
                $tablePrint .= '&nbsp;' . $dataPremis->whatsapp . '&nbsp;&nbsp;</span>';
            }else if(empty($dataPremis->bbm) && !empty($dataPremis->whatsapp)){
                $tablePrint .= '<span style="float: center">';
                $tablePrint .= '<img src="' . base_path() . 'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">';
                $tablePrint .= '&nbsp;' . $dataPremis->whatsapp . '&nbsp;&nbsp;</span>';
            }else{
                $tablePrint .= '&nbsp;';
            }
            $tablePrint .= '</td></tr>';
            $tablePrint .= '</tbody>';
            $tablePrint .= '</table>';
        }
    }
    return $tablePrint;
}

function printKasirOrderSoft($dataOrder = null){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    if (count($dataOrder)){
        $dataPremis = getPremisInformation();
        $DecDigit = $dataPremis->decimal_digit;
        $printHeader = true;
        $pathLogo = createLogoImagePath();
        if (count($dataOrder)){
            $orderInfo = $dataOrder['info'];
            $orderDetail = $dataOrder['detail'];
            $tablePrint = '<div style="width: 320px;" >';
            if ($printHeader){
                $tablePrint .= '------------------------------';
                $tablePrint .= '<h3>'.$dataPremis->nama.'</h3>';
                $tablePrint .= '<h4>'.$dataPremis->alamat.'</h4>';
                $tablePrint .= '------------------------------';
                //$tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;">SENTUL CITY - BOGOR&nbsp;&nbsp;</td></tr>';
            }
            $tablePrint .= '<div id="'.$orderInfo->nonota.'" class="barcode-place"></div>';
            $tablePrint .= '<div style="font-size: 14px;text-align: center;">'.$orderInfo->nonota.'</div>';
            if (!empty($orderInfo->namapelanggan)){
                $tablePrint .= '<div style="font-size: 14px;text-align: center;"><span style="float: left">Member</span><span style="float: right">'.$orderInfo->namapelanggan.'&nbsp;&nbsp;</span></div>';
            }
            if (!empty($orderInfo->meja)) {
                $tablePrint .= '<div style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">Meja</span><span style="float: right">' . strtoupper($orderInfo->meja) . '</span></div>';
            }
            $tablePrint .= '<div style="font-size: 14px;text-align: center;"><span style="float: left">'.ucfirst(trans('tanggal', $dataPremis->bahasa)).'</span><span style="float: right">'.substr($orderInfo->tglorder,0,16).'</span></div>';
            $totalOrder = 0;
            for ($i = 0;$i < count($orderDetail);$i++){
                $dataDetail = $orderDetail[$i];
                $tablePrint .= '<div style="font-size: 13px;">'.strtoupper($dataDetail['namaproduct']).'</div>';
                $HargaDiskon = $dataDetail['hargajual'] - ($dataDetail['hargajual'] * ($dataDetail['diskon']/100));
                $tablePrint .= '<div style="font-size: 12px;text-align: right;width: 100px;">'.number_format($HargaDiskon,$DecDigit,$decimalSep,$thousandSep).' x '.$dataDetail['jumlah'].'</div>';
                $totalOrder = $totalOrder + ($HargaDiskon*$dataDetail['jumlah']);
            }
            $tablePrint .= '<div style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL</span><span style="float: right">'.number_format($totalOrder,$DecDigit,$decimalSep,$thousandSep).'</span></td></tr>';
            $besarPpn = $dataOrder['info']->ppn;
            $totalPpn = $dataOrder['info']->ppn_value;
            $totalPlusPpn = $totalOrder + ($totalOrder * $besarPpn/100);
            if ($besarPpn > 0) {
                $tablePrint .= '<div style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.$gstSym.'(' . $besarPpn . '%)</span><span style="float: right">' . number_format($totalPpn,$DecDigit,$decimalSep,$thousandSep) . '</span></div>';
                $tablePrint .= '<div style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL+'.$gstSym.'</span><span style="float: right">' . number_format($totalPlusPpn,$DecDigit,$decimalSep,$thousandSep) . '</span></div>';
            }
            $tablePrint .= '<div style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">BAYAR</span><span style="float: right">' . number_format($dataOrder['info']->bayar,$DecDigit,$decimalSep,$thousandSep) . '</span></div>';
            $tablePrint .= '<div style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.strtoupper(trans('kembali', $dataPremis->bahasa)).'</span><span style="float: right">' . number_format($dataOrder['info']->kembali,$DecDigit,$decimalSep,$thousandSep) . '</span></div>';
            $tablePrint .= '<div style="font-size: 12px;text-align: center;">'.strtoupper(trans('terimakasih', $dataPremis->bahasa)).'</div>';
            $Madah = variable_get('madah_slogan','INGAT MAKAN INGAT TUHAN');
            $tablePrint .= '<div style="font-size: 12px;text-align: center;">'.strtoupper($Madah).'</div>';
            $tablePrint .= '<div style="font-size: 11px;text-align: center;">'.$dataPremis->kritik_saran.'</div>';
            $tablePrint .= '<div style="font-size: 12px;text-align: center;font-weight: bold;">';
            if (!empty($dataPremis->bbm)) {
                $tablePrint .= '<span style="float: left">';
                //$tablePrint .= '<img src="' . base_path() . 'misc/media/images/bbm.png" width="12" style="vertical-align:middle;">';
                $tablePrint .= '&nbsp;' . $dataPremis->bbm . '</span>';
            }
            if (!empty($dataPremis->bbm) && !empty($dataPremis->whatsapp)) {
                $tablePrint .= '<span style="float: right">';
                $tablePrint .= '<img src="' . base_path() . 'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">';
                $tablePrint .= '&nbsp;' . $dataPremis->whatsapp . '&nbsp;&nbsp;</span>';
            }else if(empty($dataPremis->bbm) && !empty($dataPremis->whatsapp)){
                $tablePrint .= '<span style="float: center">';
                $tablePrint .= '<img src="' . base_path() . 'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">';
                $tablePrint .= '&nbsp;' . $dataPremis->whatsapp . '&nbsp;&nbsp;</span>';
            }else{
                $tablePrint .= '&nbsp;';
            }
            $tablePrint .= '</div>';
        }
    }
    return $tablePrint;
}

//End function print customer order GHC
//function printout penjualan ghc
function printOutPenjualanGHC_old(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
	$dataPremis = getPremisInformation();
    $perlakuanKembalian = PERLAKUAN_KEMBALIAN();
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
    drupal_add_js($js_path);
    $tablePrint = '';
    $nidpenjualan = isset($_GET['idpenjualangh']) ? $_GET['idpenjualangh'] : null;
    $printHeader = true;
    $pathLogo = createLogoImagePath();
    if (!empty($nidpenjualan)){
        global $user;
        $dataPenjualan = getDataPenjualan($nidpenjualan);
        $tablePrint = '<table style="width: 240px;border-collapse: collapse;" >';
        $tablePrint .= '<tbody>';
        if ($printHeader){
            $tablePrint .= '<tr><td rowspan="3" style="border-bottom: 1px solid #000;border-top: 1px solid #000;width:50px;text-align: center;">';
			$tablePrint .= '<img height="48" src="'.$pathLogo.'" /></td>';
			$tablePrint .= '<td colspan="2" style="font-size: 14px;font-weight: bold;text-align: center;';
			$tablePrint .= 'border-bottom: 1px solid #000;border-top: 1px solid #000;">'.$dataPremis->nama.'</td></tr>';
            $tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;border-bottom: 1px solid #000;">'.$dataPremis->alamat.'&nbsp;&nbsp;</td></tr>';
            //$tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;border-bottom: 1px solid #000;">SENTUL CITY - BOGOR&nbsp;&nbsp;</td></tr>';
        }
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;"><span style="float: left">No.</span><span style="float: right">'.$dataPenjualan['info']->nonota.'&nbsp;&nbsp;</span></td></tr>';
        if (!empty($dataPenjualan['info']->namapelanggan)){
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;"><span style="float: left">Member</span><span style="float: right">'.$dataPenjualan['info']->namapelanggan.'&nbsp;&nbsp;</span></td></tr>';
        }
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">'.ucfirst(trans('tanggal', $dataPremis->bahasa)).'</span><span style="float: right">'.$dataPenjualan['info']->tglpenjualan.'&nbsp;&nbsp;</span></td></tr>';
        if (isset($dataPenjualan['laundry'])){
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">MASUK : '.$dataPenjualan['laundry']['info']->tglpenjualan.'</span><span style="float: right">KASIR : '.$dataPenjualan['laundry']['info']->name.'&nbsp;&nbsp;</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">Status : AMBIL</span><span style="float: right">Bayar : '.$dataPenjualan['info']->carabayar.' '.$nilaiBayar.'&nbsp;&nbsp;</span></td></tr>';
        }
        $totalPenjualan = 0;
        for ($i = 0;$i < count($dataPenjualan['detail']);$i++){
            $dataDetail = $dataPenjualan['detail'][$i];
            $tablePrint .= '<tr>';
            $tablePrint .= '<td colspan="3" style="font-size: 13px;font-weight: bold;">'.$dataDetail->namaproduct.'</td>';
            $tablePrint .= '</tr>';
            $tablePrint .= '<tr>';
            $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;">'.number_format($dataDetail->harga_diskon,2,$decimalSep,$thousandSep).'</td>';
            $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;">x '.number_format($dataDetail->jumlah,2,$decimalSep,$thousandSep).'</td>';
            $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;">'.number_format($dataDetail->harga_diskon*$dataDetail->jumlah,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</td>';
            $tablePrint .= '</tr>';
            $totalPenjualan = $totalPenjualan + ($dataDetail->harga_diskon*$dataDetail->jumlah);
        }
		$besarPpn = $dataPenjualan['info']->ppn;
		$totalPpn = $dataPenjualan['info']->ppn_value;
		$totalPlusPpn = $dataPenjualan['info']->total_plus_ppn;
		$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;border-top: 1px solid #000;"><span style="float: left">TOTAL</span><span style="float: right">'.number_format($totalPenjualan,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</span></td></tr>';
		if ($besarPpn > 0) {
			$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.$dataPremis->ppn_symbol.'(' . $besarPpn . '%)</span><span style="float: right">' . number_format($totalPpn, 2,$decimalSep,$thousandSep) . '&nbsp;&nbsp;</span></td></tr>';
			$tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL+'.$dataPremis->ppn_symbol.'</span><span style="float: right">' . number_format($totalPlusPpn,2,$decimalSep,$thousandSep) . '&nbsp;&nbsp;</span></td></tr>';
		}
        $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.strtoupper($dataPenjualan['info']->carabayar).'</span><span style="float: right">'.number_format($dataPenjualan['info']->bayar,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</span></td></tr>';
        if (($dataPenjualan['info']->carabayar == 'HUTANG' || $dataPenjualan['info']->kembali < 0) && !empty($dataPenjualan['info']->idpelanggan)){
            $totalHutang = getTotalHutangLatest($dataPenjualan['info']->idpelanggan);
            $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL HUTANG</span><span style="float: right">'.number_format($totalHutang,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</span></td></tr>';
        }
        $viewperlakuankembalian = '';
        if ($dataPenjualan['info']->perlakuankembalian != 0){
            $viewperlakuankembalian = '<span style="font-size: 13px;font-weight: bold;">'.$perlakuanKembalian[$dataPenjualan['info']->perlakuankembalian].'</span>';
        }
        if ($dataPenjualan['info']->kembali >= 0){
            $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.strtoupper(trans('kembali', $dataPremis->bahasa)).' '.$viewperlakuankembalian.'</span><span style="float: right">'.number_format($dataPenjualan['info']->kembali,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</span></td></tr>';
        }
        $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-top: 1px solid #000;">TERIMAKASIH</td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;">INGAT MAKAN INGAT TUHAN</td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-top: 1px solid #000;">'.$dataPremis->kritik_saran.'</td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;font-weight: bold;">';
		$tablePrint .= '<span style="float: left"><img src="'.base_path().'misc/media/images/bbm.png" width="12"';
		$tablePrint .= 'style="vertical-align:middle;">&nbsp;'.$dataPremis->bbm.'</span><span style="float: right"><img src="'.base_path().'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">&nbsp;'.$dataPremis->whatsapp.'&nbsp;&nbsp;</span></td></tr>';
        $tablePrint .= '</tbody>';
        $tablePrint .= '</table>';
    }
    return $tablePrint;
}
function printOrderByCategory($dataOrder = null,$orderInfo = null){
    $tablePrint = '';
    if (count($dataOrder)){
        foreach($dataOrder[0] as $idxKategori => $categoryName){
            if ($idxKategori == 7) {
                $tablePrint .= '<div class="pagebreak">&nbsp;</div>';
            }else{
                $tablePrint .= '<div>&nbsp;</div>';
            }
            $tablePrint .= '<div>&nbsp;</div>';
            $tablePrint .= '<table style="width: 240px;border-collapse: collapse;" >';
            $tablePrint .= '<tbody>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">KATEGORI</span><span style="float: right">'.strtoupper($categoryName).'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">NO. ORDER</span><span style="float: right">'.$orderInfo->nonota.'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">MEJA</span><span style="float: right">'.strtoupper($orderInfo->meja).'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">TANGGAL</span><span style="float: right">'.substr($orderInfo->tglorder,0,16).'</span></td></tr>';
            foreach($dataOrder[1][$idxKategori] as $idxDetail => $orderDetail){
                $tablePrint .= '<tr>';
                $tablePrint .= '<td colspan="2" style="font-size: 13px;">'.strtoupper($orderDetail['namaproduct']).'</td>';
                $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;width: 50px;">x '.$orderDetail['jumlah'].'</td>';
                $tablePrint .= '</tr>';
            }
            $tablePrint .= '</tbody>';
            $tablePrint .= '</table>';
        }
    }
    return $tablePrint;
}
function printOrderByCategoryNew($dataOrder = null,$orderInfo = null){
    $tablePrint = '';
    if (count($dataOrder)){
        $DataPremis = get_data_premis();
        foreach($dataOrder[0] as $idxKategori => $categoryName){
            if ($idxKategori == 7) {
                $tablePrint .= '<div class="pagebreak">&nbsp;</div>';
            }else{
                $tablePrint .= '<div>&nbsp;</div>';
            }
            $tablePrint .= '<div>&nbsp;</div>';
            $tablePrint .= '<table style="width: 240px;border-collapse: collapse;" >';
            $tablePrint .= '<tbody>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">KATEGORI</span><span style="float: right">'.strtoupper($categoryName).'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">NO. ORDER</span><span style="float: right">'.$orderInfo->nonota.'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">MEJA</span><span style="float: right">'.strtoupper($orderInfo->meja).'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">'.strtoupper(trans('tanggal', $DataPremis->bahasa)).'</span><span style="float: right">'.substr($orderInfo->tglorder,0,16).'</span></td></tr>';
            foreach($dataOrder[1][$idxKategori] as $idxDetail => $orderDetail){
                $tablePrint .= '<tr>';
                $tablePrint .= '<td colspan="2" style="font-size: 13px;">'.strtoupper($orderDetail['namaproduct']).'</td>';
                $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;width: 50px;">x '.$orderDetail['jumlah'].'</td>';
                $tablePrint .= '</tr>';
            }
            $tablePrint .= '</tbody>';
            $tablePrint .= '</table>';
        }
    }
    return $tablePrint;
}
function getDataMeja($nidMeja = null){
    $dataMeja = null;
    if (!empty($nidMeja)){
        $strSQL = 'SELECT id, barcodemeja, kodemeja, meja, keterangan FROM meja ';
        $strSQL .= 'WHERE id=%d';
        $result = db_query($strSQL,$nidMeja);
        $dataMeja['info'] = db_fetch_object($result);
    }
    return $dataMeja;
}
function getCustomerOrder($nidCustomerOrder = null){
	$dataCO = null;
	if (!empty($nidCustomerOrder)){
		$strSQL = 'SELECT custord.id, custord.nonota, custord.tglorder, custord.idpemakai, ';
		$strSQL .= 'custord.idpelanggan, custord.total, custord.totalmodal, custord.carabayar, ';
		$strSQL .= 'custord.bayar, custord.kembali, custord.nokartu, custord.keterangan, ';
		$strSQL .= 'custord.insert_date, custord.status_order, plg.namapelanggan,';
        $strSQL .= 'ppn, (total*(ppn/100)) AS ppn_value, total_plus_ppn,';
        $strSQL .= 'meja.meja,users.name FROM customer_order ';
		$strSQL .= 'custord LEFT JOIN pelanggan plg ON custord.idpelanggan = plg.idpelanggan ';
        $strSQL .= 'LEFT JOIN cms_users users ON custord.idpemakai = users.uid ';
		$strSQL .= 'LEFT JOIN meja meja ON custord.idmeja = meja.id ';
		$strSQL .= 'WHERE custord.id=%d';
		$result = db_query($strSQL,$nidCustomerOrder);
		$dataCO['info'] = db_fetch_object($result);
		$dataCO['detail'] = getDetailCustomerOrder($nidCustomerOrder);
		$dataCO['detailbykategori'] = createDetailOrderByKategori($dataCO['detail']);
	}
	return $dataCO;
}
function getDetailOrder($nidCustomerOrder = null){
	$detailOrder = null;
	if (!empty($nidCustomerOrder)) {
		$dataCustomerOrder = getCustomerOrder($nidCustomerOrder);
		$strSQL = 'SELECT b.idkategori, c.kategori, a.detailbarcode, a.jumlah, ';
		$strSQL .= 'a.outstanding, b.namaproduct,a.masuk,a.hargajual FROM detailcustomerorder a ';
		$strSQL .= 'LEFT JOIN product b ON a.idproduct = b.idproduct ';
		$strSQL .= 'LEFT JOIN kategori c ON b.idkategori = c.idkategori ';
		$strSQL .= 'WHERE a.idcustomerorder = %d ORDER BY idkategori';
		$result = db_query($strSQL,$nidCustomerOrder);
		$detailOrder = array();
		while ($data = db_fetch_object($result)){
			$detailOrder[] = array(
				'idkategori' => $data->idkategori,
				'kategori' => $data->kategori,
				'detailbarcode' => $data->detailbarcode,
				'namapelanggan' => $dataCustomerOrder['info']->namapelanggan,
				'namaproduct' => $data->namaproduct,
				'jumlah' => $data->jumlah,
				'outstanding' => $data->outstanding,
				'jammasuk' => date('H:i',$data->masuk),
				'tglmasuk' => date('d-m-Y',$data->masuk),
				'hargajual' => $data->hargajual,
			);
		}
	}
	return $detailOrder;
}
function createDetailOrderByKategori($detailOrder = null){
	$orderByKategori = array();
	if (count($detailOrder)){
        $kategoriInfo = array();
		for ($i = 0;$i < count($detailOrder);$i++){
			$idkategori = $detailOrder[$i]['idkategori'];
            $kategoriInfo[$idkategori] = $detailOrder[$i]['kategori'];
            $orderByKategori[$idkategori][] = $detailOrder[$i];
		}
	}
	return array($kategoriInfo,$orderByKategori);
}
function getDetailOrderByPrinter($nidCustomerOrder = null, $PrinterName = null){
    $detailOrder = null;
    if (!empty($nidCustomerOrder) && !empty($PrinterName)) {
        $detailOrder = null;
        if (!empty($nidCustomerOrder)) {
            $strSQL = 'SELECT b.idkategori, c.kategori, a.detailbarcode, a.jumlah, ';
            $strSQL .= 'a.outstanding, b.namaproduct,a.masuk,a.hargajual, c.selected_printer FROM detailcustomerorder a ';
            $strSQL .= 'LEFT JOIN product b ON a.idproduct = b.idproduct ';
            $strSQL .= 'LEFT JOIN kategori c ON b.idkategori = c.idkategori ';
            $strSQL .= "WHERE a.idcustomerorder = %d AND selected_printer='%s' ORDER BY selected_printer, idkategori";
            $result = db_query($strSQL,$nidCustomerOrder, $PrinterName);
            $detailOrder = array();
            while ($data = db_fetch_object($result)){
                $detailOrder[] = array(
                    'idkategori' => $data->idkategori,
                    'kategori' => $data->kategori,
                    'detailbarcode' => $data->detailbarcode,
                    'namaproduct' => $data->namaproduct,
                    'jumlah' => $data->jumlah,
                    'outstanding' => $data->outstanding,
                    'jammasuk' => date('H:i',$data->masuk),
                    'tglmasuk' => date('d-m-Y',$data->masuk),
                    'hargajual' => $data->hargajual,
                    'selected_printer' => $data->selected_printer,
                );
            }
        }
        return $detailOrder;
    }
    return $detailOrder;
}
function createDetailOrderByPrinterKategori($detailOrder = null){
    $orderByKategori = array();
    if (count($detailOrder)){
        $kategoriInfo = array();
        for ($i = 0;$i < count($detailOrder);$i++){
            $idkategori = $detailOrder[$i]['idkategori'];
            $PrinterName = $detailOrder[$i]['selected_printer'];
            $kategoriInfo[$idkategori] = $detailOrder[$i]['kategori'];
            $orderByKategori[$PrinterName][$idkategori][] = $detailOrder[$i];
        }
    }
    return array($kategoriInfo,$orderByKategori);
}
function getDataProduct($nidProduct = null){
	$dataProduct = null;
	if (!empty($nidProduct)){
		$strSQL = 'SELECT idproduct, idsupplier, idkategori, idsubkategori, barcode_type, barcode, alt_code, ';
		$strSQL .= 'namaproduct, type_product, hargapokok, hargajual, margin, minstok, maxstok, ';
		$strSQL .= 'stok, satuan, berat, keterangan, lead_time, aturan_jam_kerja, ';
		$strSQL .= 'berlaku_sebelum_zuhur ';
		$strSQL .= 'FROM product WHERE idproduct=%d';
		$result = db_query($strSQL,$nidProduct);
		$dataProduct['info'] = db_fetch_object($result);
	}
	return $dataProduct;
}
function getDetailCustomerOrder($nidCustomerOrder = null){
	$detailOrder = null;
	if (!empty($nidCustomerOrder)) {
		$strSQL = 'SELECT b.idkategori, c.kategori, a.detailbarcode, a.jumlah, ';
		$strSQL .= 'a.outstanding, b.namaproduct,a.masuk,a.hargajual, a.diskon FROM detailcustomerorder a ';
		$strSQL .= 'LEFT JOIN product b ON a.idproduct = b.idproduct ';
		$strSQL .= 'LEFT JOIN kategori c ON b.idkategori = c.idkategori ';
		$strSQL .= 'WHERE a.idcustomerorder = %d ORDER BY idkategori';
		$result = db_query($strSQL,$nidCustomerOrder);
		$detailOrder = array();
		while ($data = db_fetch_object($result)){
			$detailOrder[] = array(
				'idkategori' => $data->idkategori,
				'kategori' => $data->kategori,
				'detailbarcode' => $data->detailbarcode,
				'namaproduct' => $data->namaproduct,
				'jumlah' => $data->jumlah,
				'outstanding' => $data->outstanding,
				'jammasuk' => date('H:i',$data->masuk),
				'tglmasuk' => date('d-m-Y',$data->masuk),
				'hargajual' => $data->hargajual,
                'diskon' => $data->diskon,
			);
		}
	}
	return $detailOrder;
}
function getPremisInformation(){
	$dataPremis = null;
	if (module_exists('datapremis')){
		$dataPremis = get_data_premis();
	}else{
		$dataPremis = new stdClass();
		$dataPremis->nama = 'GH CORNER';
		$dataPremis->bbm = '5FA325FA';
		$dataPremis->telepon = '+6283818232613';
		$dataPremis->whatsapp = '+6283818232613';
		$dataPremis->alamat = 'RUKO B7 PASAR BERSIH';
		$dataPremis->alamat .= "\n";
		$dataPremis->alamat .= 'SENTUL CITY - BOGOR';
		$dataPremis->email = 'ghc@gmail.com';
		$dataPremis->website = 'ghc.ikhwanit.com';
	}
	return $dataPremis;
}

function getCustomerOrderByPrinter($IdOrder = null, $PrinterName = null){
    $dataCO = null;
    if (!empty($IdOrder) && !empty($PrinterName)) {
        $strSQL = 'SELECT custord.id, custord.nonota, custord.tglorder, custord.idpemakai, ';
        $strSQL .= 'custord.idpelanggan, custord.total, custord.totalmodal, custord.carabayar, ';
        $strSQL .= 'custord.bayar, custord.kembali, custord.nokartu, custord.keterangan, ';
        $strSQL .= 'custord.insert_date, custord.status_order, plg.namapelanggan,';
        $strSQL .= 'meja.meja,users.name FROM customer_order ';
        $strSQL .= 'custord LEFT JOIN pelanggan plg ON custord.idpelanggan = plg.idpelanggan ';
        $strSQL .= 'LEFT JOIN cms_users users ON custord.idpemakai = users.uid ';
        $strSQL .= 'LEFT JOIN meja meja ON custord.idmeja = meja.id ';
        $strSQL .= 'WHERE custord.id=%d';
        $result = db_query($strSQL,$IdOrder);
        $dataCO['info'] = db_fetch_object($result);
        $dataCO['detail'] = getDetailOrderByPrinter($IdOrder, $PrinterName);
        $dataCO['detailbykategori'] = createDetailOrderByPrinterKategori($dataCO['detail']);
    }
    return $dataCO;
}

function printOrderByPrinter($IdOrder = null, $PrinterName = null, $PrintIdx = null){
    if (!empty($IdOrder) && !empty($PrinterName) && !is_null($PrintIdx)) {
        drupal_add_js('misc/media/jquery-1.4.4.min.js');
        drupal_add_js(
            array(
                'printer_name'  => $PrinterName,
                'id_order' => $IdOrder,
                'print_idx' => $PrintIdx,
            ),
            'setting'
        );
        $js_path = drupal_get_path('module', 'penjualan') . '/js/direct.multi.print.js';
        drupal_add_js($js_path);
        $DataCustomerOrder = getCustomerOrderByPrinter($IdOrder, $PrinterName);
        $tablePrint = '';
        if (count($DataCustomerOrder)){
            $tablePrint .= printOrderByPrinterCategory($DataCustomerOrder['detailbykategori'],$DataCustomerOrder['info'], $PrinterName);
        }
        db_query('UPDATE customer_order SET printed=1 WHERE id=%d', $idGhOrder);
        return $tablePrint;
    }
}

function printOrderByPrinterCategory($dataOrder = null, $orderInfo = null, $PrinterName = null){
    $tablePrint = '';
    if (count($dataOrder) && !empty($PrinterName)){
        $DataPremis = get_data_premis();
        $TotalKat = count($dataOrder[0]);
        $Counter = 0;
        foreach($dataOrder[0] as $idxKategori => $categoryName){
            $tablePrint .= '<table style="width: 240px;border-collapse: collapse;" >';
            $tablePrint .= '<tbody>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">KATEGORI</span><span style="float: right">'.strtoupper($categoryName).'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">NO. ORDER</span><span style="float: right">'.$orderInfo->nonota.'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">MEJA</span><span style="float: right">'.strtoupper($orderInfo->meja).'</span></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">'.strtoupper(trans('tanggal', $DataPremis->bahasa)).'</span><span style="float: right">'.substr($orderInfo->tglorder,0,16).'</span></td></tr>';
            foreach($dataOrder[1][$PrinterName][$idxKategori] as $idxDetail => $orderDetail){
                $tablePrint .= '<tr>';
                $tablePrint .= '<td colspan="2" style="font-size: 13px;">'.strtoupper($orderDetail['namaproduct']).'</td>';
                $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;width: 50px;">x '.$orderDetail['jumlah'].'</td>';
                $tablePrint .= '</tr>';
            }
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">KETERANGAN</span><span style="float: right">'.$orderInfo->keterangan.'</span></td></tr>';
            $tablePrint .= '</tbody>';
            $tablePrint .= '</table>';
            $tablePrint .= '<div >&nbsp;</div>';
            $Counter++;
            if ($Counter < $TotalKat){
                $tablePrint .= '<div class="pagebreak">&nbsp;</div>';
            }
        }
    }
    return $tablePrint;
}

function printProductBarcodeWithLogo3(){
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
    _addBarcodePlugins();
    drupal_add_js($js_path);
    $js_path = drupal_get_path('module','penjualan').'/js/productlogo2.barcode.js';
    drupal_add_js($js_path);
    $tablePrint = '';
    $products = isset($_POST['selected-product-print']) ? $_POST['selected-product-print'] : null;
    $explodeProduct = explode('||',$products);
    $pathLogo = createLogoImagePath();
    if (!empty($explodeProduct) && count($explodeProduct)){
        $printPage = 0;
        for ($i = 0;$i < count($explodeProduct);$i++) {
            $productData = explode('__',$explodeProduct[$i]);
            $nidProduct = $productData[0];
            $totalPrint = $productData[1];
            $dataProduct = getDataProduct($nidProduct);
            if (count($dataProduct) && $totalPrint > 0) {
                for ($j = 0;$j < $totalPrint;$j++) {
                    if ($printPage == 0) {
                        $tablePrint .= '<table style="width: 190px;border-collapse: collapse;" >';
                        $tablePrint .= '<tbody>';
                    } else {
                        $tablePrint .= '<table style="width: 190px;border-collapse: collapse;" class="pagebreak">';
                        $tablePrint .= '<tbody>';
                        $tablePrint .= '<tr><td style="font-size: 8px;">&nbsp;</td></tr>';
                    }
                    $tablePrint .= '<tr>';
                    $tablePrint .= '<td>';
                    $tablePrint .= '<div id="' . $dataProduct['info']->barcode . '" class="barcode-place" align="center"></div>';
                    $tablePrint .= '<div class="standard-font" align="center" style="width: 100%;">' . strtoupper($dataProduct['info']->namaproduct) . '</div>';
                    $tablePrint .= '</td></tr>';
                    $tablePrint .= '</tbody>';
                    $tablePrint .= '</table>';
                    $printPage++;
                }
            }
        }
    }
    return $tablePrint;
}

function printProductBarcodeNew()
{//printProductBarcodeWithLogo2
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $js_path = drupal_get_path('module', 'penjualan') . '/js/direct.print.js';
    //_addBarcodePlugins();
    _addBarcodePlugins2();
    drupal_add_js($js_path);
    $js_path = drupal_get_path('module', 'penjualan') . '/js/productlogo.barcode.js';
    drupal_add_js($js_path);
    $tablePrint = '';
    $products = isset($_POST['selected-product-print']) ? $_POST['selected-product-print'] : null;
    $explodeProduct = explode('||', $products);
    $LabelSize = isset($_POST['sticker-size']) && !empty($_POST['sticker-size']) ? explode('x', $_POST['sticker-size']) : array(50, 25);
    if (isset($LabelSize[2]) && !empty($LabelSize[2])) {
        $TotalInRow = (int)$LabelSize[2];
        $ColSpan = 'colspan="' . ($TotalInRow + 1) . '"';
    } else {
        $TotalInRow = 1;
        $ColSpan = '';
    }
    $PrintBlack = false;
    if ($TotalInRow > 1) {
        $ColumnWidth = round($StickerWidth / $TotalInRow, 0) - 11;
        $PrintBlack = true;
    } else {
        $ColumnWidth = round($StickerWidth / $TotalInRow, 0);
    }
    $StickerWidth = mm_to_px($LabelSize[0]);
    //$pathLogo = createLogoImagePath();
    if (!empty($explodeProduct) && count($explodeProduct)) {
        $printPage = 0;
        $index = 1;
        $BarcodeData = array();
        for ($i = 0; $i < count($explodeProduct); $i++) {
            $productData = explode('__', $explodeProduct[$i]);
            $nidProduct = $productData[0];
            $totalPrint = $productData[1];
            $dataProduct = getDataProduct($nidProduct);
            if (count($dataProduct) && $totalPrint > 0) {
                for ($j = 0; $j < $totalPrint; $j++) {
                    $tablePrint .= '<div>';
                    if ($j == 0){
                        $tablePrint .= '<table style="width: ' . $StickerWidth . 'px;border-collapse: collapse;margin-top: -3px;" >';
                    }else{
                        $tablePrint .= '<table style="width: ' . $StickerWidth . 'px;border-collapse: collapse;" >';
                    }
                    $tablePrint .= '<tbody>';

                    if ($PrintBlack){
                        $tablePrint .= '<tr>';
                        for ($k = 0; $k < $TotalInRow; $k++) {
                            $tablePrint .= '<td style="background-color: #000000;text-align: center;width: ' . $ColumnWidth . 'px;height: 23px;">&nbsp;';
                            $tablePrint .= '</td>';
                            if ($TotalInRow > 1 && ($k < $TotalInRow - 1)) {
                                $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                            }
                        }
                        $tablePrint .= '</tr>';
                    }else{
                        $tablePrint .= '<tr><td ' . $ColSpan . ' style="font-size: 8px;">&nbsp;</td></tr>';
                    }
                    $tablePrint .= '<tr>';
                    for ($k = 0; $k < $TotalInRow; $k++) {
                        $tablePrint .= '<td style="text-align: center;width: ' . $ColumnWidth . 'px;">';
                        $BarcodeId = 'barcode-' . $index . '-' . $dataProduct['info']->barcode;
                        $BarInfo = new stdClass();
                        $BarInfo->id = $BarcodeId;
                        $BarInfo->barcode = trim($dataProduct['info']->barcode);
                        $BarInfo->barcode_type = $dataProduct['info']->barcode_type;
                        $BarcodeData[] = $BarInfo;
                        $tablePrint .= '<svg id="' . $BarcodeId . '" class="barcode-place"></svg>';
                        $tablePrint .= '</td>';
                        if ($TotalInRow > 1 && ($k < $TotalInRow - 1)) {
                            $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                        }
                    }
                    $tablePrint .= '</tr>';
                    $tablePrint .= '<tr class="standard-font">';
                    for ($k = 0; $k < $TotalInRow; $k++) {
                        $tablePrint .= '<td align="center" style="width: ' . $ColumnWidth . 'px;font-size: 10px;">' . $dataProduct['info']->barcode . '</td>';
                        if ($TotalInRow > 1 && ($k < $TotalInRow - 1)) {
                            $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                        }
                    }
                    $tablePrint .= '<tr class="standard-font">';
                    if ($TotalInRow > 1){
                        if (strlen($dataProduct['info']->namaproduct) <= 19){
                            $NamaProduct = $dataProduct['info']->namaproduct. str_repeat('&nbsp;', 15);
                        }else{
                            $NamaProduct = $dataProduct['info']->namaproduct;
                        }
                    }
                    for ($k = 0; $k < $TotalInRow; $k++) {
                        if ($k == 0){
                            $tablePrint .= '<td align="center" style="width: ' . $ColumnWidth . 'px;font-size: 10px;">&nbsp;' . $NamaProduct . '</td>';
                        }else {
                            $tablePrint .= '<td align="center" style="width: ' . $ColumnWidth . 'px;font-size: 10px;">' . $NamaProduct . '</td>';
                        }
                        if ($TotalInRow > 1 && ($k < $TotalInRow - 1)) {
                            $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                        }
                    }
                    $tablePrint .= '</tbody>';
                    $tablePrint .= '</table>';
                    $index++;
                }
                $printPage++;
                $tablePrint .= '</div>';
                $tablePrint .= '<div class="pagebreak"></div>';
            }
        }
    }
    drupal_add_js(
        array(
            'dataProduct' => $BarcodeData,
            'TotalInRow' => $TotalInRow,
        ),
        'setting');
    return $tablePrint;
}

/**
 * @return string
 */
function printLabelHarga(){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $dataPremis = getPremisInformation();
    $DecDigit = $dataPremis->decimal_digit;
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
    drupal_add_js($js_path);
    _addBarcodePlugins2();
    $js_path = drupal_get_path('module','penjualan').'/js/labelBarcode.js';
    drupal_add_js($js_path);
    $products = isset($_POST['selected-product-print']) ? $_POST['selected-product-print'] : null;
    $explodeProduct = explode('||',$products);
    $LabelSize = isset($_POST['sticker-size']) && !empty($_POST['sticker-size']) ? explode('x',$_POST['sticker-size']) : array(50,25);
    if (isset($LabelSize[2]) && !empty($LabelSize[2])){
        $TotalInRow = (int)$LabelSize[2];
    }else{
        $TotalInRow = 1;
    }
    if ($TotalInRow > 1){
        $Spacing = 'letter-spacing: 3px;';
    }else{
        $Spacing = 'letter-spacing: 7px;';
    }
    $StickerWidth = mm_to_px($LabelSize[0]);
    //$pathLogo = createLogoImagePath();
    $tablePrint = '';
    if (!empty($explodeProduct) && count($explodeProduct)) {
        $printPage = 0;
        $BarcodeData = array();
        $index = 1;
        for ($i = 0; $i < count($explodeProduct); $i++) {
            $productData = explode('__', $explodeProduct[$i]);
            $idProduct = $productData[0];
            $dataProduct = getDataProduct($idProduct);
            //$productsData[] = $dataProduct['info'];
            $tablePrint .= '<div>';
            $tablePrint .= '<table style="width: ' . $StickerWidth . 'px;border-collapse: collapse;">';
            $tablePrint .= '<tbody>';
            $tablePrint .= '<tr>';
            if ($TotalInRow > 1){
                $ColumnWidth = round($StickerWidth/$TotalInRow,0) - 11;
            }else{
                $ColumnWidth = round($StickerWidth/$TotalInRow,0);
            }
            for ($k = 0;$k < $TotalInRow;$k++) {
                $tablePrint .= '<td style="font-size: 11px;font-weight: bold;padding-left: 4px;width: '.$ColumnWidth.'px;">' . strtoupper($dataProduct['info']->namaproduct) . '</td>';
                if ($TotalInRow > 1 && ($k < $TotalInRow - 1)){
                    $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                }
            }
            $tablePrint .= '</tr>';
            $tablePrint .= '<tr>';
            for ($k = 0;$k < $TotalInRow;$k++) {
                $tablePrint .= '<td style="font-size: 34px;font-weight: bold;padding-left: 4px;width: '.$ColumnWidth.'px;"><sup style="font-size: 10px;">' . $currencySym . '</sup> ' . number_format($dataProduct['info']->hargajual, $DecDigit, $decimalSep, $thousandSep) . '</td>';
                if ($TotalInRow > 1 && ($k < $TotalInRow - 1)){
                    $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                }
            }
            $tablePrint .= '</tr>';
            $tablePrint .= '<tr>';
            for ($k = 0;$k < $TotalInRow;$k++) {
                $BarcodeId = 'barcode-' . $index . '-' . $dataProduct['info']->barcode;
                $BarInfo = new stdClass();
                $BarInfo->id = $BarcodeId;
                $BarInfo->barcode = trim($dataProduct['info']->barcode);
                $BarInfo->barcode_type = $dataProduct['info']->barcode_type;
                $BarcodeData[] = $BarInfo;
                $tablePrint .= '<td style="width: '.$ColumnWidth.'px;"><svg id="' . $BarcodeId . '" class="barcode"></svg></td>';
                if ($TotalInRow > 1 && ($k < $TotalInRow - 1)){
                    $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                }
                $index++;
            }
            $tablePrint .= '</tr>';
            $tablePrint .= '<tr>';
            for ($k = 0;$k < $TotalInRow;$k++) {
                $tablePrint .= '<td style="font-size: 10px;font-weight: bold;width: '.$ColumnWidth.'px;' . $Spacing . '">' . $dataProduct['info']->barcode . '</td>';
                if ($TotalInRow > 1 && ($k < $TotalInRow - 1)){
                    $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                }
            }
            $tablePrint .= '</tr>';
            $tablePrint .= '</tbody>';
            $tablePrint .= '</table>';
            $tablePrint .= '</div>';
            $tablePrint .= '<div class="pagebreak"></div>';
            $printPage++;
        }
        drupal_add_js(
            array(
                'dataProduct' => $BarcodeData,
                'TotalInRow' => $TotalInRow,
            ),
            'setting');
    }
    return $tablePrint;
}

function printProductBatchCode(){
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $css_path = drupal_get_path('module','pembelian').'/css/print_custom.css';
    drupal_add_css($css_path);
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
    _addBarcodePlugins2();
    drupal_add_js($js_path);
    $js_path = drupal_get_path('module','pembelian').'/js/batch.barcode.js';
    drupal_add_js($js_path);
    $tablePrint = '';
    $idPembelian = isset($_GET['idpembelian']) ? $_GET['idpembelian'] : null;
    if (!empty($idPembelian)){
        if (isset($_GET['iddetail'])){
            $idDetail = $_GET['iddetail'];
        }
        $printPage = 0;
        $result = db_query("SELECT iddetail, barcode_batch, expired_date, jumlah,prod.namaproduct, prod.hargajual 
        FROM detailpembelian beli LEFT JOIN product prod ON beli.idproduct=prod.idproduct
        WHERE idpembelian='%d'",$idPembelian);
        $barcodeData = array();
        while ($data = db_fetch_object($result)){
            $totalPrint = $data->jumlah;
            $barcode = $data->barcode_batch;
            $expired = $data->expired_date;
            $namaProduct = substr(strtoupper($data->namaproduct),0,15);
            $hargaJual = number_format($data->hargajual,0,',','.');
            if (!empty($barcode) && $totalPrint > 0 &&
                ((isset($idDetail) && !empty($idDetail) && $data->iddetail == $idDetail)) ||
                (!isset($idDetail) || empty($idDetail))
            ) {
                for ($j = 0;$j < $totalPrint;$j++) {
                    if ($printPage == 0) {
                        $tablePrint .= '<table style="width: 190px;border-collapse: collapse;margin-top:-2px;" >';
                        $tablePrint .= '<tbody>';
                    } else {
                        $tablePrint .= '<table style="width: 190px;border-collapse: collapse;" class="pagebreak">';
                        $tablePrint .= '<tbody>';
                        $tablePrint .= '<tr><td class="space-kosong">&nbsp;</td></tr>';
                    }
                    $tablePrint .= '<tr>';
                    $tablePrint .= '<td class="exp-cell" valign="bottom">';
                    $tablePrint .= '<div class="exp-date">'.$expired.'</div>';
                    $tablePrint .= '</td>';
                    $tablePrint .= '<td>';
                    $tablePrint .= '<svg id="barcode-' . $barcode . '" class="barcode-place" align="center"></svg>';
                    $tablePrint .= '<div class="barcode-font" align="center" style="width: 100%;">' . $namaProduct . '&nbsp;&nbsp;Rp. '.$hargaJual.'</div>';
                    $tablePrint .= '</td></tr>';
                    $tablePrint .= '</tbody>';
                    $tablePrint .= '</table>';
                    $printPage++;
                    $barcodeData[] = $barcode;
                }
            }
        }
        drupal_add_js(
            array(
                'barcodeData' => $barcodeData,
            ),
            'setting');
    }
    return $tablePrint;
}

function printBarcodeProduksi()
{
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $css_path = drupal_get_path('module', 'pembelian') . '/css/print_custom.css';
    drupal_add_css($css_path);
    $js_path = drupal_get_path('module', 'penjualan') . '/js/direct.print.js';
    _addBarcodePlugins2();
    drupal_add_js($js_path);
    $js_path = drupal_get_path('module', 'reproduksi') . '/js/produksi.barcode.js';
    drupal_add_js($js_path);
    $tablePrint = '';
    $IdProduksi = isset($_GET['idreproduksi']) ? $_GET['idreproduksi'] : null;
    if (!empty($IdProduksi)) {
        $StrSql = 'SELECT nonota FROM reproduksi WHERE idreproduksi=%d';
        $NoProduksi = db_result(db_query($StrSql, $IdProduksi));
        $StrSql = 'SELECT tglreproduksi FROM reproduksi WHERE idreproduksi=%d';
        $TglProduksi = explode(' ',db_result(db_query($StrSql, $IdProduksi)));
        $tablePrint .= '<table style="width: 190px;border-collapse: collapse;margin-top:-2px;" >';
        $tablePrint .= '<tbody>';
        $tablePrint .= '<tr>';
        $tablePrint .= '<td class="exp-cell" valign="bottom">';
        $tablePrint .= '<div class="exp-date">' . $TglProduksi[0] . '</div>';
        $tablePrint .= '</td>';
        $tablePrint .= '<td>';
        $tablePrint .= '<svg id="barcode" class="barcode-place" align="center"></svg>';
        $tablePrint .= '<div class="barcode-font" align="center" style="width: 100%;">' . $NoProduksi . '</div>';
        $tablePrint .= '</td></tr>';
        $tablePrint .= '</tbody>';
        $tablePrint .= '</table>';
        $barcodeData = $NoProduksi;
        drupal_add_js(
            array(
                'barcodeData' => $barcodeData,
            ),
            'setting');
    }
    return $tablePrint;
}

function get_close_cashier_data($user_id = null){
    if (empty($user_id)) {
        global $user;
    }else{
        $user = user_load($user_id);
    }
    date_default_timezone_set($user->timezone_name);
    $UserPettyCash = db_result(
        db_query(
            'SELECT petty_cash FROM user_petty_cash WHERE login=%d AND uid=%d',
            $user->login, $user->uid
        )
    );
    $StrSql = "SELECT total_plus_ppn, bayar_multiple, kembali, bayar FROM penjualan ";
    $StrSql .= "WHERE idpemakai=%d AND (tglpenjualan >= '%s' AND tglpenjualan <= '%s')";
    $LastLogin = date('Y-m-d H:i:s', $user->login);
    $CurrentTime = date('Y-m-d H:i:s');
    $result = db_query($StrSql,$user->uid, $LastLogin,$CurrentTime);
    $DataReport = new stdClass();
    $DataReport->petty_cash = $UserPettyCash;
    $DataReport->total = 0;
    $DataReport->total_cash = 0;
    $DataReport->total_kad_kredit = 0;
    $DataReport->total_kad_debit = 0;
    $DataReport->total_hutang = 0;
    $DataReport->total_kembali = 0;
    while ($data = db_fetch_object($result)){
        $DataReport->total = $DataReport->total + $data->total_plus_ppn;
        $ExplodeBayar = explode(';', $data->bayar_multiple);
        for ($i = 0;$i < count($ExplodeBayar);$i++){
            $ExplodeDetail = explode(':',$ExplodeBayar[$i]);
            $NilaiBayar = $ExplodeDetail[1]*1;
            if (trim($ExplodeDetail[0]) == 'TUNAI'){
                $DataReport->total_cash = $DataReport->total_cash + $NilaiBayar;
            }else if(trim($ExplodeDetail[0]) == 'KAD KREDIT'){
                $DataReport->total_kad_kredit = $DataReport->total_kad_kredit + $NilaiBayar;
            }else if(trim($ExplodeDetail[0]) == 'DEBIT'){
                $DataReport->total_kad_debit = $DataReport->total_kad_debit + $NilaiBayar;
            }else if(trim($ExplodeDetail[0]) == 'HUTANG'){
                $DataReport->total_hutang = $DataReport->total_hutang + ($data->total_plus_ppn - $data->bayar);
            }
        }
        $DataReport->total_kembali = $DataReport->total_kembali + $data->kembali;
    }
    return $DataReport;
}

function printCloseCashier()
{
    global $user;
    date_default_timezone_set($user->timezone_name);
    $js_path = drupal_get_path('module', 'penjualan') . '/js/direct.print.js';
    drupal_add_js($js_path);
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $dataPremis = getPremisInformation();
    $DecDigit = $dataPremis->decimal_digit;
    $printHeader = true;
    $pathLogo = createLogoImagePath();
    $ClosedData = get_close_cashier_data();
    if (!empty($ClosedData)) {
        $tablePrint = '<table style="width: 305px;border-collapse: collapse;" >';
        $tablePrint .= '<tbody>';
        if ($printHeader) {
            $tablePrint .= '<tr><td rowspan="3" style="border-bottom: 1px solid #000;border-top: 1px solid #000;width:50px;text-align: center;"><img height="48" src="' . $pathLogo . '" /></td><td colspan="2" style="font-size: 14px;font-weight: bold;text-align: center;border-bottom: 1px solid #000;border-top: 1px solid #000;">' . $dataPremis->nama . '</td></tr>';
            $tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;border-bottom: 1px solid #000;">' . $dataPremis->alamat . '&nbsp;&nbsp;</td></tr>';
        }
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;"><strong>LAPORAN TUTUP CASHIER</strong></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: left;"><span style="float: left"><strong>USER ID</strong></span><span style="float: right"><strong>' . $user->name . '</strong></span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;"><span style="float: left">LOGIN</span><span style="float: right">' . date('d-m-Y H:i', $user->login) . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;border-bottom: 1px solid #000;"><span style="float: left">CLOSE</span><span style="float: right">' . date('d-m-Y H:i') . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;"><span style="float: left">TOTAL</span><span style="float: right">' . number_format($ClosedData->total, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;"><span style="float: left">CASH</span><span style="float: right">' . number_format(($ClosedData->total_cash - $ClosedData->total_kembali), $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;"><span style="float: left">KAD KREDIT</span><span style="float: right">' . number_format($ClosedData->total_kad_kredit, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;"><span style="float: left">KAD DEBIT</span><span style="float: right">' . number_format($ClosedData->total_kad_debit, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;border-bottom: 1px solid #000;"><span style="float: left">HUTANG</span><span style="float: right">' . number_format($ClosedData->total_hutang, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;"><span style="float: left">PETTY CASH</span><span style="float: right">' . number_format($ClosedData->petty_cash, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;border-bottom: 1px solid #000;"><span style="float: left">CASH</span><span style="float: right">' . number_format($ClosedData->total_cash - $ClosedData->total_kembali, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;border-bottom: 1px solid #000;"><span style="float: left">TOTAL CASH</span><span style="float: right">' . number_format($ClosedData->total_cash - $ClosedData->total_kembali + $ClosedData->petty_cash, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
        $tablePrint .= '</tbody>';
        $tablePrint .= '</table>';
    }
    return $tablePrint;
}

function cekPrinterCommand(){
	$printOut = '';
	if (isset($_GET['nidlaundry'])){
		$printOut = printOutLaundry();
	}else if (isset($_GET['idpenjualan'])){
		$printOut = printOutPenjualan();
	}else if (isset($_GET['idcustomerorder'])){
		$printOut = printCustomerOrderBarcode();
	}else if (isset($_GET['idproduct'])){
		$printOut = printProductBarcode();
	}else if (isset($_GET['idproductlogo'])){
		$printOut = printProductBarcodeWithLogo();
	}else if (isset($_GET['idorderlogo'])) {
		$printOut = printCustomerOrderLogo();
	}else if (isset($_GET['idorder'])){
		$totalPrint = 1;
		if (isset($_GET['totalprint'])){
 			$totalPrint = $_GET['totalprint'];
		}
		$printOut = printCustomerOrderBarcodeNew($totalPrint);
	}else if (isset($_GET['idmeja'])){
		$printOut = printMejaBarcodeWithLogo();
	}else if (isset($_GET['idghorder'])) {
		$printOut = printGHCOrder();
	}else if (isset($_GET['idghorderonly'])){
	    if (!isset($_GET['totalprint'])){
	        $TotalPrint = 1;
        }else{
            $TotalPrint = $_GET['totalprint'];
        }
        if (!isset($_GET['print_category'])){
	        $PrintCategory = 0;
        }else{
            $PrintCategory = $_GET['print_category'];
        }
		$printOut = printGHCOrderOnly($TotalPrint,$PrintCategory);
	}else if (isset($_GET['idpenjualangh'])){
        $printOut = printPenjualanDetail();
    }else if (isset($_GET['idghordermultiprint']) && isset($_GET['printername']) && isset($_GET['print_idx'])) {
        $printOut = printOrderByPrinter($_GET['idghordermultiprint'],$_GET['printername'], $_GET['print_idx']);
    }else if (isset($_POST['selected-product-print'])){
        if (isset($_POST['sticker-type']) && $_POST['sticker-type'] == 1){
            $printOut = printProductBarcodeNew();
        //}else if(isset($_POST['sticker-type']) && $_POST['sticker-type'] == 2){
        //    $printOut = printProductBarcodeWithLogo3();
        }else if(isset($_POST['sticker-type']) && $_POST['sticker-type'] == 2){
            $printOut = printLabelHarga();
        }else{
            $printOut = printProductBarcodeNew();
        }
    }else if (isset($_GET['idpembelian'])){
        $printOut = printProductBatchCode();
    }else if (isset($_GET['idprodlabel'])){
        $printOut = printLabelHarga();
    }else if (isset($_GET['idreproduksi'])){
        $printOut = printBarcodeProduksi();
    }else if (isset($_GET['close_cashier'])){
        $printOut = printCloseCashier();
    }
	return $printOut;
}