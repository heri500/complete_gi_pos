<?php

function view_tabel_penjualan(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
	if ($_POST["tgl1"] AND $_POST["tgl2"]){
		$tgl1 = $_POST["tgl1"];
		$tgl2 = $_POST["tgl2"];
	}else{
		$tgl1 = date("Y-m-d");
		$tgl2 = date("Y-m-d");
	}
	$addedFilter = '';
	$filterId = 0;
	$nota = '';
	$produk = '';
	$kategoriproduk = '';
	if ($_POST["pilihantampil"] == 'NOTA'){
		$nota = 'selected="selected"';
		$idPelanggan = isset($_POST['idpelanggan']) ? $_POST['idpelanggan'] : 0;
		$tabeltampil = tabel_penjualan($tgl1,$tgl2, $idPelanggan);
		$urutan = 1;
		$tampilData = 0;
		$addedFilter = createPelangganSelection($idPelanggan,'ALL', false).'<input type="hidden" id="idsupplier" name="idsupplier" value="0">';
		if(isset($_POST['idpelanggan'])){
			$filterId = $_POST['idpelanggan'];
		}
	}elseif ($_POST["pilihantampil"] == 'PRODUK') {
		$produk = 'selected="selected"';
		$idSupplier = isset($_POST['idsupplier']) ? $_POST['idsupplier'] : 0;
		$tabeltampil = penjualan_produk($tgl1, $tgl2, $idSupplier);
		$urutan = 3;
		$tampilData = 1;
		$addedFilter = createSupplierSelection(
				$idSupplier,
				'ALL',
				false
			) . '<input type="hidden" id="idpelanggan" name="idpelanggan" value="0">';
		if (isset($_POST['idsupplier'])) {
			$filterId = $_POST['idsupplier'];
		}
	}else if($_POST['pilihantampil'] == 'KATEGORI PRODUK'){
		$kategoriproduk = 'selected="selected"';
		$addedFilter = '<input type="hidden" id="idsupplier" name="idsupplier" value="0"><input type="hidden" id="idsupplier" name="idsupplier" value="0">';
		$urutan = 0;
		$tampilData = 2;
		$tabeltampil = tabel_penjualan_kategori($tgl1,$tgl2);
	}else{
		$idPelanggan = isset($_POST['idpelanggan']) ? $_POST['idpelanggan'] : 0;
		$tabeltampil = tabel_penjualan($tgl1,$tgl2,$idPelanggan);
		$nota = 'selected="selected"';
		$urutan = 1;
		$tampilData = 0;
		$addedFilter = createPelangganSelection($idPelanggan,'ALL', false).'<input type="hidden" id="idsupplier" name="idsupplier" value="0">';
		if(isset($_POST['idpelanggan'])){
			$filterId = $_POST['idpelanggan'];
		}
	}
	drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
	drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
  	drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
  	drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
	drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
	drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine-en.js');
	drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine.js');
	drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
	_addJeditablePlugins();
	_addAutotabPlugins();
	drupal_add_js('misc/media/datatables.1.10/media/js/jquery.dataTables.js');
	drupal_add_js('misc/media/datatables.1.10/media/js/dataTables.jqueryui.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/dataTables.buttons.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.flash.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.html5.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.print.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.colVis.min.js');
  	$Konfirmasi = strtoupper(trans('yakin', $dataPremis->bahasa));
	drupal_add_js(
		array(
			'urutan' => $urutan,
			'tampilData' => $tampilData,
			'tglAwal' => $tgl1,
			'tglAkhir' => $tgl2,
			'filterId' => $filterId,
            'gstSym' => $gstSym,
            'dDigit' => $DecDigit,
            'konfirmasi_text' => $Konfirmasi,
		),
		'setting');
    drupal_add_js('misc/media/datatables/js/number.format.js');
	$modulePath = drupal_get_path('module','penjualan');
	$jsPath = $modulePath.'/js/tabel.penjualan.js';
	drupal_add_js($jsPath);
	$pilihantampil = '<select id="pilihantampil" name="pilihantampil">';
	$pilihantampil .= '<option '.$nota.'>NOTA</option>';
	$pilihantampil .= '<option '.$produk.'>PRODUK</option>';
	$pilihantampil .= '<option '.$kategoriproduk.'>KATEGORI PRODUK</option>';
	$pilihantampil .= '</select>';
	$pilihperiode = '<form action="'.base_path().'penjualan/viewpenjualan" method="post"><div id="pilihanperiode"><label>PILIH PERIODE PENJUALAN </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.$tgl1.'">';
	$pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.$tgl2.'">'.$pilihantampil.'&nbsp;'.$addedFilter.'<button>LIHAT PENJUALAN</button></div></form>';
	$viewpenjualan = $pilihperiode.'<div id="viewpenjualan">'.$tabeltampil.'</div>';
	$viewpenjualan .= '<div id="dialogdetail" title="DETAIL PENJUALAN">';
	$viewpenjualan .= '<div id="table-detail-wrapper"></div>';
	$viewpenjualan .= '<div id="add-new-item">';
	$viewpenjualan .= '<h5 class="no-margin">ADD NEW ITEM</h5>';
	$inputProduct = '<input type="text" id="new-product" name="new-product" size="35">';
	$inputProduct .= '<input type="hidden" id="idproduct" name="idproduct">';
	$viewpenjualan .= '<label>Product</label>'.$inputProduct;
	$inputHarga = '<input type="text" id="harga-view" name="harga-view" size="10" readonly="readonly">';
	$inputHarga .= '<input type="hidden" id="hargajual" name="hargajual">';
	$inputHarga .= '<input type="hidden" id="hargapokok" name="hargapokok">';
	$inputSubtotal = '<input type="text" id="subtotal-view" name="subtotal-view" size="14" readonly="readonly">';
	$inputQty = '<input type="text" id="qty-new" name="qty-new" size="6">';
	$viewpenjualan .= $inputHarga.'<label class="small-label">Qty</label>'.$inputQty.$inputSubtotal;
	$buttonAddNew = '<button id="add-new-button">Add Item</button>';
	$buttonPrintSlip = '<button id="print-slip">Print Slip</button>';
	$viewpenjualan .= $buttonAddNew.'</div>'.$buttonPrintSlip;
	$viewpenjualan .= '</div>';
	return $viewpenjualan;
}

