<?php

function returnsupplier(){
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
    if (isset($_GET['tanggal'])){
        $tglsekarang = date("Y-m-d", strtotime($_GET['tanggal']));
        $tgltampil = date("d-m-Y", strtotime($_GET['tanggal']));
    }else{
        $tglsekarang = date("Y-m-d");
        $tgltampil = date("d-m-Y");
    }
    drupal_add_js(
        array(
            'tglsekarang' => $tglsekarang,
            'tgltampil' => $tgltampil,
        ),
        'setting');
    $js_path = drupal_get_path('module','pembelian').'/js/returnsupplier.js';
    drupal_add_js($js_path);
    $panduantombol = '<a style="font-weight: bold;text-align: center;">Ctrl+Shift+</a>';
    $tombolcari = '<a id="tombolubahharga" class="cari" onclick="ubahharga()">F2:UBAH HARGA</a>';
    $tombolqty = '<a id="tombolubahqty" onclick="ubahqty()" class="buttonqty">F3:UBAH QTY</a>';
    $tombolhapus = '<a class="buttonhapus" onclick="hapus_latest_produk()" title="Hapus produk">F4:HAPUS</a>';
    $tombolselesai = '<a class="buttonselesai" onclick="kirim_data()">F5:SELESAI</a>';
    $tombolprint = '<a class="buttonprint" onclick="kirim_data()">F6:SELESAI & PRINT</a>';
    $returnsupplier = '<button onclick="munculkankasir();" style="font-size:12px;border: 1px solid orangered;">'.strtoupper("Klik untuk memunculkan form returnsupplier").'</button>';
    $returnsupplier .= '<div id="dialogkasir" style="font-size: 12px;" title="GLOBAL POS RETURN SUPPLIER ['.$tgltampil.']">';
    $returnsupplier .= '<div id="info-kasir-waktu">';
    $returnsupplier .= '<div id="tempattanggalkasir">';
    $returnsupplier .= '<div id="tempattanggal">';
    $returnsupplier .= '<label>TANGGAL</label><input readonly="readonly" type="text" id="tglreturn" name="tglreturn" value="'.$tgltampil.'">';
    $returnsupplier .= '<input type="hidden" id="tglreturnkirim" name="tglreturnkirim" value="'.$tglsekarang.'">';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div id="tempatnamakasir">';
    global $user;
    $returnsupplier .= '<label>KASIR</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$user->name.'">';
    $returnsupplier .= '</div>';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div id="tempatjam" align="center">';
    $returnsupplier .= 'Jam';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div id="tempatkosong">';
    $returnsupplier .= '<div id="tempatsupplier"><label>SUPPLIER</label></div>';
    $pilihsupplier = '<select id="idsupplier" name="idsupplier" onchange="inisialulangautocomplete()">';
    $resultsupplier = db_query("SELECT idsupplier,namasupplier FROM supplier");
    while ($datasupplier = db_fetch_object($resultsupplier)){
        $pilihsupplier .= '<option value="'.$datasupplier->idsupplier.'">'.$datasupplier->namasupplier.'</option>';
    }
    $pilihsupplier .= '</select>';
    $returnsupplier .= '<div id="tempatsupplier2">'.$pilihsupplier.'</div>';
    $returnsupplier .= '</div>';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div id="barcode_place">SCAN ATAU INPUT BARCODE, KODE ATAU NAMA PRODUK : <input type="text" id="barcode" name="barcode" class="barcodekasir"></div>';
    $returnsupplier .= '<div id="tempattabelkasir">'.view_kasir_returnsupplier().'</div>';
    $returnsupplier .= '<input type="hidden" id="last_id" name="last_id"><input type="hidden" id="lastqty" name="lastqty" value="1"><input type="hidden" id="lastharga" name="lastharga">';
    $returnsupplier .= '<input type="hidden" id="nilaikirim" name="nilaikirim">';
    $returnsupplier .= '<div id="tempattombolkasir" style="height: 340px;">';
    $returnsupplier .= $panduantombol;
    $returnsupplier .= $tombolcari;
    $returnsupplier .= $tombolqty;
    $returnsupplier .= $tombolhapus;
    $returnsupplier .= $tombolselesai;
    $returnsupplier .= $tombolprint;
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div id="tempattotal"><div id="totalbelanja">Total Return : Rp. 0</div></div>';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div title="Informasi" id="dialogwarning">';
    $returnsupplier .= '<p id="pesantext">Produk yang dicari tidak ada, Silahkan masukkan Barcode/Kode Produk/Nama Produk yang lain...!!!</p>';
    $returnsupplier .= '<p style="text-align: center"><button id="tutupdialog" class="buttonwarning" onclick="tutupwarning();">CLOSE</button></p>';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div title="UBAH HARGA BELI" id="dialogubahharga">';
    $returnsupplier .= '<label>Harga Baru : </label><input type="text" id="newharga" name="newharga" value="1">';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div title="UBAH QTY" id="dialogubahqty">';
    $returnsupplier .= '<label>Qty Baru : </label><input type="text" id="newqty" name="newqty" value="1">';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div title="UBAH QTY" id="dialogubahqty2">';
    $returnsupplier .= '<label>Qty Baru : </label><input type="text" id="newqty2" name="newqty2">';
    $returnsupplier .= '</div>';
    $returnsupplier .= '<div title="PEMBAYARAN" id="dialogbayar">';
    $carabayar = '<select id="carabayar" name="carabayar">';
    $result = db_query("SELECT id, carareturn FROM carareturn");
    $i = 0;
    while($data = db_fetch_object($result)){
        if ($i == 0){
            $carabayar .= '<option selected="selected" value="'.$data->id.'">'.$data->carareturn.'</option>';
        }else{
            $carabayar .= '<option value="'.$data->id.'">'.$data->carareturn.'</option>';
        }
        $i++;
    }
    $carabayar .= '</select>';
    $nilaibayar = '<input type="text" id="nilaibayar" name="nilaibayar">';
    $kembali = '<input readonly="readonly" type="text" id="kembali" name="kembali">';
    $totalbelanja = '<input type="text" id="totalbelanjauser" name="totalbelanjauser">';
    $returnsupplier .= '<div class="barisbayar"><label>Total Return</label>'.$totalbelanja.'</div>';
    $returnsupplier .= '<div class="barisbayar"><label>Cara Return</label>'.$carabayar.'</div>';
    $returnsupplier .= '<div id="cashback" class="barisbayar"><label>Cash Back</label>'.$nilaibayar.'</div>';
    $returnsupplier .= '<div id="bariskembali" class="barisbayar"><label>Kembali</label>'.$kembali.'</div>';
    $returnsupplier .= '</div>';
    return $returnsupplier;
}

