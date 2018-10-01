<?php

function view_tabel_reproduksi(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	if ($_POST["tgl1"] AND $_POST["tgl2"]){
		$tgl1 = $_POST["tgl1"];
		$tgl2 = $_POST["tgl2"];
	}else{
		$tgl1 = date("Y-m-").'01';
		$tgl2 = date("Y-m-d");	
	}
	if ($_POST["pilihantampil"] == 'NOTA'){
		$nota = 'selected="selected"';
		$tabeltampil = tabel_reproduksi($tgl1,$tgl2);
		$urutan = 2; 
	}elseif ($_POST["pilihantampil"] == 'PRODUK'){
		$produk = 'selected="selected"';
		$tabeltampil = reproduksi_produk($tgl1,$tgl2);
		$urutan = 3;
	}else{
		$tabeltampil = tabel_reproduksi($tgl1,$tgl2);
		$nota = 'selected="selected"';
		$urutan = 2;
	}
	drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
	drupal_add_css('misc/media/datatables/css/TableTools.css');
	drupal_add_css('misc/media/datatables/themes/smoothness/jquery-ui-1.8.4.custom.css');
	drupal_add_css('misc/media/datatables/css/ColVis.css');
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	drupal_add_js('misc/media/jquery-ui-1.8.custom.min.js');
	drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
	drupal_add_js('misc/media/datatables/js/ColVis.min.js');
	drupal_add_js('misc/media/datatables/ZeroClipboard/ZeroClipboard.js');
	drupal_add_js('misc/media/datatables/js/TableTools.min.js');
	drupal_add_js(
		array(
			'urutan' => $urutan,
			'tampilData' => $tampilData,
			'tglAwal' => $tgl1,
			'tglAkhir' => $tgl2,
			'filterId' => $filterId,
		),
		'setting');
	$modulePath = drupal_get_path('module','reproduksi');
	$jsPath = $modulePath.'/js/tabel.reproduksi.js';
	drupal_add_js($jsPath);
	$pilihantampil = '<select id="pilihantampil" name="pilihantampil">';
	$pilihantampil .= '<option '.$nota.'>NOTA</option>';
	$pilihantampil .= '<option '.$produk.'>PRODUK</option>';
	$pilihantampil .= '</select>';
	$pilihperiode = '<form action="'.base_path().'reproduksi/viewreproduksi" method="post"><div id="pilihanperiode"><label>PILIH PERIODE PACK ULANG </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.$tgl1.'">';
	$pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.$tgl2.'">'.$pilihantampil.'<button>LIHAT PACK ULANG</button></div></form>';
	$viewreproduksi = $pilihperiode.'<div id="viewreproduksi">'.$tabeltampil.'</div>';
	$viewreproduksi .= '<div id="dialogdetail" title="DETAIL PACK ULANG"><div id="divdetailrepro"></div><div id="divdetailhasilrepro"></div></div>';
	return $viewreproduksi;
}

