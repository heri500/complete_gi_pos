<?php

/**
 * @param null $idtitipanlaundry
 * @param null $alamatasal
 * @return string
 */
function penjualan($idtitipanlaundry = null, $alamatasal = null){
    if (isset($_POST['idcustomerorder-select'])){
        $idtitipanlaundry = $_POST['idcustomerorder-select'];
        $alamatasal = 'viewcustomerorder';
    }
	get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols(false);
	$dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
	$logoPath = base_path().file_directory_path().'/cti_flex_logo.png';
	$Title = 'GLOBAL POS '.strtoupper(trans('kasir', $dataPremis->bahasa));
	drupal_set_title($Title);
	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
	drupal_add_css('misc/media/themes/jquery-ui-1.8.11.custom.css');
	drupal_add_css('misc/media/css/validationEngine.jquery.css');
	//drupal_add_js('misc/media/jquery-1.4.4.min.js');
    _addJqueryPlugins();
    drupal_add_js('misc/media/jqClock.min.js');
	drupal_add_js('misc/media/jquery.validationEngine-en.js');
	drupal_add_js('misc/media/jquery.validationEngine.js');
	drupal_add_js('misc/media/jquery-ui-1.8.11.custom.min.js');
	drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
	drupal_add_js('misc/media/datatables/js/dataTables.addtr.js');
	drupal_add_js('misc/media/datatables/js/number.format.js');
	drupal_add_js('misc/media/jquery.autotab-1.1b.js');
    _addBlockUiPlugins(true);
    _add_chosen_jquery_plugins_old();
    _addJeditablePlugins();
    _addPreventBackPage();
	$idPelanggan = 0;
	if (!empty($idtitipanlaundry)){
		if (empty($alamatasal)){
			$alamatasal = 'viewlaundry';
		}
		drupal_add_js(
			array(
				'idtitipanlaundry' => $idtitipanlaundry,
				'alamatasal' => $alamatasal,
			), 
		'setting');
		if ($alamatasal == 'viewlaundry' || $alamatasal == 'laundrykeluar'){
			$titipanlaundry = db_fetch_array(db_query("SELECT idtitipanlaundry, nonota, tglpenjualan, idpemakai,
			idpelanggan, total, totalmodal, carabayar, bayar, kembali, nokartu, keterangan, insert_date, status_laundry,
			users.name,laundry.nomer_rak FROM titipanlaundry laundry LEFT JOIN cms_users users ON laundry.idpemakai = users.uid
			WHERE idtitipanlaundry=%d",$idtitipanlaundry));
			if (count($titipanlaundry)){
				$tgllaundry = explode(' ', $titipanlaundry['tglpenjualan']);
				$splittgllaundry = explode('-',$tgllaundry[0]);
				$tglTampilLaundry = $splittgllaundry[2].'-'.$splittgllaundry[1].'-'.$splittgllaundry[0];
				$idPelanggan = $titipanlaundry['idpelanggan'];
				$kasirlaundry = $titipanlaundry['name'];
				$nomerRak = $titipanlaundry['nomer_rak'];
				$labelKasir = 'LAUNDRY';
				$labelTgl = 'MASUK';
			}
		}else if($alamatasal == 'viewcustomerorder' || $alamatasal == 'orderkeluar'){
            if (!is_array($idtitipanlaundry)) {
                $titipanlaundry = db_fetch_array(db_query("SELECT custord.id, nonota, tglorder, idpemakai,idmeja,
			    idpelanggan, total, totalmodal, carabayar, bayar, kembali, nokartu, custord.keterangan, insert_date, status_order,
			    users.name,m.meja AS namameja FROM customer_order custord 
			    LEFT JOIN cms_users users ON custord.idpemakai = users.uid
			    LEFT JOIN meja m ON custord.idmeja = m.id 
			    WHERE custord.id=%d", $idtitipanlaundry));
                if (count($titipanlaundry)) {
                    $tgllaundry = explode(' ', $titipanlaundry['tglorder']);
                    $splittgllaundry = explode('-', $tgllaundry[0]);
                    $tglTampilLaundry = $splittgllaundry[2] . '-' . $splittgllaundry[1] . '-' . $splittgllaundry[0];
                    $idPelanggan = $titipanlaundry['idpelanggan'];
                    $kasirlaundry = $titipanlaundry['name'];
                    $idMeja = $titipanlaundry['idmeja'];
                    $namaMeja = $titipanlaundry['namameja'];
                    $labelKasir = 'ORDER';
                    $labelTgl = 'ORDER';
                    drupal_add_js(
                        array(
                            'idmeja' => $idMeja,
                            'namaMeja' => $namaMeja,
                        ),
                        'setting');
                }
            }else{
                $tglTampilLaundry = '';
                $kasirlaundry = '';
                $idMeja = array();
                $namaMeja = array();
                $labelKasir = 'ORDER';
                $labelTgl = 'ORDER';
                $ArrayTgl = array();
                $ArrayCashier = array();
                for ($i = 0;$i < count($idtitipanlaundry);$i++){
                    $titipanlaundry = db_fetch_array(
                        db_query(
                            "SELECT custord.id, nonota, tglorder, idpemakai,idmeja,
			                idpelanggan, total, totalmodal, carabayar, bayar, kembali, nokartu, custord.keterangan, 
                            insert_date, status_order, users.name,m.meja AS namameja FROM customer_order custord 
			                LEFT JOIN cms_users users ON custord.idpemakai = users.uid
			                LEFT JOIN meja m ON custord.idmeja = m.id 
			                WHERE custord.id=%d", $idtitipanlaundry[$i]
                        )
                    );
                    if (count($titipanlaundry)) {
                        $idMeja[] = $titipanlaundry['idmeja'];
                        $namaMeja[] = $titipanlaundry['namameja'];
                        $idPelanggan = $titipanlaundry['idpelanggan'];
                        $tgllaundry = explode(' ', $titipanlaundry['tglorder']);
                        if (!in_array($tgllaundry[0],$ArrayTgl)){
                            $ArrayTgl[] = $tgllaundry[0];
                            $splittgllaundry = explode('-', $tgllaundry[0]);
                            if ($tglTampilLaundry == ''){
                                $tglTampilLaundry = $splittgllaundry[2] . '-' . $splittgllaundry[1] . '-' . $splittgllaundry[0];
                            }else{
                                $tglTampilLaundry .= ', '.$splittgllaundry[2] . '-' . $splittgllaundry[1] . '-' . $splittgllaundry[0];
                            }
                        }
                        if (!in_array($titipanlaundry['name'],$ArrayCashier)){
                            $ArrayCashier[] = $titipanlaundry['name'];
                            if ($kasirlaundry == ''){
                                $kasirlaundry = $titipanlaundry['name'];
                            }else{
                                $kasirlaundry .= ', '.$titipanlaundry['name'];
                            }
                        }
                    }
                }
                drupal_add_js(
                    array(
                        'idmeja' => $idMeja,
                        'namaMeja' => $namaMeja,
                    ),
                    'setting');
            }

		}
	}
	$uploadData = false;
	if (isset($_GET['afterinsert']) && $_GET['afterinsert']){
		$uploadData = true;
	}
	$tglsekarang = date("Y-m-d");
	$tgltampil = date("d-m-Y");
    $DecDigitCashier = variable_get('decimal_digit_cashier',0);
	drupal_add_js(
		array(
			'tglsekarang' => $tglsekarang,
			'tgltampil' => $tgltampil,
			'logo' => $logoPath,
			'upload_data' => $uploadData,
            'dec_digit' => $DecDigit,
            'dec_digit_cashier' => $DecDigitCashier,
		),
	'setting');
	$js_path = drupal_get_path('module','penjualan').'/js/kasir.form.new.js';
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
	//$tomboltutupkasir = '<a class="buttontutup">F7:TUTUP KASIR</a>';
	$tombollihatomset = '<a class="buttonomset" onclick="hitung_omset();">F8:TOTAL</a>';
    $tombollihatpelanggan = '<a class="buttonpelanggan" onclick="pilih_pelanggan();">F9:PELANGGAN</a>';
	$penjualan = '<button onclick="munculkankasir();" style="font-size:12px;border: 1px solid orangered;">Klik untuk memunculkan '.trans('kasir', $dataPremis->bahasa).'</button>';
    $penjualan .= '&nbsp;&nbsp;<button onclick="close_kasir();" style="font-size:12px;border: 1px solid orangered;">'.strtoupper(trans('tutup', $dataPremis->bahasa)).' '.strtoupper(trans('kasir', $dataPremis->bahasa)).'</button>';
	$penjualan .= '<div id="dialogkasir" style="font-size: 12px;" title="GLOBAL POS '.strtoupper(trans('kasir', $dataPremis->bahasa)).' ['.$tanggal.']">';
	$penjualan .= '<div id="info-kasir-waktu">';
	$penjualan .= '<div id="tempattanggalkasir">';
	$penjualan .= '<div id="tempattanggal">';
	$penjualan .= '<label>'.strtoupper(trans('tanggal', $dataPremis->bahasa)).'</label><input readonly="readonly" type="text" id="tgljual" name="tgljual" value="'.$tanggal.'">';
	$penjualan .= '<input type="hidden" id="tgljualkirim" name="tgljualkirim" value="'.$tanggalkirim.'">';
	$penjualan .= '</div>';
	$penjualan .= '<div id="tempatnamakasir">';
	global $user;
	$penjualan .= '<label>'.strtoupper(trans('kasir', $dataPremis->bahasa)).'</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$user->name.'">';
	$penjualan .= '</div>';
	if (!empty($idtitipanlaundry)){
		$penjualan .= '<div id="tempattanggal">';
		$penjualan .= '<label>'.strtoupper(trans('kasir', $dataPremis->bahasa)).' '.$labelKasir.'</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$kasirlaundry.'">';
		$penjualan .= '</div>';
	}
	$penjualan .= '</div>';
	$penjualan .= '<div id="tempatjam" align="center">';
	$penjualan .= 'Jam';
	$penjualan .= '</div>';
	$penjualan .= '<div id="tempatkosong">';
	$penjualan .= '<div id="tempatsupplier"><label>PELANGGAN</label></div>';
	$penjualan .= '<div id="tempatsupplier2">'.createPelangganSelection($idPelanggan).'</div>';
	if (!empty($idtitipanlaundry)){
		$penjualan .= '<div id="tempatsupplier"><label>'.strtoupper(trans('tanggal', $dataPremis->bahasa)).' '.$labelTgl.'</label><input readonly="readonly" type="text" id="tglmasuk" name="tglmasuk" value="'.$tglTampilLaundry.'">';
		$penjualan .= '</div>';
	}
	$penjualan .= '</div>';
	$penjualan .= '</div>';
	$penjualan .= '<div id="barcode_place">SCAN ATAU INPUT BARCODE, KODE ATAU NAMA PRODUK : <input type="text" id="barcode" name="barcode" class="barcodekasir"><input type="hidden" id="hiddenbarcode" name="hiddenbarcode"></div>';
	$tablekasir = view_kasir($idtitipanlaundry,$alamatasal);
	$penjualan .= '<div id="tempattabelkasir">'.$tablekasir.'</div>';
	$penjualan .= '<input type="hidden" id="last_id" name="last_id">';
	$penjualan .= '<input type="hidden" id="lastqty" name="lastqty" value="1">';
	$penjualan .= '<input type="hidden" id="lastharga" name="lastharga">';
	$penjualan .= '<input type="hidden" id="lastdiskon" name="lastdiskon">';
	$penjualan .= '<input type="hidden" id="lastbarcode" name="lastbarcode">';
	$penjualan .= '<input type="hidden" id="nilaikirim" name="nilaikirim">';
	//$penjualan .= '<div id="tempattombolkasir">';
	if ($nomerRak != ''){
		$penjualan .= '<div id="tempattombolkasir" style="height:330px;">';
		$penjualan .= '<a style="padding: 0.4em 0.3em;margin-top: 2px;margin-bottom: 4px;" class="buttonprint" onclick="preventDefault();">NO.RAK : '.$nomerRak.'</a>';
	}else{
		$penjualan .= '<div id="tempattombolkasir">';
        $penjualan .= $panduantombol;
	}
	$penjualan .= $tombolcari;
	$penjualan .= $tombolqty;
	$penjualan .= $tombolhapus;
	$penjualan .= $tombolselesai;
	$penjualan .= $tombolprint;
	//$penjualan .= $tombolprint;
	//$penjualan .= $tomboltutupkasir;
	$penjualan .= $tombollihatomset;
    $penjualan .= $tombollihatpelanggan;
	$penjualan .= '</div>';
    $penjualan .= '<div id="tempattotal"><div id="totalbelanja">TOTAL : '.$currencySym.' 0</div></div>';
	$penjualan .= '</div>';
	$penjualan .= '<div title="Informasi" id="dialogwarning">';
	$penjualan .= '<p id="pesantext">Produk yang dicari tidak ada, Silahkan masukkan Barcode/Kode Produk/Nama Produk yang lain...!!!</p>';
	$penjualan .= '<p style="text-align: center"><button id="tutupdialog" class="buttonwarning" onclick="tutupwarning();">CLOSE</button></p>';
	$penjualan .= '</div>';
	$penjualan .= '<div title="UBAH HARGA" id="dialogubahharga">';
	$penjualan .= '<label>Harga Baru : </label><input type="text" id="newharga" name="newharga" value="1">';
	$penjualan .= '</div>';
    $penjualan .= '<div title="UBAH HARGA" id="dialogubahharga2">';
    $penjualan .= '<label>Harga Baru : </label><input type="text" id="newharga2" name="newharga2" value="1">';
    $penjualan .= '</div>';
	$penjualan .= '<div title="UBAH QTY" id="dialogubahqty">';
	$penjualan .= '<label>Qty Baru : </label><input type="text" id="newqty" name="newqty" value="1">';
	$penjualan .= '</div>';
	$penjualan .= '<div title="UBAH QTY" id="dialogubahqty2">';
	$penjualan .= '<label>Qty Baru : </label><input type="text" id="newqty2" name="newqty2">';
	$penjualan .= '</div>';
	$penjualan .= '<div title="PEMBAYARAN" id="dialogbayar">';
	$carabayar = '<select id="carabayar" name="carabayar" style="font-weight: bold;">';
    $carabayar2 = '<select id="carabayar2" name="carabayar2" style="font-weight: bold;" disabled="disabled">';
	$result = db_query("SELECT carabayar FROM carabayar");
	$i = 0;
	while($data = db_fetch_object($result)){
		if ($i == 0){
			$carabayar .= '<option selected="selected" value="'.$data->carabayar.'">'.$data->carabayar.'</option>';
            $carabayar2 .= '<option disabled="disabled" value="'.$data->carabayar.'">'.$data->carabayar.'</option>';
		}else{
			$carabayar .= '<option value="'.$data->carabayar.'">'.$data->carabayar.'</option>';
            if ($i == 1){
                $carabayar2 .= '<option selected="selected" value="'.$data->carabayar.'">'.$data->carabayar.'</option>';
            }else{
                $carabayar2 .= '<option value="'.$data->carabayar.'">'.$data->carabayar.'</option>';
            }
		}
		$i++;
	}
	$carabayar .= '</select>';
    $carabayar2 .= '</select>';
	$perlakuankembalian = '<select id="kembalian" name="kembalian">';
	$arrayPerlakuan = PERLAKUAN_KEMBALIAN();
	for ($i = 0;$i < count($arrayPerlakuan);$i++){
		if ($i == 0){
			$perlakuankembalian .= '<option value="'.$i.'" selected>'.$arrayPerlakuan[$i].'</option>';
		}else{
			$perlakuankembalian .= '<option value="'.$i.'">'.$arrayPerlakuan[$i].'</option>';
		}
	}
	$perlakuankembalian .= '</select>';
	//Pelanggan
	/*$pelanggan = '<select id="idpelanggan" name="idpelanggan">';
	$result = db_query("SELECT idpelanggan,namapelanggan FROM pelanggan");
	$i = 0;
	while($data = db_fetch_object($result)){
		if ($i == 0){
			$pelanggan .= '<option selected="selected" value="'.$data->idpelanggan.'">'.$data->namapelanggan.'</option>';	
		}else{
			$pelanggan .= '<option value="'.$data->idpelanggan.'">'.$data->namapelanggan.'</option>';	
		}
		$i++;
	}
	$pelanggan .= '</select>';*/
    $nokartudebit = '<input type="text" id="nomerkartu" name="nomerkartu" tabindex="1">';
    $nokartukredit = '<input type="text" id="nomer_kartu_kredit" name="nomer_kartu_kredit" tabindex="2">';
    $nilaibayar = '<input type="text" id="nilaibayar" name="nilaibayar" tabindex="3">';
    $NilaiBayarHutang = '<input type="text" id="nilai_bayar_hutang" name="nilai_bayar_hutang" tabindex="4">';
    $NilaiBayarDebit = '<input type="text" id="nilai_bayar_debit" name="nilai_bayar_debit" tabindex="5">';
    $NilaiBayarKredit = '<input type="text" id="nilai_bayar_kredit" name="nilai_bayar_kredit" tabindex="6">';
    $NilaiBayarDeposit = '<input type="text" id="nilai_bayar_deposit" name="nilai_bayar_deposit" tabindex="7">';
	$depositpelanggan = '<input type="text" id="depositpelanggan" name="depositpelanggan" readonly="readonly">';
	$kembali = '<input readonly="readonly" type="text" id="kembali" name="kembali">';
	$totalbelanja = '<input type="text" id="totalbelanjauser" name="totalbelanjauser">';
	//$penjualan .= '<div class="barisbayar"><label>Pelanggan</label>'.$pelanggan.'</div>';
    if (isset($namaMeja) && isset($idMeja)){
        $meja = '<input type="text" id="nomormeja" name="nomormeja" value="'.$namaMeja.'">';
        $meja .= '<input type="hidden" id="idmeja" name="idmeja" value="'.$idMeja.'">';
    }else {
        $meja = '<input type="text" id="nomormeja" name="nomormeja">';
        $meja .= '<input type="hidden" id="idmeja" name="idmeja">';
    }
    //$penjualan .= '<div class="barisbayar"><label>Meja</label>'.$meja.'</div>';
    $penjualan .= '<div id="status-message"></div>';
	$penjualan .= '<div class="barisbayar"><label style="width: 250px;">TOTAL</label>'.$totalbelanja.'</div>';
	$totalppn = '<input type="hidden" id="ppn_value" name="ppn_value" value="'.$dataPremis->ppn_value.'">';
	$totalppn .= '<input type="text" id="total_ppn" name="total_ppn" value="0" disabled="disabled">';
	$ceklistPpn = '<input type="checkbox" id="use-ppn" name="use-ppn" value="1" checked="checked" class="checkbox-ppn">';
	$penjualan .= '<div class="barisbayar"><label style="width: 250px;" >'.$gstSym.' ('.$dataPremis->ppn_value.'%)&nbsp;'.$ceklistPpn.'</label>'.$totalppn.'</div>';

	//Payment By Voucher
    $VoucherPayment = '<input type="text" id="voucher_value" name="voucher_value" value="0" disabled="disabled">';
    $VoucherNumber = '<textarea id="voucher_number" name="voucher_number" disabled="disabled" class="textarea-bayar"></textarea>';
    $CekListVoucher = '<input type="checkbox" id="use-voucher" name="use-voucher" value="1" class="checkbox-ppn">';
    $penjualan .= '<div class="barisbayar"><label style="width: 250px;" >'.strtoupper(trans('voucher', $dataPremis->bahasa)).' '.$CekListVoucher.'</label>'.$VoucherPayment.'</div>';
    $penjualan .= '<div class="barisbayar"><label style="width: 250px;" >&nbsp;</label>'.$VoucherNumber.'</div>';
	//End Payment By Voucher

    $totalplusppn = '<input type="text" id="total_plus_ppn" name="total_plus_ppn" value="0" disabled="disabled">';
    $penjualan .= '<div class="barisbayar"><label style="width: 250px;">TOTAL + '.$gstSym.'</label>'.$totalplusppn.'</div>';
    $Penggenapan = '<input type="text" id="penggenapan" name="penggenapan" value="0" disabled="disabled" style="margin-bottom: 8px;">';
    $penjualan .= '<div class="barisbayar"><label style="width: 250px;">PENGGENAPAN</label>'.$Penggenapan.'</div>';
    $penjualan .= '<div class="barisbayar"><label style="width: 250px;">CARA BAYAR</label>'.$carabayar.'</div>';

    $CekListCaraBayar = '<input type="checkbox" id="use-carabayar2" name="use-carabayar2" value="1" class="checkbox-ppn">';
    $penjualan .= '<div class="barisbayar"><label style="width: 250px;">CARA BAYAR 2'.$CekListCaraBayar.'</label>'.$carabayar2.'</div>';

	$penjualan .= '<div id="field_no_kartu_debit" class="barisbayar" style="display: none;"><label style="width: 250px;">NO. KAD DEBIT</label>'.$nokartudebit.'</div>';
    $penjualan .= '<div id="field_no_kartu_kredit" class="barisbayar" style="display: none;"><label style="width: 250px;">NO. KAD KREDIT</label>'.$nokartukredit.'</div>';
	$penjualan .= '<div id="baris-deposit" class="barisbayar"><label id="label-deposit" style="width: 250px;">Deposit</label>'.$depositpelanggan.'</div>';
	$penjualan .= '<div id="baris_bayar_tunai" class="barisbayar"><label style="width: 250px;">TUNAI</label>'.$nilaibayar.'</div>';
    $penjualan .= '<div id="baris_bayar_hutang" class="barisbayar" style="display: none;"><label style="width: 250px;">HUTANG</label>'.$NilaiBayarHutang.'</div>';
    $penjualan .= '<div id="baris_bayar_debit" class="barisbayar" style="display: none;"><label style="width: 250px;">KAD DEBIT</label>'.$NilaiBayarDebit.'</div>';
    $penjualan .= '<div id="baris_bayar_kredit" class="barisbayar" style="display: none;"><label style="width: 250px;">KAD KREDIT</label>'.$NilaiBayarKredit.'</div>';
    $penjualan .= '<div id="baris_bayar_deposit" class="barisbayar" style="display: none;"><label style="width: 250px;">USE DEPOSIT</label>'.$NilaiBayarDeposit.'</div>';
	$penjualan .= '<div id="field_kembali" class="barisbayar"><label style="width: 250px;">'.strtoupper(trans('kembali', $dataPremis->bahasa)).'</label>'.$kembali.'</div>';
	//$penjualan .= '<div id="field_kembalian" class="barisbayar" style="display: none;"><label style="width: 250px;">Kembalian</label>'.$perlakuankembalian.'</div>';
    $penjualan .= '<div align="center"><button id="simpan-transaksi">'.strtoupper(trans('simpan', $dataPremis->bahasa)).'</button>&nbsp;&nbsp;<button id="selesai-transaksi">SELESAI</button></div>';
	$penjualan .= '</div>';
	return $penjualan;
}

function simpan_penjualan(){
    global $user;
    //nonota, idpemakai, total, bayar, kembali
    if (isset($_POST["detail_produk"]) AND isset($_POST["bayar"]) >= 0 AND
        isset($_POST["totalbelanja"]) AND isset($_POST['totalbelanjappn'])
    ){
        global $user;
        if (!empty($user->timezone_name)) {
            date_default_timezone_set($user->timezone_name);
        }else{
            set_default_time_zone();
        }
        $waktujual = $_POST["tgljual"]." ".date("H:i:s");
        $splitTanggal = explode('-', $_POST["tgljual"]);
        $splitJam = explode(':',date("H:i:s"));
        $intTanggal = mktime($splitJam[0],$splitJam[1],$splitJam[2],$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
        $result = db_query("SELECT idpenjualan FROM penjualan_temp ORDER BY idpenjualan DESC LIMIT 1");
        $data = db_fetch_object($result);
        if ($data->idpenjualan > 0){
            $next_id = $data->idpenjualan + 1;
        }else{
            $next_id = 1;
        }
        $no_nota = buat_nota($next_id);
        $kembali = $_POST["bayar"] - $_POST["totalbelanjaround"];
        $perkuankembalian = isset($_POST["perlakuankembalian"]) ? $_POST["perlakuankembalian"] : 0;
        $idtitipanlaundry = 0;
        if (isset($_POST['idtitipanlaundry']) && !empty($_POST['idtitipanlaundry'])){
            $idtitipanlaundry = $_POST['idtitipanlaundry'];
        }

        if (isset($_POST['idcustomerorder']) && !empty($_POST['idcustomerorder'])){
            $IdCustomerOrder = $_POST['idcustomerorder'];
        }
        //$idMeja = $_POST['idmeja'];
        $PpnType = variable_get('ppn_type',1);
        if ($PpnType == 1){
            $PpnValue = $_POST["totalbelanja"] * $_POST["ppn"]/100;
        }else{
            $PpnValue = $_POST["totalbelanjappn"] - ($_POST["totalbelanja"] / ((100 + $_POST["ppn"])/100));
        }
        $NomerKartu = '';
        if (isset($_POST['nomerkartu'])){
            $NomerKartu = $_POST['nomerkartu'];
        }
        if (isset($_POST['nomer_kartu_kredit']) && !isset($_POST['nomerkartu'])){
            $NomerKartu = $_POST['nomer_kartu_kredit'];
        }else if (isset($_POST['nomer_kartu_kredit']) && isset($_POST['nomerkartu'])){
            $NomerKartu .= '-'.$_POST['nomer_kartu_kredit'];
        }
        if (module_exists('gst_separator')){
            $StrSql = "SELECT gst_flag FROM penjualan_temp WHERE tglpenjualan BETWEEN '%s' AND '%s' ";
            $StrSql .= "ORDER BY idpenjualan DESC LIMIT 1";
            $UseGst = db_result(db_query($StrSql, $_POST["tgljual"].' 00:00', $_POST["tgljual"].' 23:59'));
            $GstFlag = 1;
            if ($UseGst == 1 && $PpnValue > 0){
                $GstFlag = 0;
            }else if($PpnValue <= 0){
                $GstFlag = 0;
            }
            db_query("INSERT INTO penjualan_temp (nonota, idpemakai, total, bayar, kembali, tglpenjualan, idpelanggan, 
        carabayar, idtitipanlaundry, perlakuankembalian, ppn, total_plus_ppn, ppn_value,bayar_multiple,total_round,
        voucher_value,voucher_number,total_after_voucher, nokartu, gst_flag) 
		VALUES ('%s', '%d', '%f', '%f', '%f', '%s', '%d', '%s', '%d', '%d', '%f', '%f', '%f', '%s', '%f',
		'%f','%s','%f','%s','%d')",
                $no_nota, $user->uid, $_POST["totalbelanja"], $_POST["bayar"], $kembali, $waktujual,
                $_POST["idpelanggan"],$_POST["carabayar"], $idtitipanlaundry,$perkuankembalian,
                $_POST["ppn"],$_POST["totalbelanjappn"],$PpnValue,$_POST["bayar_multiple"],$_POST["totalbelanjaround"],
                $_POST["voucher_value"], $_POST["voucher_number"], $_POST["total_after_voucher"],$NomerKartu, $GstFlag);
        }else{
            db_query("INSERT INTO penjualan_temp (nonota, idpemakai, total, bayar, kembali, tglpenjualan, idpelanggan, 
        carabayar, idtitipanlaundry, perlakuankembalian, ppn, total_plus_ppn, ppn_value,bayar_multiple,total_round,
        voucher_value,voucher_number,total_after_voucher, nokartu) 
		VALUES ('%s', '%d', '%f', '%f', '%f', '%s', '%d', '%s', '%d', '%d', '%f', '%f', '%f', '%s', '%f',
		'%f','%s','%f','%s')",
                $no_nota, $user->uid, $_POST["totalbelanja"], $_POST["bayar"], $kembali, $waktujual,
                $_POST["idpelanggan"],$_POST["carabayar"], $idtitipanlaundry,$perkuankembalian,
                $_POST["ppn"],$_POST["totalbelanjappn"],$PpnValue,$_POST["bayar_multiple"],$_POST["totalbelanjaround"],
                $_POST["voucher_value"], $_POST["voucher_number"], $_POST["total_after_voucher"],$NomerKartu);
        }

        $next_id = db_last_insert_id('penjualan_temp', 'idpenjualan');
        $ExplodeBayar = explode(';',$_POST["bayar_multiple"]);
        for ($i = 0;$i < count($ExplodeBayar);$i++){
            $ExplodeCaraBayar = explode(':',$ExplodeBayar[$i]);
            $StrSqlCek = "SELECT COUNT(*) FROM penjualan_bayar_temp WHERE idpenjualan=%d AND carabayar='%s'";
            $DataBayarExists = db_result(db_query($StrSqlCek, $next_id, $ExplodeCaraBayar[0]));
            if (empty($DataBayarExists)) {
                $StrSqlInsertBayar = "INSERT INTO penjualan_bayar_temp (idpenjualan, carabayar, total_bayar) VALUES (%d, '%s', '%f')";
                db_query($StrSqlInsertBayar, $next_id, $ExplodeCaraBayar[0], $ExplodeCaraBayar[1]);
            }
        }
        print $next_id;
        //Hitungan Deposit
        if ($_POST["carabayar"] == "DEPOSIT") {
            if (isset($_POST["idpelanggan"]) && !empty($_POST["idpelanggan"])) {
                $sisaDeposit = getTotalPembayaran($_POST["idpelanggan"]) - (getTotalHutang($_POST["idpelanggan"]) + getTotalPenggunaanDeposit($_POST["idpelanggan"]));
                $variables['idpelanggan'] = $_POST["idpelanggan"];
                $variables['deposit'] = $sisaDeposit;
                $variables['penggunaan'] = $_POST["bayar"];
                $variables['tglbayar'] = $waktujual;
                $variables['keterangan'] = 'Penggunaan deposit untuk pembayaran nota : ' . $no_nota . ' Tanggal : ' . $waktujual;
                $variables['idpenjualan'] = $next_id;
                penggunaandeposit($variables);
                syncHutangPelanggan($_POST["idpelanggan"]);
            }
        }
        //End Hitungan deposit

        $pecahdetail = explode("&",$_POST["detail_produk"]);
        $totalmodal = 0;
        $StrSqlExecute = "";
        $StrSqlUpdate = "";
        $PostPpn = $_POST['ppn'];
        $Counter = 0;
        $detailTitipanLaundry = null;
        $detailCustomerOrder = null;
        if (isset($_POST['idtitipanlaundry']) && !empty($_POST['idtitipanlaundry'])){
            $result = db_query("SELECT iddetail, idproduct, sisa, diambil FROM 
			detaillaundry WHERE idtitipanlaundry='%d' AND sisa > 0",$_POST['idtitipanlaundry']);
            while ($data = db_fetch_object($result)){
                $detailTitipanLaundry[$data->idproduct] = $data;
            }
        }else if (isset($_POST['idcustomerorder']) && !empty($_POST['idcustomerorder'])){
            $IdCustomerOrder = $_POST['idcustomerorder'];
            if (!is_array($IdCustomerOrder)) {
                $result = db_query("SELECT id, idproduct, sisa, diambil FROM
			    detailcustomerorder WHERE idcustomerorder='%d' AND sisa > 0", $IdCustomerOrder);
                while ($data = db_fetch_object($result)) {
                    $detailCustomerOrder[$IdCustomerOrder][$data->idproduct] = $data;
                }
            }else{
                for ($i = 0;$i < count($IdCustomerOrder);$i++){
                    $result = db_query("SELECT id, idproduct, sisa, diambil FROM
			        detailcustomerorder WHERE idcustomerorder='%d' AND sisa > 0", $IdCustomerOrder[$i]);
                    while ($data = db_fetch_object($result)) {
                        $detailCustomerOrder[$IdCustomerOrder[$i]][$data->idproduct] = $data;
                    }
                }
            }
        }
        foreach ($pecahdetail as $parameter){
            $pecahparameter = explode("=",$parameter);
            $pecahnilai = explode("___",$pecahparameter[1]);
            $IDPRODUK =	(int)$pecahnilai[0];
            $QTY = $pecahnilai[1];
            $HARGAJUAL = $pecahnilai[2];
            $DISKON = $pecahnilai[3];
            $Barcode = $pecahnilai[4];
            $IdSupplier = $pecahnilai[7];
            $HARGAPOKOK = $pecahnilai[6];
            $totalmodal = $totalmodal + ($HARGAPOKOK*$QTY);
            if ($Counter == 0) {
                $StrSqlInsert = "INSERT INTO detailpenjualan_temp (idpenjualan, idproduct, jumlah, hargapokok, hargajual, diskon, ppn, idsupplier, qty_pcs) ";
                $StrSqlInsert .= " VALUES ";
            }else{
                $StrSqlInsert = ",";
            }
            $StrSqlInsert .= "('".$next_id."', '".$IDPRODUK."', '".$QTY."', '".$HARGAPOKOK."', '";
            $StrSqlInsert .= $HARGAJUAL."', '".$DISKON."', '".$PostPpn."','".$IdSupplier."','".$pecahnilai[5]."')";
            $StrSqlExecute .= $StrSqlInsert;
            if (!empty($detailTitipanLaundry) && isset($detailTitipanLaundry[$IDPRODUK])){
                if ($detailTitipanLaundry[$IDPRODUK]->sisa - $QTY <= 0){
                    $statusDetail = 2;
                }else{
                    $statusDetail = 1;
                }
                $strSQL = "UPDATE detaillaundry SET sisa = sisa - ".$QTY.", diambil = ".$intTanggal.", status_laundry = ".$statusDetail ;
                $strSQL .= " WHERE iddetail = ".$detailTitipanLaundry[$IDPRODUK]->iddetail.";";
                db_query($strSQL);
            }else if(!empty($detailCustomerOrder)){
                $IdCustomerOrder = $_POST['idcustomerorder'];
                if (!is_array($IdCustomerOrder) && isset($detailCustomerOrder[$IdCustomerOrder][$IDPRODUK])) {
                    if ($detailCustomerOrder[$IdCustomerOrder][$IDPRODUK]->sisa - $QTY <= 0) {
                        $statusDetail = 2;
                    } else {
                        $statusDetail = 1;
                    }
                    $strSQL = "UPDATE detailcustomerorder SET sisa = sisa - %f, diambil = %d, status_order = %d WHERE id = %d";
                    db_query($strSQL, $QTY, $intTanggal, $statusDetail, $detailCustomerOrder[$IdCustomerOrder][$IDPRODUK]->id);
                }else{
                    $QtyAll = $QTY;
                    for ($i = 0;$i < count($IdCustomerOrder);$i++){
                        if (isset($detailCustomerOrder[$IdCustomerOrder[$i]][$IDPRODUK])){
                            if ($detailCustomerOrder[$IdCustomerOrder[$i]][$IDPRODUK]->sisa - $QtyAll <= 0) {
                                $statusDetail = 2;
                            } else {
                                $statusDetail = 1;
                            }
                            $strSQL = "UPDATE detailcustomerorder SET sisa = sisa - %f, diambil = %d, status_order = %d WHERE id = %d";
                            db_query($strSQL, $QTY, $intTanggal, $statusDetail, $detailCustomerOrder[$IdCustomerOrder[$i]][$IDPRODUK]->id);
                            $QtyAll = $QtyAll - $detailCustomerOrder[$IdCustomerOrder[$i]][$IDPRODUK]->sisa;
                        }
                    }
                }
            }
            $Counter++;
        }
        if (isset($_POST['idtitipanlaundry']) && !empty($_POST['idtitipanlaundry'])){
            $totalLaundry = db_result(db_query("SELECT COUNT(*) FROM detaillaundry WHERE idtitipanlaundry='%d'",$_POST['idtitipanlaundry']));
            $totalSelesai = db_result(db_query("SELECT COUNT(*) FROM detaillaundry WHERE idtitipanlaundry='%d' AND status_laundry=2",$_POST['idtitipanlaundry']));
            if ($totalSelesai > 0 && $totalSelesai == $totalLaundry){
                $statusLaundry = 2;
            }else if($totalSelesai > 0 && $totalSelesai < $totalLaundry){
                $statusLaundry = 1;
            }else{
                $statusLaundry = 0;
            }
            $strSQL = "UPDATE titipanlaundry SET status_laundry=".$statusLaundry." WHERE idtitipanlaundry = ".$_POST['idtitipanlaundry'].";";
            db_query($strSQL);
        }else if(isset($_POST['idcustomerorder']) && !empty($_POST['idcustomerorder'])){
            $IdCustomerOrder = $_POST['idcustomerorder'];
            if (!is_array($IdCustomerOrder)) {
                $totalOrder = db_result(db_query("SELECT COUNT(*) FROM detailcustomerorder WHERE idcustomerorder='%d'", $IdCustomerOrder));
                $totalSelesai = db_result(db_query("SELECT COUNT(*) FROM detailcustomerorder WHERE idcustomerorder='%d' AND status_order=2", $IdCustomerOrder));
                if ($totalSelesai > 0 && $totalSelesai == $totalOrder) {
                    $statusOrder = 2;
                } else if ($totalSelesai > 0 && $totalSelesai < $totalOrder) {
                    $statusOrder = 1;
                } else {
                    $statusOrder = 0;
                }
                $strSQL = "UPDATE customer_order SET status_order=%d WHERE id=%d";
                db_query($strSQL, $statusOrder, $IdCustomerOrder);
            }else{
                for ($i = 0;$i < count($IdCustomerOrder);$i++){
                    $totalOrder = db_result(db_query("SELECT COUNT(*) FROM detailcustomerorder WHERE idcustomerorder='%d'", $IdCustomerOrder[$i]));
                    $totalSelesai = db_result(db_query("SELECT COUNT(*) FROM detailcustomerorder WHERE idcustomerorder='%d' AND status_order=2", $IdCustomerOrder[$i]));
                    if ($totalSelesai > 0 && $totalSelesai == $totalOrder) {
                        $statusOrder = 2;
                    } else if ($totalSelesai > 0 && $totalSelesai < $totalOrder) {
                        $statusOrder = 1;
                    } else {
                        $statusOrder = 0;
                    }
                    $strSQL = "UPDATE customer_order SET status_order=%d WHERE id=%d";
                    db_query($strSQL, $statusOrder, $IdCustomerOrder[$i]);
                }
            }
        }
        $StrSql = "UPDATE penjualan_temp SET totalmodal='".$totalmodal."' WHERE idpenjualan = ".$next_id.";";
        db_query($StrSql);
        db_query($StrSqlExecute);
    }else{
        print 'error';
    }
    exit();
}

function buat_nota_05052019($idpenjualan){
	if ($idpenjualan > 0 AND $idpenjualan < 10){
		$no_nota = "N000000".$idpenjualan;
	}elseif ($idpenjualan >= 10 AND $idpenjualan < 100){
		$no_nota = "N00000".$idpenjualan;	
	}elseif ($idpenjualan >= 100 AND $idpenjualan < 1000){
		$no_nota = "N0000".$idpenjualan;	
	}elseif ($idpenjualan >= 1000 AND $idpenjualan < 10000){
		$no_nota = "N000".$idpenjualan;
	}elseif ($idpenjualan >= 10000 AND $idpenjualan < 100000){
		$no_nota = "N00".$idpenjualan;
	}elseif ($idpenjualan >= 100000 AND $idpenjualan < 1000000){
		$no_nota = "N0".$idpenjualan;
	}elseif ($idpenjualan >= 1000000){
		$no_nota = "N".$idpenjualan;
	}
	return $no_nota;
}
function buat_nota($idpenjualan){
    global $user;
    if (!empty($user->timezone_name)) {
        date_default_timezone_set($user->timezone_name);
    }else{
        set_default_time_zone();
    }
    $DateToday = date('ymd');
    if ($idpenjualan > 0 AND $idpenjualan < 10){
        $no_nota = "N".$DateToday."000".$idpenjualan;
    }elseif ($idpenjualan >= 10 AND $idpenjualan < 100){
        $no_nota = "N".$DateToday."00".$idpenjualan;
    }elseif ($idpenjualan >= 100 AND $idpenjualan < 1000){
        $no_nota = "N".$DateToday."0".$idpenjualan;
    }elseif ($idpenjualan >= 1000){
        $no_nota = "N".$DateToday.$idpenjualan;
    }
    return $no_nota;
}
function hitung_omset($tgl = null){
    global $user;
    if (!empty($user->timezone_name)) {
        date_default_timezone_set($user->timezone_name);
    }else{
        set_default_time_zone();
    }
	if ($_POST["tglpost"]){
		$tglomset = $_POST["tglpost"];
	}else{
		if (is_null($tgl)){
			$tglomset = date("Y-m-d");
		}else{
			$tglomset = $tgl;
		}
	}
	if (module_exists('gst_separator')){
        $GstStatus = variable_get('gst_separator_status', 1);
        if ($GstStatus == 1){
            if (user_access('Access GST Separator')){
                $result = db_query("SELECT SUM(total_round) AS totalbelanja FROM penjualan WHERE SUBSTR(tglpenjualan,1,10)='%s'",$tglomset);
            }else{
                $result = db_query("SELECT SUM(total_round) AS totalbelanja FROM penjualan WHERE SUBSTR(tglpenjualan,1,10)='%s' AND gst_flag = 1",$tglomset);
            }
        }else{
            $result = db_query("SELECT SUM(total_round) AS totalbelanja FROM penjualan WHERE SUBSTR(tglpenjualan,1,10)='%s'",$tglomset);
        }
    }else{
        $result = db_query("SELECT SUM(total_round) AS totalbelanja FROM penjualan WHERE SUBSTR(tglpenjualan,1,10)='%s'",$tglomset);
    }
	$data = db_fetch_object($result);
	if ($_POST["tglpost"]){
		print $data->totalbelanja;
		exit();
	}else{
		return $data->totalbelanja;
	}
}

function cari_produk(){
    if (isset($_POST["barcode"])){
        $strSQL = "SELECT idproduct FROM detailpembelian WHERE barcode_batch='%s'";
        $idBarang = db_result(db_query($strSQL,$_POST["barcode"]));
        if (!empty($idBarang)){
            //$result = db_query("SELECT barcode, alt_code, namaproduct, stok, hargajual FROM product WHERE idproduct=%d", $idBarang);
            $result = db_query("SELECT idproduct,idkategori,namaproduct,hargajual,lead_time,aturan_jam_kerja,
		    berlaku_sebelum_zuhur, barcode,hargapokok,type_product FROM product WHERE idproduct=%d LIMIT 1", $idBarang);
            $data = db_fetch_object($result);
            if ($data->idproduct) {
                if (isset($_POST["idpelanggan"]) && !empty($_POST['idpelanggan'])) {
                    $DISKON = get_diskon_product($_POST["idpelanggan"],$data->idproduct);
                }else{
                    $DISKON = 0;
                }
                if ($_POST["idpelanggan"] == 0) {
                    $DISKON = 0;
                }
                $takeTime = calculateTakeTime($data->lead_time, $_POST["waktumasuk"], $data->aturan_jam_kerja, $data->berlaku_sebelum_zuhur);
                $IdSupplier = db_result(db_query('SELECT idsupplier FROM product_supplier WHERE idproduct=%d LIMIT 1', $idBarang));
                $RetVal = $data->idproduct . ';' . $data->namaproduct . ';' . $data->hargajual . ';' . $DISKON;
                $RetVal .= ';' . $takeTime[0] . ';' . $takeTime[1] .';'. $_POST["katacari"].';'.$data->barcode;
                $RetVal .= ';' . $data->hargapokok .';'. $IdSupplier;
                print $RetVal;
            } else {
                print "error";
            }
        }else {
            $Barcode = $_POST["barcode"];
            if (isset($_POST["katacari"]) && !empty($_POST["katacari"])) {
                $SearchTerm = '%' . $_POST["katacari"] . '%';
            }
            if (isset($_POST['freshproduct']) && !empty($_POST['freshproduct'])){
                $result = db_query("SELECT idproduct,idkategori,namaproduct,hargajual,lead_time,aturan_jam_kerja,
		        berlaku_sebelum_zuhur, barcode, hargapokok,type_product FROM product WHERE barcode = '%s' AND status_product = 1 LIMIT 1", $Barcode);
            }else {
                //$sql = "SELECT idproduct,idkategori,namaproduct,hargajual FROM product WHERE alt_code LIKE '$KATACARI' OR barcode LIKE '$KATACARI' OR namaproduct LIKE '$KATACARI'";
                $result = db_query("SELECT idproduct,idkategori,namaproduct,hargajual,lead_time,aturan_jam_kerja,
		        berlaku_sebelum_zuhur, barcode, hargapokok,type_product FROM product WHERE barcode = '%s' AND status_product = 1 LIMIT 50",
                $Barcode);
            }
            $data = db_fetch_object($result);
            if ($data->idproduct) {
                if (isset($_POST["idpelanggan"]) && !empty($_POST['idpelanggan'])) {
                    $DISKON = get_diskon_product($_POST["idpelanggan"],$data->idproduct);
                }else{
                    $DISKON = 0;
                }
                if ($_POST["idpelanggan"] == 0) {
                    $DISKON = 0;
                }
                $takeTime = calculateTakeTime($data->lead_time, $_POST["waktumasuk"], $data->aturan_jam_kerja, $data->berlaku_sebelum_zuhur);
                $IdSupplier = db_result(db_query('SELECT idsupplier FROM product_supplier WHERE idproduct=%d LIMIT 1', $data->idproduct));
                $RetVal = $data->idproduct . ';' . $data->namaproduct . ';' . $data->hargajual . ';' . $DISKON;
                $RetVal .= ';' . $takeTime[0] . ';' . $takeTime[1] .';0;'.$data->barcode;
                $RetVal .= ';' . $data->hargapokok .';'. $IdSupplier;
                print $RetVal;
            } else {
                print "error";
            }
        }
    }
    exit();
}

/*function cari_produk(){
	if ($_POST["katacari"]){
		$KATACARI = '%'.$_POST["katacari"].'%';
		//$sql = "SELECT idproduct,idkategori,namaproduct,hargajual FROM product WHERE alt_code LIKE '$KATACARI' OR barcode LIKE '$KATACARI' OR namaproduct LIKE '$KATACARI'";
		$result = db_query("SELECT idproduct,barcode,idkategori,namaproduct,hargajual,lead_time,aturan_jam_kerja,
		berlaku_sebelum_zuhur FROM product WHERE alt_code LIKE '%s' OR barcode LIKE '%s' 
		OR UPPER(namaproduct) LIKE '%s' AND status_product=1 LIMIT 50",$KATACARI,$KATACARI,$KATACARI);
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
			$takeTime = calculateTakeTime($data->lead_time, $_POST["waktumasuk"],$data->aturan_jam_kerja,$data->berlaku_sebelum_zuhur);
			print $data->idproduct.';'.$data->namaproduct.';'.$data->hargajual.';'.$DISKON.';'.$takeTime[0].';'.$takeTime[1].';'.$data->barcode;
		}else{
			print "error";
		}
	}
	exit();
}*/

function cari_barang(){
    if (isset($_GET["term"])){
        get_number_format($currencySym, $thousandSep, $decimalSep, false);
        $dataPremis = get_data_premis();
        $DecDigit = $dataPremis->decimal_digit;
        $strSQL = "SELECT idproduct FROM detailpembelian WHERE barcode_batch='%s'";
        $idBarang = db_result(db_query($strSQL,$_GET["term"]));
        $items = array();
        if (!empty($idBarang)){
            $result = db_query("SELECT barcode, alt_code, namaproduct, stok, hargajual FROM product WHERE idproduct=%d", $idBarang);
            while ($data = db_fetch_object($result)) {
                $items[] = array(
                    'value' => $data->barcode .'--->'. $data->namaproduct . '--->['.$currencySym.' ' . number_format($data->hargajual,$DecDigit,$decimalSep,$thousandSep) . '][' . $data->stok . ']', 'barcode' => $data->barcode, 'alt_code' => $data->alt_code, 'barcode_batch' => $_GET["term"]);
            }
        }else {
            $KATACARI = '%' . $_GET["term"] . '%';
            $result = db_query("SELECT idproduct, barcode, alt_code, namaproduct, stok, hargajual FROM product WHERE (barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s') AND status_product=1 LIMIT 50", $KATACARI, $KATACARI, $KATACARI);
            while ($data = db_fetch_object($result)) {
                $items[] = array('value' => $data->barcode .'--->'.$data->namaproduct . '--->['.$currencySym.' ' . number_format($data->hargajual,$DecDigit,$decimalSep,$thousandSep) . '][' . $data->stok . ']', 'barcode' => $data->barcode, 'alt_code' => $data->alt_code, 'barcode_batch' => '0');
            }
        }
        print json_encode($items);
    }
    exit();
}

/*function cari_barang(){
	if ($_GET["term"]){
        get_number_format($currencySym, $thousandSep, $decimalSep, false);
        $dataPremis = get_data_premis();
        $DecDigit = $dataPremis->decimal_digit;
		$KATACARI = '%'.$_GET["term"].'%';
		$result = db_query("SELECT barcode, alt_code, namaproduct, stok, hargajual FROM product WHERE alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s' AND status_product=1 LIMIT 50",$KATACARI,$KATACARI,$KATACARI);
		$items = array();
		while ($data = db_fetch_object($result)) {
			$items[] = array(
				'value' => $data->namaproduct . '--->['.$currencySym.' ' . number_format($data->hargajual,$DecDigit,$decimalSep,$thousandSep) . '][' . $data->stok . ']',
				'barcode'   => $data->barcode,
				'alt_code'  => $data->alt_code,
				'hargajual' => $data->hargajual,
			);
		}
		print json_encode($items);
	}
	exit();	
}*/
function view_kasir($idtitipanlaundry = null, $alamatasal = null){
	$kasirtabel ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_kasir">';
	$kasirtabel .= '<thead>';
	$kasirtabel .= '<tr>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th>PRODUK</th>';
	$kasirtabel .= '<th>HARGA</th>';
	$kasirtabel .= '<th class="diskon2">%</th>';
	$kasirtabel .= '<th class="diskon2">QTY</th>';
	$kasirtabel .= '<th class="subtotal">SUBTOTAL</th>';
    $kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '</tr>';
	$kasirtabel .= '</thead>';
	$kasirtabel .= '<tbody>';
	if (!empty($idtitipanlaundry)){
		if (empty($alamatasal) || $alamatasal == 'viewlaundry' || $alamatasal == 'laundrykeluar'){
			$result = db_query("SELECT b.iddetail, b.idproduct, b.iddetail,c.barcode, c.namaproduct, b.jumlah,
			b.hargajual, (b.hargajual*b.jumlah) AS subtotal, sisa, diambil, b.detail_nomer_rak FROM
			detaillaundry b LEFT JOIN product c ON b.idproduct=c.idproduct LEFT JOIN
			supplier a ON c.idsupplier=a.idsupplier WHERE b.idtitipanlaundry='%d' AND sisa > 0",$idtitipanlaundry);
			$dataLaundry = array();
			while ($data = db_fetch_object($result)){
				$dataLaundry[] = $data;
			}
			drupal_add_js(
				array(
					'data_laundry' => $dataLaundry,
				),
				'setting');
		}else if ($alamatasal == 'viewcustomerorder' || $alamatasal == 'orderkeluar'){
			if (!is_array($idtitipanlaundry)) {
                $result = db_query("SELECT b.id, b.idproduct, c.barcode, c.namaproduct, b.jumlah,
			b.hargajual, (b.hargajual*b.jumlah) AS subtotal, sisa, diambil, b.qty_pcs, c.satuan FROM
			detailcustomerorder b LEFT JOIN product c ON b.idproduct=c.idproduct LEFT JOIN
			supplier a ON c.idsupplier=a.idsupplier WHERE b.idcustomerorder='%d' AND
			sisa > 0", $idtitipanlaundry);
                $dataOrder = array();
                while ($data = db_fetch_object($result)) {
                    $dataOrder[] = $data;
                }
            }else{
                $dataOrder = array();
                $DataOrderDetail = array();
                $ProductOrder = array();
                for ($i = 0;$i < count($idtitipanlaundry);$i++){
                    $result = db_query("SELECT b.id, b.idproduct, c.barcode, c.namaproduct, b.jumlah,
			        b.hargajual, (b.hargajual*b.jumlah) AS subtotal, sisa, diambil, b.qty_pcs, c.satuan FROM
			        detailcustomerorder b LEFT JOIN product c ON b.idproduct=c.idproduct LEFT JOIN
			        supplier a ON c.idsupplier=a.idsupplier WHERE b.idcustomerorder='%d' AND
			        sisa > 0", $idtitipanlaundry[$i]);
                    while ($data = db_fetch_object($result)) {
                        if (!in_array($data->idproduct,$ProductOrder)){
                            $ProductOrder[] = $data->idproduct;
                            $DataOrderDetail[$data->idproduct] = $data;
                        }else{
                            $DataOrderDetail[$data->idproduct]->sisa = $DataOrderDetail[$data->idproduct]->sisa + $data->sisa;
                            $DataOrderDetail[$data->idproduct]->jumlah = $DataOrderDetail[$data->idproduct]->jumlah + $data->jumlah;
                            $DataOrderDetail[$data->idproduct]->qty_pcs = $DataOrderDetail[$data->idproduct]->qty_pcs + $data->qty_pcs;
                        }
                    }
                }
                foreach ($DataOrderDetail as $IdKey => $DataDetail){
                    $dataOrder[] = $DataDetail;
                }
            }
			drupal_add_js(
				array(
					'data_order' => $dataOrder,
				),
				'setting');
		}
	}
	$kasirtabel .= '</tbody>';
	$kasirtabel .= '</table>';
	return $kasirtabel;
}
function get_pelanggan_diskon(){
	$besarDiskon = array();
	if (isset($_POST['idpelanggan']) && isset($_REQUEST['idproduk'])){
		$idProduct = $_REQUEST['idproduk'];
		$IdPelanggan = $_POST['idpelanggan'];
		if (count($idProduct)){
			for ($i = 0;$i < count($idProduct);$i++) {
				if ($IdPelanggan != 0) {
                    $besarDiskon[$idProduct[$i]] = get_diskon_product($IdPelanggan, $idProduct[$i]);
                }else{
                    $besarDiskon[$idProduct[$i]] = 0;
                }
			}
		}
	}
	$returnData = array($besarDiskon);
	print json_encode($returnData);
	exit();
}
