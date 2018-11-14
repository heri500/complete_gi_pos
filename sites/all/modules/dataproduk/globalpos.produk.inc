<?php

function data_produk(){
    get_number_format($currencySym, $tSep, $dSep);
    $dataPremis = get_data_premis(false);
    $DecDigit = $dataPremis->decimal_digit;
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    //$variables['styles'] = drupal_get_css();
	drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
	drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
  	drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
  	drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
	drupal_add_css('misc/media/jqueryValidate/css/validationEngine.jquery.css');
	drupal_add_css('misc/media/jqueryValidate/css/template.css');
	drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
	drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine-en.js');
	drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine.js');
	drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
	drupal_add_js('misc/media/datatables.1.10/media/js/jquery.dataTables.js');
	drupal_add_js('misc/media/datatables.1.10/media/js/dataTables.jqueryui.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/dataTables.buttons.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.flash.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.html5.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.print.js');
  	drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.colVis.min.js');
    _addJeditablePlugins();
	drupal_add_js('misc/media/jquery.autotab-1.1b.js');
    drupal_add_js('sites/all/libraries/barcode/jquery-barcode.min.js');
	drupal_add_js('sites/all/libraries/uploadify/jquery.uploadify.v2.1.0.min.js');
	drupal_add_js('sites/all/libraries/uploadify/swfobject.js');
    drupal_add_js('misc/media/datatables/js/number.format.js');
    _addChosenJqueryPlugins();
    _addJsRounding();
	$filePath = base_path().file_directory_path();
    $IdKategori = 0;
    if (isset($_GET['idkategori']) && !empty($_GET['idkategori'])){
        $IdKategori = $_GET['idkategori'];
    }
	drupal_add_js(
		array(
			'filePath'  => $filePath,
            'idkategori' => $IdKategori,
            'dec_digit' => $DecDigit,
		),
		'setting'
	);
	$js_path = drupal_get_path('module','dataproduk').'/tabelproduknew.js';
	drupal_add_js($js_path);
	drupal_set_title('<span style="float: left;margin-right: 6px;">Data Produk</span><button id="tambahprodukbaru" class="tombolutama">Tambah Produk</button><button id="refreshprodukbaru" class="tombolutama">Refresh Tabel Produk</button>');
	//Pilihan Kategori
	$result = db_query("SELECT idkategori,kodekategori,kategori FROM kategori ORDER BY kodekategori,kategori");
	$pilihankategori = '<select id="idkategori" name="idkategori" onkeyup="filtersubkategori();" onchange="filtersubkategori();">';
	$i = 0;
	while ($data = db_fetch_object($result)){
		if ($i == 0){
			$idkategoriawal = $data->idkategori;
		}
		$pilihankategori .= '<option value="'.$data->idkategori.'">'.$data->kodekategori.'-'.$data->kategori.'</option>';
		$i++;
	}
	$pilihankategori .= '</select>';
	$buttonadd = '<img id="addkategori" title="Tambah Kategori Baru" src="'.base_path().'misc/media/images/add-small.ico">';
	//Pilihan Sub Kategori
	$result = db_query("SELECT idsubkategori,kodesubkategori,subkategori FROM subkategori WHERE idkategori='%d' 
	ORDER BY kodesubkategori,subkategori",$idkategoriawal);
	$pilihansubkategori = '<select id="idsubkategori" name="idsubkategori" onkeyup="ubahkodebarang();" onchange="ubahkodebarang();">';
	while ($data = db_fetch_object($result)){
		$pilihansubkategori .= '<option value="'.$data->idsubkategori.'">'.$data->kodesubkategori.'-'.$data->subkategori.'</option>';
	}
	$pilihansubkategori .= '</select>';
	$buttonadd2 = '<img id="addsubkategori" title="Tambah Subkategori Baru" src="'.base_path().'misc/media/images/add-small.ico">';
	//Pilihan Supplier
	$result = db_query("SELECT idsupplier,kodesupplier,namasupplier FROM supplier ORDER BY kodesupplier,namasupplier");
	$pilihansupplier = '<select id="idsupplier" name="idsupplier" multiple="true">';
	while ($data = db_fetch_object($result)){
		$pilihansupplier .= '<option value="'.$data->idsupplier.'">'.$data->kodesupplier.'-'.$data->namasupplier.'</option>';
	}
	$pilihansupplier .= '</select>';
	$buttonadd3 = '<img id="addsupplier" title="Tambah Supplier Baru" src="'.base_path().'misc/media/images/add-small.ico">';
	//Pilihan Satuan
	$result = db_query("SELECT satuan FROM satuan ORDER BY satuan");
	$pilihansatuan = '<select id="satuan" name="satuan" >';
	while ($data = db_fetch_object($result)){
		if ($data->satuan == "PCS"){
			$pilihansatuan .= '<option selected value="'.$data->satuan.'">'.$data->satuan.'</option>';
		}else{
			$pilihansatuan .= '<option value="'.$data->satuan.'">'.$data->satuan.'</option>';
		}
	}
	$pilihansatuan .= '</select>';
	$buttonadd4 = '<img id="addsatuan" title="Tambah satuan Baru" src="'.base_path().'misc/media/images/add-small.ico">';
	//Pilihan Kategori untuk Dialog Box Tambah Sub kategori
	$result = db_query("SELECT idkategori,kodekategori,kategori FROM kategori ORDER BY kodekategori,kategori");
	$pilihankategori2 = '<select disabled="disabled" id="idkategori2" name="idkategori2">';
    $FilterKategori = '<select id="kategori-filter" name="kategori-filter">';
    if ($IdKategori == 0){
        $FilterKategori .= '<option value="0" selected>Seluruh Kategori</option>';
    }else {
        $FilterKategori .= '<option value="0">Seluruh Kategori</option>';
    }
	while ($data = db_fetch_object($result)){
		$pilihankategori2 .= '<option value="'.$data->idkategori.'">'.$data->kodekategori.'-'.$data->kategori.'</option>';
        if ($data->idkategori == $IdKategori){
            $FilterKategori .= '<option value="' . $data->idkategori . '" selected>' . $data->kodekategori . '-' . $data->kategori . '</option>';
        }else {
            $FilterKategori .= '<option value="' . $data->idkategori . '">' . $data->kodekategori . '-' . $data->kategori . '</option>';
        }
	}
	$pilihankategori2 .= '</select>';
    $FilterKategori .= '</select>';
    $PilihanTypeBarcode = '<select id="barcode_type" name="barcode_type">';
    $PilihanTypeBarcode .= '<option value="ean13">EAN13</option>';
    $PilihanTypeBarcode .= '<option value="code39">CODE39</option>';
    $PilihanTypeBarcode .= '</select>';
	//idsupplier,idkategori,idsubkategori,barcode,alt_code,namaproduct,hargapokok,hargajual,margin,minstok,maxstok,stok,satuan,berat,keterangan
	$barcloseform = '<div id="formpenambahanproduk"><div id="barplace"><div id="barcloseform" onclick="tutupformtambahproduk();" title="Tutup form penambahan data produk">Tutup Form</div></div>';
	$viewtabelproduk = $barcloseform.'<div id="formplace" class="produk"><form id="formproduk" action="'.base_path().'dataproduk/simpanproduk" method="post">';
    $viewtabelproduk .= '<label>BARCODE TYPE</label>'.$PilihanTypeBarcode;
	$viewtabelproduk .= '<label>BARCODE</label><input type="text" class="validate[required,funcCall[cek_barcode]]" id="barcode" name="barcode"><div id="validating"></div>';
	$viewtabelproduk .= '<label>KODE ALTERNATIF</label><input readonly="readonly" type="text" id="kodepoduk" name="kodeproduk">';
	$viewtabelproduk .= '<label>KATEGORI</label>'.$pilihankategori.$buttonadd;
	$viewtabelproduk .= '<label>SUBKATEGORI</label>'.$pilihansubkategori.$buttonadd2;
	$viewtabelproduk .= '<label>SUPPLIER</label>'.$pilihansupplier.$buttonadd3;
	$viewtabelproduk .= '<label>NAMA PRODUK</label><input type="text" class="validate[required]" id="namaproduk" name="namaproduk">';
	$RadioLock = '<input id="rad-hargapokok" type="radio" name="lock-value" value="hargapokok">H.POKOK';
    $RadioLock .= '<input id="rad-hargajual" type="radio" name="lock-value" value="hargajual">H.JUAL';
    $RadioLock .= '<input id="rad-margin" type="radio" name="lock-value" value="margin" checked>MARGIN';
    $viewtabelproduk .= '<label>LOCK</label><div style="float: left;">'.$RadioLock.'</div>';
	$viewtabelproduk .= '<label>HARGA POKOK</label><input type="text" class="validate[required]" id="hargapokok" name="hargapokok" onkeyup="hitungmargin(\'hargapokok\')">';
	$viewtabelproduk .= '<label>HARGA JUAL</label><input type="text" class="validate[required]" id="hargajual" name="hargajual" onkeyup="hitungmargin(\'hargajual\')">';
	$viewtabelproduk .= '<label>MARGIN HARGA</label><input type="text" maxlength="2" id="margin" name="margin" onkeyup="hitungmargin(\'margin\')" readonly="readonly">';
	$pilihantype = '<select id="type_product" name="type_product">';
	$pilihantype .= '<option value="0">Produk</option>';
	$pilihantype .= '<option value="1">Jasa</option>';
	$pilihantype .= '</select>';
	$viewtabelproduk .= '<label>TYPE ITEM</label>'.$pilihantype;
	$viewtabelproduk .= '<div style="display: none;"><label>LEAD TIME (MENIT)</label><input type="text" id="lead_time" name="lead_time"></div>';
	$pilihanjamkerja = '<select id="ikut_jam_kerja" name="ikut_jam_kerja">';
	$pilihanjamkerja .= '<option value="0">Tidak</option>';
	$pilihanjamkerja .= '<option value="1">Ya</option>';
	$pilihanjamkerja .= '</select>';
	$viewtabelproduk .= '<div style="display: none;"><label>IKUT JAM KERJA</label>'.$pilihanjamkerja.'</div>';
	$pilihansebelumzuhur = '<select id="sebelum_zuhur" name="sebelum_zuhur">';
	$pilihansebelumzuhur .= '<option value="0">Tidak</option>';
	$pilihansebelumzuhur .= '<option value="1">Ya</option>';
	$pilihansebelumzuhur .= '</select>';
	$viewtabelproduk .= '<div style="display: none;"><label>BERLAKU SEBELUM JAM 12</label>'.$pilihansebelumzuhur.'</div>';
	$viewtabelproduk .= '<label>MINIMUM STOK</label><input type="text" id="minstok" name="minstok" value="2">';
	$viewtabelproduk .= '<label>MAKSIMUM STOK</label><input type="text" id="maxstok" name="maxstok" value="100">';
	$viewtabelproduk .= '<label>STOK SAAT INI</label><input type="text" id="stok" name="stok">';
	$viewtabelproduk .= '<label>SATUAN</label>'.$pilihansatuan.$buttonadd4;
	$viewtabelproduk .= '<label>GAMBAR</label><input id="uploadify" name="uploadify" type="file">';
	$viewtabelproduk .= '<label>KETERANGAN</label><input type="text" id="keteranganproduk" name="keteranganproduk">';
	$viewtabelproduk .= '</form><div><button onclick="save_produk();" style="font-size:12px;">Tambah Produk</button></div></div></div>';
	$pilihanstatus = '<div id="pilihanstatus">';
	$pilihanstatus .= '<img onclick="view_status(\'minimum\')" title="Klik untuk melihat produk dengan stok dibawah minimum" src="'.base_path().'misc/media/images/statusmerah.png">';
	$pilihanstatus .= '<img onclick="view_status(\'maksimum\')" title="Klik untuk melihat produk dengan stok diatas maksimum" src="'.base_path().'misc/media/images/statuskuning.png">';
	$pilihanstatus .= '<img onclick="view_status(\'aman\')" title="Klik untuk melihat produk dengan stok aman" src="'.base_path().'misc/media/images/statushijau.png">';
	$pilihanstatus .= '<img onclick="view_status(\'all\')" title="Klik untuk melihat seluruh produk" src="'.base_path().'misc/media/images/alldata.png">';
	$pilihanstatus .= '<img onclick="view_status(\'nonaktif\')" title="Klik untuk melihat seluruh produk non aktif" src="'.base_path().'misc/media/images/nonaktifdata.png">';
	$pilihanstatus .= '<span class="float-right">';
    //$pilihanstatus .= '<button id="export-xls" onclick="export_to_xls();">Export XLS</button>&nbsp;&nbsp;';
    $FileTypeOpt = '<select id="file_type" name="file_type" class="file-selection">';
    $FileTypeOpt .= '<option value="xls">EXCEL</option>';
    $FileTypeOpt .= '<option value="csv">CSV</option>';
    $FileTypeOpt .= '</select>';
    $pilihanstatus .= $FileTypeOpt.'<button id="export-data" onclick="export_data();" class="button-prod-table">Export</button>&nbsp;&nbsp;';
	$pilihanstatus .= '<button id="nonaktifkan" class="button-prod-table">Non Aktifkan</button>&nbsp;&nbsp;';
	$pilihanstatus .= '<button id="aktifkan" class="button-prod-table">Aktifkan</button>&nbsp;&nbsp;';
	//$pilihanstatus .= '<button id="print-barcode-biasa">Print Barcode Biasa</button>&nbsp;&nbsp;';
	//$pilihanstatus .= '<button id="print-barcode-logo">Print Barcode Dengan Logo</button>';
    $BarcodeTypeOpt = barcode_selection();
    $pilihanstatus .= $BarcodeTypeOpt.'<button id="print-barcode" class="button-prod-table">Print Barcode</button>&nbsp;&nbsp;';
    //$pilihanstatus .= '<button id="print-barcode-50x15">Barcode 50x15</button>';
    $BarcodeTypeOpt = barcode_selection('price-label','price-label');
    $pilihanstatus .= $BarcodeTypeOpt.'<button id="print-price-label" class="button-prod-table">Print Price Label</button>&nbsp;&nbsp;';
	$pilihanstatus .= '</span>';
	$pilihanstatus .= '</div>';
	$statusProduct = 1;
	if (isset($_GET["status_product"])){
		drupal_add_js(
			array(
				'statusproduct' => $_GET["status_product"],
			),
			'setting'
		);
		$statusProduct = $_GET["status_product"];
	}else{
		drupal_add_js(
			array(
				'statusproduct' => 1,
			),
			'setting'
		);
	}
    $viewtabelproduk .= $FilterKategori;
	if (isset($_GET["statusstok"])){
		$statusStok = $_GET["statusstok"];
		drupal_add_js(
			array(
				'statusstokfilter' => $statusStok,
			),
			'setting'
		);
		$viewtabelproduk .= $pilihanstatus.'<div id="tempattabel" style="width: 100%;">'.tabel_produk($statusStok, $statusProduct).'</div>';
	}else{
		drupal_add_js(
			array(
				'statusstokfilter' => 0,
			),
			'setting'
		);
		$viewtabelproduk .= $pilihanstatus.'<div id="tempattabel" style="width: 100%;">'.tabel_produk(null, $statusProduct).'</div>';
	}
	$viewtabelproduk .= '<div id="dialogtambahkategori" title="TAMBAH KATEGORI PRODUK">';
	$viewtabelproduk .= '<form id="tambahkategoriform"><div><label>KODE</label><input type="text" class="validate[required]" id="kodekategori" name="kodekategori"></div>';
	$viewtabelproduk .= '<div><label>KATEGORI</label><input class="validate[required]" type="text" id="kategori" name="kategori"></div>';
	$viewtabelproduk .= '<div><label>KETERANGAN</label><input type="text" id="keterangan" name="keterangan"></div></form>';
	$viewtabelproduk .= '<div><button id="tambahkategori" onclick="simpankategori();">Tambah Kategori</button></div>';
	$viewtabelproduk .= '</div>';
	$viewtabelproduk .= '<div id="dialogtambahsubkategori" title="TAMBAH SUBKATEGORI PRODUK">';
	$viewtabelproduk .= '<form id="tambahsubkategoriform"><div>';
	$viewtabelproduk .= '<div><label>KATEGORI</label>'.$pilihankategori2.'</div>';
	$viewtabelproduk .= '<div><label>KODE SUBKATEGORI</label><input type="text" class="validate[required]" id="kodesubkategori" name="kodesubkategori"></div>';
	$viewtabelproduk .= '<div><label>SUBKATEGORI</label><input type="text" class="validate[required]" id="subkategori" name="subkategori"></div>';
	$viewtabelproduk .= '<div><label>KETERANGAN</label><input type="text" id="keterangansub" name="keterangansub"></div></div></form>';
	$viewtabelproduk .= '<div><button id="tambahsubkategori" onclick="simpansubkategori();">Tambah Sub Kategori</button></div>';
	$viewtabelproduk .= '</div>';
	$viewtabelproduk .= '<div id="dialogtambahsupplier" title="TAMBAH SUPPLIER">';
	$viewtabelproduk .= '<form id="tambahsupplierform"><div><label>KODE SUPPLIER</label><input type="text" class="validate[required]" id="kodesupplier" name="kodesupplier"></div>';
	$viewtabelproduk .= '<div><label>NAMA SUPPLIER</label><input class="validate[required]" type="text" id="namasupplier" name="namasupplier"></div>';
	$viewtabelproduk .= '<div><label>TELP</label><input type="text" id="telp" name="telp"></div>';
	$viewtabelproduk .= '<div><label>ALAMAT</label><input type="text" id="alamat" name="alamat"></div>';
	$viewtabelproduk .= '<div><label>EMAIL</label><input type="text" id="email" name="email"></div>';
	$viewtabelproduk .= '</form>';
	$viewtabelproduk .= '<div><button id="tambahsupplier" onclick="simpansupplier();">Tambah Supplier</button></div></div>';
	$viewtabelproduk .= '<div id="dialogtambahsatuan" title="TAMBAH SATUAN">';
	$viewtabelproduk .= '<form id="tambahsatuanform"><div><label>SATUAN</label><input type="text" class=\"validate[required]\" id="namasatuan" name="namasatuan"></div>';
	$viewtabelproduk .= '</form>';
	$viewtabelproduk .= '<div><button id="tambahsatuan" onclick="simpansatuan();">Tambah Satuan</button></div>';
	$viewtabelproduk .= '</div>';
    $viewtabelproduk .= '<div id="dialoghargasupplier" title="HARGA PRODUK/SUPPLIER">';
    $viewtabelproduk .= '<div id="table-harga-wrapper"></div>';
    $viewtabelproduk .= '<div>';
    $viewtabelproduk .= '<button id="simpanhargasupplier" onclick="simpanhargasupplier();">Simpan Harga Supplier</button>&nbsp;';
    $viewtabelproduk .= '<button id="closehargasupplier" onclick="closehargasupplier();">Close</button>';
    $viewtabelproduk .= '</div>';
    $viewtabelproduk .= '</div>';
    $viewtabelproduk .= '<form id="form-print" method="post" action="'.base_path().'print/6" target="_blank" style="display:none;">';
    $viewtabelproduk .= '<textarea id="selected-product-print" name="selected-product-print"></textarea>';
    $viewtabelproduk .= '<input type="hidden" id="sticker-type" name="sticker-type" value="1">';
    $viewtabelproduk .= '<input type="hidden" id="sticker-size" name="sticker-size" value="52.5x37">';
    $viewtabelproduk .= '</form>';
	return $viewtabelproduk;
}

/**
 * @param string $ObjId
 * @param string $ObjName
 * @param string $class
 * @return string
 */
function barcode_selection($ObjId = 'barcode-select', $ObjName = 'barcode-select', $class = 'file-selection'){
    $BarcodeTypeOpt = '<select id="'.$ObjId.'" name="'.$ObjId.'" class="'.$class.'">';
    $BarcodeTypeOpt .= '<option value="52.5x37">Barcode 52.5x37</option>';
    $BarcodeTypeOpt .= '<option value="65x23x2">Barcode 65x23 (2 Column)</option>';
    $BarcodeTypeOpt .= '<option value="50x25">Barcode 50x25</option>';
    $BarcodeTypeOpt .= '<option value="50x15">Barcode 50x15</option>';
    $BarcodeTypeOpt .= '</select>';
    return $BarcodeTypeOpt;
}

function cek_kode_alt($kodealt = null){
	if ($_POST["kodealternatif"] OR !(is_null($kodealt))){
		if (is_null($kodealt)){
			$katacari = '%'.$_POST["kodealternatif"].'%';
		}else{
			$katacari = '%'.$kodealt.'%';
		}
		$result = db_query("SELECT COUNT(*) AS jumlahproduk FROM product WHERE alt_code LIKE '$katacari'");
		$data = db_fetch_object($result);
		if ($data->jumlahproduk){
			$jumlahkode = (($data->jumlahproduk)*1) + 1;
		}else{
			$jumlahkode = 1;
		}
		if (is_null($kodealt)){
			print $jumlahkode;
		}else{
			return $jumlahkode;
		}
	}
	exit();
}
function filter_kategori(){
	if ($_POST["idkategori"]){
		$result = db_query("SELECT idsubkategori,kodesubkategori,subkategori FROM subkategori WHERE idkategori='%d'",
		$_POST["idkategori"]);
		$tambahanpilihan = '';
		while($data = db_fetch_object($result)){
			$tambahanpilihan .= '<option value="'.$data->idsubkategori.'">'.$data->kodesubkategori.'-'.$data->subkategori.'</option>';	
		}
		print $tambahanpilihan;
	}
	exit();	
}

function tabel_produk($statusstok = null, $statusProduct = 1){
	get_number_format($currencySym, $thousandSep, $decimalSep);
	$tabelproduk = '<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_produk">';
	$tabelproduk .= '<thead>';
	$tabelproduk .= '<tr>';
	$tabelproduk .= '<th class="tablebutton"></th>';
	$tabelproduk .= '<th>KATEGORI</th>';
	$tabelproduk .= '<th>SUBKATEGORI</th>';
	$tabelproduk .= '<th>SUPPLIER</th>';
	$tabelproduk .= '<th>BARCODE</th>';
	$tabelproduk .= '<th>KOD ALT</th>';
	$tabelproduk .= '<th>NAMA</th>';
	$tabelproduk .= '<th>H.POKOK</th>';
	$tabelproduk .= '<th>H.JUAL</th>';
	$tabelproduk .= '<th>MARGIN</th>';
	$tabelproduk .= '<th>MIN</th>';
	$tabelproduk .= '<th>MAX</th>';
	$tabelproduk .= '<th>STOK</th>';
	$tabelproduk .= '<th>STATUS</th>';
	$tabelproduk .= '<th>SATUAN</th>';
	$tabelproduk .= '<th>KETERANGAN</th>';
	$tabelproduk .= '<th>TOTAL('.trim($currencySym).')</th>';
	$tabelproduk .= '<th>GRUP</th>';
    $tabelproduk .= '<th class="tablebutton"></th>';
    $tabelproduk .= '<th class="tablebutton"></th>';
    $tabelproduk .= '<th class="tablebutton"></th>';
	$tabelproduk .= '</tr>';
	$tabelproduk .= '</thead>';
	$tabelproduk .= '<tbody>';
	$tabelproduk .= '</tbody>';
	$totalNilaiBarang = calculateTotalNilaiBarang($statusstok, $statusProduct);
	$tabelproduk .= '<tfoot>';
	$tabelproduk .= '<tr>';
	$tabelproduk .= '<th class="right" colspan="16">TOTAL NILAI BARANG :</th>';
	$tabelproduk .= '<th class="right"><strong><div id="total-nilai-barang">'.$currencySym.' '.number_format($totalNilaiBarang,2,$decimalSep,$thousandSep).'</div></strong></th>';
	$tabelproduk .= '<th colspan="4"></th>';
	$tabelproduk .= '</tr>';
	$tabelproduk .= '</tfoot>';
	$tabelproduk .= '</table>';
	if ($_POST["asal"]){
		print $tabelproduk;
	}else{
		return $tabelproduk;
	}
}
function nama_field($field_id = null,$namafield_id = null,$nama_field = null,$namatabel = null){
	if ($field_id AND $namatabel AND $nama_field){
		$result = db_query("SELECT $nama_field AS namafield FROM $namatabel WHERE $namafield_id='$field_id'");
		$data = db_fetch_object($result);
		return $data->namafield;
	}else{
		return "-";
	}
}
function simpan_satuan(){
	if ($_POST["namasatuan"]){
		$result = db_query("SELECT COUNT(*) AS jumlahsama FROM satuan WHERE satuan='%s'",$_POST["namasatuan"]);
		$data = db_fetch_object($result);
		if (!($data->jumlahsama > 0)){
			db_query("INSERT INTO satuan VALUES ('%s')",$_POST["namasatuan"]);
			print $_POST["namasatuan"];
		}else{
			print "error";	
		}
	}
	exit();	
}
function cek_barcode(){
	if ($_POST["barcode"]){
		$result = db_query("SELECT COUNT(*) AS jumlahsama FROM product WHERE barcode='%s'",$_POST["barcode"]);
		$data = db_fetch_object($result);
		if ($data->jumlahsama > 0){
			print "sama";	
		}else{
			print "taksama";
		}	
	}	
	exit();
}
function cek_barcode2(){
	if ($_POST["barcode"] AND $_POST["idproduk"]){
		$BARCODE = $_POST["barcode"];
		$IDPRODUK = $_POST["idproduk"];
		$sql = "SELECT COUNT(*) AS jumlahsama FROM product WHERE barcode='$BARCODE' AND idproduct<>'$IDPRODUK'";
		$result = db_query("SELECT COUNT(*) AS jumlahsama FROM product WHERE barcode='%s' AND idproduct<>'%d'",$_POST["barcode"],$_POST["idproduk"]);
		$data = db_fetch_object($result);
		if ($data->jumlahsama > 0){
			print "sama";	
		}else{
			print "taksama";
		}	
	}
	//print $_POST["idproduk"]." ".$_POST["barcode"];
	exit();
}
function simpan_produk(){
	/*idsupplier,idkategori,idsubkategori,barcode,alt_code,namaproduct,hargapokok,
	hargajual,margin,minstok,maxstok,stok,satuan,berat,keterangan*/
	global $user;
    $dataPremis = get_data_premis(false);
    $DecDigit = $dataPremis->decimal_digit;
	$altCode = remove_utf8_bom($_POST["alt_code"]);
	if (isset($_POST["idproduk"])){
		print $_POST["idproduk"];
		if (isset($_POST["barcode"]) AND isset($_POST["namaproduk"])){
			$result = db_query("SELECT idkategori,idsubkategori,stok,hargapokok,alt_code FROM product WHERE idproduct='%d'",$_POST["idproduk"]);
			$data = db_fetch_object($result);
			$STOKSEBELUM = $data->stok;
			$HARGAPOKOK = $data->hargapokok;
            $Supplier = $_REQUEST["idsupplier"];
			if (!($data->idkategori == $_POST["idkategori"] AND $data->idsubkategori == $_POST["idsubkategori"])){
				$resultkategori = db_query("SELECT kodekategori FROM kategori WHERE idkategori='%d'",$_POST["idkategori"]);
				$datakategori = db_fetch_object($resultkategori);
				$resultsubkategori = db_query("SELECT kodesubkategori FROM subkategori WHERE idsubkategori='%d'",$_POST["idsubkategori"]);
				$datasubkategori = db_fetch_object($resultsubkategori);
				if (empty($data->alt_code)){
					$kodealt = $datakategori->kodekategori."-".$datasubkategori->kodesubkategori;
					if (isset($_POST["alt_code"])){
						$kodealternatif = $altCode;
					}else{
						$kodealternatif = $kodealt.cek_kode_alt(remove_utf8_bom($kodealt));
					}
				}else{
					$kodealternatif = $data->alt_code;
				}
				//$sql = "UPDATE product SET idsupplier='%d',idkategori='%d',idsubkategori='%d',barcode='%s',alt_code='%s',namaproduct='%s',hargapokok='%f',hargajual='%f',margin='%f',minstok='%f',maxstok='%f',stok='%f',satuan='%s',keterangan='%s' WHERE idproduct='%d'";
				//$nilai = $_POST["idsupplier"].'-'.$_POST["idkategori"].'-'.$_POST["idsubkategori"].'-'.$_POST["barcode"].'-'.$kodealternatif.'-'.$_POST["namaproduk"].'-'.$_POST["hargapokok"].'-'.$_POST["hargajual"].'-'.$_POST["margin"].'-'.$_POST["minstok"].'-'.$_POST["maxstok"].'-'.$_POST["stok"].'-'.$_POST["satuan"].'-'.$_POST["keterangan"].'-'.$_POST["idproduk"];
				db_query("UPDATE product SET idkategori='%d',idsubkategori='%d',
				barcode='%s',alt_code='%s',namaproduct='%s',hargapokok='%f',hargajual='%f',
				margin='%f',minstok='%f',maxstok='%f',stok='%f',satuan='%s',keterangan='%s',
				type_product='%d', lead_time='%d', aturan_jam_kerja='%d',
				berlaku_sebelum_zuhur='%d',uploaded=0, changed=1,barcode_type='%s'  WHERE idproduct='%d'",
				$_POST["idkategori"],$_POST["idsubkategori"],
				$_POST["barcode"],$kodealternatif,$_POST["namaproduk"],round($_POST["hargapokok"],$DecDigit),
				round($_POST["hargajual"],$DecDigit),$_POST["margin"],$_POST["minstok"],$_POST["maxstok"],
				$_POST["stok"],$_POST["satuan"],$_POST["keterangan"],$_POST["type_product"],
				$_POST["lead_time"],$_POST["jam_kerja"],$_POST["sebelum_zuhur"],$_POST["barcode_type"],
				$_POST["idproduk"]);
				//print $sql;
				if ($_POST["hargapokok"] <> $HARGAPOKOK){
					global $user;
					db_query("INSERT INTO historyhargabeli (hargasebelum, hargasesudah, uid) VALUES('%f','%f','%d')",
					$HARGAPOKOK,round($_POST["hargapokok"],$DecDigit),$user->uid);
				}
                db_query('DELETE FROM product_supplier WHERE idproduct=%d', $_POST["idproduk"]);
                for ($i = 0;$i < count($Supplier);$i++){
                    $IdSupplier = $Supplier[$i];
                    $HargaPokok = round($_POST["hargapokok"],$DecDigit);
                    $StrSql = "INSERT INTO product_supplier (idproduct, idsupplier, harga, uid) ";
                    $StrSql .= "VALUES (%d, %d, '%f', %d)";
                    db_query($StrSql, $_POST["idproduk"], $IdSupplier,$HargaPokok, $user->uid);
                }
			}else{
				db_query("UPDATE product SET barcode='%s',
				namaproduct='%s',hargapokok='%f',hargajual='%f',margin='%f',
				minstok='%f',maxstok='%f',stok='%f',satuan='%s',keterangan='%s',
				idkategori='%d', type_product='%d',lead_time='%d', aturan_jam_kerja='%d',
				berlaku_sebelum_zuhur='%d',uploaded=0, changed=1, barcode_type='%s'  WHERE idproduct='%d'",
				$_POST["barcode"],
				$_POST["namaproduk"],round($_POST["hargapokok"],$DecDigit),round($_POST["hargajual"],$DecDigit),
				$_POST["margin"],$_POST["minstok"],$_POST["maxstok"],$_POST["stok"],
				$_POST["satuan"],$_POST["keterangan"],$_POST["idkategori"],$_POST["type_product"],
				$_POST["lead_time"],$_POST["jam_kerja"],$_POST["sebelum_zuhur"],$_POST["barcode_type"],
				$_POST["idproduk"]);
				if ($_POST["hargapokok"] <> $HARGAPOKOK){
					global $user;
					db_query("INSERT INTO historyhargabeli (hargasebelum, hargasesudah, uid) VALUES('%f','%f','%d')",
					$HARGAPOKOK,$_POST["hargapokok"],$user->uid);
				}
                db_query('DELETE FROM product_supplier WHERE idproduct=%d', $_POST["idproduk"]);
                for ($i = 0;$i < count($Supplier);$i++){
                    $IdSupplier = $Supplier[$i];
                    $HargaPokok = $_POST["hargapokok"];
                    $StrSql = "INSERT INTO product_supplier (idproduct, idsupplier, harga, uid) ";
                    $StrSql .= "VALUES (%d, %d, '%f', %d)";
                    db_query($StrSql, $_POST["idproduk"], $IdSupplier,$HargaPokok, $user->uid);
                }
			}
			if ($_POST["type_product"] == 0) {
				$SELISIHSTOK = ($_POST["stok"] * 1) - $STOKSEBELUM;
				if ($SELISIHSTOK < 0) {
					$KELUAR = abs($SELISIHSTOK);
					$KETERANGAN = 'Perubahan Stock-Stok Berkurang';
					db_query(
						"INSERT INTO transaksistock (idproduk, stocksebelum, keluar, stocksetelah, keterangan) VALUES
					('%d', '%f', '%f', '%f', '%s')",
						$_POST["idproduk"],
						$STOKSEBELUM,
						$KELUAR,
						$_POST["stok"],
						$KETERANGAN
					);
				} else {
					$MASUK = $SELISIHSTOK;
					$KETERANGAN = 'Perubahan Stock-Stok Bertambah';
					db_query(
						"INSERT INTO transaksistock (idproduk, stocksebelum, masuk, stocksetelah, keterangan) VALUES
					('%d', '%f', '%f', '%f', '%s')",
						$_POST["idproduk"],
						$STOKSEBELUM,
						$MASUK,
						$_POST["stok"],
						$KETERANGAN
					);
				}
			}
		}
	}else{
		if (isset($_POST["barcode"]) AND isset($_POST["namaproduk"])){
            $Supplier = $_REQUEST["idsupplier"];
            db_query("INSERT INTO product(idkategori,idsubkategori,barcode,
			alt_code,namaproduct,hargapokok,hargajual,margin,minstok,maxstok,stok,satuan,
			keterangan,type_product,lead_time, aturan_jam_kerja,berlaku_sebelum_zuhur,barcode_type)
			VALUES(
			'%d','%d','%s','%s',
			'%s','%f','%f','%f','%d','%d','%d','%s','%s','%d','%d','%d','%d','%s')",
			$_POST["idkategori"],$_POST["idsubkategori"],
			$_POST["barcode"],$altCode,$_POST["namaproduk"],round($_POST["hargapokok"],$DecDigit),
			round($_POST["hargajual"],$DecDigit),$_POST["margin"],$_POST["minstok"],$_POST["maxstok"],
			$_POST["stok"],$_POST["satuan"],$_POST["keterangan"],$_POST["type_product"],
			$_POST["lead_time"],$_POST["jam_kerja"],$_POST["sebelum_zuhur"],$_POST["barcode_type"]);
			$result = db_query("SELECT idproduct FROM product WHERE barcode='%s'",$_POST["barcode"]);
			$data = db_fetch_object($result);
			$IDPRODUK = $data->idproduct;
            for ($i = 0;$i < count($Supplier);$i++){
                $IdSupplier = $Supplier[$i];
                $HargaPokok = round($_POST["hargapokok"],$DecDigit);
                $StrSql = "INSERT INTO product_supplier (idproduct, idsupplier, harga, uid) ";
                $StrSql .= "VALUES (%d, %d, '%f', %d)";
                db_query($StrSql, $IDPRODUK, $IdSupplier,$HargaPokok, $user->uid);
            }
			if ($_POST["type_product"] == 0) {
				$KETERANGAN = 'Inisialisasi Stock';
				db_query("INSERT INTO transaksistock (idproduk, stocksebelum,
				masuk, stocksetelah, keterangan) VALUES ('%d', '%f', '%f', '%f', '%s')",
				$IDPRODUK,0,$_POST["stok"],$_POST["stok"],$KETERANGAN
				);
			}
			if (trim($_POST['namafile']) != 'tanpafile'){
				$node = new stdClass();
				$node->title = $_POST["namaproduk"];
				$node->body = $_POST["keterangan"];
				$node->type = 'gambar_produk';
				$node->created = time();
				$node->changed = $node->created;
				$node->status = 1;          // Published?
				$node->promote = 0;       // Display on front page?
				$node->sticky = 0;          // Display top of page?
				$node->format = 0;         // Filtered HTML?
				$node->uid = $user->uid;	             //  Content owner uid (author)?
				$node->name = $user->name;
				$node->field_id_produk[0]['value'] = $IDPRODUK;
				$files_path = file_directory_path().'/';
				$file = $files_path.$_POST['namafile'];
				$details = stat($file);
				$filesize = $details['size'];
				$name = basename($file);
				$file_obj = new stdClass();
				$file_obj->filename = $name;
				$file_obj->filepath = $file;
				$file_obj->filemime = file_get_mimetype($name);
				$file_obj->filesize = $filesize;
				$file_obj->filesource = $name;
				// You can change this to the UID you want
				$file_obj->uid = $user->uid;
				$file_obj->status = FILE_STATUS_TEMPORARY;
				$file_obj->timestamp = time();
				$file_obj->list = 1;
				$file_obj->new = true;
				// Save file to files table
				drupal_write_record('files', $file_obj);
				file_set_status($file_obj,1);

				// Attach the file object to your node
				$node->field_gambar_file = array(
					array(
						'fid' => $file_obj->fid,
						'title' => $name,
						'filename' => $file_obj->filename,
						'filepath' => $file_obj->filepath,
						'filesize' => $file_obj->filesize,
						'mimetype' => file_get_mimetype($name),
						'description' => $name,
						'list' => 1,
					),
				);
				//$node->files[$file_obj->fid] = $file_obj;
				dpm($node);
				node_save($node);
			}
		}
	}
	exit();	
}

