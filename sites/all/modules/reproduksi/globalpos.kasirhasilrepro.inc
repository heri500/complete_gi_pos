<?php

function hasilreproduksi(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	createLogoImagePath();
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
	drupal_add_css('misc/media/themes/jquery-ui-1.8.11.custom.css');
	drupal_add_css('misc/media/css/validationEngine.jquery.css');
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	drupal_add_js('misc/media/jqClock.min.js');
	drupal_add_js('misc/media/jquery.validationEngine-en.js');
	drupal_add_js('misc/media/jquery.validationEngine.js');
	drupal_add_js('misc/media/jquery-ui-1.8.11.custom.min.js');
	drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
	drupal_add_js('misc/media/datatables/js/dataTables.addtr.js');
	drupal_add_js('misc/media/datatables/js/number.format.js');
	drupal_add_js('misc/media/jquery.autotab-1.1b.js');
	$tglsekarang = date("Y-m-d");
	$tgltampil = date("d-m-Y");
	drupal_add_js(
		array(
			'tglsekarang' => $tglsekarang,
			'tgltampil' => $tgltampil,
		),
		'setting');
	$path = drupal_get_path('module', 'reproduksi');
	$jspath = $path.'/js/kasirhasilrepro.js';
	drupal_add_js($jspath);
	$panduantombol = '<a style="font-weight: bold;text-align: center;">Ctrl+Shift+</a>';
	$tombolqty = '<a id="tombolubahqty" onclick="ubahqty()" class="buttonqty">F3:UBAH QTY</a>';
	$tombolhapus = '<a class="buttonhapus" onclick="hapus_latest_produk()">F4:HAPUS PRODUK</a>';
	$tombolselesai = '<a class="buttonselesai" onclick="kirim_data()">F5:SELESAI</a>';
	$tombolprint = '<a class="buttonprint" onclick="kirim_data()">F6:SELESAI&PRINT</a>';
	$hasilreproduksi = '<button onclick="munculkankasir();" style="font-size:12px;border: 1px solid orangered;">Klik untuk access form</button>';
	$hasilreproduksi .= '<div id="dialogkasir" style="font-size: 12px;" title="GLOBAL POS OUTPUT '.strtoupper(trans("packing ulang", $dataPremis->bahasa)).' ['.date("d-m-Y").']">';
	$hasilreproduksi .= '<div id="info-kasir-waktu">';
	$hasilreproduksi .= '<div id="tempattanggalkasir">';
	$hasilreproduksi .= '<div id="tempattanggal">';
	$hasilreproduksi .= '<label>'.strtoupper(trans('tanggal', $dataPremis->bahasa)).'</label><input readonly="readonly" type="text" id="tglhasilreproduksi" name="tglhasilreproduksi" value="'.date("d-m-Y").'">';
	$hasilreproduksi .= '<input type="hidden" id="tglhasilreproduksikirim" name="tglhasilreproduksikirim" value="'.date("Y-m-d").'">';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '<div id="tempatnamakasir">';
	global $user;
	$hasilreproduksi .= '<label>'.strtoupper(trans('kasir', $dataPremis->bahasa)).'</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$user->name.'">';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '<div id="tempatjam" align="center">';
	$hasilreproduksi .= 'Jam';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '<div id="tempatkosong">';
	$hasilreproduksi .= '<div id="tempatsupplier"><label>NO. NOTA</label><label>'.$_GET["nonota"].'</label></div>';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '<div id="barcode_place">SCAN ATAU INPUT BARCODE, KODE ATAU NAMA PRODUK : <input type="text" id="barcode" name="barcode" class="barcodekasir"></div>';
	$hasilreproduksi .= '<div id="tempattabelkasir">'.view_kasir().'</div>';
	$hasilreproduksi .= '<input type="hidden" id="last_id" name="last_id">';
	$hasilreproduksi .= '<input type="hidden" id="lastqty" name="lastqty" value="1">';
	$hasilreproduksi .= '<input type="hidden" id="lastharga" name="lastharga">';
	$hasilreproduksi .= '<input type="hidden" id="lastdiskon" name="lastdiskon">';
	$hasilreproduksi .= '<input type="hidden" id="idreproduksi" name="idreproduksi" value="'.$_GET["idreproduksi"].'">';
	$hasilreproduksi .= '<input type="hidden" id="nilaikirim" name="nilaikirim">';
	$hasilreproduksi .= '<div id="tempattombolkasir">';
	$hasilreproduksi .= $panduantombol;
	$hasilreproduksi .= $tombolqty;
	$hasilreproduksi .= $tombolhapus;
	$hasilreproduksi .= $tombolselesai;
	$hasilreproduksi .= $tombolprint;
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '<div id="tempattotal"><div id="totalbelanja">Total Hasil Packing Ulang : '.$currencySym.' 0</div></div>';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '<div title="Informasi" id="dialogwarning">';
	$hasilreproduksi .= '<p id="pesantext">Produk yang dicari tidak ada, Silahkan masukkan Barcode/Kode Produk/Nama Produk yang lain...!!!</p>';
	$hasilreproduksi .= '<p style="text-align: center"><button id="tutupdialog" class="buttonwarning" onclick="tutupwarning();">CLOSE</button></p>';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '<div title="UBAH QTY" id="dialogubahqty">';
	$hasilreproduksi .= '<label>Qty Baru : </label><input type="text" id="newqty" name="newqty" value="1">';
	$hasilreproduksi .= '</div>';
	$hasilreproduksi .= '<div title="UBAH QTY" id="dialogubahqty2">';
	$hasilreproduksi .= '<label>Qty Baru : </label><input type="text" id="newqty2" name="newqty2">';
	$hasilreproduksi .= '</div>';
	return $hasilreproduksi;
}