function simpan_returnsupplier(){
    //nonota, idpemakai, total, bayar, kembali
    if ($_POST['detail_produk'] && $_POST['totalbelanja']){
        $result = db_query("SELECT idreturn FROM returnsupplier ORDER BY idreturn DESC LIMIT 1");
        $data = db_fetch_object($result);
        if ($data->idreturn > 0){
            $next_id = $data->idreturn + 1;
        }else{
            $next_id = 1;
        }
        $no_nota = buat_nota($next_id);
        $totalReturn = $_POST["totalbelanja"];
        date_default_timezone_set('Asia/Jakarta');
        $waktuReturn = $_POST['tglreturn'].' '.date('H:i:s');
        $caraReturn = $_POST['carareturn'];
        global $user;
        db_query("INSERT INTO returnsupplier (nonota, idpemakai, total, tglreturn, carareturn) VALUES ('%s', '%d', '%f', '%s', '%d')",
                 $no_nota, $user->uid, $_POST["totalbelanja"], $waktuReturn, $caraReturn);
        $next_id = db_last_insert_id('returnsupplier', 'idreturn');
        print $_POST["detail_produk"];
        if ($caraReturn == 2){
            $variables = array();
            $variables['hutang'] = db_result(db_query("SELECT besarhutang FROM hutang WHERE idsupplier='%d'",$_POST['idsupplier']));
            $variables['pembayaran'] = $totalReturn;
            $variables['idsupplier'] = $_POST['idsupplier'];
            $variables['tglbayar'] = $waktuReturn;
            $variables['keterangan'] = 'Return supplier potong hutang';
            $variables['simpanpembayaran'] = true;
            do_pembayaran_hutang($variables);
        }
        $pecahdetail = explode("&",$_POST["detail_produk"]);
        foreach ($pecahdetail as $parameter){
            $pecahparameter = explode("=",$parameter);
            $pecahnilai = explode("___",remove_utf8_bom($pecahparameter[1]));
            $IDPRODUK =	$pecahnilai[0];
            $QTY = $pecahnilai[1];
            $HARGABELI = $pecahnilai[2];
            $result = db_query("SELECT hargapokok,stok FROM product WHERE idproduct='%d'",$IDPRODUK);
            $data = db_fetch_object($result);
            $HARGAPOKOK = $data->hargapokok;
            $STOKSEBELUM = $data->stok;
            //idreturn, idproduct, jumlah, hargapokok, hargapokok
            db_query("INSERT INTO detailreturn(idreturn, idproduct, jumlah, hargapokok, hargapokoksebelumnya) VALUES
			('%d', '%d', '%f', '%f','%f')",
                     $next_id,$IDPRODUK,$QTY,$HARGABELI,$HARGAPOKOK);
            if ($HARGAPOKOK <> $HARGABELI){
                db_query("INSERT INTO historyhargareturn (hargasebelum, hargasesudah, uid) VALUES('%f','%f','%d')",
                         $HARGAPOKOK,$HARGABELI,$user->uid);
                db_query("UPDATE product SET hargapokok='%f' WHERE idproduct='%d'",$HARGABELI,$IDPRODUK);
            }
            $STOKSEKARANG = $STOKSEBELUM - $QTY;
            db_query("UPDATE product SET stok='%f' WHERE idproduct='%d'",$STOKSEKARANG,$IDPRODUK);
            $KETERANGAN = 'Return ke supplier';
            db_query("INSERT INTO transaksistock (idproduk, idreturn, stocksebelum, masuk, stocksetelah, keterangan) VALUES
			('%d', '%d', '%f', '%f', '%f', '%s')",$IDPRODUK,$next_id,$STOKSEBELUM,$QTY,$STOKSEKARANG,$KETERANGAN);
        }
    }else{
        print 'error';
    }
    exit();
}
function buat_nota($idreturn){
    if ($idreturn > 0 AND $idreturn < 10){
        $no_nota = "RS000000".$idreturn;
    }elseif ($idreturn >= 10 AND $idreturn < 100){
        $no_nota = "RS00000".$idreturn;
    }elseif ($idreturn >= 100 AND $idreturn < 1000){
        $no_nota = "RS0000".$idreturn;
    }elseif ($idreturn >= 1000 AND $idreturn < 10000){
        $no_nota = "RS000".$idreturn;
    }elseif ($idreturn >= 10000 AND $idreturn < 100000){
        $no_nota = "RS00".$idreturn;
    }elseif ($idreturn >= 100000 AND $idreturn < 1000000){
        $no_nota = "RS0".$idreturn;
    }elseif ($idreturn >= 1000000){
        $no_nota = "RS".$idreturn;
    }
    return $no_nota;
}
function view_kasir_returnsupplier(){
    $kasirtabel ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_kasir">';
    $kasirtabel .= '<thead>';
    $kasirtabel .= '<tr>';
    $kasirtabel .= '<th class="tablebutton"></th>';
    $kasirtabel .= '<th class="tablebutton"></th>';
    $kasirtabel .= '<th>PRODUK</th>';
    $kasirtabel .= '<th>HARGA</th>';
    $kasirtabel .= '<th class="qty">QTY</th>';
    $kasirtabel .= '<th class="subtotal">SUBTOTAL</th>';
    $kasirtabel .= '<th class="qty">STOK</th>';
    $kasirtabel .= '</tr>';
    $kasirtabel .= '</thead>';
    $kasirtabel .= '<tbody>';
    $kasirtabel .= '</tbody>';
    $kasirtabel .= '</table>';
    return $kasirtabel;
}