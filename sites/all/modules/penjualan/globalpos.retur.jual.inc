<?php

function return_penjualan(){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
	$logoPath = base_path().file_directory_path().'/cti_flex_logo.png';
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
    _add_chosen_jquery_plugins_old();
    _addJeditablePlugins();
	$tglsekarang = date("Y-m-d");
	$tgltampil = date("d-m-Y");
	drupal_add_js(
		array(
			'tglsekarang' => $tglsekarang,
			'tgltampil' => $tgltampil,
			'logo' => $logoPath,
            'dec_digit' => $DecDigit,
		), 
	'setting');
	$js_path = drupal_get_path('module','penjualan').'/js/return.form.js';
	drupal_add_js($js_path);
	if (isset($_GET['tanggal'])){
		$tanggal = $_GET['tanggal'];
		$tanggal = date('d-m-Y', strtotime($tanggal));
		$tanggalkirim = date('Y-m-d', strtotime($tanggal));
	}else{
		$tanggal = date('d-m-Y');
		$tanggalkirim = date('Y-m-d');
	}
	$panduantombol = '<a style="font-weight: bold;text-align: center;font-size:12px;margin:0;padding: .4em .3em;">Ctrl+Shift+</a>';
	$tombolcari = '<a id="tombolubahharga" class="cari" onclick="ubahharga()">F2:UBAH HARGA</a>';
	$tombolqty = '<a id="tombolubahqty" onclick="ubahqty()" class="buttonqty">F3:UBAH QTY</a>';
	$tombolhapus = '<a class="buttonhapus" onclick="hapus_latest_produk()">F4:HAPUS ITEM</a>';
	$tombolselesai = '<a class="buttonselesai" onclick="kirim_data(1)">F5:BAYAR&PRINT</a>';
	$tombolprint = '<a class="buttonprint" onclick="kirim_data(0)">F6:BAYAR</a>';
	$tombollihatpelanggan = '<a class="buttonpelanggan" onclick="pilih_pelanggan();">F9:PELANGGAN</a>';
	$return = '<button onclick="munculkankasir();" style="font-size:12px;border: 1px solid orangered;">Klik untuk memunculkan kasir</button>';
	$return .= '<div id="dialogkasir" style="font-size: 12px;" title="GLOBAL POS KASIR ['.$tanggal.']">';
	$return .= '<div id="info-kasir-waktu">';
	$return .= '<div id="tempattanggalkasir">';
	$return .= '<div id="tempattanggal">';
	$return .= '<label>TANGGAL</label><input readonly="readonly" type="text" id="tgljual" name="tgljual" value="'.$tanggal.'">';
	$return .= '<input type="hidden" id="tgljualkirim" name="tgljualkirim" value="'.$tanggalkirim.'">';
	$return .= '</div>';
	$return .= '<div id="tempatnamakasir">';
	global $user;
	$return .= '<label>KASIR</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$user->name.'">';
	$return .= '</div>';
	$return .= '</div>';
	$return .= '<div id="tempatjam" align="center">';
	$return .= 'Jam';
	$return .= '</div>';
	$return .= '<div id="tempatkosong">';
	$return .= '<div id="tempatsupplier"><label>PELANGGAN</label></div>';
	$return .= '<div id="tempatsupplier2">'.createPelangganSelection($_GET['idpelanggan']).'</div>';
	$return .= '</div>';
	$return .= '</div>';
	$return .= '<div id="barcode_place">SCAN ATAU INPUT BARCODE, KODE ATAU NAMA PRODUK : <input type="text" id="barcode" name="barcode" class="barcodekasir"></div>';
	$tablekasir = view_kasir();
	$return .= '<div id="tempattabelkasir">'.$tablekasir.'</div>';
	$return .= '<input type="hidden" id="last_id" name="last_id">';
	$return .= '<input type="hidden" id="lastqty" name="lastqty" value="1">';
	$return .= '<input type="hidden" id="lastharga" name="lastharga">';
	$return .= '<input type="hidden" id="lastdiskon" name="lastdiskon">';
	$return .= '<input type="hidden" id="nilaikirim" name="nilaikirim">';
	$return .= '<div id="tempattombolkasir">';
	$return .= $panduantombol;
	$return .= $tombolcari;
	$return .= $tombolqty;
	$return .= $tombolhapus;
	$return .= $tombolselesai;
	$return .= $tombolprint;
	$return .= $tombollihatpelanggan;
	$return .= '</div>';
    $return .= '<div id="tempattotal"><div id="totalbelanja">Total Return : Rp. 0</div></div>';
	$return .= '</div>';
	$return .= '<div title="Informasi" id="dialogwarning">';
	$return .= '<p id="pesantext">Produk yang dicari tidak ada, Silahkan masukkan Barcode/Kode Produk/Nama Produk yang lain...!!!</p>';
	$return .= '<p style="text-align: center"><button id="tutupdialog" class="buttonwarning" onclick="tutupwarning();">CLOSE</button></p>';
	$return .= '</div>';
	$return .= '<div title="UBAH HARGA" id="dialogubahharga">';
	$return .= '<label>Harga Baru : </label><input type="text" id="newharga" name="newharga" value="1">';
	$return .= '</div>';
    $return .= '<div title="UBAH HARGA" id="dialogubahharga2">';
    $return .= '<label>Harga Baru : </label><input type="text" id="newharga2" name="newharga2" value="1">';
    $return .= '</div>';
	$return .= '<div title="UBAH QTY" id="dialogubahqty">';
	$return .= '<label>Qty Baru : </label><input type="text" id="newqty" name="newqty" value="1">';
	$return .= '</div>';
	$return .= '<div title="UBAH QTY" id="dialogubahqty2">';
	$return .= '<label>Qty Baru : </label><input type="text" id="newqty2" name="newqty2">';
	$return .= '</div>';
	$return .= '<div title="PEMBAYARAN" id="dialogbayar">';
	$carareturn = '<select id="carabayar" name="carabayar">';
    $result = db_query("SELECT id, carareturn FROM carareturn");
    $i = 0;
    while($data = db_fetch_object($result)){
        if ($i == 0){
            $carareturn .= '<option selected="selected" value="'.$data->id.'">'.$data->carareturn.'</option>';
        }else{
            $carareturn .= '<option value="'.$data->id.'">'.$data->carareturn.'</option>';
        }
        $i++;
    }
    $carareturn .= '</select>';
	$nilaibayar = '<input type="text" id="nilaibayar" name="nilaibayar">';
	$depositpelanggan = '<input type="text" id="depositpelanggan" name="depositpelanggan" readonly="readonly">';
	$kembali = '<input readonly="readonly" type="text" id="kembali" name="kembali">';
	$totalbelanja = '<input readonly="readonly" type="text" id="totalbelanjauser" name="totalbelanjauser">';
	$return .= '<div class="barisbayar"><label>Total Belanja</label>'.$totalbelanja.'</div>';
	$return .= '<div class="barisbayar"><label>Cara Bayar</label>'.$carareturn.'</div>';
	$return .= '<div class="barisbayar"><label id="label-deposit">Deposit</label>'.$depositpelanggan.'</div>';
	$return .= '<div id="cashback" class="barisbayar"><label>Cash Back</label>'.$nilaibayar.'</div>';
    $return .= '<div id="bariskembali" class="barisbayar"><label>Kembali</label>'.$kembali.'</div>';
	$return .= '</div>';
	return $return;
}

