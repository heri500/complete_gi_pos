<?php

function tabel_pengeluaran_view(){
	_addCustomCSS();
	_addNewDatabalesPlugins();
	_addBlockUiPlugins(true);
	$js_path = drupal_get_path('module','keuangan').'/js/tabel.pengeluaran.js';
	drupal_add_js($js_path);
	$idtabelkategori = 'tabel-kategori-pengeluaran';
	$idtabelpengeluaran = 'tabel-pengeluaran';
	$idtabelpemasukan = 'tabel-pemasukan';
	drupal_add_js(
		array(
			'idtabelkategori' => $idtabelkategori,
			'idtabelpengeluaran' => $idtabelpengeluaran,
			'idtabelpemasukan' => $idtabelpemasukan,
		),
		'setting');
	$tabelkategoripengeluaran = '<div class="small-table-wrapper">'.tabel_kategori_pengeluaran($idtabelkategori).'</div>';
	$tabelpengeluaran = '<div class="small-table-wrapper">'.tabel_pengeluaran($idtabelpengeluaran).'</div>';
	$tabelpemasukan = '<div class="small-table-wrapper">'.tabel_pemasukan($idtabelpemasukan).'</div>';
	$variables = array();
	$header = new stdClass();
	$header->title = 'Pemasukan';
	$header->tabid = 'tabpemasukan';
	$variables['element']['header'][0] = $header;
	$header = new stdClass();
	$header->title = 'Pengeluaran';
	$header->tabid = 'tabpengeluaran';
	$variables['element']['header'][1] = $header;
	$header = new stdClass();
	$header->title = 'Kategori Pemasukan/Pengeluaran';
	$header->tabid = 'tabkategori';
	$variables['element']['header'][2] = $header;
	$header = new stdClass();
	$header->title = 'Laporan Keuangan';
	$header->tabid = 'tablaporan';
	$variables['element']['header'][3] = $header;
	$content = new stdClass();
	$content->tabid = 'tabpemasukan';
	$formPemasukan = '<div id="form-input-pemasukan" class="small-form-wrapper">'.drupal_get_form('keuangan_input_pemasukan_form').'</div>';
	$content->tabcontent = $tabelpemasukan.$formPemasukan;
	$variables['element']['div'][0] = $content;
	$content = new stdClass();
	$content->tabid = 'tabpengeluaran';
	$formPengeluaran = '<div id="form-input-pengeluaran" class="small-form-wrapper">'.drupal_get_form('keuangan_input_form').'</div>';
	$content->tabcontent = $tabelpengeluaran.$formPengeluaran;
	$variables['element']['div'][1] = $content;
	$content = new stdClass();
	$content->tabid = 'tabkategori';
	$formkategori = '<div id="form-input-kategori" class="small-form-wrapper">'.drupal_get_form('keuangan_input_kategori_form').'</div>';
	$content->tabcontent = $tabelkategoripengeluaran.$formkategori;
	$variables['element']['div'][2] = $content;
	$content = new stdClass();
	$content->tabid = 'tablaporan';
	$pilihperiode = '<form action="'.base_path().'keuangan/report" method="post"><div id="pilihanperiode"><label>PILIH PERIODE LAPORAN </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.$tgl1.'">';
	$pilihperiode .= '<input type="hidden" id="edit-tgl1" name="edit-tgl1">';
	$pilihperiode .= '<input type="hidden" id="edit-tgl2" name="edit-tgl2">';
	$pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.$tgl2.'">'.$pilihantampil.'<button id="view-report">LIHAT LAPORAN</button></div></form>';
	$content->tabcontent = '<div>'.$pilihperiode.'<div id="tempat-report">&nbsp;</div><p>&nbsp;</p></div>';
	$variables['element']['div'][3] = $content;
	return theme('jquery_ui_tab',$variables);
}
function tabel_kategori_pengeluaran($idtabelkategori){
	$header_array = array(
		'edit' => array('data' => '&nbsp;', 'class' => 'tablebutton'),
		'kategori' => array('data' => 'Kategori', 'class' => 'center'),
		'jeniskategoriview' => array('data' => 'Jenis Kategori', 'class' => 'center'),
		'katerangan' => array('data' => 'Keterangan', 'class' => 'center'),
		'jeniskategori' => array('data' => '', 'class' => 'hidden'),
		'id' => array('data' => '', 'class' => 'hidden'),
	);
	$arraykategori = getKategoriPengeluaran();
	$rows_array = array();
	$jenisKategori = array('Pengeluaran', 'Pemasukkan');
	for ($i = 0;$i < count($arraykategori);$i++){
		$editbutton = '<img title="Klik untuk mengubah kategori pengeluaran" onclick="edit_kategori('.$arraykategori[$i]->id.', this.parentNode.parentNode);" src="'.base_path().'misc/media/images/edit.ico" width="22">';
		$cell_array = array();
		$cell_array['edit'] = array('data' => $editbutton, 'class' => 'center');
		$cell_array['kategori'] = array('data' => ucfirst($arraykategori[$i]->kategori), 'class' => 'align-left');
		$cell_array['jeniskategoriview'] = array('data' => $jenisKategori[$arraykategori[$i]->jeniskategori], 'class' => 'center');
		$cell_array['katerangan'] = array('data' => $arraykategori[$i]->keterangan, 'class' => 'align-left');
		$cell_array['jeniskategoriview'] = array('data' => $arraykategori[$i]->jeniskategori, 'class' => 'hidden');
		$cell_array['id'] = array('data' => $arraykategori[$i]->id, 'class' => 'hidden');
		$rows_array[] = $cell_array;
	}
	if (!count($rows_array)){
		$cell_array = array();
		$cell_array['edit'] = array('data' => '-', 'class' => 'center');
		$cell_array['jeniskategoriview'] = array('data' => '-', 'class' => 'center');
		$cell_array['kategori'] = array('data' => '-', 'class' => 'align-left');
		$cell_array['katerangan'] = array('data' => '-', 'class' => 'align-left');
		$cell_array['jeniskategori'] = array('data' => '-', 'class' => 'hidden');
		$cell_array['id'] = array('data' => '-', 'class' => 'hidden');
		$rows_array[0] = $cell_array;
	}
	return theme('table', $header_array, $rows_array, array('id' => $idtabelkategori,));
}
function tabel_pengeluaran($idtabelpengeluaran, $periode = null){
	$array_pengeluaran = getPengeluaranByPeriodeKategori(null,$periode);
	$array_kategori_pengeluaran = getKategoriPengeluaran();
	$arrayhari = arrayHari();
	$header_array = array(
		'edit' => array('data' => '&nbsp;', 'class' => 'tablebutton'),
		'hapus' => array('data' => '&nbsp;', 'class' => 'tablebutton'),
		'hari' => array('data' => 'Hari', 'class' => 'center'),
		'tanggal' => array('data' => 'Tanggal', 'class' => 'center'),
		'kategori' => array('data' => 'Kategori', 'class' => 'center'),
		'keterangan' => array('data' => 'Keterangan', 'class' => 'center'),
		'nilai' => array('data' => 'Nilai', 'class' => 'center'),
		'idkategori' => array('data' => '', 'class' => 'hidden'),
	);
	$rows_array = array();
	for ($i = 0;$i < count($array_pengeluaran);$i++){
		$editbutton = '<img title="Klik untuk mengubah pengeluaran" onclick="edit_pengeluaran('.$array_pengeluaran[$i]->id.', this.parentNode.parentNode);" src="'.base_path().'misc/media/images/edit.ico" width="22">';
		$deletebutton = '<img title="Klik untuk menghapus pengeluaran" onclick="hapus_pengeluaran('.$array_pengeluaran[$i]->id.');" src="'.base_path().'misc/media/images/del.ico" width="22">';
		$cell_array = array();
		$cell_array['edit'] = array('data' => $editbutton, 'class' => 'center');
		$cell_array['hapus'] = array('data' => $deletebutton, 'class' => 'center');
		$index_hari = date('w', $array_pengeluaran[$i]->tglpengeluaran);
		$tglpengeluaran = date('Y-m-d', $array_pengeluaran[$i]->tglpengeluaran);
		$cell_array['hari'] = array('data' => $arrayhari[$index_hari], 'class' => 'center');
		$cell_array['tanggal'] = array('data' => $tglpengeluaran, 'class' => 'center');
		$cell_array['kategori'] = array('data' => $array_pengeluaran[$i]->kategori, 'class' => 'align-left');
		$cell_array['katerangan'] = array('data' => $array_kategori_pengeluaran[$array_pengeluaran[$i]->keterangan], 'class' => 'center');
		$cell_array['nilai'] = array('data' => number_format($array_pengeluaran[$i]->nilai,0,',','.'), 'class' => 'angka');
		$cell_array['idkategori'] = array('data' => $array_pengeluaran[$i]->kategori, 'class' => 'hidden');
		$rows_array[] = $cell_array;
	}
	if (!count($rows_array)){
		$cell_array = array();
		$cell_array['edit'] = array('data' => '-', 'class' => 'center');
		$cell_array['hapus'] = array('data' => '-', 'class' => 'center');
		$cell_array['hari'] = array('data' => '-', 'class' => 'center');
		$cell_array['tanggal'] = array('data' => '-', 'class' => 'center');
		$cell_array['kategori'] = array('data' => '-', 'class' => 'center');
		$cell_array['katerangan'] = array('data' => '-', 'class' => 'align-left');
		$cell_array['nilai'] = array('data' => '-', 'class' => 'angka');
		$cell_array['idkategori'] = array('data' => '-', 'class' => 'hidden');
		$rows_array[0] = $cell_array;
	}
	return theme('table', $header_array, $rows_array, array('id' => $idtabelpengeluaran,'class' => 'display',));
}
function tabel_pemasukan($idtabelpemasukan, $periode = null){
	$array_pemasukan = getPemasukanByPeriodeKategori(null,$periode);
	$array_kategori_pemasukan = getKategoriPemasukan();
	$arrayhari = arrayHari();
	$header_array = array(
		'edit' => array('data' => '&nbsp;', 'class' => 'tablebutton'),
		'hapus' => array('data' => '&nbsp;', 'class' => 'tablebutton'),
		'hari' => array('data' => 'Hari', 'class' => 'center'),
		'tanggal' => array('data' => 'Tanggal', 'class' => 'center'),
		'kategori' => array('data' => 'Kategori', 'class' => 'center'),
		'keterangan' => array('data' => 'Keterangan', 'class' => 'center'),
		'nilai' => array('data' => 'Nilai', 'class' => 'center'),
		'idkategori' => array('data' => '', 'class' => 'hidden'),
	);
	$rows_array = array();
	for ($i = 0;$i < count($array_pemasukan);$i++){
		$editbutton = '<img title="Klik untuk mengubah pemasukan" onclick="edit_pemasukan('.$array_pemasukan[$i]->id.', this.parentNode.parentNode);" src="'.base_path().'misc/media/images/edit.ico" width="22">';
		$deletebutton = '<img title="Klik untuk menghapus pemasukan" onclick="hapus_pemasukan('.$array_pemasukan[$i]->id.');" src="'.base_path().'misc/media/images/del.ico" width="22">';
		$cell_array = array();
		$cell_array['edit'] = array('data' => $editbutton, 'class' => 'center');
		$cell_array['hapus'] = array('data' => $deletebutton, 'class' => 'center');
		$index_hari = date('w', $array_pemasukan[$i]->tglpemasukan);
		$tglpemasukan = date('Y-m-d', $array_pemasukan[$i]->tglpemasukan);
		$cell_array['hari'] = array('data' => $arrayhari[$index_hari], 'class' => 'center');
		$cell_array['tanggal'] = array('data' => $tglpemasukan, 'class' => 'center');
		$cell_array['kategori'] = array('data' => $array_pemasukan[$i]->kategori, 'class' => 'align-left');
		$cell_array['katerangan'] = array('data' => $array_kategori_pemasukan[$array_pemasukan[$i]->keterangan], 'class' => 'center');
		$cell_array['nilai'] = array('data' => number_format($array_pemasukan[$i]->nilai,0,',','.'), 'class' => 'angka');
		$cell_array['idkategori'] = array('data' => $array_pemasukan[$i]->kategori, 'class' => 'hidden');
		$rows_array[] = $cell_array;
	}
	if (!count($rows_array)){
		$cell_array = array();
		$cell_array['edit'] = array('data' => '-', 'class' => 'center');
		$cell_array['hapus'] = array('data' => '-', 'class' => 'center');
		$cell_array['hari'] = array('data' => '-', 'class' => 'center');
		$cell_array['tanggal'] = array('data' => '-', 'class' => 'center');
		$cell_array['kategori'] = array('data' => '-', 'class' => 'center');
		$cell_array['katerangan'] = array('data' => '-', 'class' => 'align-left');
		$cell_array['nilai'] = array('data' => '-', 'class' => 'angka');
		$cell_array['idkategori'] = array('data' => '-', 'class' => 'hidden');
		$rows_array[0] = $cell_array;
	}
	return theme('table', $header_array, $rows_array, array('id' => $idtabelpemasukan,'class' => 'display',));
}
function createLaporanKeuangan($tgl1 = null, $tgl2 = null){
	if (!empty($tgl1) && !empty($tgl2)){
		get_number_format($currencySym, $thousandSep, $decimalSep);
		$totalPenjualan = getTotalPenjualan('total',$tgl1,$tgl2);
		$totalPembelian = getTotalPembelian('total',$tgl1,$tgl2);
		$periodePemasukan = array('awal' => $tgl1, 'akhir' => $tgl2);
		$arrayPemasukan = getPemasukanByPeriodeKategori(null, $periodePemasukan);
		$pemasukanByKategori = array();
		for ($i = 0;$i < count($arrayPemasukan);$i++){
			$pemasukan = $arrayPemasukan[$i];
			if (isset($pemasukanByKategori[$pemasukan->kategori])){
				$pemasukanByKategori[$pemasukan->kategori] = $pemasukanByKategori[$pemasukan->kategori] + $pemasukan->nilai;
			}else{
				$pemasukanByKategori[$pemasukan->kategori] = $pemasukan->nilai;
			}
		}
		$arrayPengeluaran = getPengeluaranByPeriodeKategori(null, $periodePengeluaran);
		$pengeluaranByKategori = array();
		for ($i = 0;$i < count($arrayPengeluaran);$i++){
			$pengeluaran = $arrayPengeluaran[$i];
			if (isset($pengeluaranByKategori[$pengeluaran->kategori])){
				$pengeluaranByKategori[$pengeluaran->kategori] = $pengeluaranByKategori[$pengeluaran->kategori] + $pengeluaran->nilai;
			}else{
				$pengeluaranByKategori[$pengeluaran->kategori] = $pengeluaran->nilai;
			}
		}
		$kategoriPengeluaran = getKategoriPengeluaran();
		$arrayKategoriPengeluaran = array();
		if (count($kategoriPengeluaran)){
			foreach ($kategoriPengeluaran AS $kategori){
				$arrayKategoriPengeluaran[$kategori->id] = $kategori->kategori;
			}
		}
		$kategoriPemasukan = getKategoriPemasukan();
		$arrayKategoriPemasukan = array();
		if (count($kategoriPemasukan)){
			foreach ($kategoriPemasukan AS $kategori){
				$arrayKategoriPemasukan[$kategori->id] = $kategori->kategori;
			}
		}
		$header_array = array(
			'keterangan' => array('data' => 'Keterangan'),
			'total' => array('data' => 'Sub Total', 'class' => 'angka'),
		);
		$rows_array = array();
		$rows_array[] = array(
			'keterangan' => array('data' => 'Total Penjualan'),
			'total' => array('data' => number_format($totalPenjualan,2,$decimalSep,$thousandSep), 'class' => 'angka'),
		);
		$totalPembayaranHutang = getTotalPembayaran(null,$periodePemasukan);
		$rows_array[] = array(
			'keterangan' => array('data' => 'Total Pembayaran Hutang'),
			'total' => array('data' => number_format($totalPembayaranHutang,2,$decimalSep,$thousandSep), 'class' => 'angka'),
		);
		$grandTotalPemasukan = $totalPenjualan + $totalPembayaranHutang;
		if (count($pemasukanByKategori)){
			$grandTotalPemasukanSaja = 0;
			foreach ($pemasukanByKategori AS $idKategori => $totalPemasukan){
				$cell_array = array();
				$keteranganPemasukan = '-';
				if (isset($arrayKategoriPemasukan[$idKategori])){
					$keteranganPemasukan = $arrayKategoriPemasukan[$idKategori];
				}
				$cell_array['keterangan'] = array('data' => $keteranganPemasukan);
				$cell_array['total'] = array('data' => number_format($totalPemasukan,2,$decimalSep,$thousandSep), 'class' => 'angka');
				$rows_array[] = $cell_array;
				$grandTotalPemasukan = $grandTotalPemasukan + $totalPemasukan;
				$grandTotalPemasukanSaja = $grandTotalPemasukanSaja + $totalPemasukan;
			}
		}
		$cell_array['keterangan'] = array('data' => '<strong>Grand Total Pemasukan</strong>');
		$cell_array['total'] = array('data' => '<strong>'.number_format($grandTotalPemasukan,2,$decimalSep,$thousandSep).'</strong>', 'class' => 'angka');
		$rows_array[] = $cell_array;
		$laporanPemasukan = theme('table', $header_array, $rows_array, array('id' => 'laporan-pemasukan','class' => 'display',));
		$rows_array = array();
		$rows_array[] = array(
			'keterangan' => array('data' => 'Total Pembelian'),
			'total' => array('data' => number_format($totalPembelian,2,$decimalSep,$thousandSep), 'class' => 'angka'),
		);
		$totalPembayaranSupplier = getTotalPembayaranSupplier(null, $periodePemasukan);
		$rows_array[] = array(
			'keterangan' => array('data' => 'Total Pembayaran Hutang'),
			'total' => array('data' => number_format($totalPembayaranSupplier,2,$decimalSep,$thousandSep), 'class' => 'angka'),
		);
		$grandTotalPengeluaran = $totalPembelian + $totalPembayaranSupplier;
		if (count($pengeluaranByKategori)){
			$grandTotalPengeluaranSaja = 0;
			foreach ($pengeluaranByKategori AS $idKategori => $totalPengeluaran){
				$cell_array = array();
				$keteranganPengeluaran = '-';
				if (isset($arrayKategoriPengeluaran[$idKategori])){
					$keteranganPengeluaran = $arrayKategoriPengeluaran[$idKategori];
				}
				$cell_array['keterangan'] = array('data' => strtoupper($keteranganPengeluaran));
				$cell_array['total'] = array('data' => number_format($totalPengeluaran,2,$decimalSep,$thousandSep), 'class' => 'angka');
				$rows_array[] = $cell_array;
				$grandTotalPengeluaran = $grandTotalPengeluaran + $totalPengeluaran;
				$grandTotalPengeluaranSaja = $grandTotalPengeluaranSaja + $totalPengeluaran;
			}
		}
		$cell_array['keterangan'] = array('data' => '<strong>Grand Total Pengeluaran</strong>');
		$cell_array['total'] = array('data' => '<strong>'.number_format($grandTotalPengeluaran,2,$decimalSep,$thousandSep).'</strong>', 'class' => 'angka');
		$rows_array[] = $cell_array;
	}
	$laporanPengeluaran = theme('table', $header_array, $rows_array, array('id' => 'laporan-pengeluaran','class' => 'display',));
	$totalCashPembelian = getTotalPembelian('total',$tgl1,$tgl2,1);
	$totalCashPenjualan = getTotalPenjualan('total',$tgl1,$tgl2,1) + $grandTotalPemasukanSaja + $totalPembayaranHutang - $totalPembayaranSupplier - $totalCashPembelian - $grandTotalPengeluaranSaja;
	$totalHutangPenjualan = getTotalPenjualan('total',$tgl1,$tgl2,2);
	$totalDebitPenjualan = getTotalPenjualan('total',$tgl1,$tgl2,3);
	$totalDepositPenjualan = getTotalPenjualan('total',$tgl1,$tgl2,4);
	$reportData = array(
		$laporanPemasukan,$laporanPengeluaran,$grandTotalPemasukan,
		$grandTotalPengeluaran,$totalCashPenjualan,$totalHutangPenjualan,
		$totalDebitPenjualan,$totalDepositPenjualan
	);
	return $reportData;
}
function createReportAjax(){
	$reportView = '<div class="report-view">';
	if (isset($_POST['tglawal']) && isset($_POST['tglakhir'])){
		get_number_format($currencySym, $thousandSep, $decimalSep,false);
		$arrayLaporan = createLaporanKeuangan($_POST['tglawal'],$_POST['tglakhir']);
		$reportView .= '<div class="kolom-pemasukan">'.$arrayLaporan[0].'</div>';
		$reportView .= '<div class="kolom-pengeluaran">'.$arrayLaporan[1].'</div>';
		$reportView .= '<div class="kolom-grand-total">Pendapatan Bersih : '.$currencySym.' '.number_format($arrayLaporan[2] - $arrayLaporan[3],2,$decimalSep,$thousandSep).'</div>';
		$reportView .= '<div class="kolom-grand-total-2">Total Cash: '.$currencySym.' '.number_format($arrayLaporan[4],2,$decimalSep,$thousandSep).'  <div style="font-size: 10px;">(Cash Penjualan + Pemasukan Lain + Pembayaran Hutang Member - Pembayaran Hutang Ke Supplier - Cash Pembelian - Pengeluaran Cash)</div></div>';
		$reportView .= '<div class="kolom-grand-total-2">Total Piutang: '.$currencySym.' '.number_format(abs($arrayLaporan[5]),2,$decimalSep,$thousandSep).'</div>';
		$reportView .= '<div class="kolom-grand-total-2">Total Debit/Giro: '.$currencySym.' '.number_format($arrayLaporan[6],2,$decimalSep,$thousandSep).'</div>';
		$reportView .= '<div class="kolom-grand-total-2">Total Penggunaan Deposit: '.$currencySym.' '.number_format($arrayLaporan[7],2,$decimalSep,$thousandSep).'</div>';
	}
	$reportView .= '</div>';
	print $reportView;
	exit();
}