function tabel_penjualan($tgl1,$tgl2,$idPelanggan){
	//nonota, tglpenjualan, idpemakai, idlangganan, total, 
	//totalmodal, carabayar, bayar, kembali
	//$tgl2 = DateAdd(1,$tgl2,"Y-m-d");
    get_number_format($currencySym, $thousandSep, $decimalSep,false);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	$tabelpenjualan ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_penjualan">';
	$tabelpenjualan .= '<thead>';
	$tabelpenjualan .= '<tr>';
	$tabelpenjualan .= '<th class="tablebutton"></th>';
	$tabelpenjualan .= '<th>'.strtoupper(trans('no. nota', $dataPremis->bahasa)).'</th>';
	$tabelpenjualan .= '<th class="tanggal">'.strtoupper(trans('tanggal', $dataPremis->bahasa)).'</th>';
	$tabelpenjualan .= '<th class="tanggal">JAM</th>';
	$tabelpenjualan .= '<th>TOTAL</th>';
	$tabelpenjualan .= '<th>'.strtoupper($gstSym).'</th>';
	$tabelpenjualan .= '<th>TOTAL+'.strtoupper($gstSym).'</th>';
	$tabelpenjualan .= '<th>MODAL</th>';
	$tabelpenjualan .= '<th>LABA</th>';
	$tabelpenjualan .= '<th>CARA BAYAR</th>';
	$tabelpenjualan .= '<th>BAYAR</th>';
	$tabelpenjualan .= '<th>'.strtoupper(trans('kembali', $dataPremis->bahasa)).'</th>';
	$tabelpenjualan .= '<th>'.strtoupper(trans('kasir', $dataPremis->bahasa)).'</th>';
	$tabelpenjualan .= '<th>PELANGGAN</th>';
    $tabelpenjualan .= '<th class="tablebutton">SLIP</th>';
    $tabelpenjualan .= '<th class="tablebutton">'.strtoupper(trans('faktur', $dataPremis->bahasa)).'</th>';
    $tabelpenjualan .= '<th class="tablebutton">&nbsp;</th>';
	$tabelpenjualan .= '</tr>';
	$tabelpenjualan .= '</thead>';
	$tabelpenjualan .= '<tbody>';
	$tabelpenjualan .= '</tbody>';
	$tabelpenjualan .= '<tfoot>';
	$tabelpenjualan .= '<tr>';
	$tabelpenjualan .= '<th style="text-align:right" colspan="4">Total:&nbsp;&nbsp;</th>';
	$totaljual = getTotalPenjualan('total',$tgl1,$tgl2,0,$idPelanggan);
	$totalmodal = getTotalPenjualan('totalmodal',$tgl1,$tgl2,0,$idPelanggan);
	$totallaba = $totaljual - $totalmodal;
	$totalbayar = getTotalPenjualan('bayar',$tgl1,$tgl2,0,$idPelanggan);
	$totalkembali = getTotalPenjualan('kembali',$tgl1,$tgl2,0,$idPelanggan);
	$totalppn = 0;
	$totalplusppn = 0;
	$tabelpenjualan .= '<th style="text-align: right">'.$currencySym.' '.number_format($totaljual,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalppn,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalplusppn,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalmodal,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">'.$currencySym.' '.number_format($totallaba,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th>&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalbayar,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalkembali,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th>&nbsp;</th>';
	$tabelpenjualan .= '<th>&nbsp;</th>';
	$tabelpenjualan .= '<th>&nbsp;</th>';
    $tabelpenjualan .= '<th>&nbsp;</th>';
    $tabelpenjualan .= '<th>&nbsp;</th>';
	$tabelpenjualan .= '</tr>';
	$tabelpenjualan .= '</tfoot>';
	$tabelpenjualan .= '</table>';
	return $tabelpenjualan;
}

function detail_penjualan(){
	if ($_POST["idpenjualan"]){
        get_number_format($currencySym, $thousandSep, $decimalSep,false);
        $gstSym = get_gst_symbols();
        $dataPremis = get_data_premis(false);
		$tabelpenjualan ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_penjualan">';
		$tabelpenjualan .= '<thead>';
		$tabelpenjualan .= '<tr>';
		$tabelpenjualan .= '<th class="center tablebutton">&nbsp;</th>';
		$tabelpenjualan .= '<th>BARCODE</th>';
		$tabelpenjualan .= '<th>PRODUK</th>';
		$tabelpenjualan .= '<th class="qty">QTY</th>';
		$tabelpenjualan .= '<th>H.JUAL</th>';
        $tabelpenjualan .= '<th>'.strtoupper(trans('diskon', $dataPremis->bahasa)).'</th>';
        $tabelpenjualan .= '<th>PPN</th>';
		$tabelpenjualan .= '<th>H.MODAL</th>';
		$tabelpenjualan .= '<th>SUBTOTAL</th>';
		$tabelpenjualan .= '<th>MODAL</th>';
		$tabelpenjualan .= '<th>'.strtoupper(trans('laba', $dataPremis->bahasa)).'</th>';
		$tabelpenjualan .= '</tr>';
		$tabelpenjualan .= '</thead>';
		$tabelpenjualan .= '<tbody>';
		$tabelpenjualan .= '</tbody>';
		$tabelpenjualan .= '<tfoot>';
		$tabelpenjualan .= '<tr>';
		$tabelpenjualan .= '<th class="right" colspan="8">TOTAL</th>';
		$tabelpenjualan .= '<th>&nbsp;</th>';
		$tabelpenjualan .= '<th>&nbsp;</th>';
		$tabelpenjualan .= '<th>&nbsp;</th>';
		$tabelpenjualan .= '</tr>';
		$tabelpenjualan .= '</tfoot>';
		$tabelpenjualan .= '</table>';
		print $tabelpenjualan;
	}
	exit();	
}
function penjualan_produk($tgl1,$tgl2,$idSupplier){
	//$tgl2 = DateAdd(1,$tgl2,"Y-m-d");
	$tabelpenjualan ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_penjualan">';
	$tabelpenjualan .= '<thead>';
	$tabelpenjualan .= '<tr>';
	$tabelpenjualan .= '<th>BARCODE</th>';
	$tabelpenjualan .= '<th>PRODUK</th>';
	$tabelpenjualan .= '<th>SUPPLIER</th>';
	$tabelpenjualan .= '<th class="qty">QTY</th>';
	$tabelpenjualan .= '<th>H.JUAL MIN</th>';
	$tabelpenjualan .= '<th>H.JUAL MAX</th>';
	$tabelpenjualan .= '<th>H.MODAL MIN</th>';
	$tabelpenjualan .= '<th>H.MODAL MAX</th>';
	$tabelpenjualan .= '<th>SUBTOTAL</th>';
	$tabelpenjualan .= '<th>MODAL</th>';
	$tabelpenjualan .= '<th>LABA</th>';
	$tabelpenjualan .= '</tr>';
	$tabelpenjualan .= '</thead>';
	$tabelpenjualan .= '<tbody>';
	$totaljual = getTotalPenjualan2('detail.jumlah','detail.hargajual',$tgl1,$tgl2, $idSupplier);
	$totalmodal = getTotalPenjualan2('detail.jumlah','detail.hargapokok',$tgl1,$tgl2, $idSupplier);
	$totallaba = $totaljual - $totalmodal;
	$tabelpenjualan .= '</tbody>';
	$tabelpenjualan .= '<tfoot>';
	$tabelpenjualan .= '<tr>';
	$tabelpenjualan .= '<th style="text-align:right" colspan="8">Total:&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">Rp. '.number_format($totaljual,0,",",".").'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">Rp. '.number_format($totalmodal,0,",",".").'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">Rp. '.number_format($totallaba,0,",",".").'&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '</tr>';
	$tabelpenjualan .= '</tfoot>';
	$tabelpenjualan .= '</table>';
	return $tabelpenjualan;	
}

function tabel_penjualan_kategori(){
	$tabelpenjualan ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_penjualan">';
	$tabelpenjualan .= '<thead>';
	$tabelpenjualan .= '<tr>';
	$tabelpenjualan .= '<th>KODE</th>';
	$tabelpenjualan .= '<th>KATEGORI</th>';
	$tabelpenjualan .= '<th>SUBTOTAL</th>';
	$tabelpenjualan .= '<th>MODAL</th>';
	$tabelpenjualan .= '<th>LABA</th>';
	$tabelpenjualan .= '</tr>';
	$tabelpenjualan .= '</thead>';
	$tabelpenjualan .= '<tbody>';
	$tabelpenjualan .= '<tfoot>';
	$tabelpenjualan .= '<tr>';
	$tabelpenjualan .= '<th style="text-align:right" colspan="2">Total:&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">Rp. 0&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">Rp. 0&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '<th style="text-align: right">Rp. 0&nbsp;&nbsp;</th>';
	$tabelpenjualan .= '</tr>';
	$tabelpenjualan .= '</tfoot>';
	$tabelpenjualan .= '</table>';
	return $tabelpenjualan;
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
