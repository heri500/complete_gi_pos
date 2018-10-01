<?php

function view_tabel_laundry(){
	$path = drupal_get_path('theme', 'cti_flex');
	$form_style = $path.'/css/custom-style.css';
	drupal_add_css($form_style, 'theme', 'all', FALSE);
	$variables['styles'] = drupal_get_css();
	if ($_POST["tgl1"] AND $_POST["tgl2"]){
		$tgl1 = $_POST["tgl1"];
		$splitTgl = explode('-', $tgl1);
		$tgl1 = $splitTgl[2].'-'.$splitTgl[1].'-'.$splitTgl[0];
		$tgl2 = $_POST["tgl2"];
		$splitTgl = explode('-', $tgl2);
		$tgl2 = $splitTgl[2].'-'.$splitTgl[1].'-'.$splitTgl[0];
	}else{
		$tgl1 = date("Y-m-").'01';
		$tgl2 = date("Y-m-d");
	}
	if ($_POST["pilihantampil"] == 'NOTA'){
		$nota = 'selected="selected"';
		$tabeltampil = tabel_laundry($tgl1,$tgl2);
		$urutan = 1;
	}elseif ($_POST["pilihantampil"] == 'PRODUK'){
		$produk = 'selected="selected"';
		$tabeltampil = laundry_produk($tgl1,$tgl2);
		$urutan = 2;
	}else{
		$tabeltampil = tabel_laundry($tgl1,$tgl2);
		$nota = 'selected="selected"';
		$urutan = 1;
	}
	drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
	drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
	drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
	drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
	//_addJqueryPlugins();
	drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
	drupal_add_js('misc/media/jquery.jeditable.js');
	drupal_add_js('misc/media/jquery-ui-1.8.custom.min.js');
	_addTimePickerPlugins();
	_addJeditablePlugins();
	_addAutotabPlugins();
	drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
	drupal_add_js('misc/media/datatables.1.10/media/js/jquery.dataTables.js');
	drupal_add_js('misc/media/datatables.1.10/media/js/dataTables.jqueryui.js');
	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/dataTables.buttons.js');
	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.flash.js');
	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.html5.js');
	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.print.js');
	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.colVis.min.js');
	drupal_add_js(
		array(
			'urutan'  => $urutan,
			'tglawal' => $tgl1,
			'tglakhir'=> $tgl2
		),
		'setting'
	);
	$js_path = drupal_get_path('module','penjualan').'/js/tabel.laundry.js';
	drupal_add_js($js_path);
	$pilihantampil = '<select id="pilihantampil" name="pilihantampil">';
	$pilihantampil .= '<option '.$nota.'>NOTA</option>';
	$pilihantampil .= '<option '.$produk.'>PRODUK</option>';
	$pilihantampil .= '</select>';
	$pilihperiode = '<form action="'.base_path().'penjualan/viewlaundry" method="post"><div id="pilihanperiode"><label>PILIH PERIODE LAUNDRY </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.date('d-m-Y',strtotime($tgl1)).'">';
	$pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.date('d-m-Y', strtotime($tgl2)).'">'.$pilihantampil.'<button>LIHAT LAUNDRY</button></div></form>';
	$viewlaundry = $pilihperiode.'<div id="viewlaundry">'.$tabeltampil.'</div>';
	$viewlaundry .= '<div id="dialogdetail" title="DETAIL LAUNDRY"></div>';
	return $viewlaundry;
}

