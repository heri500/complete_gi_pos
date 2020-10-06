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
function getDataPenjualan($nidpenjualan = null, $TempData = false){
	$dataPenjualan = null;
	if (!empty($nidpenjualan)){
		$dataPenjualan = array();
        $AddOnTable = '';
		if ($TempData){
		    $AddOnTable = '_temp';
        }
		$strSQL = "SELECT idpenjualan, name, nonota, tglpenjualan, a.idpemakai, a.idpelanggan, total,
		totalmodal, carabayar, bayar, bayar_multiple, kembali, nokartu, a.keterangan, insert_date,idtitipanlaundry,
		perlakuankembalian,namapelanggan, ppn, (total*(ppn/100)) AS ppn_value, total_plus_ppn, a.idmeja, d.meja,
		total_round, voucher_value, total_after_voucher
		FROM penjualan".$AddOnTable." a 
		LEFT JOIN cms_users b ON a.idpemakai = b.uid
		LEFT JOIN pelanggan c ON a.idpelanggan = c.idpelanggan 
		LEFT JOIN meja d ON a.idmeja = d.id
		WHERE idpenjualan=%d";
		$result = db_query($strSQL,$nidpenjualan);
		$dataPenjualan['info'] = db_fetch_object($result);
        $dataPenjualan['info']->tglorder = $dataPenjualan['info']->tglpenjualan;

        $args = array($dataPenjualan['info']->idpemakai);
        $UserData = views_get_view('user_data');
        if (!empty($UserData)) {
            $UserData->set_display('default');
            $UserData->set_arguments($args);
            $UserData->pre_execute();
            $UserData->execute();
            if (!empty($UserData->result)) {
                $UserProfile = $UserData->result[0];
                $dataPenjualan['info']->kasir = $UserProfile->node_title;
            } else {
                $dataPenjualan['info']->kasir = $dataPenjualan['info']->idpemakai;
            }
        }else{
            $dataPenjualan['info']->kasir = $dataPenjualan['info']->idpemakai;
        }
		if (!empty($dataPenjualan['info']->idtitipanlaundry)){
			$dataPenjualan['laundry'] = getDataLaundry($dataPenjualan['info']->idtitipanlaundry);
		}
		$dataDetail = getDetailPenjualan($nidpenjualan, $TempData);
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
function getDetailPenjualan($nidpenjualan = null, $TempData = false)
{
    $AddOnTable = '';
    if ($TempData){
        $AddOnTable = '_temp';
    }
    $detailPenjualan = null;
    if (!empty($nidpenjualan)) {
        $detailPenjualan = array();
        $strSQL = "SELECT iddetail, idpenjualan, a.idproduct, jumlah, a.hargapokok, a.hargajual, 
		a.diskon, b.namaproduct, b.satuan, (a.hargajual - (a.hargajual * a.diskon/100)) AS harga_diskon,
		b.idkategori, c.kategori 
		FROM detailpenjualan".$AddOnTable." a 
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
function printPenjualanDetail($TempData = false){
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    if ($TempData){
        $idPenjualan = isset($_GET['idpenjualantemp']) ? $_GET['idpenjualantemp'] : null;
    }else {
        $idPenjualan = isset($_GET['idpenjualangh']) ? $_GET['idpenjualangh'] : null;
    }
    if (module_exists('gst_separator')) {
        $GstApply = db_result(db_query('SELECT gst_flag FROM penjualan_temp WHERE idpenjualan=%d', $idPenjualan));
        $PrinterGst = variable_get('printer_gst', 0);
        $PrinterNonGst = variable_get('printer_non_gst', 0);
        $GstStatus = variable_get('gst_separator_status', 1);
        drupal_add_js(
                array(
                    'printer_gst' => $PrinterGst,
                    'printer_non_gst' => $PrinterNonGst,
                    'gst_status' => $GstStatus,
                    'gst_apply' => $GstApply,
                ),
            'setting');
    }else{
        $GstStatus = 0;
        drupal_add_js(
            array(
                'gst_status' => $GstStatus,
            ),
            'setting');
    }
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.cashier.js';
    _addBarcodePlugins();
    drupal_add_js($js_path);
    $js_path = drupal_get_path('module','penjualan').'/js/ghorder.barcode.js';
    drupal_add_js($js_path);
    $dataOrder = getDataPenjualan($idPenjualan,$TempData);
    $tablePrint = '';
    if (count($dataOrder)){
        $tablePrint = printKasirOrder($dataOrder);
        $tablePrint .= '<div>&nbsp;</div>';
        //$tablePrint .= '<div class="pagebreak">&nbsp;</div>';
        //$tablePrint .= printKasirOrder($dataOrder);
        //$tablePrint .= printOrderByCategory($dataOrder['detailbykategori'],$dataOrder['info']);
    }
    /*db_query('UPDATE penjualan SET printed=1 WHERE idpenjualan=%d', $idPenjualan);*/
    return $tablePrint;
}
function printOutLaundry_03122019(){
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
        $tablePrint = '<table style="width: 305px;border-collapse: collapse;" >';
        $tablePrint .= '<tbody>';
        if ($printHeader){
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-bottom: 1px dotted #444;border-top: 1px dotted #444;">LAUNDRY IKHWAN</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-top: 1px dotted #444;"><span style="float: left"><img src="'.base_path().'misc/media/images/bbm.png" width="12" style="vertical-align:middle;">&nbsp;5AB041B8</span><span style="float: right"><img src="'.base_path().'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">&nbsp;+628982485685&nbsp;&nbsp;</span></td></tr>';
        }
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;">No. <strong>'.$dataLaundry['info']->nonota.'&nbsp;&nbsp;</strong></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;"><div style="width: 30%;float: left;">Nama</div><div style="width: 70%;float: left;">: <strong>'.$dataLaundry['info']->namapelanggan.'&nbsp;&nbsp;</strong></div></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;"><div style="width: 30%;float: left;">Telp/ HP</div><div style="width: 70%;float: left;">: '.$dataLaundry['info']->telp.'&nbsp;&nbsp;</div></td></tr>';
        $waktuPejualan = explode(' ', $dataLaundry['info']->tglpenjualan);
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;"><div style="width: 30%;float: left;">Terima</div><div style="width: 70%;float: left;"><span style="float: left;">: '.$waktuPejualan[0].'</span><span style="float: right;">'.substr($waktuPejualan[1],0,5).'&nbsp;</span></div></td></tr>';
        $waktuSelesai = explode(' ', $dataLaundry['detail'][0]->perkiraan_ambil);
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;border-bottom: 1px dotted #444;line-height: 13px;"><div style="width: 30%;float: left;">Selesai</div><div style="width: 70%;float: left;"><span style="float: left;">: '.$waktuSelesai[0].'</span><span style="float: right;">'.$waktuSelesai[1].'&nbsp;</span></div></td></tr>';
        $totalLaundry = 0;
        $totalDiskon = 0;
        $totalItem = array();
        for ($i = 0;$i < count($dataLaundry['detail']);$i++){
            $dataDetail = $dataLaundry['detail'][$i];
            $tablePrint .= '<tr>';
            $tablePrint .= '<td colspan="3" style="font-size: 14px;line-height: 15px;"><strong>'.$dataDetail->namaproduct.'</strong></td>';
            $tablePrint .= '</tr>';
            if ($i == (count($dataLaundry['detail']) - 1)){
                $tablePrint .= '<tr style="border-bottom: 1px dotted #444;">';
            }else{
                $tablePrint .= '<tr>';
            }
            $tablePrint .= '<td style="font-size: 14px;width:30%;line-height: 15px;">'.$dataDetail->jumlah.' '.$dataDetail->satuan.'</td>';
            if (isset($totalItem[$dataDetail->satuan])){
                $totalItem[$dataDetail->satuan] = $totalItem[$dataDetail->satuan] + $dataDetail->jumlah;
            }else{
                $totalItem[$dataDetail->satuan] = $dataDetail->jumlah;
            }
            $tablePrint .= '<td style="font-size: 14px;width:30%;line-height: 15px;">Rp. '.number_format($dataDetail->hargajual,0,",",".").'</td>';
            $tablePrint .= '<td style="font-size: 14px;line-height: 13px;line-height: 15px;text-align: right;"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($dataDetail->hargajual*$dataDetail->jumlah,0,",",".").'&nbsp;</span></td>';
            $tablePrint .= '</tr>';
            $totalLaundry = $totalLaundry + ($dataDetail->hargajual*$dataDetail->jumlah);
            $totalDiskon = $totalDiskon + round(($dataDetail->diskon*$dataDetail->hargajual*$dataDetail->jumlah)/100,0);
        }
        $nilaiBayar = 0;
        if ($dataLaundry['info']->carabayar != 'KEMUDIAN'){
            $nilaiBayar = $dataLaundry['info']->bayar;
        }
        $sisaYangHarusDibayar = $totalLaundry - $nilaiBayar - $totalDiskon;
        $tablePrint .= '<tr>';
        $tablePrint .= '<td colspan="2" style="font-size: 14px;text-align: right;border-top: 1px dotted #444;">TOTAL :</td>';
        $tablePrint .= '<td style="font-size: 14px;text-align: right;border-top: 1px dotted #444;text-align: right"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($totalLaundry,0,",",".").'&nbsp;</span></td>';
        $tablePrint .= '</tr>';
        if ($totalDiskon > 0){
            $tablePrint .= '<tr>';
            $tablePrint .= '<td colspan="2" style="font-size: 14px;text-align: right;">DISKON :</td>';
            $tablePrint .= '<td style="font-size: 14px;text-align: right;text-align: right"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($totalDiskon,0,",",".").'&nbsp;</span></td>';
            $tablePrint .= '</tr>';
        }
        $tablePrint .= '<tr>';
        $tablePrint .= '<td colspan="2" style="font-size: 14px;text-align: right;">BAYAR :</td>';
        $tablePrint .= '<td style="font-size: 14px;text-align: right;text-align: right"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($nilaiBayar,0,",",".").'&nbsp;</span></td>';
        $tablePrint .= '</tr>';
        $tablePrint .= '<tr>';
        $tablePrint .= '<td colspan="2" style="font-size: 14px;text-align: right;border-bottom: 1px dotted #444;">SISA :</td>';
        $tablePrint .= '<td style="font-size: 14px;text-align: right;border-bottom: 1px dotted #444;text-align: right"><span style="float: left;">Rp.</span><span style="float: right;">'.number_format($sisaYangHarusDibayar,0,",",".").'&nbsp;</span></td>';
        $tablePrint .= '</tr>';
        if ($printHeader){
            $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;">Jl. Babakan Madang, Ruko Victoria,&nbsp;&nbsp;</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-bottom: 1px dotted #444;">Blok C No.22, Sentul City - Bogor&nbsp;&nbsp;</td></tr>';
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
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;">Total Qty Item : '.$totalItemView.'</td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;">Status : '.$statusPembayaran.'</td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;">Ket : '.$keterangan.'</td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;">Kasir : '.$user->name.'</td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;font-weight: bold;line-height: 15px;padding-top: 2px;">Pengambilan Cucian Harus Disertakan Dengan Nota Asli</td></tr>';
        $tablePrint .= '</tbody>';
        $tablePrint .= '</table>';
    }
    return $tablePrint;
}

function printOutLaundry(){
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    $js_path = drupal_get_path('module','penjualan').'/js/direct.print.js';
    drupal_add_js($js_path);
    global $user;
    $tablePrint = '';
    $nidtitipanlaundry = isset($_GET['nidlaundry']) ? $_GET['nidlaundry'] : null;
    $printHeader = true;
    $TotalPrint = isset($_GET['total_print']) && !empty($_GET['total_print']) ? (int)$_GET['total_print'] : 1;
    if (!empty($nidtitipanlaundry)){
        global $user;
        get_number_format($CurrSym, $tSep, $dSep);
        $dataPremis = getPremisInformation();
        $DecDigit = $dataPremis->decimal_digit;
        $CurrSym = $dataPremis->currency_symbol;
        $dataLaundry = getDataLaundry($nidtitipanlaundry);
        for ($count = 0;$count < $TotalPrint;$count++) {
            $tablePrint .= '<table style="width: 305px;border-collapse: collapse;" >';
            $tablePrint .= '<tbody>';
            if ($printHeader) {
                $tablePrint .= '<tr><td colspan="3" style="font-size: 15px;"><u><strong>'.strtoupper($dataPremis->nama).'</strong></u></td></tr>';
                $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;"><span style="float: left"><i>HP '.$dataPremis->telepon.'</i></span></td></tr>';
                $tablePrint .= '<tr><td colspan="3">&nbsp;</td></tr>';
            }
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;">No. <strong>' . $dataLaundry['info']->nonota . '&nbsp;&nbsp;</strong></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;"><div style="width: 30%;float: left;">Nama</div><div style="width: 70%;float: left;">: <strong>' . $dataLaundry['info']->namapelanggan . '&nbsp;&nbsp;</strong></div></td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;"><div style="width: 30%;float: left;">Telp/ HP</div><div style="width: 70%;float: left;">: ' . $dataLaundry['info']->telp . '&nbsp;&nbsp;</div></td></tr>';
            $waktuPejualan = explode(' ', $dataLaundry['info']->tglpenjualan);
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;"><div style="width: 30%;float: left;">Terima</div><div style="width: 70%;float: left;"><span style="float: left;">: ' . $waktuPejualan[0] . '</span><span style="float: right;">' . substr($waktuPejualan[1], 0, 5) . '&nbsp;</span></div></td></tr>';
            $waktuSelesai = explode(' ', $dataLaundry['detail'][0]->perkiraan_ambil);
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;border-bottom: 1px dotted #444;line-height: 13px;padding: 2px 0;">';
            $tablePrint .= '<div style="width: 30%;float: left;">Selesai</div>';
            $tablePrint .= '<div style="width: 70%;float: left;">';
            $tablePrint .= '<span style="float: left;">: ' . $waktuSelesai[0] . '</span>';
            $tablePrint .= '<span style="float: right;">' . $waktuSelesai[1] . '&nbsp;</span>';
            $tablePrint .= '</div>';
            $tablePrint .= '</td></tr>';
            $totalLaundry = 0;
            $totalDiskon = 0;
            $totalItem = array();
            for ($i = 0; $i < count($dataLaundry['detail']); $i++) {
                $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;">&nbsp;</td></tr>';
                $dataDetail = $dataLaundry['detail'][$i];
                $tablePrint .= '<tr>';
                $tablePrint .= '<td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;"><strong>' . $dataDetail->namaproduct . '</strong></td>';
                $tablePrint .= '</tr>';
                if ($i == (count($dataLaundry['detail']) - 1)) {
                    $tablePrint .= '<tr style="border-bottom: 1px dotted #444;">';
                } else {
                    $tablePrint .= '<tr>';
                }
                $tablePrint .= '<td style="font-size: 14px;width:30%;line-height: 15px;padding: 2px 0;">' . $dataDetail->jumlah . ' ' . $dataDetail->satuan . '</td>';
                if (isset($totalItem[$dataDetail->satuan])) {
                    $totalItem[$dataDetail->satuan] = $totalItem[$dataDetail->satuan] + $dataDetail->jumlah;
                } else {
                    $totalItem[$dataDetail->satuan] = $dataDetail->jumlah;
                }
                $tablePrint .= '<td style="font-size: 14px;width:30%;line-height: 15px;">'.$CurrSym.' ' . number_format($dataDetail->hargajual, $DecDigit, $dSep, $tSep) . '</td>';
                $tablePrint .= '<td style="font-size: 14px;line-height: 13px;line-height: 15px;text-align: right;"><span style="float: left;">'.$CurrSym.'</span><span style="float: right;">' . number_format($dataDetail->hargajual * $dataDetail->jumlah, $DecDigit, $dSep, $tSep) . '&nbsp;</span></td>';
                $tablePrint .= '</tr>';
                $totalLaundry = $totalLaundry + ($dataDetail->hargajual * $dataDetail->jumlah);
                $totalDiskon = $totalDiskon + round(($dataDetail->diskon * $dataDetail->hargajual * $dataDetail->jumlah) / 100, 0);
            }
            $nilaiBayar = 0;
            if ($dataLaundry['info']->carabayar != 'KEMUDIAN') {
                $nilaiBayar = $dataLaundry['info']->bayar;
            }
            $sisaYangHarusDibayar = $totalLaundry - $nilaiBayar - $totalDiskon;
            $tablePrint .= '<tr>';
            $tablePrint .= '<td colspan="2" style="font-size: 14px;text-align: right;border-top: 1px dotted #444;padding: 2px 0;">TOTAL :</td>';
            $tablePrint .= '<td style="font-size: 14px;text-align: right;border-top: 1px dotted #444;text-align: right"><span style="float: left;">'.$CurrSym.'</span><span style="float: right;">' . number_format($totalLaundry, $DecDigit, $dSep, $tSep) . '&nbsp;</span></td>';
            $tablePrint .= '</tr>';
            if ($totalDiskon > 0) {
                $tablePrint .= '<tr>';
                $tablePrint .= '<td colspan="2" style="font-size: 14px;text-align: right;padding: 2px 0;">DISKON :</td>';
                $tablePrint .= '<td style="font-size: 14px;text-align: right;text-align: right"><span style="float: left;">'.$CurrSym.'</span><span style="float: right;">' . number_format($totalDiskon, $DecDigit, $dSep, $tSep) . '&nbsp;</span></td>';
                $tablePrint .= '</tr>';
            }
            $tablePrint .= '<tr>';
            $tablePrint .= '<td colspan="2" style="font-size: 14px;text-align: right;padding: 2px 0;">BAYAR :</td>';
            $tablePrint .= '<td style="font-size: 14px;text-align: right;text-align: right"><span style="float: left;">'.$CurrSym.'</span><span style="float: right;">' . number_format($nilaiBayar, $DecDigit, $dSep, $tSep) . '&nbsp;</span></td>';
            $tablePrint .= '</tr>';
            $tablePrint .= '<tr>';
            $tablePrint .= '<td colspan="2" style="font-size: 14px;text-align: right;border-bottom: 1px dotted #444;padding: 2px 0;">'.strtoupper(trans('sisa', $dataPremis->bahasa)).' :</td>';
            $tablePrint .= '<td style="font-size: 14px;text-align: right;border-bottom: 1px dotted #444;text-align: right"><span style="float: left;">'.$CurrSym.'</span><span style="float: right;">' . number_format($sisaYangHarusDibayar, $DecDigit, $dSep, $tSep) . '&nbsp;</span></td>';
            $tablePrint .= '</tr>';
            /*if ($printHeader) {
                $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;padding: 2px 0;">'.$dataPremis->alamat.'&nbsp;&nbsp;</td></tr>';
                $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-bottom: 1px dotted #444;">&nbsp;</td></tr>';
            }*/
            $totalItemView = '';
            if (count($totalItem)) {
                $itemCount = 0;
                foreach ($totalItem as $key => $value) {
                    if ($itemCount == 0) {
                        $totalItemView = $value . ' ' . $key;
                    } else {
                        $totalItemView .= '+' . $value . ' ' . $key;
                    }
                    $itemCount++;
                }
            }
            $statusPembayaran = '<b>'.ucfirst(trans('lunas', $dataPremis->bahasa)).'</b>';
            if ($sisaYangHarusDibayar > 0) {
                $statusPembayaran = '<b>'.ucfirst(trans('belum lunas', $dataPremis->bahasa)).'</b>';
            }
            $keterangan = !empty($dataLaundry['info']->keterangan) ? $dataLaundry['info']->keterangan : '-';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;">Total Qty Item : ' . $totalItemView . '</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;">Status : ' . $statusPembayaran . '</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;">&nbsp;</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;">Ket : ' . nl2br($keterangan) . '</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;">&nbsp;</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;line-height: 15px;padding: 2px 0;">'.ucfirst(trans('kasir', $dataPremis->bahasa)).' : ' . $user->name . '</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;">&nbsp;</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;font-weight: bold;line-height: 15px;padding-top: 2px;">Pengambilan Cucian Harus Disertakan Dengan Nota Asli</td></tr>';
            $tablePrint .= '</tbody>';
            $tablePrint .= '</table>';
            if ($TotalPrint > 1){
                $tablePrint .= '<p>&nbsp;</p>';
                $tablePrint .= '<p>&nbsp;</p>';
                $tablePrint .= '<div class="pagebreak">&nbsp;</div>';
            }
        }
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
            $nilaiBayar =
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
                $PrintKasir = printKasirOrder($dataOrder);
                if ($PrintKasir != '') {
                    $tablePrint .= $PrintKasir;
                    $tablePrint .= '<div>&nbsp;</div>';
                    if ($i < ($TotalPrint - 1)) {
                        $tablePrint .= '<div class="pagebreak">&nbsp;</div>';
                    }
                }
            }
        }
        db_query('UPDATE customer_order SET printed=1 WHERE id=%d', $idGhOrder);
    }
	return $tablePrint;
}
function printKasirOrder($dataOrder = null, $priceView =  true){
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
                $tablePrint .= '<tr><td rowspan="2" style="border-bottom: 1px solid #000;border-top: 1px solid #000;width:50px;text-align: center;"><img height="48" src="'.$pathLogo.'" /></td><td colspan="2" style="font-size: 14px;font-weight: bold;text-align: center;border-bottom: 1px solid #000;border-top: 1px solid #000;">'.$dataPremis->nama.'</td></tr>';
                $tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;border-bottom: 1px solid #000;">'.$dataPremis->alamat.'&nbsp;&nbsp;</td></tr>';
                //$tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;">SENTUL CITY - BOGOR&nbsp;&nbsp;</td></tr>';
            }
            $tablePrint .= '<tr><td colspan="3" style="text-align: center;padding: 4px 0 0;border-bottom: 1px solid #000;border-top: 1px solid #000;"><div align="center"><div id="'.$orderInfo->nonota.'" class="barcode-place"></div></div></td></tr>';
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
            $TotalDiskon = 0;
            for ($i = 0;$i < count($orderDetail);$i++){
                $dataDetail = $orderDetail[$i];
                $tablePrint .= '<tr>';
                $tablePrint .= '<td colspan="3" style="font-size: 13px;">'.strtoupper($dataDetail['namaproduct']).'</td>';
                $tablePrint .= '</tr>';
                if ($priceView) {
                    $tablePrint .= '<tr>';
                    $BesarDiskon = $dataDetail['hargajual'] * ($dataDetail['diskon'] / 100);
                    //$HargaDiskon = $dataDetail['hargajual'] - $BesarDiskon;
                    //if (strtoupper($dataDetail['satuan']) == 'KG'){
                    //    $tablePrint .= '<td colspan="2" style="font-size: 12px;text-align: left">' . number_format($HargaDiskon, $DecDigit, $decimalSep, $thousandSep) . ' x ' . $dataDetail['jumlah'] . ' ('.$dataDetail['qty_pcs'].' PCS/EKOR)</td>';
                    //}else {
                        $tablePrint .= '<td colspan="2" style="font-size: 12px;text-align: left">' . number_format($dataDetail['hargajual'], $DecDigit, $decimalSep, $thousandSep) . ' x ' . $dataDetail['jumlah'] . '</td>';
                    //}
                    $TotalDiskon = $TotalDiskon + ($BesarDiskon * $dataDetail['jumlah']);
                    $SubTotal = $dataDetail['hargajual'] * $dataDetail['jumlah'];
                    $tablePrint .= '<td style="font-size: 12px;text-align: right;width: 100px;"><strong>' . number_format($SubTotal, $DecDigit, $decimalSep, $thousandSep) . '</strong></td>';
                    $tablePrint .= '</tr>';
                    if ($dataDetail['keterangan'] != '' && !is_null($dataDetail['keterangan']) && $dataDetail['keterangan'] != 'undefined'){
                        $tablePrint .= '<tr>';
                        $tablePrint .= '<td colspan="3" style="font-size: 11px;text-align: right;"><strong>' . urldecode($dataDetail['keterangan']) . '</strong></td>';
                        $tablePrint .= '</tr>';
                    }
                }else{
                    $tablePrint .= '<tr>';
                    $tablePrint .= '<td style="font-size: 12px;text-align: left">Total Qty</td>';
                    //if (strtoupper($dataDetail['satuan']) == 'KG'){
                    //    $tablePrint .= '<td colspan="2" style="font-size: 12px;text-align: right;"><strong>' . $dataDetail['jumlah'] . ' ('.$dataDetail['qty_pcs'].' PCS/EKOR)</strong></td>';
                    //}else {
                    $tablePrint .= '<td colspan="2" style="font-size: 12px;text-align: right;"><strong>' . $dataDetail['jumlah'] . '</strong></td>';
                    //}
                    $tablePrint .= '</tr>';
                    if ($dataDetail['keterangan'] != '' && !is_null($dataDetail['keterangan'])){
                        $tablePrint .= '<tr>';
                        $tablePrint .= '<td colspan="3" style="font-size: 11px;text-align: right;"><strong>' . urldecode($dataDetail['keterangan']) . '</strong></td>';
                        $tablePrint .= '</tr>';
                    }
                }
				$totalOrder = $totalOrder + ($dataDetail['hargajual']*$dataDetail['jumlah']);
            }
            if ($priceView) {
                $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;border-top: 1px solid #000;"><span style="float: left">TOTAL</span><span style="float: right">' . number_format($totalOrder, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                if ($TotalDiskon > 0){
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">'.strtoupper(trans('diskon', $dataPremis->bahasa)).'</span><span style="float: right">' . number_format($TotalDiskon, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                }
                $Penggenapan = variable_get('round_decimal_digit_cashier', 0);
                $TotalOrderDiskon = $totalOrder - $TotalDiskon;
                if ($TotalDiskon > 0) {
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL - ' . strtoupper(trans('diskon', $dataPremis->bahasa)) . '</span><span style="float: right">' . number_format($TotalOrderDiskon, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                }
                if ($orderInfo->voucher_value > 0) {
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">' . strtoupper(trans('voucher', $dataPremis->bahasa)) . '</span><span style="float: right">' . number_format($orderInfo->voucher_value, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                    $TotalOrderDiskon = $TotalOrderDiskon - $orderInfo->voucher_value;
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL WITH ' . strtoupper(trans('voucher', $dataPremis->bahasa)) . '</span><span style="float: right">' . number_format($TotalOrderDiskon, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                }
                $besarPpn = $dataOrder['info']->ppn;
                $totalPpn = $dataOrder['info']->ppn_value;
                $totalPlusPpn = $TotalOrderDiskon + ($TotalOrderDiskon * $besarPpn / 100);
                if ($besarPpn > 0) {
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">' . $gstSym . '(' . $besarPpn . '%)</span><span style="float: right">' . number_format($totalPpn, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">TOTAL+' . $gstSym . '</span><span style="float: right">' . number_format($totalPlusPpn, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                }
                $TotalPenggenapan = $totalPlusPpn;
                if ($Penggenapan) {
                    $TotalPenggenapan = penggenapan($totalPlusPpn,$DecDigit);
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">PENGGENAPAN</span><span style="float: right">' . number_format($TotalPenggenapan, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                }
                $BayarMultiple = $dataOrder['info']->bayar_multiple;
                $ExplodeBayar = explode(';', $BayarMultiple);
                if (count($ExplodeBayar)) {
                    $Bayar = '';
                    $TotalBayaran = '';
                    for ($i = 0; $i < count($ExplodeBayar); $i++) {
                        $ExplodeDetail = explode(':', $ExplodeBayar[$i]);
                        if ($i == 0) {
                            $Bayar = $ExplodeDetail[0];
                            $TotalBayaran = number_format($ExplodeDetail[1], $DecDigit, $decimalSep, $thousandSep);
                        } else {
                            $Bayar .= ',' . $ExplodeDetail[0];
                            $TotalBayaran .= ' + ' . number_format($ExplodeDetail[1], $DecDigit, $decimalSep, $thousandSep);
                        }
                    }
                }
                $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">BAYAR (' . $Bayar . ')</span><span style="float: right">' . $TotalBayaran . '</span></td></tr>';
                $Kembali = $orderInfo->bayar - $TotalPenggenapan;
                $tablePrint .= '<tr><td colspan="3" style="font-size: 13px;font-weight: bold;text-align: center;"><span style="float: left">' . strtoupper(trans('kembali', $dataPremis->bahasa)) . '</span><span style="float: right">' . number_format($Kembali, $DecDigit, $decimalSep, $thousandSep) . '</span></td></tr>';
                $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-top: 1px solid #000;">' . $orderInfo->keterangan . '</td></tr>';
                $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;border-top: 1px solid #000;">' . strtoupper(trans('terimakasih', $dataPremis->bahasa)) . '</td></tr>';
                $Madah = variable_get('madah_slogan', 'INGAT MAKAN INGAT TUHAN');
                $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;text-align: center;">' . strtoupper($Madah) . '</td></tr>';
                $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;text-align: center;border-top: 1px solid #000;">' . $dataPremis->kritik_saran . '</td></tr>';
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
                } else if (empty($dataPremis->bbm) && !empty($dataPremis->whatsapp)) {
                    $tablePrint .= '<span style="float: left;">';
                    $tablePrint .= '<img src="' . base_path() . 'misc/media/images/whatsapp.png" width="11" style="vertical-align:middle;">';
                    $tablePrint .= '&nbsp;' . $dataPremis->whatsapp . '&nbsp;&nbsp;</span>';
                } else {
                    $tablePrint .= '&nbsp;';
                }
                $tablePrint .= '</td></tr>';
            }else{
                $tablePrint .= '<tr><td colspan="3" style="font-size: 12px;border-top: 1px solid #000;">KETERANGAN : ' . $orderInfo->keterangan . '</td></tr>';
            }
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
                $tablePrint .= '<span style="float: left">';
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
            $nilaiBayar = $dataPenjualan['laundry']->bayar;
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
            $IpAddress = ip_address();
            $strSQL = 'SELECT b.idkategori, c.kategori, a.detailbarcode, a.jumlah, a.qty_pcs, b.satuan, ';
            $strSQL .= 'a.outstanding, b.namaproduct,a.masuk,a.hargajual, d.selected_printer, a.keterangan FROM detailcustomerorder a ';
            $strSQL .= 'LEFT JOIN product b ON a.idproduct = b.idproduct ';
            $strSQL .= 'LEFT JOIN kategori c ON b.idkategori = c.idkategori ';
            $strSQL .= 'LEFT JOIN kategori_printer d ON c.idkategori = d.idkategori AND b.idkategori = d.idkategori ';
            $strSQL .= "WHERE a.idcustomerorder = %d AND (d.ip_address = '%s' AND d.selected_printer = '%s') ";
            $strSQL .= "ORDER BY d.selected_printer, d.idkategori";
            $result = db_query($strSQL, $nidCustomerOrder, $IpAddress,  $PrinterName);
            $detailOrder = array();
            while ($data = db_fetch_object($result)){
                $detailOrder[] = array(
                    'idkategori' => $data->idkategori,
                    'kategori' => $data->kategori,
                    'detailbarcode' => $data->detailbarcode,
                    'namaproduct' => $data->namaproduct,
                    'jumlah' => $data->jumlah,
                    'qty_pcs' => $data->qty_pcs,
                    'satuan' => $data->satuan,
                    'outstanding' => $data->outstanding,
                    'jammasuk' => date('H:i',$data->masuk),
                    'tglmasuk' => date('d-m-Y',$data->masuk),
                    'hargajual' => $data->hargajual,
                    'selected_printer' => $data->selected_printer,
                    'keterangan' => urldecode($data->keterangan),
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
		$strSQL = 'SELECT a.id, b.idkategori, c.kategori, a.detailbarcode, a.jumlah, a.qty_pcs, b.satuan, ';
		$strSQL .= 'a.outstanding, b.namaproduct,a.masuk,a.hargajual, a.diskon, c.selected_printer,a.keterangan FROM detailcustomerorder a ';
		$strSQL .= 'LEFT JOIN product b ON a.idproduct = b.idproduct ';
		$strSQL .= 'LEFT JOIN kategori c ON b.idkategori = c.idkategori ';
		$strSQL .= 'WHERE a.idcustomerorder = %d ORDER BY idkategori';
		$result = db_query($strSQL,$nidCustomerOrder);
		$detailOrder = array();
        $IpAddress = ip_address();
		while ($data = db_fetch_object($result)) {
		    $StrSqlPrinter = "SELECT selected_printer FROM kategori_printer ";
            $StrSqlPrinter .= "WHERE idkategori=%d AND ip_address='%s'";
		    $PrinterName = db_result(db_query($StrSqlPrinter, $data->idkategori, $IpAddress));
		    $Keterangan = !empty($data->keterangan) ? urldecode($data->keterangan) : null;
            $detailOrder[] = array(
                'id' => $data->id,
                'idkategori' => $data->idkategori,
                'kategori' => $data->kategori,
                'detailbarcode' => $data->detailbarcode,
                'namaproduct' => $data->namaproduct,
                'jumlah' => $data->jumlah,
                'qty_pcs' => $data->qty_pcs,
                'satuan' => $data->satuan,
                'outstanding' => $data->outstanding,
                'jammasuk' => date('H:i', $data->masuk),
                'tglmasuk' => date('d-m-Y', $data->masuk),
                'hargajual' => $data->hargajual,
                'diskon' => $data->diskon,
                'selected_printer' => $PrinterName,
                'keterangan' => $Keterangan,
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
        db_query('UPDATE customer_order SET printed=1 WHERE id=%d', $IdOrder);
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
                if ($orderDetail['satuan'] == 'KG'){
                    $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;width: 100px;">x ' . $orderDetail['jumlah'] . '('.$orderDetail['qty_pcs'].' EKOR)</td>';
                } else {
                    $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;width: 100px;">x ' . $orderDetail['jumlah'] . '</td>';
                }
                $tablePrint .= '</tr>';
                if ($orderDetail['keterangan'] != '' && !is_null($orderDetail['keterangan']) && $orderDetail['keterangan'] != 'undefined'){
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;"><strong>' . $orderDetail['keterangan'] . '</strong></td></tr>';
                }
            }
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;">KETERANGAN</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;">'.$orderInfo->keterangan.'</td></tr>';
            $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;border-bottom: 1px solid #000;">&nbsp;</span></td></tr>';
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
    $StickerWidth = mm_to_px($LabelSize[0]);
    $StickerHeight = $LabelSize[1];
    if ($TotalInRow > 1) {
        $ColumnWidth = round($StickerWidth / $TotalInRow, 0) - 11;
        $PrintBlack = true;
    } else {
        $ColumnWidth = round($StickerWidth / $TotalInRow, 0);
    }
    $ArrProductSize = null;
    if (function_exists('data_produk_size')){
        $ArrProductSize = data_produk_size();
    }
    //$pathLogo = createLogoImagePath();
    if (!empty($explodeProduct) && count($explodeProduct)) {
        $printPage = 0;
        $index = 1;
        $BarcodeData = array();
        for ($i = 0; $i < count($explodeProduct); $i++) {
            $productData = explode('__', $explodeProduct[$i]);
            $nidProduct = $productData[0];
            $totalPrint = $productData[1];
            $SizeProduct = null;
            if (isset($productData[2]) && !is_null($productData[2])) {
                if (isset($ArrProductSize[$productData[2]])){
                    $SizeProduct = $ArrProductSize[$productData[2]];
                }else{
                    $SizeProduct = $productData[2];
                }

            }
            $dataProduct = getDataProduct($nidProduct);
            if (count($dataProduct) && $totalPrint > 0) {
                for ($j = 0; $j < $totalPrint; $j++) {
                    $tablePrint .= '<div>';
                    if ($index == 0){
                        $tablePrint .= '<table style="width: ' . $StickerWidth . 'px;border-collapse: collapse;margin-top: -6px;" >';
                    }else{
                        $tablePrint .= '<table style="width: ' . $StickerWidth . 'px;border-collapse: collapse;margin-top: 6px;" >';
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
                        if (!is_null($SizeProduct)){
                            $ColSpan = 'colspan="2"';
                        }
                        $tablePrint .= '<tr><td ' . $ColSpan . ' style="font-size: 8px;">&nbsp;</td></tr>';
                    }
                    $tablePrint .= '<tr>';
                    for ($k = 0; $k < $TotalInRow; $k++) {
                        if (!is_null($SizeProduct)){
                            $tablePrint .= '<td colspan="2" style="text-align: center;width: ' . $ColumnWidth . 'px;">';
                        } else {
                            $tablePrint .= '<td style="text-align: center;width: ' . $ColumnWidth . 'px;">';
                        }
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
                        if (!is_null($SizeProduct)){
                            $tablePrint .= '<td align="left" style="width: ' . (strval($ColumnWidth) - 40) . 'px;font-size: 10px;padding-left: 12px;">' . $dataProduct['info']->barcode . '</td>';
                            if (strlen($SizeProduct) > 3){
                                $tablePrint .= '<td class="vertical-text-small" align="center" rowspan="4" style="width: 40px;">' . $SizeProduct . '</td>';
                            }else{
                                if (is_int((int)$SizeProduct) && (int)$SizeProduct > 0){
                                    $tablePrint .= '<td class="vertical-text-bigger" align="center" rowspan="4" style="width: 40px;padding-bottom: 25px;"><u>' . $SizeProduct . '</u></td>';
                                }else {
                                    $tablePrint .= '<td class="vertical-text-big" align="center" rowspan="4" style="width: 40px;">' . $SizeProduct . '</td>';
                                }
                            }
                        }else{
                            $tablePrint .= '<td align="center" style="width: ' . strval($ColumnWidth) . 'px;font-size: 10px;">' . $dataProduct['info']->barcode . '</td>';
                        }

                        if ($TotalInRow > 1 && ($k < $TotalInRow - 1)) {
                            $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                        }
                    }
                    $tablePrint .= '</tr>';
                    $TextAlign = 'center';
                    $Padding = '';
                    if (!is_null($SizeProduct)){
                        $TextAlign = 'left';
                        $Padding = 'padding-left: 12px;';
                    }
                    if ($StickerHeight > 25){
                        $tablePrint .= '<tr class="standard-font">';
                        $tablePrint .= '<td align="'.$TextAlign.'" style="width: ' . strval($ColumnWidth) . 'px;font-size: 11px;'.$Padding.'">' . $dataProduct['info']->alt_code . '</td>';
                        $tablePrint .= '</tr>';
                        $tablePrint .= '<tr class="standard-font">';
                        $tablePrint .= '<td align="'.$TextAlign.'" style="width: ' . strval($ColumnWidth) . 'px;font-size: 11px;font-weight: bold;'.$Padding.'">' . $dataProduct['info']->namaproduct . '</td>';
                        $tablePrint .= '</tr>';
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
                            $tablePrint .= '<td align="center" style="width: ' . strval($ColumnWidth) . 'px;font-size: 10px;">&nbsp;' . $NamaProduct . '</td>';
                        }else {
                            $tablePrint .= '<td align="center" style="width: ' . strval($ColumnWidth) . 'px;font-size: 10px;">' . $NamaProduct . '</td>';
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
                $tablePrint .= '<td style="font-size: 11px;font-weight: bold;padding-left: 4px;width: '.strval($ColumnWidth).'px;">' . strtoupper($dataProduct['info']->namaproduct) . '</td>';
                if ($TotalInRow > 1 && ($k < $TotalInRow - 1)){
                    $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                }
            }
            $tablePrint .= '</tr>';
            $tablePrint .= '<tr>';
            for ($k = 0;$k < $TotalInRow;$k++) {
                $tablePrint .= '<td style="font-size: 34px;font-weight: bold;padding-left: 4px;width: '.strval($ColumnWidth).'px;"><sup style="font-size: 10px;">' . $currencySym . '</sup> ' . number_format($dataProduct['info']->hargajual, $DecDigit, $decimalSep, $thousandSep) . '</td>';
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
                $tablePrint .= '<td style="width: '.strval($ColumnWidth).'px;"><svg id="' . $BarcodeId . '" class="barcode"></svg></td>';
                if ($TotalInRow > 1 && ($k < $TotalInRow - 1)){
                    $tablePrint .= '<td style="width:11px;">&nbsp;</td>';
                }
                $index++;
            }
            $tablePrint .= '</tr>';
            $tablePrint .= '<tr>';
            for ($k = 0;$k < $TotalInRow;$k++) {
                $tablePrint .= '<td style="font-size: 10px;font-weight: bold;width: '.strval($ColumnWidth).'px;' . $Spacing . '">' . $dataProduct['info']->barcode . '</td>';
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

function get_close_cashier_data($user_id = null, $date_from = null, $date_thru = null){
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
    $IdPeetyCash = db_result(
        db_query(
            'SELECT id FROM user_petty_cash WHERE login=%d AND uid=%d',
            $user->login, $user->uid
        )
    );
    $StrSql = "SELECT total_round, bayar_multiple, kembali, bayar FROM penjualan ";
    $StrSql .= "WHERE idpemakai=%d AND (tglpenjualan >= '%s' AND tglpenjualan <= '%s')";
    if (empty($date_from)) {
        $LastLogin = date('Y-m-d H:i:s', $user->login);
    }else{
        $LastLogin = date('Y-m-d H:i:s', $date_from);
    }
    if (empty($date_thru)) {
        $CurrentTime = date('Y-m-d H:i:s');
    }else{
        $CurrentTime = date('Y-m-d H:i:s', $date_thru);
    }
    $result = db_query($StrSql,$user->uid, $LastLogin,$CurrentTime);
    $DataReport = new stdClass();
    $DataReport->id = $IdPeetyCash;
    $DataReport->username = $user->name;
    $DataReport->login = $LastLogin;
    $DataReport->logout = $CurrentTime;
    $DataReport->petty_cash = $UserPettyCash;
    $DataReport->total = 0;
    $DataReport->total_cash = 0;
    $DataReport->total_kad_kredit = 0;
    $DataReport->total_kad_debit = 0;
    $DataReport->total_hutang = 0;
    $DataReport->total_kembali = 0;
    while ($data = db_fetch_object($result)){
        $DataReport->total = $DataReport->total + $data->total_round;
        $ExplodeBayar = explode(';', $data->bayar_multiple);
        for ($i = 0;$i < count($ExplodeBayar);$i++){
            $ExplodeDetail = explode(':',$ExplodeBayar[$i]);
            $NilaiBayar = floatval($ExplodeDetail[1])*1;
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
    if (isset($_GET['id_petty_cash']) && !empty($_GET['id_petty_cash'])){

    }else {
        drupal_add_js(
            array(
                'closeCashier' => true,
            ),
            'setting');
    }
    $js_path = drupal_get_path('module', 'penjualan') . '/js/direct.print.cashier.js';
    drupal_add_js($js_path);
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $dataPremis = getPremisInformation();
    $DecDigit = $dataPremis->decimal_digit;
    $printHeader = true;
    $pathLogo = createLogoImagePath();
    $UserId = null;
    $DateFrom = null;
    $DateThru = null;
    if (isset($_GET['id_petty_cash']) && !empty($_GET['id_petty_cash'])){
        $UserId = db_result(db_query('SELECT uid FROM user_petty_cash WHERE id=%d', $_GET['id_petty_cash']));
        $DateFrom = db_result(db_query('SELECT login FROM user_petty_cash WHERE id=%d', $_GET['id_petty_cash']));
        $DateThru = db_result(db_query('SELECT log_out FROM user_petty_cash WHERE id=%d', $_GET['id_petty_cash']));
        if (empty($DateThru)){
            $DateThru = null;
        }
    }
    $ClosedData = get_close_cashier_data($UserId, $DateFrom, $DateThru);
    if (!empty($ClosedData)) {
        $tablePrint = '<table style="width: 305px;border-collapse: collapse;" >';
        $tablePrint .= '<tbody>';
        if ($printHeader) {
            $tablePrint .= '<tr><td rowspan="3" style="border-bottom: 1px solid #000;border-top: 1px solid #000;width:50px;text-align: center;"><img height="48" src="' . $pathLogo . '" /></td><td colspan="2" style="font-size: 14px;font-weight: bold;text-align: center;border-bottom: 1px solid #000;border-top: 1px solid #000;">' . $dataPremis->nama . '</td></tr>';
            $tablePrint .= '<tr><td colspan="2" style="font-size: 11px;text-align: center;border-bottom: 1px solid #000;">' . $dataPremis->alamat . '&nbsp;&nbsp;</td></tr>';
        }
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;"><strong>LAPORAN TUTUP CASHIER</strong></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: left;"><span style="float: left"><strong>USER ID</strong></span><span style="float: right"><strong>' . $ClosedData->username . '</strong></span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;"><span style="float: left">LOGIN</span><span style="float: right">' . $ClosedData->login . '</span></td></tr>';
        $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: left;border-bottom: 1px solid #000;"><span style="float: left">CLOSE</span><span style="float: right">' . $ClosedData->logout . '</span></td></tr>';
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
    if (isset($_GET['id_petty_cash']) && !empty($_GET['id_petty_cash'])){

    }else {
        if (!empty($ClosedData->id)) {
            db_query('UPDATE user_petty_cash SET log_out=%d WHERE id=%d', time(), $ClosedData->id);
        }
    }
    return $tablePrint;
}

function print_detail_order($id_order = null, $id_detail = null, $qty_print = null)
{
    global $user;
    date_default_timezone_set($user->timezone_name);
    $js_path = drupal_get_path('module', 'penjualan') . '/js/direct.print.order.js';
    drupal_add_js($js_path);
    $tablePrint = '';
    if (!empty($id_order) && !empty($id_detail)) {
        $DataPremis = get_data_premis(false);
        $DataOrder = getCustomerOrder($id_order);
        if (count($DataOrder)) {
            $orderInfo = $DataOrder['info'];
            $TotalOrder = count($DataOrder['detail']);
            for ($i = 0;$i < count($DataOrder['detail']);$i++) {
                if ($DataOrder['detail'][$i]['id'] == $id_detail) {
                    $categoryName = $DataOrder['detail'][$i]['kategori'];
                    $orderDetail = $DataOrder['detail'][$i];
                    drupal_add_js(
                        array(
                            'printer_name' => $orderDetail['selected_printer'],
                        ),
                        'setting');
                    $tablePrint .= '<table style="width: 240px;border-collapse: collapse;" >';
                    $tablePrint .= '<tbody>';
                    $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">KATEGORI</span><span style="float: right">' . strtoupper($categoryName) . '</span></td></tr>';
                    $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">NO. ORDER</span><span style="float: right">' . $orderInfo->nonota . '</span></td></tr>';
                    $tablePrint .= '<tr><td colspan="3" style="font-weight: bold;font-size: 14px;text-align: center;"><span style="float: left">MEJA</span><span style="float: right">' . strtoupper($orderInfo->meja) . '</span></td></tr>';
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">' . strtoupper(trans('tanggal', $DataPremis->bahasa)) . '</span><span style="float: right">' . substr($orderInfo->tglorder, 0, 16) . '</span></td></tr>';
                    $tablePrint .= '<tr>';
                    $tablePrint .= '<td colspan="2" style="font-size: 13px;">' . strtoupper($orderDetail['namaproduct']) . '</td>';
                    if (!empty($qty_print)){
                        $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;width: 50px;">x ' . $qty_print . '</td>';
                    }else{
                        $tablePrint .= '<td style="font-size: 13px;font-weight: bold;text-align: right;width: 50px;">x ' . $orderDetail['jumlah'] . '</td>';
                    }

                    $tablePrint .= '</tr>';
                    if ($orderDetail['keterangan'] != '' && !is_null($orderDetail['keterangan'])){
                        $tablePrint .= '<tr><td colspan="3" style="font-size: 11px;"><strong>' . $orderDetail['keterangan'] . '</strong></td></tr>';
                    }
                    $tablePrint .= '<tr><td colspan="3" style="font-size: 14px;text-align: center;border-bottom: 1px solid #000;"><span style="float: left">KETERANGAN</span><span style="float: right">' . $orderInfo->keterangan . '</span></td></tr>';
                    $tablePrint .= '</tbody>';
                    $tablePrint .= '</table>';
                    $tablePrint .= '<div >&nbsp;</div>';
                    break;
                }
            }
        }
    }
    return $tablePrint;
}

function sync_penjualan_temp(){
    global $user;
    if (module_exists('gst_separator')){
        $StrSql = "SELECT idpenjualan, nonota, idtitipanlaundry, idcustomerorder, tglpenjualan, idpemakai, ";
        $StrSql .= "idpelanggan, total, totalmodal, carabayar, bayar, bayar_multiple, kembali, ";
        $StrSql .= "perlakuankembalian, nokartu, keterangan, insert_date, uploaded, ";
        $StrSql .= "changed, ppn, ppn_value, total_plus_ppn, idmeja, total_round, android_order, ";
        $StrSql .= "voucher_value, voucher_number, total_after_voucher, synced, gst_flag FROM penjualan_temp ";
        $StrSql .= "WHERE synced = 0 AND sync_process = 0";
    }else {
        $StrSql = "SELECT idpenjualan, nonota, idtitipanlaundry, idcustomerorder, tglpenjualan, idpemakai, ";
        $StrSql .= "idpelanggan, total, totalmodal, carabayar, bayar, bayar_multiple, kembali, ";
        $StrSql .= "perlakuankembalian, nokartu, keterangan, insert_date, uploaded, ";
        $StrSql .= "changed, ppn, ppn_value, total_plus_ppn, idmeja, total_round, android_order, ";
        $StrSql .= "voucher_value, voucher_number, total_after_voucher, synced FROM penjualan_temp WHERE synced = 0 AND sync_process = 0";
    }
    $Result = db_query($StrSql);
    $DataToUpdate = array();
    while ($Data = db_fetch_object($Result)){
        $DataToUpdate[] = $Data;
        db_query("UPDATE penjualan_temp SET sync_process = 1 WHERE idpenjualan=%d", $Data->idpenjualan);
    }
    for ($i = 0;$i < count($DataToUpdate);$i++) {
        $Data = $DataToUpdate[$i];
        $StrSqlCheck = "SELECT COUNT(idpenjualan) FROM penjualan WHERE nonota='%s'";
        $PenjualanExists = db_result(db_query($StrSqlCheck, $Data->nonota));
        $Synced = false;
        if (!$PenjualanExists) {
            if (module_exists('gst_separator')){
                $StrSqlInsert = "INSERT INTO penjualan (nonota, idtitipanlaundry, idcustomerorder, tglpenjualan,";
                $StrSqlInsert .= "idpemakai, idpelanggan, total, totalmodal, carabayar, bayar, bayar_multiple, kembali, ";
                $StrSqlInsert .= "perlakuankembalian, nokartu, keterangan, insert_date, uploaded, changed, ppn, ppn_value, ";
                $StrSqlInsert .= "total_plus_ppn, idmeja, total_round, android_order, voucher_value, voucher_number, ";
                $StrSqlInsert .= "total_after_voucher, gst_flag) VALUES ";
                $StrSqlInsert .= "('%s', %d, %d, '%s', %d, %d, '%f', '%f', '%s', '%f', '%s', '%f', %d, '%s', '%s', ";
                $StrSqlInsert .= "'%s', %d, %d, '%f', '%f', '%f', %d, '%f', %d, '%f', '%s', '%f', '%d')";
                $Synced = db_query(
                    $StrSqlInsert,
                    $Data->nonota, $Data->idtitipanlaundry, $Data->idcustomerorder, $Data->tglpenjualan,
                    $Data->idpemakai, $Data->idpelanggan, $Data->total, $Data->totalmodal, $Data->carabayar,
                    $Data->bayar, $Data->bayar_multiple, $Data->kembali, $Data->perlakuankembalian, $Data->nokartu,
                    $Data->keterangan, $Data->insert_date, $Data->uploaded, $Data->changed, $Data->ppn,
                    $Data->ppn_value, $Data->total_plus_ppn, $Data->idmeja, $Data->total_round, $Data->andrabuya313oid_order,
                    $Data->voucher_value, $Data->voucher_number, $Data->total_after_voucher, $Data->gst_flag
                );
            }else {
                $StrSqlInsert = "INSERT INTO penjualan (nonota, idtitipanlaundry, idcustomerorder, tglpenjualan,";
                $StrSqlInsert .= "idpemakai, idpelanggan, total, totalmodal, carabayar, bayar, bayar_multiple, kembali, ";
                $StrSqlInsert .= "perlakuankembalian, nokartu, keterangan, insert_date, uploaded, changed, ppn, ppn_value, ";
                $StrSqlInsert .= "total_plus_ppn, idmeja, total_round, android_order, voucher_value, voucher_number, ";
                $StrSqlInsert .= "total_after_voucher) VALUES ";
                $StrSqlInsert .= "('%s', %d, %d, '%s', %d, %d, '%f', '%f', '%s', '%f', '%s', '%f', %d, '%s', '%s', ";
                $StrSqlInsert .= "'%s', %d, %d, '%f', '%f', '%f', %d, '%f', %d, '%f', '%s', '%f')";
                $Synced = db_query(
                    $StrSqlInsert,
                    $Data->nonota, $Data->idtitipanlaundry, $Data->idcustomerorder, $Data->tglpenjualan,
                    $Data->idpemakai, $Data->idpelanggan, $Data->total, $Data->totalmodal, $Data->carabayar,
                    $Data->bayar, $Data->bayar_multiple, $Data->kembali, $Data->perlakuankembalian, $Data->nokartu,
                    $Data->keterangan, $Data->insert_date, $Data->uploaded, $Data->changed, $Data->ppn,
                    $Data->ppn_value, $Data->total_plus_ppn, $Data->idmeja, $Data->total_round, $Data->andrabuya313oid_order,
                    $Data->voucher_value, $Data->voucher_number, $Data->total_after_voucher
                );
            }
            $IdPenjualan = db_last_insert_id('penjualan', 'idpenjualan');
            $Synced = true;
        }else{
            $DetailNotSyncExists = db_result(db_query("SELECT COUNT(iddetail) FROM detailpenjualan_temp WHERE idpenjualan=%d", $Data->idpenjualan));
            if ($DetailNotSyncExists){
                $Synced = true;
            }
        }
        if ($Synced) {
            $no_nota = $Data->nonota;
            $SyncedAll = true;
            $StrSqlDetail = "SELECT iddetail, idpenjualan, idproduct, idsupplier, jumlah, ";
            $StrSqlDetail .= "hargapokok, hargajual, diskon, ppn, qty_pcs, synced ";
            $StrSqlDetail .= "FROM detailpenjualan_temp WHERE idpenjualan=%d AND synced = 0";
            $ResultDetail = db_query($StrSqlDetail, $Data->idpenjualan);
            while ($DataDetail = db_fetch_object($ResultDetail)) {
                $StrSqlInsertDetail = "INSERT INTO detailpenjualan (idpenjualan, ";
                $StrSqlInsertDetail .= "idproduct, idsupplier, jumlah, hargapokok, hargajual, diskon, ppn, qty_pcs) ";
                $StrSqlInsertDetail .= "VALUES ";
                $StrSqlInsertDetail .= "(%d, %d, %d, '%f', '%f', '%f', '%f', '%f', %d)";
                $InsertDetail = db_query(
                    $StrSqlInsertDetail,
                    $IdPenjualan, $DataDetail->idproduct, $DataDetail->idsupplier,
                    $DataDetail->jumlah, $DataDetail->hargapokok, $DataDetail->hargajual, $DataDetail->diskon,
                    $DataDetail->ppn, $DataDetail->qty_pcs
                );
                if (!$InsertDetail) {
                    $SyncedAll = false;
                } else {
                    db_query('UPDATE detailpenjualan_temp SET synced = 1 WHERE iddetail=%d', $DataDetail->iddetail);
                    $StrSqlTypeProduct = 'SELECT type_product FROM product WHERE idproduct=%d';
                    $StrSqlStock = 'SELECT stok FROM product WHERE idproduct=%d';
                    $TypeProduct = db_result(db_query($StrSqlTypeProduct, $DataDetail->idproduct));
                    if ($TypeProduct == 0) {
                        $StockSebelum = db_result(db_query($StrSqlStock, $DataDetail->idproduct));
                        $StockSekarang = $StockSebelum - $DataDetail->jumlah;
                        db_query("UPDATE product SET stok='%f' WHERE idproduct='%d'", $StockSekarang, $DataDetail->idproduct);
                        $Keterangan = 'Penjualan';
                        db_query(
                            "INSERT INTO transaksistock (idproduk, idpenjualan, stocksebelum, 
                                keluar, stocksetelah, keterangan) VALUES ('%d', '%d', '%f', '%f', '%f', '%s')",
                            $DataDetail->idproduct, $IdPenjualan, $StockSebelum, $DataDetail->jumlah, $StockSekarang, $Keterangan
                        );
                    }
                }
            }

            $StrSqlPB = "SELECT idpenjualan, carabayar, total_bayar FROM penjualan_bayar_temp WHERE idpenjualan = %d AND synced = 0";
            $ResultPB = db_query($StrSqlPB, $Data->idpenjualan);
            while ($DataPB = db_fetch_object($ResultPB)) {
                $StrSqlInsertPB = "INSERT INTO penjualan_bayar (idpenjualan, carabayar, total_bayar) VALUES ";
                $StrSqlInsertPB .= "(%d, '%s', '%f')";
                $InsertPB = db_query(
                    $StrSqlInsertPB,
                    $IdPenjualan, $DataPB->carabayar, $DataPB->total_bayar
                );
                if (!$InsertPB) {
                    $SyncedAll = false;
                } else {
                    db_query(
                        "UPDATE penjualan_bayar_temp SET synced = 1 WHERE idpenjualan=%d AND carabayar = '%s'",
                        $DataPB->idpenjualan, $DataPB->carabayar
                    );
                }
            }
            if ($SyncedAll) {
                db_query("UPDATE penjualan_temp SET synced = 1, sync_process = 2 WHERE idpenjualan = %d", $Data->idpenjualan);
                if ($Data->kembali > 0 && $Data->perlakuankembalian == 2) {
                    if (!empty($Data->idpelanggan)) {
                        $variables['idpelanggan'] = $Data->idpelanggan;
                        $variables['hutang'] = getTotalHutang($Data->idpelanggan);
                        $variables['pembayaran'] = $Data->kembali;
                        $variables['tglbayar'] = $Data->tglpenjualan;
                        $variables['idpenjualan'] = $IdPenjualan;
                        $variables['idtitipanlaundry'] = $Data->idtitipanlaundry;
                        $variables['keterangan'] = 'Deposit Uang Dari Kembalian Nota Penjualan : ' . $no_nota . ' Tanggal : ' . $Data->tglpenjualan;
                        pembayaranhutang($variables);
                    }
                } else if ($Data->kembali < 0 && !empty($Data->idpelanggan)) {
                    db_query(
                        "INSERT INTO detailpiutang (idpelanggan, idpenjualan, besarhutang) VALUES ('%d','%d','%f')",
                        $Data->idpelanggan, $IdPenjualan, abs($Data->kembali)
                    );
                    $resultpiutang = db_query(
                        "SELECT idpelanggan, besarhutang  FROM piutang WHERE idpelanggan='%d'",
                        $Data->idpelanggan
                    );
                    $datapiutang = db_fetch_object($resultpiutang);
                    if ($datapiutang->idpelanggan) {
                        $besarhutangskrg = $datapiutang->besarhutang + abs($Data->kembali);
                        db_query("UPDATE piutang SET besarhutang='%f',uid='%d' WHERE idpelanggan='%d'",
                            $besarhutangskrg, $user->uid, $Data->idpelanggan);
                    } else {
                        db_query("INSERT INTO piutang(idpelanggan,besarhutang,uid)VALUES('%d','%f','%d')",
                            $Data->idpelanggan, abs($Data->kembali), $user->uid);
                    }
                }
            } else {
                db_query("UPDATE penjualan_temp SET synced = 0, sync_process = 0 WHERE idpenjualan = %d", $Data->idpenjualan);
            }
        }
    }
}


function sync_penjualan_temp_rest(){
    sync_penjualan_temp();
    exit();
}

function fix_double_sync(){
    $StrSql = "SELECT idpenjualan, nonota FROM penjualan WHERE tglpenjualan BETWEEN '2019-05-06 00:00' AND '2019-05-06 23:59'";
    $Result = db_query($StrSql);
    while ($Data = db_fetch_object($Result)){
        $StrSqlCheck = "SELECT idpenjualan FROM penjualan WHERE nonota='%s' AND idpenjualan != %d";
        $IdPenjualanDouble = db_result(db_query($StrSqlCheck, $Data->nonota, $Data->idpenjualan));
        if (!empty($IdPenjualanDouble)){
            $StrSqlDel = "DELETE FROM penjualan WHERE idpenjualan=%d";
            $StrSqlDetail = "DELETE FROM detailpenjualan WHERE idpenjualan=%d";
            $StrSqlBayar = "DELETE FROM penjualan_bayar WHERE idpenjualan=%d";
            db_query($StrSqlDetail, $IdPenjualanDouble);
            db_query($StrSqlBayar, $IdPenjualanDouble);
            db_query($StrSqlDel, $IdPenjualanDouble);
        }
    }
}

function delete_all_temp_penjualan(){
    global $user;
    date_default_timezone_set($user->timezone_name);
    $DateCheck = date('Y-m-d').' 00:00';
    $StrSqlTemp = "SELECT idpenjualan FROM penjualan_temp WHERE tglpenjualan < '%s' AND (synced = 1 OR sync_process = 1)";
    $Result = db_query($StrSqlTemp, $DateCheck);
    $DeletedId = array();
    while ($Data = db_fetch_object($Result)){
        $IdPenjualan = $Data->idpenjualan;
        $DetailExist = db_result(
            db_query("SELECT COUNT(iddetail) FROM detailpenjualan_temp WHERE synced = 0 AND idpenjualan = %d",$IdPenjualan)
        );
        if ($DetailExist == 0){
            db_query("DELETE FROM detailpenjualan_temp WHERE idpenjualan = %d AND synced = 1", $IdPenjualan);
        }else{
            db_query("UPDATE penjualan_temp SET synced=0, sync_process=0 WHERE idpenjualan=%d",$IdPenjualan);
        }
        $BayarExist = db_result(
            db_query("SELECT COUNT(*) FROM penjualan_bayar_temp WHERE synced = 0 AND idpenjualan=%d", $IdPenjualan)
        );
        if ($BayarExist  == 0){
            db_query("DELETE FROM penjualan_bayar_temp WHERE idpenjualan = %d AND synced = 1", $IdPenjualan);
        }else{
            db_query("UPDATE penjualan_temp SET synced=0, sync_process=0 WHERE idpenjualan=%d",$IdPenjualan);
        }
        if ($DetailExist == 0 && $BayarExist == 0){
            db_query("DELETE FROM penjualan_temp WHERE idpenjualan = %d AND (synced = 1 OR sync_process = 1)", $IdPenjualan);
            $DeletedId[] = $IdPenjualan;
        }
    }
}

function penggenapan($value = 0 , $dDigit = 2)
{

    $valueInString = strval(round($value,$dDigit));
    if (strpos($valueInString, ".") == 0) $valueInString = $valueInString.".00";
    $valueArray = explode(".", $valueInString);
    $substringValue = substr($valueArray[1], 1);

    if ($substringValue >= 1 && $substringValue < 5) {
        $tempValue = str_replace(substr($valueArray[1], 1), 0, substr($valueArray[1], 1));
        $tempValue = substr($valueArray[1],0,1).$tempValue;
        $newvalue = floatval($valueArray[0].".".$tempValue);
    } elseif($substringValue == 0 || $substringValue == 5 ) {
        $newvalue = floatval($value);
    } else {
        /* Penggenapan keatas*/
        /*$newFloat = floatval($valueArray[0].".".substr($valueArray[1],0,1));
        $newvalue = ($newFloat+0.1);*/
        /* Penggenapan kebawah */
        $tempValue = str_replace(substr($valueArray[1], 1), 5, substr($valueArray[1], 1));
        $tempValue = substr($valueArray[1],0,1).$tempValue;
        $newvalue = floatval($valueArray[0].".".$tempValue);
    }
    return $newvalue;
}

function penjualan_batch_code($batchCode = 0, $next_id = 0, $QTY = 0){
    //Process Check Batch Code pembelian jika ada, jika tidak asumsi FIFO
    if ($batchCode != 0){
        $strSQL = "SELECT jumlah FROM detailpembelian WHERE barcode_batch='%s'";
        $qtyBuy = db_result(db_query($strSQL,$batchCode));
        $strSQL = "SELECT qty_sell FROM detailpembelian WHERE barcode_batch='%s'";
        $qtySell = db_result(db_query($strSQL,$batchCode));
        $qtyLeft = $qtyBuy - $qtySell;
        $strSQL = "SELECT idpenjualan FROM detailpembelian WHERE barcode_batch='%s'";
        $idPenjualan = db_result(db_query($strSQL,$batchCode));
        if ($qtyLeft >= $QTY){
            if (!empty($idPenjualan)){
                $idPenjualan .= ';'.$next_id;
            }else{
                $idPenjualan = $next_id;
            }
            $qtySell = $qtySell + $QTY;
            $strSQL = "UPDATE detailpembelian SET qty_sell = '%f', idpenjualan = '%s' WHERE barcode_batch='%s'";
            db_query($strSQL,$qtySell,$idPenjualan,$batchCode);
        }else{
            if (!empty($idPenjualan)){
                $idPenjualan .= ';'.$next_id;
            }else{
                $idPenjualan = $next_id;
            }
            $qtyOther = $qtySell + $QTY - $qtyBuy;
            $qtySell = $qtyBuy;
            $strSQL = "UPDATE detailpembelian SET qty_sell = '%f', idpenjualan = '%s' WHERE barcode_batch='%s'";
            db_query($strSQL,$qtySell,$idPenjualan,$batchCode);
            while ($qtyOther > 0){
                $strSQL = "SELECT barcode_batch FROM detailpembelian WHERE jumlah > qty_sell AND (barcode_batch != '' AND  barcode_batch IS NOT NULL) ORDER BY expired_date LIMIT 1";
                $batchCodeOther = db_result(db_query($strSQL));
                if (!empty($batchCodeOther)) {
                    $strSQL = "SELECT jumlah FROM detailpembelian WHERE barcode_batch='%s'";
                    $qtyBuy = db_result(db_query($strSQL, $batchCodeOther));
                    $strSQL = "SELECT qty_sell FROM detailpembelian WHERE barcode_batch='%s'";
                    $qtySell = db_result(db_query($strSQL, $batchCodeOther));
                    $qtyLeft = $qtyBuy - $qtySell;
                    $strSQL = "SELECT idpenjualan FROM detailpembelian WHERE barcode_batch='%s'";
                    $idPenjualan = db_result(db_query($strSQL, $batchCodeOther));
                    if ($qtyLeft >= $qtyOther) {
                        if (!empty($idPenjualan)) {
                            $idPenjualan .= ';' . $next_id;
                        } else {
                            $idPenjualan = $next_id;
                        }
                        $qtySell = $qtySell + $qtyOther;
                        $strSQL = "UPDATE detailpembelian SET qty_sell = '%f', idpenjualan = '%s' WHERE barcode_batch='%s'";
                        db_query($strSQL, $qtySell, $idPenjualan, $batchCodeOther);
                        $qtyOther = 0;
                    } else {
                        if (!empty($idPenjualan)) {
                            $idPenjualan .= ';' . $next_id;
                        } else {
                            $idPenjualan = $next_id;
                        }
                        $qtyOther = $qtySell + $qtyOther - $qtyBuy;
                        $qtySell = $qtyBuy;
                        $strSQL = "UPDATE detailpembelian SET qty_sell = '%f', idpenjualan = '%s' WHERE barcode_batch='%s'";
                        db_query($strSQL, $qtySell, $idPenjualan, $batchCodeOther);
                    }
                }else{
                    $qtyOther = 0;
                }
            }
        }
    }else{
        $qtyOther = $QTY;
        while ($qtyOther > 0){
            $strSQL = "SELECT barcode_batch FROM detailpembelian WHERE jumlah > qty_sell AND (barcode_batch != '' AND  barcode_batch IS NOT NULL) ORDER BY expired_date LIMIT 1";
            $batchCodeOther = db_result(db_query($strSQL));
            if (!empty($batchCodeOther)) {
                $strSQL = "SELECT jumlah FROM detailpembelian WHERE barcode_batch='%s'";
                $qtyBuy = db_result(db_query($strSQL, $batchCodeOther));
                $strSQL = "SELECT qty_sell FROM detailpembelian WHERE barcode_batch='%s'";
                $qtySell = db_result(db_query($strSQL, $batchCodeOther));
                $qtyLeft = $qtyBuy - $qtySell;
                $strSQL = "SELECT idpenjualan FROM detailpembelian WHERE barcode_batch='%s'";
                $idPenjualan = db_result(db_query($strSQL, $batchCodeOther));
                if ($qtyLeft >= $qtyOther) {
                    if (!empty($idPenjualan)) {
                        $idPenjualan .= ';' . $next_id;
                    } else {
                        $idPenjualan = $next_id;
                    }
                    $qtySell = $qtySell + $qtyOther;
                    $strSQL = "UPDATE detailpembelian SET qty_sell = '%f', idpenjualan = '%s' WHERE barcode_batch='%s'";
                    db_query($strSQL, $qtySell, $idPenjualan, $batchCodeOther);
                    $qtyOther = 0;
                } else {
                    if (!empty($idPenjualan)) {
                        $idPenjualan .= ';' . $next_id;
                    } else {
                        $idPenjualan = $next_id;
                    }
                    $qtyOther = $qtySell + $qtyOther - $qtyBuy;
                    $qtySell = $qtyBuy;
                    $strSQL = "UPDATE detailpembelian SET qty_sell = '%f', idpenjualan = '%s' WHERE barcode_batch='%s'";
                    db_query($strSQL, $qtySell, $idPenjualan, $batchCodeOther);
                }
            }else{
                $qtyOther = 0;
            }
        }
    }
}

function print_multi_nota(){
    if (isset($_REQUEST['idpenjualan-select'])){
        drupal_add_js('misc/media/jquery-1.4.4.min.js');
        $js_path = drupal_get_path('module','penjualan').'/js/direct.print.cashier.js';
        _addBarcodePlugins();
        drupal_add_js($js_path);
        $js_path = drupal_get_path('module','penjualan').'/js/ghorder.barcode.js';
        drupal_add_js($js_path);
        $PrintId = $_REQUEST['idpenjualan-select'];
        if (count($PrintId) > 0){
            $tablePrint = '';
            for ($i = 0;$i < count($PrintId);$i++){
                $dataOrder = getDataPenjualan($PrintId[$i]);
                if (count($dataOrder)){
                    $tablePrint .= printKasirOrder($dataOrder);
                    $tablePrint .= '<div>&nbsp;</div>';
		    if ($i < (count($PrintId) - 1)){
                         $tablePrint .= '<div class="pagebreak">&nbsp;</div>';
		    }	
                }
            }
            return $tablePrint;
        }
    }
    return true;
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
	}else if (isset($_GET['idpenjualangh'])) {
        $printOut = printPenjualanDetail();
    }else if (isset($_GET['idpenjualantemp'])){
        $printOut = printPenjualanDetail(true);
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
    }else if (isset($_GET['id_order']) && isset($_GET['id_detail_order'])){
        $printOut = print_detail_order($_GET['id_order'], $_GET['id_detail_order'], $_GET['qtyprint']);
    }else if (isset($_POST['multi_print'])){
        $printOut = print_multi_nota();
    }
	return $printOut;
}