function simpan_hasilreproduksi(){
	//nonota, idpemakai, total, bayar, kembali
	if ($_POST["detail_produk"] AND $_POST["totalbelanja"]){
        $idreproduksi = $_POST['idreproduksi'];
		$next_id = db_result(db_query('SELECT COUNT(*) FROM hasilreproduksi')) + 1;
		$no_nota = buat_nota($next_id);
		date_default_timezone_set('Asia/Jakarta');
		$waktujual = $_POST["tglhasilreproduksi"]." ".date("H:i:s");
		global $user;
		db_query("INSERT INTO hasilreproduksi (idreproduksi, nonota, idpemakai, total, tglhasilreproduksi,ref_idreproduksi) 
		VALUES ('%d','%s', '%d', '%f', '%s','%d')",
            $next_id, $no_nota, $user->uid, $_POST["totalbelanja"], $waktujual,$idreproduksi);
		print $_POST["detail_produk"];
		$pecahdetail = explode("&",$_POST["detail_produk"]);
		$totalmodal = 0;
		foreach ($pecahdetail as $parameter){
			$pecahparameter = explode("=",$parameter);
			$pecahnilai = explode("___",remove_utf8_bom($pecahparameter[1]));
			$IDPRODUK =	$pecahnilai[0];
			$QTY = $pecahnilai[1];
			$HARGAJUAL = round($pecahnilai[2],2);
			$result = db_query("SELECT hargapokok,stok FROM product WHERE idproduct='%d'",$IDPRODUK);
			$data = db_fetch_object($result);
			$HARGAPOKOK = $data->hargapokok;
			$totalmodal = $totalmodal + ($HARGAPOKOK*$QTY);
			$STOKSEBELUM = $data->stok;
			//idreproduksi, idproduct, jumlah, hargapokok, hargajual
			db_query("INSERT INTO detailhasilreproduksi(idhasilreproduksi,idreproduksi, idproduct, jumlah, hargapokok, hargajual) VALUES
			('%d','%d', '%d', '%f', '%f', '%f')",
                $next_id, $idreproduksi,$IDPRODUK,$QTY,$HARGAPOKOK,$HARGAJUAL);
			$STOKSEKARANG = $STOKSEBELUM + $QTY;
			db_query("UPDATE product SET stok='%f' WHERE idproduct='%d'",$STOKSEKARANG,$IDPRODUK);
			$KETERANGAN = 'Hasil Packing Ulang';
			db_query("INSERT INTO transaksistock (idproduk, idreproduksi, stocksebelum, masuk, stocksetelah, keterangan) VALUES 
			('%d', '%d', '%f', '%f', '%f', '%s')",$IDPRODUK,$next_id,$STOKSEBELUM,$QTY,$STOKSEKARANG,$KETERANGAN);
		}
		db_query("UPDATE hasilreproduksi SET totalmodal='%f' WHERE idreproduksi='%d'",$totalmodal,$next_id);
	}else{
		print 'error';	
	}
	exit();
}
function buat_nota($idreproduksi){
	if ($idreproduksi > 0 AND $idreproduksi < 10){
		$no_nota = "HR000000".$idreproduksi;	
	}elseif ($idreproduksi >= 10 AND $idreproduksi < 100){
		$no_nota = "HR00000".$idreproduksi;	
	}elseif ($idreproduksi >= 100 AND $idreproduksi < 1000){
		$no_nota = "HR0000".$idreproduksi;	
	}elseif ($idreproduksi >= 1000 AND $idreproduksi < 10000){
		$no_nota = "HR000".$idreproduksi;	
	}elseif ($idreproduksi >= 10000 AND $idreproduksi < 100000){
		$no_nota = "HR00".$idreproduksi;	
	}elseif ($idreproduksi >= 100000 AND $idreproduksi < 1000000){
		$no_nota = "HR0".$idreproduksi;	
	}elseif ($idreproduksi >= 1000000){
		$no_nota = "HR".$idreproduksi;	
	}
	return $no_nota;
}
function cari_produk(){
	if ($_POST["katacari"]){
		$KATACARI = '%'.$_POST["katacari"].'%';
		$sql = "SELECT idproduct,idkategori,namaproduct,hargajual FROM product WHERE alt_code LIKE '$KATACARI' OR barcode LIKE '$KATACARI' OR namaproduct LIKE '$KATACARI'";
		$result = db_query("SELECT idproduct,idkategori,namaproduct,hargajual FROM product WHERE alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s'",$KATACARI,$KATACARI,$KATACARI);
		$data = db_fetch_object($result);
		if ($data->idproduct){
			$result2 = db_query("SELECT besardiskon FROM diskonkategori WHERE idpelanggan='%d' AND idkategori='%d'",
			$_POST["idpelanggan"],$data->idkategori);
			$datadiskon = db_fetch_object($result2);
			if ($datadiskon->besardiskon >=0 ){
				$DISKON = $datadiskon->besardiskon;
			}else{
				$DISKON = 0;
			}
			if ($_POST["idpelanggan"] == 0){
				$DISKON = 0;
			}
			print $data->idproduct.';'.$data->namaproduct.';'.$data->hargajual.';'.$DISKON;
		}else{
			print "error";
		}
	}
	exit();	
}
function cari_barang(){
	if ($_GET["term"]){
		$KATACARI = '%'.$_GET["term"].'%';
		$result = db_query("SELECT barcode, namaproduct FROM product WHERE alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s'",$KATACARI,$KATACARI,$KATACARI);
		$items = array();
		while ($data = db_fetch_object($result)){
			$items[] = array('value' => $data->namaproduct,'barcode' => $data->barcode,'alt_code' => $data->alt_code);
		}
		print json_encode($items);
	}
	exit();	
}
function view_kasir(){
	$kasirtabel ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_kasir">';
	$kasirtabel .= '<thead>';
	$kasirtabel .= '<tr>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th>PRODUK</th>';
	$kasirtabel .= '<th>HARGA</th>';
	$kasirtabel .= '<th class="diskon2">QTY</th>';
	$kasirtabel .= '<th class="subtotal">SUBTOTAL</th>';
	$kasirtabel .= '</tr>';
	$kasirtabel .= '</thead>';
	$kasirtabel .= '<tbody>';
	$kasirtabel .= '</tbody>';
	$kasirtabel .= '</table>';
	return $kasirtabel;
}