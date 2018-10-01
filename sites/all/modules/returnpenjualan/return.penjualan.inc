<?php

function returnPenjualan($idpenjualan = null, $alamatasal = null){
    $returnpenjualan = '';
    if (!empty($idpenjualan)){
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
        $idPelanggan = 0;
        if (!empty($idpenjualan)){
            if (empty($alamatasal)){
                $alamatasal = 'viewlaundry';
            }
            drupal_add_js(
                array(
                    'idtitipanlaundry' => $idpenjualan,
                    'alamatasal' => $alamatasal,
                ),
                'setting');
            if ($alamatasal == 'viewlaundry' || $alamatasal == 'laundrykeluar'){
                $titipanlaundry = db_fetch_array(db_query("SELECT idtitipanlaundry, nonota, tglpenjualan, idpemakai,
			idpelanggan, total, totalmodal, carabayar, bayar, kembali, nokartu, keterangan, insert_date, status_laundry,
			users.name,laundry.nomer_rak FROM titipanlaundry laundry LEFT JOIN cms_users users ON laundry.idpemakai = users.uid
			WHERE idtitipanlaundry=%d",$idpenjualan));
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
                $titipanlaundry = db_fetch_array(db_query("SELECT id, nonota, tglorder, idpemakai,
			idpelanggan, total, totalmodal, carabayar, bayar, kembali, nokartu, keterangan, insert_date, status_order,
			users.name FROM customer_order custord LEFT JOIN cms_users users ON custord.idpemakai = users.uid
			WHERE id=%d",$idpenjualan));
                if (count($titipanlaundry)){
                    $tgllaundry = explode(' ', $titipanlaundry['tglorder']);
                    $splittgllaundry = explode('-',$tgllaundry[0]);
                    $tglTampilLaundry = $splittgllaundry[2].'-'.$splittgllaundry[1].'-'.$splittgllaundry[0];
                    $idPelanggan = $titipanlaundry['idpelanggan'];
                    $kasirlaundry = $titipanlaundry['name'];
                    $labelKasir = 'ORDER';
                    $labelTgl = 'ORDER';
                }
            }
        }
        $tglsekarang = date("Y-m-d");
        $tgltampil = date("d-m-Y");
        drupal_add_js(
            array(
                'tglsekarang' => $tglsekarang,
                'tgltampil' => $tgltampil,
                'logo' => $logoPath,
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
        $tombollihatomset = '<a class="buttonomset" onclick="hitung_omset();">F8:OMSET</a>';
        $returnpenjualan = '<button onclick="munculkankasir();" style="font-size:12px;border: 1px solid orangered;">Klik untuk memunculkan kasir</button>';
        $returnpenjualan .= '<div id="dialogkasir" style="font-size: 12px;" title="GLOBAL POS KASIR ['.$tanggal.']">';
        $returnpenjualan .= '<div id="info-kasir-waktu">';
        $returnpenjualan .= '<div id="tempattanggalkasir">';
        $returnpenjualan .= '<div id="tempattanggal">';
        $returnpenjualan .= '<label>TANGGAL</label><input readonly="readonly" type="text" id="tgljual" name="tgljual" value="'.$tanggal.'">';
        $returnpenjualan .= '<input type="hidden" id="tgljualkirim" name="tgljualkirim" value="'.$tanggalkirim.'">';
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div id="tempatnamakasir">';
        global $user;
        $returnpenjualan .= '<label>KASIR</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$user->name.'">';
        $returnpenjualan .= '</div>';
        if (!empty($idpenjualan)){
            $returnpenjualan .= '<div id="tempattanggal">';
            $returnpenjualan .= '<label>KASIR '.$labelKasir.'</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$kasirlaundry.'">';
            $returnpenjualan .= '</div>';
        }
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div id="tempatjam" align="center">';
        $returnpenjualan .= 'Jam';
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div id="tempatkosong">';
        $returnpenjualan .= '<div id="tempatsupplier"><label>PELANGGAN</label></div>';
        $returnpenjualan .= '<div id="tempatsupplier2">'.createPelangganSelection($_GET['idpelanggan']).'</div>';
        if (!empty($idpenjualan)){
            $returnpenjualan .= '<div id="tempatsupplier"><label>TGL '.$labelTgl.'</label><input readonly="readonly" type="text" id="tglmasuk" name="tglmasuk" value="'.$tglTampilLaundry.'">';
            $returnpenjualan .= '</div>';
        }
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div id="barcode_place">SCAN ATAU INPUT BARCODE, KODE ATAU NAMA PRODUK : <input type="text" id="barcode" name="barcode" class="barcodekasir"></div>';
        $tablekasir = view_kasir($idpenjualan,$alamatasal);
        $returnpenjualan .= '<div id="tempattabelkasir">'.$tablekasir.'</div>';
        $returnpenjualan .= '<input type="hidden" id="last_id" name="last_id">';
        $returnpenjualan .= '<input type="hidden" id="lastqty" name="lastqty" value="1">';
        $returnpenjualan .= '<input type="hidden" id="lastharga" name="lastharga">';
        $returnpenjualan .= '<input type="hidden" id="lastdiskon" name="lastdiskon">';
        $returnpenjualan .= '<input type="hidden" id="nilaikirim" name="nilaikirim">';
        //$returnpenjualan .= '<div id="tempattombolkasir">';
        if ($nomerRak != ''){
            $returnpenjualan .= '<div id="tempattombolkasir" style="height:330px;">';
            $returnpenjualan .= '<a style="padding: 0.4em 0.3em;margin-top: 2px;margin-bottom: 4px;" class="buttonprint" onclick="preventDefault();">NO.RAK : '.$nomerRak.'</a>';
        }else{
            $returnpenjualan .= '<div id="tempattombolkasir">';
        }
        $returnpenjualan .= $panduantombol;
        $returnpenjualan .= $tombolcari;
        $returnpenjualan .= $tombolqty;
        $returnpenjualan .= $tombolhapus;
        $returnpenjualan .= $tombolselesai;
        $returnpenjualan .= $tombolprint;
        //$returnpenjualan .= $tombolprint;
        //$returnpenjualan .= $tomboltutupkasir;
        $returnpenjualan .= $tombollihatomset;
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div id="tempattotal"><div id="totalbelanja">Total Transaksi : Rp. 0</div></div>';
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div title="Informasi" id="dialogwarning">';
        $returnpenjualan .= '<p id="pesantext">Produk yang dicari tidak ada, Silahkan masukkan Barcode/Kode Produk/Nama Produk yang lain...!!!</p>';
        $returnpenjualan .= '<p style="text-align: center"><button id="tutupdialog" class="buttonwarning" onclick="tutupwarning();">CLOSE</button></p>';
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div title="UBAH HARGA" id="dialogubahharga">';
        $returnpenjualan .= '<label>Harga Baru : </label><input type="text" id="newharga" name="newharga" value="1">';
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div title="UBAH QTY" id="dialogubahqty">';
        $returnpenjualan .= '<label>Qty Baru : </label><input type="text" id="newqty" name="newqty" value="1">';
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div title="UBAH QTY" id="dialogubahqty2">';
        $returnpenjualan .= '<label>Qty Baru : </label><input type="text" id="newqty2" name="newqty2">';
        $returnpenjualan .= '</div>';
        $returnpenjualan .= '<div title="PEMBAYARAN" id="dialogbayar">';
        $carabayar = '<select id="carabayar" name="carabayar">';
        $result = db_query("SELECT carabayar FROM carabayar");
        $i = 0;
        while($data = db_fetch_object($result)){
            if ($i == 0){
                $carabayar .= '<option selected="selected" value="'.$data->carabayar.'">'.$data->carabayar.'</option>';
            }else{
                $carabayar .= '<option value="'.$data->carabayar.'">'.$data->carabayar.'</option>';
            }
            $i++;
        }
        $carabayar .= '</select>';
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
        $nilaibayar = '<input type="text" id="nilaibayar" name="nilaibayar">';
        $depositpelanggan = '<input type="text" id="depositpelanggan" name="depositpelanggan" readonly="readonly">';
        $nokartudebit = '<input type="text" id="nomerkartu" name="nomerkartu">';
        $kembali = '<input readonly="readonly" type="text" id="kembali" name="kembali">';
        $totalbelanja = '<input type="text" id="totalbelanjauser" name="totalbelanjauser">';
        //$returnpenjualan .= '<div class="barisbayar"><label>Pelanggan</label>'.$pelanggan.'</div>';
        $returnpenjualan .= '<div class="barisbayar"><label>Total Belanja</label>'.$totalbelanja.'</div>';
        $returnpenjualan .= '<div class="barisbayar"><label>Cara Bayar</label>'.$carabayar.'</div>';
        $returnpenjualan .= '<div id="field_no_kartu" class="barisbayar" style="display: none;"><label>No. Kartu/Giro</label>'.$nokartudebit.'</div>';
        $returnpenjualan .= '<div class="barisbayar"><label id="label-deposit">Deposit</label>'.$depositpelanggan.'</div>';
        $returnpenjualan .= '<div class="barisbayar"><label>Pembayaran</label>'.$nilaibayar.'</div>';
        $returnpenjualan .= '<div id="field_kembali" class="barisbayar"><label>Kembali</label>'.$kembali.'</div>';
        $returnpenjualan .= '<div id="field_kembalian" class="barisbayar" style="display: none;"><label>Kembalian</label>'.$perlakuankembalian.'</div>';
        $returnpenjualan .= '</div>';
    }
    return $returnpenjualan;
}