function update_subkategori(){
	$NILAI = $_POST['value'];
	$KOLOM = $_POST['kol_id'];
	$SUBKATEGORI_ID = $_POST['row_id'];
	$NILAITAMPIL = $NILAI;
	if ($KOLOM == "1"){
		$NILAI = strtoupper($NILAI);
		$sql_update = "UPDATE subkategori SET kodesubkategori='%s' WHERE idsubkategori='%d'";
	}elseif ($KOLOM == "2"){
		$sql_update = "UPDATE subkategori SET subkategori='%s' WHERE idsubkategori='%d'";
	}elseif ($KOLOM == "3"){
		$sql_update = "UPDATE subkategori SET keterangan='%s' WHERE idsubkategori='%d'";
	}elseif ($KOLOM == "0"){
		$sql_update = "UPDATE subkategori SET idkategori='%d' WHERE idsubkategori='%d'";
		$result = db_query("SELECT kodekategori,kategori FROM kategori WHERE idkategori='%d'",$NILAI);
		$data = db_fetch_object($result);
		$NILAITAMPIL = $data->kodekategori."-".$data->kategori;	
	}
	db_query($sql_update,$NILAI,$SUBKATEGORI_ID);
	echo $NILAITAMPIL;
	exit();	
}

function edit_data_produk(){
	if ($_GET["idproduk"]){
        get_number_format($currencySym, $tSep, $dSep);
        $dataPremis = get_data_premis(false);
        $DecDigit = $dataPremis->decimal_digit;
		$IDPRODUK = $_GET["idproduk"];
		$resultproduk = db_query("SELECT idsupplier, idkategori, idsubkategori, barcode, 
		alt_code, namaproduct, hargapokok, hargajual, margin, minstok, maxstok, stok, satuan,
		keterangan, type_product, lead_time,aturan_jam_kerja,berlaku_sebelum_zuhur,barcode_type FROM product
		WHERE idproduct='%d'",$IDPRODUK);
		$dataproduk = db_fetch_object($resultproduk);
        $path = drupal_get_path('theme', 'cti_flex');
        $form_style = $path.'/css/custom-style.css';
        drupal_add_css($form_style, 'theme', 'all', FALSE);
        $variables['styles'] = drupal_get_css();
        drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
        drupal_add_css('misc/media/jqueryValidate/css/validationEngine.jquery.css');
        drupal_add_css('misc/media/jqueryValidate/css/template.css');
        drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
        drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine-en.js');
        drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine.js');
        drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
		drupal_add_js('misc/media/jquery.autotab-1.1b.js');
        _add_chosen_jquery_plugins_old();
        _addJsRounding();
		$filePath = base_path().file_directory_path();
		$idSubKategori = $dataproduk->idsubkategori;
		drupal_add_js(
			array(
				'filePath' => $filePath,
				'idSubKategori' => $idSubKategori,
                'dec_digit' => $DecDigit,
			),
			'setting');
		$js_path = drupal_get_path('module','dataproduk').'/editproduct.js';
		drupal_add_js($js_path);
		//Pilihan Kategori
		$result = db_query("SELECT idkategori,kodekategori,kategori FROM kategori ORDER BY kodekategori,kategori");
		$pilihankategori = '<select id="idkategori" name="idkategori" onkeypress="filtersubkategori();" onchange="filtersubkategori();">';
		while ($data = db_fetch_object($result)){
			if ($data->idkategori == $dataproduk->idkategori){
				$pilihankategori .= '<option selected value="'.$data->idkategori.'">'.$data->kodekategori.'-'.$data->kategori.'</option>';
			}else{
				$pilihankategori .= '<option value="'.$data->idkategori.'">'.$data->kodekategori.'-'.$data->kategori.'</option>';
			}
		}
		$pilihankategori .= '</select>';
		$buttonadd = '<img id="addkategori" title="Tambah Kategori Baru" src="'.base_path().'misc/media/images/add-small.ico">';
		//Pilihan Sub Kategori
		$pilihansubkategori = '<select id="idsubkategori" name="idsubkategori" onkeypress="ubahkodebarang();" onchange="ubahkodebarang();">';
		$pilihansubkategori .= '</select>';
		$buttonadd2 = '<img id="addsubkategori" title="Tambah Subkategori Baru" src="'.base_path().'misc/media/images/add-small.ico">';
		//Pilihan Supplier
        $StrSQL = 'SELECT idsupplier FROM product_supplier WHERE idproduct=%d';
        $result = db_query($StrSQL, $IDPRODUK);
        $SelectedSupplier = array();
        while ($data = db_fetch_object($result)){
            $SelectedSupplier[] = $data->idsupplier;
        }
		$result = db_query("SELECT idsupplier,kodesupplier,namasupplier FROM supplier ORDER BY kodesupplier,namasupplier");
		$pilihansupplier = '<select id="idsupplier" name="idsupplier" multiple="true" >';
		while ($data = db_fetch_object($result)){
            if (in_array($data->idsupplier, $SelectedSupplier)){
				$pilihansupplier .= '<option selected value="'.$data->idsupplier.'">'.$data->kodesupplier.'-'.$data->namasupplier.'</option>';
			}else{
				$pilihansupplier .= '<option value="'.$data->idsupplier.'">'.$data->kodesupplier.'-'.$data->namasupplier.'</option>';
			}
		}
		$pilihansupplier .= '</select>';
		$buttonadd3 = '<img id="addsupplier" title="Tambah Supplier Baru" src="'.base_path().'misc/media/images/add-small.ico">';
		//Pilihan Satuan
		$result = db_query("SELECT satuan FROM satuan ORDER BY satuan");
		$pilihansatuan = '<select id="satuan" name="satuan" >';
		while ($data = db_fetch_object($result)){
			if ($data->satuan == $dataproduk->satuan){
				$pilihansatuan .= '<option selected value="'.$data->satuan.'">'.$data->satuan.'</option>';
			}else{
				$pilihansatuan .= '<option value="'.$data->satuan.'">'.$data->satuan.'</option>';
			}
		}
		$pilihansatuan .= '</select>';
		$buttonadd4 = '<img id="addsatuan" title="Tambah satuan Baru" src="'.base_path().'misc/media/images/add-small.ico">';
		//Pilihan Kategori untuk Dialog Box Tambah Sub kategori
		$result = db_query("SELECT idkategori,kodekategori,kategori FROM kategori ORDER BY kodekategori,kategori");
		$pilihankategori2 = '<select disabled="disabled" id="idkategori2" name="idkategori2">';
		while ($data = db_fetch_object($result)){
			$pilihankategori2 .= '<option value="'.$data->idkategori.'">'.$data->kodekategori.'-'.$data->kategori.'</option>';
		}
		$pilihankategori2 .= '</select>';
		//idsupplier,idkategori,idsubkategori,barcode,alt_code,namaproduct,hargapokok,hargajual,margin,minstok,maxstok,stok,satuan,berat,keterangan
		$editproduk = '<div id="formplace" class="produk"><form id="formproduk">';
        $PilihanTypeBarcode = '<select id="barcode_type" name="barcode_type">';
        if ( $dataproduk->barcode_type == 'ean13') {
            $PilihanTypeBarcode .= '<option value="ean13" selected>EAN13</option>';
            $PilihanTypeBarcode .= '<option value="code39">CODE39</option>';
        }else if ($dataproduk->barcode_type == 'code39'){
            $PilihanTypeBarcode .= '<option value="ean13">EAN13</option>';
            $PilihanTypeBarcode .= '<option value="code39" selected>CODE39</option>';
        }else{
            $PilihanTypeBarcode .= '<option value="ean13" selected>EAN13</option>';
            $PilihanTypeBarcode .= '<option value="code39">CODE39</option>';
        }
        $PilihanTypeBarcode .= '</select>';
		$dataproduk->barcode = empty($dataproduk->barcode) ? createEAN13Code(getRandomString(9)) : $dataproduk->barcode;
        $editproduk .= '<label>BARCODE TYPE</label>'.$PilihanTypeBarcode;
		$editproduk .= '<label>BARCODE</label><input type="text" class="validate[required,funcCall[cek_barcode]]" id="barcode" name="barcode" value="'.$dataproduk->barcode.'"><div id="validating"></div>';
		$editproduk .= '<input type="hidden" id="barcodelama" name="barcodelama" value="'.$dataproduk->barcode.'">';
		$editproduk .= '<input type="hidden" id="kodepoduk" name="kodeproduk" value="'.$dataproduk->alt_code.'">';
		$editproduk .= '<label>KATEGORI</label>'.$pilihankategori.$buttonadd;
		$editproduk .= '<label>SUBKATEGORI</label>'.$pilihansubkategori.$buttonadd2;
		$editproduk .= '<label>SUPPLIER</label>'.$pilihansupplier.$buttonadd3;
		$editproduk .= '<label>NAMA PRODUK</label><input type="text" class="validate[required]" id="namaproduk" name="namaproduk" value="'.$dataproduk->namaproduct.'">';
        $RadioLock = '<input id="rad-hargapokok" type="radio" name="lock-value" value="hargapokok">H.POKOK';
        $RadioLock .= '<input id="rad-hargajual" type="radio" name="lock-value" value="hargajual">H.JUAL';
        $RadioLock .= '<input id="rad-margin" type="radio" name="lock-value" value="margin" checked>MARGIN';
        $editproduk .= '<label>LOCK</label><div style="float: left;">'.$RadioLock.'</div>';
		$editproduk .= '<label>HARGA POKOK</label><input type="text" class="validate[required]" id="hargapokok" name="hargapokok" onkeyup="hitungmargin(\'hargapokok\')" value="'.$dataproduk->hargapokok.'">';
		$editproduk .= '<label>HARGA JUAL</label><input type="text" class="validate[required]" id="hargajual" name="hargajual" onkeyup="hitungmargin(\'hargajual\')" value="'.$dataproduk->hargajual.'">';
		$editproduk .= '<label>MARGIN HARGA</label><input type="text" maxlength="2" id="margin" name="margin" onkeyup="hitungmargin(\'margin\')" value="'.$dataproduk->margin.'" readonly="readonly">';
		$editproduk .= '<label>MINIMUM STOK</label><input type="text" id="minstok" name="minstok" value="'.$dataproduk->minstok.'">';
		$editproduk .= '<label>MAKSIMUM STOK</label><input type="text" id="maxstok" name="maxstok" value="'.$dataproduk->maxstok.'">';
		$editproduk .= '<label>STOK SAAT INI</label><input type="text" id="stok" name="stok" value="'.$dataproduk->stok.'">';
		$editproduk .= '<label>LEAD TIME (MENIT)</label><input type="text" id="lead_time" name="lead_time" value="'.$dataproduk->lead_time.'">';
		$selected_no = ($dataproduk->aturan_jam_kerja == 0) ? 'selected' : '';
		$selected_yes = ($dataproduk->aturan_jam_kerja == 1) ? 'selected' : '';
		$pilihanjamkerja = '<select id="ikut_jam_kerja" name="ikut_jam_kerja">';
		$pilihanjamkerja .= '<option value="0" '.$selected_no.'>Tidak</option>';
		$pilihanjamkerja .= '<option value="1" '.$selected_yes.'>Ya</option>';
		$pilihanjamkerja .= '</select>';
		$editproduk .= '<label>IKUT JAM KERJA</label>'.$pilihanjamkerja;
		$selected_no = ($dataproduk->berlaku_sebelum_zuhur == 0) ? 'selected' : '';
		$selected_yes = ($dataproduk->berlaku_sebelum_zuhur == 1) ? 'selected' : '';
		$pilihansebelumzuhur = '<select id="sebelum_zuhur" name="sebelum_zuhur">';
		$pilihansebelumzuhur .= '<option value="0" '.$selected_no.'>Tidak</option>';
		$pilihansebelumzuhur .= '<option value="1" '.$selected_yes.'>Ya</option>';
		$pilihansebelumzuhur .= '</select>';
		$editproduk .= '<label>BERLAKU SEBELUM JAM 12</label>'.$pilihansebelumzuhur;
		$editproduk .= '<label>SATUAN</label>'.$pilihansatuan.$buttonadd4;
		$editproduk .= '<label>KETERANGAN</label><input type="text" id="keteranganproduk" name="keteranganproduk" value="'.$dataproduk->keterangan.'">';
		$editproduk .= '<input type="hidden" id="idproduk" name="idproduk" value="'.$IDPRODUK.'"></form><div><button onclick="simpanproduk();" style="font-size:12px;border: 1px solid blue;">Update Produk</button><button onclick="batal();" style="font-size:12px;margin-left: 6px;border: 1px solid red;">Batal</button></div></div></div>';
		$editproduk .= '<div id="dialogtambahkategori" title="TAMBAH KATEGORI PRODUK">';
		$editproduk .= '<form id="tambahkategoriform"><div><label>KODE</label><input type="text" class="validate[required]" id="kodekategori" name="kodekategori"></div>';
		$editproduk .= '<div><label>KATEGORI</label><input class="validate[required]" type="text" id="kategori" name="kategori"></div>';
		$editproduk .= '<div><label>KETERANGAN</label><input type="text" id="keterangan" name="keterangan"></div></form>';
		$editproduk .= '<div><button id="tambahkategori" onclick="simpankategori();">Tambah Kategori</button></div>';
		$editproduk .= '</div>';
		$editproduk .= '<div id="dialogtambahsubkategori" title="TAMBAH SUBKATEGORI PRODUK">';
		$editproduk .= '<form id="tambahsubkategoriform"><div>';
		$editproduk .= '<div><label>KATEGORI</label>'.$pilihankategori2.'</div>';
		$editproduk .= '<div><label>KODE SUBKATEGORI</label><input type="text" class="validate[required]" id="kodesubkategori" name="kodesubkategori"></div>';
		$editproduk .= '<div><label>SUBKATEGORI</label><input type="text" class="validate[required]" id="subkategori" name="subkategori"></div>';
		$editproduk .= '<div><label>KETERANGAN</label><input type="text" id="keterangansub" name="keterangansub"></div></div></form>';
		$editproduk .= '<div><button id="tambahsubkategori" onclick="simpansubkategori();">Tambah Sub Kategori</button></div>';
		$editproduk .= '</div>';
		$editproduk .= '<div id="dialogtambahsupplier" title="TAMBAH SUPPLIER">';
		$editproduk .= '<form id="tambahsupplierform"><div><label>KODE SUPPLIER</label><input type="text" class="validate[required]" id="kodesupplier" name="kodesupplier"></div>';
		$editproduk .= '<div><label>NAMA SUPPLIER</label><input class="validate[required]" type="text" id="namasupplier" name="namasupplier"></div>';
		$editproduk .= '<div><label>TELP</label><input type="text" id="telp" name="telp"></div>';
		$editproduk .= '<div><label>ALAMAT</label><input type="text" id="alamat" name="alamat"></div>';
		$editproduk .= '<div><label>EMAIL</label><input type="text" id="email" name="email"></div>';
		$editproduk .= '</form>';
		$editproduk .= '<div><button id="tambahsupplier" onclick="simpansupplier();">Tambah Supplier</button></div>';
		$editproduk .= '<div id="dialogtambahsatuan" title="TAMBAH SATUAN">';
		$editproduk .= '<form id="tambahsatuanform"><div><label>SATUAN</label><input type="text" class=\"validate[required]\" id="namasatuan" name="namasatuan"></div>';
		$editproduk .= '</form>';
		$editproduk .= '<div><button id="tambahsatuan" onclick="simpansatuan();">Tambah Satuan</button></div>';
		$editproduk .= '</div>';
		return $editproduk;
	}else{
		drupal_goto("dataproduk/produk");
	}
}

function update_data_produk(){
    get_number_format($currencySym, $tSep, $dSep);
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	$nilai = $_POST['value'];
	$kolom = $_POST['kol_id'];
	$productId = $_POST['row_id'];
	$result = db_query("SELECT stok,hargapokok,hargajual FROM product WHERE idproduct='%d'",$productId);
	$data = db_fetch_object($result);
	$stokSebelum = $data->stok;
	$hargaPokok = $data->hargapokok;
	$hargaJual = $data->hargajual;
	$nilaiRet = $nilai;
	if ($kolom == "1") {
		$sql_update = "UPDATE product SET idkategori='%d' WHERE idproduct='%d'";
		$nilaiRet = db_result(db_query('SELECT kategori FROM kategori WHERE idkategori=%d', $nilai));
	}else if ($kolom == "2") {
		$sql_update = "UPDATE product SET idsubkategori='%d' WHERE idproduct='%d'";
		$nilaiRet = db_result(db_query('SELECT subkategori FROM subkategori WHERE idsubkategori=%d',$nilai));
	}else if ($kolom == "3"){
		$sql_update = "UPDATE product SET barcode='%s',uploaded=0, changed=1  WHERE idproduct='%d'";
	}else if ($kolom == "4"){
		$sql_update = "UPDATE product SET alt_code='%s',uploaded=0, changed=1  WHERE idproduct='%d'";
	}else if ($kolom == "5"){
		$sql_update = "UPDATE product SET namaproduct='%s',uploaded=0, changed=1  WHERE idproduct='%d'";
	}else if ($kolom == "6"){
		$sql_update = "UPDATE product SET hargapokok='%f',uploaded=0, changed=1  WHERE idproduct='%d'";
		$nilaiRet = number_format($nilai,$DecDigit,$dSep,$tSep);
		if ($nilai <> $hargaPokok){
			global $user;
			db_query("INSERT INTO historyhargabeli (hargasebelum, hargasesudah, uid) VALUES('%f','%f','%d')",
			$hargaPokok,$nilai,$user->uid);
		}
        $sql_update2 = "UPDATE product_supplier SET harga = '%f' WHERE idproduct='%d'";
		db_query($sql_update2, $nilai, $productId);
	}else if ($kolom == "7"){
		$sql_update = "UPDATE product SET hargajual='%f' WHERE idproduct='%d'";
		$nilaiRet = number_format($nilai,$DecDigit,$dSep,$tSep);
		if ($nilai <> $hargaJual){
			global $user;
			db_query("INSERT INTO historyhargajual (hargasebelum, hargasesudah, uid) VALUES('%f','%f','%d')",
			$hargaJual,$nilai,$user->uid);
		}
	}else if ($kolom == "8"){
		$sql_update = "UPDATE product SET stok='%f',uploaded=0, changed=1  WHERE idproduct='%d'";
		$nilaiRet = number_format($nilai,$DecDigit,$dSep,$tSep);
		$selisihStok = ($nilai*1)-$stokSebelum;
		if ($selisihStok < 0){
			$keluar = abs($selisihStok);
			$keterangan = 'Perubahan Stock-Stok Berkurang';
			db_query("INSERT INTO transaksistock (idproduk, stocksebelum, keluar, stocksetelah, keterangan) VALUES 
			('%d', '%f', '%f', '%f', '%s')",$productId,$stokSebelum,$keluar,$nilai,$keterangan);
		}else{
			$masuk = $selisihStok;
			$keterangan = 'Perubahan Stock-Stok Bertambah';
			db_query("INSERT INTO transaksistock (idproduk, stocksebelum, masuk, stocksetelah, keterangan) VALUES 
			('%d', '%f', '%f', '%f', '%s')",$productId,$stokSebelum,$masuk,$nilai,$keterangan);
		}
	}else if ($kolom == "11") {
        db_query('DELETE FROM product_supplier WHERE idproduct=%d',$productId);
        for ($i = 0;$i < count($nilai);$i++){
            $StrSql = "INSERT INTO product_supplier (idproduct, idsupplier, harga, uid) ";
            $StrSql .= "VALUES (%d, %d, '%f', %d)";
            db_query($StrSql, $productId, $nilai[$i],$hargaPokok,$user->uid);

        }
        $sql_update = null;
        $StrSQL = "SELECT GROUP_CONCAT(namasupplier SEPARATOR '<br/>') FROM product_supplier ";
        $StrSQL .= "AS ps LEFT JOIN supplier supp ON ps.idsupplier = supp.idsupplier ";
        $StrSQL .= "WHERE ps.idproduct = %d GROUP BY ps.idproduct";
        $nilaiRet = db_result(db_query($StrSQL,$productId));
    }else if ($kolom == "12") {
		$sql_update = "UPDATE product SET id_grup_menu='%d' WHERE idproduct='%d'";
		$nilaiRet = db_result(db_query('SELECT nama_grup FROM grup_menu WHERE id=%d', $nilai));
	}else if ($kolom == "10") {
        $sql_update = "UPDATE product SET satuan='%s' WHERE idproduct='%d'";
        $strSQL = "SELECT satuan FROM product WHERE idproduct=%d";
        $satuanAsal = db_result(db_query($strSQL,$productId));
        $konversiSatuan = get_konversi_satuan($nilai,$satuanAsal);
        $hargaJualBaru = $hargaJual*$konversiSatuan;
        $hargaPokokBaru = $hargaPokok*$konversiSatuan;
        $stokBaru = $stokSebelum/$konversiSatuan;
        $sqlUpdateStok = "UPDATE product SET stok='%f',hargajual='%f',hargapokok='%f' WHERE idproduct=%d";
        db_query($sqlUpdateStok,$stokBaru,$hargaJualBaru, $hargaPokokBaru,$productId);
        $selisihStok = $stokBaru - $stokSebelum;
        if ($selisihStok < 0){
            $keluar = abs($selisihStok);
            $keterangan = 'Perubahan Stock-Stok Berkurang';
            db_query("INSERT INTO transaksistock (idproduk, stocksebelum, keluar, stocksetelah, keterangan) VALUES 
			('%d', '%f', '%f', '%f', '%s')",$productId,$stokSebelum,$keluar,$stokBaru,$keterangan);
        }else{
            $masuk = $selisihStok;
            $keterangan = 'Perubahan Stock-Stok Bertambah';
            db_query("INSERT INTO transaksistock (idproduk, stocksebelum, masuk, stocksetelah, keterangan) VALUES 
			('%d', '%f', '%f', '%f', '%s')",$productId,$stokSebelum,$masuk,$stokBaru,$keterangan);
        }
        $nilaiRet = $nilai;
    }
    if (!empty($sql_update)){
        db_query($sql_update,$nilai,$productId);
    }
	echo $nilaiRet;
	exit();	
}
function ubah_status_product(){
	if (isset($_REQUEST['selectedproduct']) && isset($_POST['statusproduct'])){
        $selectedProduct = $_REQUEST['selectedproduct'];
        for ($i = 0;$i < count($selectedProduct);$i++){
            $idProduct = $selectedProduct[$i];
            db_query('UPDATE product SET status_product=%d,uploaded=0, changed=1  WHERE idproduct=%d', $_POST['statusproduct'],$idProduct);
        }
    }
    exit();
}
function ubah_supplier_price(){
    global $user;
    if (isset($_POST['idproduct']) && !empty($_POST['idproduct']) &&
        isset($_POST['dataHarga']) && !empty($_POST['dataHarga'])){
        $IdProduct = $_POST['idproduct'];
        $ExplodeData = explode('__', $_POST['dataHarga']);
        for ($i = 0;$i < count($ExplodeData);$i++){
            $PriceData = explode('_', $ExplodeData[$i]);
            $StrSql = 'SELECT COUNT(*) FROM product_supplier WHERE idproduct=%d AND idsupplier=%d';
            $ExplodeId = explode('-',$PriceData[0]);
            $IdSupplier = $ExplodeId[2];
            $HargaPokok = $PriceData[1];
            $PriceExists = db_result(db_query($StrSql, $IdProduct, $IdSupplier));
            if ($PriceExists){
                $StrSql = "UPDATE product_supplier SET harga='%f' WHERE idproduct=%d AND idsupplier=%d";
                db_query($StrSql, $HargaPokok, $IdProduct, $IdSupplier);
            }else{
                $StrSql = "INSERT INTO product_supplier (idproduct, idsupplier, harga, uid) ";
                $StrSql .= "VALUES (%d, %d, '%f', %d)";
                db_query($StrSql, $IdProduct, $IdSupplier,$HargaPokok, $user->uid);
            }
        }
    }
    exit();
}