function tabel_laundry($tgl1,$tgl2){
	$tabellaundry ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_laundry">';
	$tabellaundry .= '<thead>';
	$tabellaundry .= '<tr>';
	$tabellaundry .= '<th class="center">&nbsp;</th>';
	$tabellaundry .= '<th class="center">&nbsp;</th>';
	$tabellaundry .= '<th class="center">&nbsp;</th>';
	$tabellaundry .= '<th class="center">&nbsp;</th>';
	$tabellaundry .= '<th>NO. NOTA</th>';
	$tabellaundry .= '<th class="center">HARI</th>';
	$tabellaundry .= '<th class="tanggal">TGL</th>';
	$tabellaundry .= '<th class="tanggal">JAM</th>';
	$tabellaundry .= '<th>TOTAL</th>';
	$tabellaundry .= '<th>CARA BAYAR</th>';
	$tabellaundry .= '<th>BAYAR</th>';
	$tabellaundry .= '<th>SISA</th>';
	$tabellaundry .= '<th>KASIR</th>';
	$tabellaundry .= '<th>PELANGGAN</th>';
	$tabellaundry .= '<th>STATUS</th>';
	$tabellaundry .= '<th>PERKIRAAN</th>';
	$tabellaundry .= '<th>KETERANGAN</th>';
	$tabellaundry .= '<th class="diskon">NO. RAK</th>';
	$tabellaundry .= '</tr>';
	$tabellaundry .= '</thead>';
	$tabellaundry .= '<tbody>';
	$tabellaundry .= '</tbody>';
	$tabellaundry .= '<tfoot>';
	$tabellaundry .= '<tr>';
	if (cekUserRoles()){
		$tabellaundry .= '<th style="text-align:right" colspan="8">Total:&nbsp;&nbsp;</th>';
	}else{
		$tabellaundry .= '<th style="text-align:right" colspan="7">Total:&nbsp;&nbsp;</th>';
	}
	$totaljual = 0;
	$totalbayar = 0;
	$totalSisa = 0;
	$tabellaundry .= '<th style="text-align: right">Rp. '.number_format($totaljual,0,",",".").'&nbsp;&nbsp;</th>';
	$tabellaundry .= '<th>&nbsp;</th>';
	$tabellaundry .= '<th style="text-align: right">Rp. '.number_format($totalbayar,0,",",".").'&nbsp;&nbsp;</th>';
	$tabellaundry .= '<th style="text-align: right">Rp. '.number_format($totalSisa,0,",",".").'&nbsp;&nbsp;</th>';
	$tabellaundry .= '<th>&nbsp;</th>';
	$tabellaundry .= '<th>&nbsp;</th>';
	$tabellaundry .= '<th>&nbsp;</th>';
	$tabellaundry .= '<th>&nbsp;</th>';
	$tabellaundry .= '<th>&nbsp;</th>';
	$tabellaundry .= '<th>&nbsp;</th>';
	$tabellaundry .= '</tr>';
	$tabellaundry .= '</tfoot>';
	$tabellaundry .= '</table>';
	return $tabellaundry;
}