function simpan_return_penjualan(){
	//nonota, idpemakai, total, bayar, kembali
	if ($_POST["detail_produk"] AND $_POST["bayar"] >= 0 AND $_POST["totalbelanja"]){
		global $user;
		//date_default_timezone_set('Asia/Jakarta');
		$waktujual = $_POST["tglreturn"]." ".date("H:i:s");
		$splitTanggal = explode('-', $_POST["tglreturn"]);
		$splitJam = explode(':',date("H:i:s"));
		$intTanggal = mktime($splitJam[0],$splitJam[1],$splitJam[2],$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
		$result = db_query("SELECT idreturn FROM returnpelanggan ORDER BY idreturn DESC LIMIT 1");
		$data = db_fetch_object($result);
		if ($data->idreturn > 0){
			$next_id = $data->idreturn + 1;
		}else{
			$next_id = 1;
		}
		$no_nota = buat_nota_return($next_id);
		db_query("INSERT INTO returnpelanggan (nonota, idpemakai, total, tglreturn, idpelanggan, carareturn) 
		VALUES ('%s', '%d', '%f', '%s', '%d', '%d')",
		$no_nota, $user->uid, $_POST["totalbelanja"], $waktujual,
		$_POST["idpelanggan"],$_POST["carareturn"]);
		$next_id = db_last_insert_id('returnpelanggan', 'idreturn');
		print $next_id;
		if ($_POST["carareturn"] == 2){
			if (isset($_POST["idpelanggan"]) && !empty($_POST["idpelanggan"])){
				$variables['idpelanggan'] = $_POST["idpelanggan"];
				$variables['hutang'] = getTotalHutang($_POST["idpelanggan"]);
				$variables['pembayaran'] = $_POST["totalbelanja"];
				$variables['tglbayar'] = $waktujual;
				$variables['idreturn'] = $next_id;
				$variables['keterangan'] = 'Potong Hutang dari Nota Return : '.$no_nota.' Tanggal : '.$waktujual;
				pembayaranhutang($variables);	
			}
		}
		$pecahdetail = explode("&",$_POST["detail_produk"]);
		$totalmodal = 0;
		foreach ($pecahdetail as $parameter){
			$pecahparameter = explode("=",$parameter);
			$pecahnilai = explode("___",$pecahparameter[1]);
			$IDPRODUK =	$pecahnilai[0];
			$QTY = $pecahnilai[1];
			$HARGAJUAL = $pecahnilai[2];
			$DISKON = $pecahnilai[3];
			$PPN = $pecahnilai[5];
			$result = db_query("SELECT type_product, hargapokok,stok FROM product WHERE idproduct='%d'",$IDPRODUK);	
			$data = db_fetch_object($result);
			$HARGAPOKOK = $data->hargapokok;
			$totalmodal = $totalmodal + ($HARGAPOKOK*$QTY);
			$STOKSEBELUM = $data->stok;
			db_query("INSERT INTO detailreturnpelanggan(idreturn, idproduct, jumlah, hargapokok, hargajual, diskon, ppn) VALUES
			('%d', '%d', '%f', '%f', '%f', '%f', '%f')",
			$next_id,$IDPRODUK,$QTY,$HARGAPOKOK,$HARGAJUAL,$DISKON,$PPN);
			if ($data->type_product == 0){
				$STOKSEKARANG = $STOKSEBELUM + $QTY;
				db_query("UPDATE product SET stok='%f' WHERE idproduct='%d'",$STOKSEKARANG,$IDPRODUK);
				$KETERANGAN = 'Return Pelanggan No. Return '.$no_nota.' Tanggal : '.$waktujual;
				db_query("INSERT INTO transaksistock (idproduk, idreturn, stocksebelum, keluar, stocksetelah, keterangan) VALUES 
				('%d', '%d', '%f', '%f', '%f', '%s')",$IDPRODUK,$next_id,$STOKSEBELUM,$QTY,$STOKSEKARANG,$KETERANGAN);
			}
		}
	}else{
		print 'error';	
	}
	exit();
}
function buat_nota_return($idreturn){
	if ($idreturn > 0 AND $idreturn < 10){
		$no_nota = "RJ000000".$idreturn;
	}elseif ($idreturn >= 10 AND $idreturn < 100){
		$no_nota = "RJ00000".$idreturn;
	}elseif ($idreturn >= 100 AND $idreturn < 1000){
		$no_nota = "RJ0000".$idreturn;
	}elseif ($idreturn >= 1000 AND $idreturn < 10000){
		$no_nota = "RJ000".$idreturn;
	}elseif ($idreturn >= 10000 AND $idreturn < 100000){
		$no_nota = "RJ00".$idreturn;
	}elseif ($idreturn >= 100000 AND $idreturn < 1000000){
		$no_nota = "RJ0".$idreturn;
	}elseif ($idreturn >= 1000000){
		$no_nota = "RJ".$idreturn;
	}
	return $no_nota;
}
function view_kasir(){
	$kasirtabel ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_kasir">';
	$kasirtabel .= '<thead>';
	$kasirtabel .= '<tr>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th>PRODUK</th>';
	$kasirtabel .= '<th>HARGA</th>';
	$kasirtabel .= '<th class="diskon2">%</th>';
    $kasirtabel .= '<th class="diskon2">PPN</th>';
	$kasirtabel .= '<th class="diskon2">QTY</th>';
	$kasirtabel .= '<th class="subtotal">SUBTOTAL</th>';
    $kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '</tr>';
	$kasirtabel .= '</thead>';
	$kasirtabel .= '<tbody>';
	$kasirtabel .= '</tbody>';
	$kasirtabel .= '</table>';
	return $kasirtabel;
}