function tabel_reproduksi($tgl1,$tgl2){
	//nonota, tglreproduksi, idpemakai, idlangganan, total, 
	//totalmodal, carabayar, bayar, kembali
	get_number_format($currencySym, $thousandSep, $decimalSep, false);
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	$tgl2 = DateAdd(1,$tgl2,"Y-m-d");
	$tabelreproduksi ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_reproduksi">';
	$tabelreproduksi .= '<thead>';
	$tabelreproduksi .= '<tr>';
	$tabelreproduksi .= '<th class="tablebutton"></th>';
	$tabelreproduksi .= '<th class="tablebutton"></th>';
	$tabelreproduksi .= '<th>NO. NOTA</th>';
    $TglTitle = strtoupper(trans('tanggal', $dataPremis->bahasa));
	$tabelreproduksi .= '<th class="tanggal">'.$TglTitle.'</th>';
	$tabelreproduksi .= '<th class="tanggal">JAM</th>';
	$tabelreproduksi .= '<th>TOTAL</th>';
	$tabelreproduksi .= '<th>MODAL</th>';
	$tabelreproduksi .= '<th>LABA</th>';
	$tabelreproduksi .= '<th title="Total nilai jual hasil packing">T. HASIL</th>';
	$tabelreproduksi .= '<th title="Total nilai modal hasil packing">M. HASIL</th>';
	$tabelreproduksi .= '<th title="Total nilai laba hasil packing">L. HASIL</th>';
	$tabelreproduksi .= '<th>'.strtoupper(trans('kasir', $dataPremis->bahasa)).'</th>';
    $tabelreproduksi .= '<th>KETERANGAN</th>';
	$tabelreproduksi .= '</tr>';
	$tabelreproduksi .= '</thead>';
	$tabelreproduksi .= '<tbody>';
	$result = db_query("SELECT idreproduksi, name, nonota, SUBSTR(tglreproduksi,1,10) AS
	tanggal,SUBSTR(tglreproduksi,11,9) AS waktu, idpemakai, total, totalmodal,
	(total-totalmodal) AS laba, keterangan FROM
	reproduksi a LEFT JOIN cms_users b
	ON a.idpemakai = b.uid
	WHERE  SUBSTR(tglreproduksi,1,10) BETWEEN '%s' AND '%s' ORDER BY idreproduksi",
	$tgl1,$tgl2);
	$totaljual = 0;
	$totalmodal = 0;
	$totallaba = 0;
	$totaljualhasil = 0;
	$totalmodalhasil = 0;
	$totallabahasil = 0;
	while($data = db_fetch_object($result)){
		$hasilrepro = hitung_total_hasil($data->idreproduksi, 'total');
		$modalrepro = hitung_total_hasil($data->idreproduksi, 'totalmodal');
		$labarepro = $hasilrepro - $modalrepro;
		$tabelreproduksi .= '<tr>';
		$tomboldetail = '<img title="Klik untuk melihat detail packing ulang" onclick="view_detail(\''.$data->idreproduksi.'\',\''.$data->nonota.'\');" src="'.base_path().'misc/media/images/forward_enabled.ico">';
		//if ($hasilrepro > 0){
		//	$tombolhasil = '<img title="Hasil packing ulang sudah diisi" src="'.base_path().'misc/media/images/del.ico" width="24">';
		//}else{
		$tombolhasil = '<img title="Klik untuk mengisi hasil packing ulang" onclick="add_hasil_pack(\''.$data->idreproduksi.'\',\''.$data->nonota.'\');" src="'.base_path().'misc/media/images/add-small.ico">';
		//}
		$tabelreproduksi .= '<td class="center">'.$tomboldetail.'</td>';
		$tabelreproduksi .= '<td class="center">'.$tombolhasil.'</td>';
		$tabelreproduksi .= '<td class="center">'.$data->nonota.'</td>';
		$tabelreproduksi .= '<td class="center">'.$data->tanggal.'</td>';
		$tabelreproduksi .= '<td class="center">'.$data->waktu.'</td>';
		$totaljualhasil = $totaljualhasil + $hasilrepro;
		$totalmodalhasil = $totalmodalhasil + $modalrepro;
		$totallabahasil = $totallabahasil + $labarepro;
		$tabelreproduksi .= '<td class="angka">'.number_format($data->total,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($data->totalmodal,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($data->laba,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($hasilrepro,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($modalrepro,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($labarepro,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="center">'.$data->name.'</td>';
        $tabelreproduksi .= '<td class="center">'.$data->keterangan.'</td>';
		$tabelreproduksi .= '</tr>';
		$totaljual = $totaljual + $data->total;
		$totalmodal = $totalmodal + $data->totalmodal;
		$totallaba = $totallaba + $data->laba;
	}
	$tabelreproduksi .= '</tbody>';
	$tabelreproduksi .= '<tfoot>';
	$tabelreproduksi .= '<tr>';
	$tabelreproduksi .= '<th style="text-align:right" colspan="5">Total:&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totaljual,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalmodal,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totallaba,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totaljualhasil,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalmodalhasil,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totallabahasil,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th></th>';
    $tabelreproduksi .= '<th></th>';
	$tabelreproduksi .= '</tr>';
	$tabelreproduksi .= '</tfoot>';
	$tabelreproduksi .= '</table>';
	return $tabelreproduksi;
}

function detail_reproduksi(){
	if ($_POST["idreproduksi"]){
		get_number_format($currencySym, $thousandSep, $decimalSep,false);
        $dataPremis = get_data_premis();
        $DecDigit = $dataPremis->decimal_digit;
		$tabelreproduksi = '<div class="titletable">BAHAN PACKING ULANG</div>';
		$tabelreproduksi .='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_reproduksi">';
		$tabelreproduksi .= '<thead>';
		$tabelreproduksi .= '<tr>';
		$tabelreproduksi .= '<th>BARCODE</th>';
		$tabelreproduksi .= '<th>PRODUK</th>';
		$tabelreproduksi .= '<th class="qty">QTY</th>';
		$tabelreproduksi .= '<th>H.JUAL</th>';
		$tabelreproduksi .= '<th>H.MODAL</th>';
		$tabelreproduksi .= '<th>SUBTOTAL</th>';
		$tabelreproduksi .= '<th>MODAL</th>';
		$tabelreproduksi .= '<th>LABA</th>';
		$tabelreproduksi .= '</tr>';
		$tabelreproduksi .= '</thead>';
		$tabelreproduksi .= '<tbody>';
		$result = db_query("SELECT c.barcode, c.namaproduct, b.jumlah, b.hargajual,
		b.hargapokok, (b.hargajual-b.hargapokok)*b.jumlah AS laba,
		(b.hargajual*b.jumlah) AS subtotal, (b.hargapokok*b.jumlah) AS totalmodal FROM 
		detailreproduksi b LEFT JOIN product c ON b.idproduct=c.idproduct
		LEFT JOIN supplier a ON a.idsupplier=c.idsupplier WHERE
		b.idreproduksi='%d'",$_POST["idreproduksi"]);
		while($data = db_fetch_object($result)){
			$tabelreproduksi .= '<tr>';
			$tabelreproduksi .= '<td>'.$data->barcode.'</td>';
			$tabelreproduksi .= '<td>'.$data->namaproduct.'</td>';
			$tabelreproduksi .= '<td class="angka">'.$data->jumlah.'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->hargajual,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->hargapokok,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->subtotal,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->totalmodal,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->laba,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '</tr>';
		}
		$tabelreproduksi .= '</tbody>';
		$tabelreproduksi .= '</table><br><br>';
		print $tabelreproduksi;
	}
	exit();	
}
function detail_hasilreproduksi(){
	if ($_POST["idreproduksi"]){
		get_number_format($currencySym, $thousandSep, $decimalSep,false);
        $dataPremis = get_data_premis();
        $DecDigit = $dataPremis->decimal_digit;
		$tabelreproduksi = '<div class="titletable">HASIL PACKING ULANG</div>';
		$tabelreproduksi .='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_hasilreproduksi">';
		$tabelreproduksi .= '<thead>';
		$tabelreproduksi .= '<tr>';
		$tabelreproduksi .= '<th>BARCODE</th>';
		$tabelreproduksi .= '<th>PRODUK</th>';
		$tabelreproduksi .= '<th class="qty">QTY</th>';
		$tabelreproduksi .= '<th>H.JUAL</th>';
		$tabelreproduksi .= '<th>H.MODAL</th>';
		$tabelreproduksi .= '<th>SUBTOTAL</th>';
		$tabelreproduksi .= '<th>MODAL</th>';
		$tabelreproduksi .= '<th>LABA</th>';
		$tabelreproduksi .= '</tr>';
		$tabelreproduksi .= '</thead>';
		$tabelreproduksi .= '<tbody>';
		$result = db_query("SELECT c.barcode, c.namaproduct, b.jumlah, b.hargajual,
		b.hargapokok, (b.hargajual-b.hargapokok)*b.jumlah AS laba,
		(b.hargajual*b.jumlah) AS subtotal, (b.hargapokok*b.jumlah) AS totalmodal FROM 
		detailhasilreproduksi b LEFT JOIN product c ON b.idproduct=c.idproduct
		LEFT JOIN supplier a ON a.idsupplier=c.idsupplier WHERE
		b.idreproduksi='%d'",$_POST["idreproduksi"]);
		while($data = db_fetch_object($result)){
			$tabelreproduksi .= '<tr>';
			$tabelreproduksi .= '<td>'.$data->barcode.'</td>';
			$tabelreproduksi .= '<td>'.$data->namaproduct.'</td>';
			$tabelreproduksi .= '<td class="angka">'.$data->jumlah.'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->hargajual,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->hargapokok,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->subtotal,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->totalmodal,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '<td class="angka">'.number_format($data->laba,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelreproduksi .= '</tr>';
		}
		$tabelreproduksi .= '</tbody>';
		$tabelreproduksi .= '</table><br><br>';
		print $tabelreproduksi;
	}
	exit();	
}
function reproduksi_produk($tgl1,$tgl2){
	get_number_format($currencySym, $thousandSep, $decimalSep, false);
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	$tgl2 = DateAdd(1,$tgl2,"Y-m-d");
	$tabelreproduksi ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_reproduksi">';
	$tabelreproduksi .= '<thead>';
	$tabelreproduksi .= '<tr>';
	$tabelreproduksi .= '<th>BARCODE</th>';
	$tabelreproduksi .= '<th>PRODUK</th>';
	$tabelreproduksi .= '<th>SUPPLIER</th>';
	$tabelreproduksi .= '<th class="qty">QTY</th>';
	$tabelreproduksi .= '<th>H.JUAL</th>';
	$tabelreproduksi .= '<th>H.MODAL</th>';
	$tabelreproduksi .= '<th>SUBTOTAL</th>';
	$tabelreproduksi .= '<th>MODAL</th>';
	$tabelreproduksi .= '<th>LABA</th>';
	$tabelreproduksi .= '</tr>';
	$tabelreproduksi .= '</thead>';
	$tabelreproduksi .= '<tbody>';
	$result = db_query("SELECT d.namasupplier, a.tglreproduksi, c.barcode, c.namaproduct, SUM(b.jumlah) AS totalqty, c.hargajual, c.hargapokok, SUM(b.hargajual*b.jumlah) AS subtotal, 
	SUM(b.hargapokok*b.jumlah) AS totalmodal, SUM((b.hargajual-b.hargapokok)*b.jumlah) AS laba FROM 
	detailreproduksi b LEFT JOIN reproduksi a ON a.idreproduksi=b.idreproduksi
	LEFT JOIN product c ON b.idproduct=c.idproduct
	LEFT JOIN supplier d ON c.idsupplier=d.idsupplier WHERE
	a.tglreproduksi BETWEEN '%s' AND '%s' GROUP BY b.idproduct",$tgl1,$tgl2);
	$totaljual = 0;
	$totalmodal = 0;
	$totallaba = 0;
	while($data = db_fetch_object($result)){
		$tabelreproduksi .= '<tr>';
		$tabelreproduksi .= '<td>'.$data->barcode.'</td>';
		$tabelreproduksi .= '<td>'.$data->namaproduct.'</td>';
		$tabelreproduksi .= '<td>'.$data->namasupplier.'</td>';
		$tabelreproduksi .= '<td class="angka">'.$data->totalqty.'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($data->hargajual,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($data->hargapokok,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($data->subtotal,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($data->totalmodal,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '<td class="angka">'.number_format($data->laba,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelreproduksi .= '</tr>';
		$totaljual = $totaljual + $data->subtotal;
		$totalmodal = $totalmodal + $data->totalmodal;
		$totallaba = $totallaba + $data->laba;
	}
	$tabelreproduksi .= '</tbody>';
	$tabelreproduksi .= '<tfoot>';
	$tabelreproduksi .= '<tr>';
	$tabelreproduksi .= '<th style="text-align:right" colspan="6">Total:&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totaljual,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalmodal,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '<th style="text-align: right">'.$currencySym.' '.number_format($totallaba,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelreproduksi .= '</tr>';
	$tabelreproduksi .= '</tfoot>';
	$tabelreproduksi .= '</table>';
	return $tabelreproduksi;	
}

function DateAdd($v,$d=null , $f="m/d/Y"){ 
 /*
 To Use
 $TGLHITUNG = date("m/d/Y");
 $TGLAKHIRHITUNG = DateAdd(30,$TGLHITUNG);
 */	
 $d=($d?$d:date("Y-m-d")); 
 return date($f,strtotime($v." days",strtotime($d))); 
}

function hitung_total_hasil($idreproduksi, $sumber = null){
	if ($idreproduksi > 0){
		if ($sumber == 'total'){
			$result = db_query("SELECT total FROM hasilreproduksi WHERE idreproduksi='%d'",$idreproduksi);
			$data = db_fetch_object($result);
			$totalhitung = $data->total;
		}else{
			$result = db_query("SELECT totalmodal FROM hasilreproduksi WHERE idreproduksi='%d'",$idreproduksi);
			$data = db_fetch_object($result);
			$totalhitung = $data->totalmodal;
		}
	}else{
		$totalhitung = 0;	
	}	
	return $totalhitung;
}