function detail_laundry(){
	if ($_POST["idtitipanlaundry"]){
		$tabellaundry ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_laundry">';
		$tabellaundry .= '<thead>';
		$tabellaundry .= '<tr>';
		$tabellaundry .= '<th>BARCODE</th>';
		$tabellaundry .= '<th>PRODUK</th>';
		$tabellaundry .= '<th class="qty">QTY</th>';
		$tabellaundry .= '<th class="qty">DIAMBIL</th>';
		$tabellaundry .= '<th class="qty">SISA</th>';
		$tabellaundry .= '<th class="qty">PERKIRAAN</th>';
		$tabellaundry .= '<th class="qty">TGL AMBIL</th>';
		$tabellaundry .= '<th>H.JUAL</th>';
		$tabellaundry .= '<th>SUBTOTAL</th>';
		$tabellaundry .= '</tr>';
		$tabellaundry .= '</thead>';
		$tabellaundry .= '<tbody>';
		$result = db_query("SELECT b.iddetail,c.barcode, c.namaproduct, b.jumlah, b.hargajual, (b.hargajual*b.jumlah) AS subtotal,
		sisa, diambil,perkiraan_ambil FROM 
		detaillaundry b LEFT JOIN product c ON b.idproduct=c.idproduct LEFT JOIN supplier a ON c.idsupplier=a.idsupplier WHERE b.idtitipanlaundry='%d'",$_POST["idtitipanlaundry"]);
		while($data = db_fetch_object($result)){
			$iddetail = $data->iddetail;
			$tabellaundry .= '<tr>';
			$tabellaundry .= '<td>'.$data->barcode.'</td>';
			$tabellaundry .= '<td>'.$data->namaproduct.'</td>';
			$tabellaundry .= '<td id="detailjumlah-'.$iddetail.'" class="angka edit-jumlah">'.$data->jumlah.'</td>';
			$tabellaundry .= '<td class="angka">'.($data->jumlah - $data->sisa).'</td>';
			$tabellaundry .= '<td class="angka">'.$data->sisa.'</td>';
			$tglambil = '-';
			if (!empty($data->diambil)){
				$tglambil = date('d-m-Y H:i', $data->diambil);
			}
			$tabellaundry .= '<td id="tglambil-'.$iddetail.'" class="center edit-tanggal">'.date('d-m-Y H:i', $data->perkiraan_ambil).'</td>';
			$tabellaundry .= '<td id="tglambil-'.$iddetail.'" class="center edit-tanggal">'.$tglambil.'</td>';
			$tabellaundry .= '<td id="detailharga-'.$iddetail.'" class="angka edit-hargajual">'.number_format($data->hargajual,0,",",".").'</td>';
			$tabellaundry .= '<td class="angka">'.number_format($data->subtotal,0,",",".").'</td>';
			$tabellaundry .= '</tr>';
		}
		$tabellaundry .= '</tbody>';
		$tabellaundry .= '</table><br><br>';
		print $tabellaundry;
	}
	exit();
}
function laundry_produk($tgl1,$tgl2){
	$tgl1 = date('Y-m-d', strtotime($tgl1)).' 00:00';
	$tgl2 = date('Y-m-d', strtotime($tgl2)).' 23:59';
	$tabellaundry ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_laundry">';
	$tabellaundry .= '<thead>';
	$tabellaundry .= '<tr>';
	$tabellaundry .= '<th>BARCODE</th>';
	$tabellaundry .= '<th>JASA</th>';
	$tabellaundry .= '<th class="qty">QTY</th>';
	$tabellaundry .= '<th>H.JUAL</th>';
	$tabellaundry .= '<th>SUBTOTAL</th>';
	$tabellaundry .= '</tr>';
	$tabellaundry .= '</thead>';
	$tabellaundry .= '<tbody>';
	$result = db_query("SELECT d.namasupplier, a.tglpenjualan, c.barcode, c.namaproduct, SUM(b.jumlah) AS totalqty, c.hargajual, 
	SUM(b.hargajual*b.jumlah) AS subtotal FROM detaillaundry b LEFT JOIN titipanlaundry a ON a.idtitipanlaundry=b.idtitipanlaundry
	LEFT JOIN product c ON b.idproduct=c.idproduct LEFT JOIN supplier d ON c.idsupplier=d.idsupplier WHERE  a.tglpenjualan BETWEEN '%s' AND '%s' 
	GROUP BY b.idproduct",$tgl1,$tgl2);
	$totaljual = 0;
	$totalmodal = 0;
	$totallaba = 0;
	while($data = db_fetch_object($result)){
		$tabellaundry .= '<tr>';
		$tabellaundry .= '<td>'.$data->barcode.'</td>';
		$tabellaundry .= '<td>'.$data->namaproduct.'</td>';
		$tabellaundry .= '<td class="angka">'.$data->totalqty.'</td>';
		$tabellaundry .= '<td class="angka">'.$data->hargajual.'</td>';
		$tabellaundry .= '<td class="angka">'.$data->subtotal.'</td>';
		$tabellaundry .= '</tr>';
		$totaljual = $totaljual + $data->subtotal;
	}
	$tabellaundry .= '</tbody>';
	$tabellaundry .= '<tfoot>';
	$tabellaundry .= '<tr>';
	$tabellaundry .= '<th style="text-align:right" colspan="4">Total:&nbsp;&nbsp;</th>';
	$tabellaundry .= '<th style="text-align: right">Rp. '.number_format($totaljual,0,",",".").'&nbsp;&nbsp;</th>';
	$tabellaundry .= '</tr>';
	$tabellaundry .= '</tfoot>';
	$tabellaundry .= '</table>';
	return $tabellaundry;
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
function update_no_rak_laundry(){
	$nilai = $_POST['value'];
	$kolom = $_POST['kol_id'];
	$titipanId = $_POST['row_id'];
	$nilaiRet = $nilai;
	if ($kolom == '16'){
		$sql_update = "UPDATE titipanlaundry SET nomer_rak='%s' WHERE idtitipanlaundry='%d'";
	}
	db_query($sql_update,$nilai,$titipanId);
	echo $nilaiRet;
	exit();
}