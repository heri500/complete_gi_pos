<?php

/*
 * DataTables example server-side processing script.
 *
 * Please note that this script is intentionally extremely simply to show how
 * server-side processing can be implemented, and probably shouldn't be used as
 * the basis for a large complex system. It is suitable for simple use cases as
 * for learning.
 *
 * See http://datatables.net/usage/server-side for full details on the server-
 * side processing requirements of DataTables.
 *
 * @license MIT - http://datatables.net/license_mit
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Easy set variables
 */
$drupal_root_dir = '../../../../';

# make your drupal directory php's current directory
chdir($drupal_root_dir);

# not shown in the book, i had to set this constant
define('DRUPAL_ROOT', $drupal_root_dir);

# bootstrap drupal up to the point the database is loaded
include_once('./includes/bootstrap.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
$baseDirectory = '';
if (!file_exists('/misc/media/images/forward_enabled.ico')){
	$baseDirectory = '..';
}


function serverSidePelanggan($request){
	global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
	get_number_format_server_side($currencySym, $thousandSep, $decimalSep);
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		'','','','plg.kodepelanggan','plg.namapelanggan','plg.telp', 'plg.alamat',
		'plg.email','ptg.besarhutang','ptg.pembayaranterakhir','bayarterakhir'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
	if (is_null($pageLength)){
		$pageLength = 25;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT plg.idpelanggan,plg.kodepelanggan,plg.namapelanggan,plg.telp,plg.alamat,plg.email,";
	$strSQLFilteredTotal = "SELECT COUNT(plg.idpelanggan) ";
	$strSQL .= "ptg.besarhutang, ptg.pembayaranterakhir, SUBSTR(ptg.last_update,1,10) AS bayarterakhir ";
	$strSQL .= "FROM pelanggan AS plg ";
	$strSQLFilteredTotal .= "FROM pelanggan AS plg ";
	$strSQL .= "LEFT JOIN piutang AS ptg ON ptg.idpelanggan = plg.idpelanggan WHERE 1=1 ";
	$strSQLFilteredTotal .= "LEFT JOIN piutang AS ptg ON ptg.idpelanggan = plg.idpelanggan WHERE 1=1 ";
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (plg.kodepelanggan LIKE '%%%s%%' OR plg.namapelanggan LIKE '%%%s%%' ";
		$strCriteria .= "OR plg.alamat LIKE '%%%s%%' OR plg.email LIKE '%%%s%%')";
	}
	$strSQL .= $strCriteria." ORDER BY $orderColumn LIMIT %d, %d";
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		$result = db_query($strSQL,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$searchQuery,$searchQuery,$searchQuery,$searchQuery));
	}else{
		$result = db_query($strSQL,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal));
	}
	$output = array();
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$lihathutang = "<img title=\"Klik untuk melihat detail hutang\" src=\"$baseDirectory/misc/media/images/forward_enabled.ico\" onclick=\"view_detail_hutang(".$data->idpelanggan.",'".$data->namapelanggan."','".$data->besarhutang."');\">";
		$bayarhutang = "<img title=\"Klik untuk mengisi form pembayaran\" src=\"$baseDirectory/misc/media/images/money2_24.png\" onclick=\"pembayaran(".$data->idpelanggan.",'".$data->namapelanggan."','".$data->besarhutang."','".$data->besarhutang."');\">";
		$lihatdiskon = "<img title=\"Klik untuk melihat dan menambah diskon pelanggan\" src=\"$baseDirectory/misc/media/images/diskon.png\" onclick=\"tabeldiskon(".$data->idpelanggan.",'".$data->namapelanggan."')\">";
		$rowData[] = $lihathutang;
		$rowData[] = $bayarhutang;
		$rowData[] = $lihatdiskon;
		$rowData[] = $data->kodepelanggan;
		$rowData[] = $data->namapelanggan;
		$rowData[] = $data->telp;
		$rowData[] = $data->alamat;
		$rowData[] = $data->email;
		$rowData[] = number_format($data->besarhutang,$DDigit,$decimalSep,$thousandSep);
		$rowData[] = number_format($data->pembayaranterakhir,$DDigit,$decimalSep,$thousandSep);
		$rowData[] = $data->bayarterakhir;
        $rowData[] = $data->idpelanggan;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(db_query("SELECT COUNT(idpelanggan) FROM pelanggan"));
	return array(
			"draw"            => isset ( $request['draw'] ) ?
				intval( $request['draw'] ) :
				0,
			"recordsTotal"    => intval( $recordsTotal ),
			"recordsFiltered" => intval( $recordsFiltered ),
			"data"            => $output
		);
}
function serverSideProduk($request){
	global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
	get_number_format_server_side($currencySym, $thousandSep, $decimalSep);
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		'prod.idproduct', 'kat.kategori','subkat.subkategori','supp.namasupplier', 'prod.barcode',
		'prod.alt_code','prod.namaproduct','prod.hargapokok','prod.hargajual','prod.margin'
		,'prod.minstok','prod.maxstok','prod.stok','prod.stok','prod.satuan','prod.keterangan','prod.stok*prod.hargajual',
        'gmenu.nama_grup'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
	if (is_null($pageLength)){
		$pageLength = 25;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT prod.idproduct,gmenu.nama_grup,prod.idsupplier,prod.idkategori,prod.idsubkategori,prod.barcode,prod.alt_code,";
	$strSQL .= "prod.namaproduct,prod.hargapokok,prod.hargajual,prod.margin,prod.minstok,prod.maxstok,";
	$strSQL .= "prod.stok,prod.satuan,prod.berat,prod.keterangan ,";
	$strSQL .= "kat.kategori, subkat.subkategori, prod.stok*prod.hargajual AS total_nilai, ";
    $strSQL .= "(SELECT GROUP_CONCAT(namasupplier SEPARATOR '<br/>') FROM product_supplier ";
    $strSQL .= "AS ps LEFT JOIN supplier supp ON ps.idsupplier = supp.idsupplier ";
    $strSQL .= "WHERE ps.idproduct = prod.idproduct GROUP BY ps.idproduct) AS namasupplier ";
	$strSQL .= "FROM product AS prod ";
    $strSQL .= "LEFT JOIN kategori AS kat ON kat.idkategori = prod.idkategori ";
	$strSQL .= "LEFT JOIN subkategori AS subkat ON subkat.idsubkategori = prod.idsubkategori ";
    $strSQL .= "LEFT JOIN grup_menu AS gmenu ON prod.id_grup_menu = gmenu.id ";
	$strSQL .= "WHERE 1=1 ";
    $strSQLFilteredTotal = "SELECT COUNT(prod.idproduct) ";
    $strSQLFilteredTotal .= "FROM product AS prod ";
    $strSQLFilteredTotal .= "LEFT JOIN kategori AS kat ON kat.idkategori = prod.idkategori ";
	$strSQLFilteredTotal .= "LEFT JOIN subkategori AS subkat ON subkat.idsubkategori = prod.idsubkategori ";
	//$strSQLFilteredTotal .= "LEFT JOIN supplier AS supp ON supp.idsupplier = prod.idsupplier ";
	$strSQLFilteredTotal .= "LEFT JOIN grup_menu AS gmenu ON prod.id_grup_menu = gmenu.id ";
	$strSQLFilteredTotal .= "WHERE 1=1 ";
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (prod.alt_code LIKE '%%%s%%' OR prod.barcode LIKE '%%%s%%' ";
		$strCriteria .= "OR prod.namaproduct LIKE '%%%s%%' OR kat.kategori LIKE '%%%s%%' ";
		$strCriteria .= "OR kat.kodekategori LIKE '%%%s%%' OR subkat.subkategori LIKE '%%%s%%' ";
		$strCriteria .= "OR subkat.kodesubkategori LIKE '%%%s%%' ";
		//$strCriteria .= "OR namasupplier LIKE '%%%s%%' OR gmenu.nama_grup LIKE '%%%s%%' ";
        $strCriteria .= "OR (SELECT GROUP_CONCAT(namasupplier SEPARATOR '<br/>') FROM product_supplier ";
        $strCriteria .= "AS ps LEFT JOIN supplier supp ON ps.idsupplier = supp.idsupplier ";
        $strCriteria .= "WHERE ps.idproduct = prod.idproduct GROUP BY ps.idproduct) LIKE '%%%s%%' ";
        $strCriteria .= ") ";
	}
	if (isset($_REQUEST['statusstok']) && $_REQUEST['statusstok'] != '0'){
		if ($_REQUEST['statusstok'] == 'aman'){
			$strCriteria .= "AND (stok >= prod.minstok AND stok <= prod.maxstok && prod.minstok != prod.maxstok) ";
		}else if($_REQUEST['statusstok'] == 'maksimum'){
			$strCriteria .= "AND (stok > prod.maxstok) ";
		}else if($_REQUEST['statusstok'] == 'minimum'){
			$strCriteria .= "AND (stok < prod.minstok) ";
		}
	}
	if (isset($_REQUEST['status_product'])){
		$strCriteria .= "AND (status_product = ".$_REQUEST['status_product'].") ";
	}else{
		$strCriteria .= "AND (status_product = 1) ";
	}
    if (isset($_REQUEST['idkategori']) && !empty($_REQUEST['idkategori'])){
        $strCriteria .= "AND (prod.idkategori = ".$_REQUEST['idkategori'].") ";
    }
	if ($pageLength != '-1'){
		$strSQL .= $strCriteria." ORDER BY $orderColumn LIMIT %d, %d";
	}else{
		$strSQL .= $strCriteria." ORDER BY $orderColumn";
	}
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		$result = db_query($strSQL,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery));
	}else{
		$result = db_query($strSQL,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal));
	}
	$output = array();
	$totalNilaiBarang = 0;
	while ($data = db_fetch_object($result)) {
        $rowData = array();
        $imgEdit = "<img title=\"Edit Produk " . $data->namaproduct . "\" src=\"$baseDirectory/misc/media/images/edit.ico\" onclick=\"editproduk(" . $data->idproduct . ")\">";
        $rowData[] = $imgEdit;
        $rowData[] = $data->kategori;
        $rowData[] = $data->subkategori;
        $rowData[] = $data->namasupplier;
        $rowData[] = $data->barcode;
        $rowData[] = $data->alt_code;
        $rowData[] = $data->namaproduct;
        $rowData[] = number_format($data->hargapokok, $DDigit, $decimalSep, $thousandSep);
        $rowData[] = number_format($data->hargajual, $DDigit, $decimalSep, $thousandSep);
        $rowData[] = number_format($data->margin, $DDigit, $decimalSep, $thousandSep);
        $rowData[] = $data->minstok;
        $rowData[] = $data->maxstok;
        $rowData[] = number_format($data->stok, 2, $decimalSep, $thousandSep);
        if ($data->stok < $data->minstok) {
            $rowData[] = "<img title=\"Stok dibawah minimum\" src=\"$baseDirectory/misc/media/images/statusmerah.png\">";
        } elseif ($data->stok > $data->maxstok) {
            $rowData[] = "<img title=\"Stok berlebihan/diatas maksimum stok\" src=\"$baseDirectory/misc/media/images/statuskuning.png\">";
        } elseif ($data->stok <= $data->maxstok AND $data->stok >= $data->minstok AND $data->stok > 0) {
            $rowData[] = "<img title=\"Stok aman\" src=\"$baseDirectory/misc/media/images/statushijau.png\">";
        } elseif ($data->stok <= 0) {
            $rowData[] = "<img title=\"Stok Kosong\" src=\"$baseDirectory/misc/media/images/statusmerah.png\">";
        }
        $rowData[] = $data->satuan;
        $rowData[] = $data->keterangan;
        $rowData[] = number_format($data->total_nilai, $DDigit, $decimalSep, $thousandSep);
        $rowData[] = $data->nama_grup;
        $rowData[] = '<input type="text" id="print-' . $data->idproduct . '" name="print-' . $data->idproduct . '" class="total-print" value="' . $data->stok . '" size="2">';
        $rowData[] = '<input class="barcode-select" type="checkbox" id="check-' . $data->idproduct . '" name="check-' . $data->idproduct . '" value="' . $data->idproduct . '">';
        $imgHargaSupp = "<img width=\"20\" title=\"Edit Harga Produk " . $data->namaproduct . " per supplier\" src=\"$baseDirectory/misc/media/images/document.ico\" onclick=\"edithargaproduk(" . $data->idproduct . ")\">";
        $rowData[] = $imgHargaSupp;
        $imgDiscount = "<img width=\"20\" title=\"Harga discount\" src=\"$baseDirectory/misc/media/images/diskon.png\" onclick=\"discount_produk(" . $data->idproduct . ")\">";
        $rowData[] = $imgDiscount;
        $totalNilaiBarang = $totalNilaiBarang + $data->total_nilai;
        $rowData[] = $data->idproduct;
        $output[] = $rowData;
    }
	$recordsTotal = db_result(db_query("SELECT COUNT(idproduct) FROM product"));
	return array(
			"draw"            => isset ( $request['draw'] ) ?
				intval( $request['draw'] ) :
				0,
			"recordsTotal"    => intval( $recordsTotal ),
			"recordsFiltered" => intval( $recordsFiltered ),
			"data"            => $output,
			"sql" 			  => $strSQL,
		);
}

function serverSidePenjualan($request){
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
	get_number_format_server_side($currSym,$tSep,$dSep);
	global $baseDirectory;
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$tglAwal = $_REQUEST['tglawal'].' 00:00';
	$tglAkhir = $_REQUEST['tglakhir'].' 23:59';
	$idpelanggan = $_REQUEST['idpelanggan'];
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		'penj.idpenjualan','penj.nonota','penj.tglpenjualan','penj.tglpenjualan',
		'penj.total','penj.ppn_value','penj.total_plus_ppn','penj.totalmodal','(penj.total - penj.totalmodal)','penj.carabayar',
		'penj.bayar','penj.kembali','user.name','plg.namapelanggan'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
	if (is_null($pageLength) || $pageLength != -1){
		$pageLength = 100;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT penj.idpenjualan,penj.nonota,SUBSTR(penj.tglpenjualan,1,10) AS tanggal,";
	$strSQL .= "SUBSTR(penj.tglpenjualan,11,9) AS waktu, penj.idpemakai,penj.total,penj.totalmodal,";
	$strSQL .= "(penj.total-penj.totalmodal) AS laba, penj.carabayar,penj.bayar,penj.kembali,";
	$strSQL .= "(penj.total * (penj.ppn/100)) AS ppn_value, penj.total_plus_ppn, ";
	$strSQL .= "penj.nokartu,penj.keterangan,penj.insert_date, user.name, ";
    $strSQL .= "penj.idpelanggan, plg.namapelanggan ";
	$strSQL .= "FROM penjualan AS penj ";
	$strSQL .= "LEFT JOIN cms_users AS user ON user.uid = penj.idpemakai ";
	$strSQL .= "LEFT JOIN pelanggan AS plg ON plg.idpelanggan = penj.idpelanggan ";
	if (empty($idpelanggan)){
		$strSQL .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' ";
	}else{
		$strSQL .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' AND penj.idpelanggan=%d ";
	}
    $strSQLFilteredTotal = "SELECT COUNT(penj.idpenjualan) ";
    $strSQLFilteredTotal .= "FROM penjualan AS penj ";
	$strSQLFilteredTotal .= "LEFT JOIN cms_users AS user ON user.uid = penj.idpemakai ";
	$strSQLFilteredTotal .= "LEFT JOIN pelanggan AS plg ON plg.idpelanggan = penj.idpelanggan ";
	if (empty($idpelanggan)){
		$strSQLFilteredTotal .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' ";
	}else{
		$strSQLFilteredTotal .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' AND penj.idpelanggan=%d ";
	}
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (penj.nonota LIKE '%%%s%%' OR SUBSTR(penj.tglpenjualan,1,10) LIKE '%%%s%%' ";
		$strCriteria .= "OR SUBSTR(penj.tglpenjualan,11,9) LIKE '%%%s%%' OR user.name LIKE '%%%s%%' ";
		$strCriteria .= "OR plg.namapelanggan LIKE '%%%s%%' OR penj.carabayar LIKE '%%%s%%' ";
		$strCriteria .= ")";
	}
	if ($pageLength != -1) {
		$strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
	}else{
		$strSQL .= $strCriteria . " ORDER BY $orderColumn";
	}
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		if (empty($idpelanggan)) {
			$result = db_query(
				$strSQL,
				$tglAwal,
				$tglAkhir,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$firstRecord,
				$lastRecord
			);
			$recordsFiltered = db_result(
				db_query(
					$strSQLFilteredTotal,
					$tglAwal,
					$tglAkhir,
					$searchQuery,
					$searchQuery,
					$searchQuery,
					$searchQuery,
					$searchQuery,
					$searchQuery
				)
			);
		}else{
			$result = db_query(
				$strSQL,
				$tglAwal,
				$tglAkhir,
				$idpelanggan,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$firstRecord,
				$lastRecord
			);
			$recordsFiltered = db_result(
				db_query(
					$strSQLFilteredTotal,
					$tglAwal,
					$tglAkhir,
					$idpelanggan,
					$searchQuery,
					$searchQuery,
					$searchQuery,
					$searchQuery,
					$searchQuery,
					$searchQuery
				)
			);
		}
	}else{
		if (empty($idpelanggan)) {
			$result = db_query($strSQL, $tglAwal, $tglAkhir, $firstRecord, $lastRecord);
			$recordsFiltered = db_result(db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir));
		}else{
			$result = db_query($strSQL, $tglAwal, $tglAkhir, $idpelanggan, $firstRecord, $lastRecord);
			$recordsFiltered = db_result(db_query($strSQLFilteredTotal, $idpelanggan, $tglAwal, $tglAkhir));
		}
	}
	$output = array();
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$imgDetail = "<img title=\"Klik untuk melihat detail penjualan\" onclick=\"view_detail(".$data->idpenjualan.",'".$data->nonota."',".$data->idpelanggan.");\" src=\"$baseDirectory/misc/media/images/forward_enabled.ico\">";
		$rowData[] = $imgDetail;
		$rowData[] = $data->nonota;
		$rowData[] = $data->tanggal;
		$rowData[] = $data->waktu;
		$rowData[] = number_format($data->total,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->ppn_value,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->total_plus_ppn,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->totalmodal,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->laba,$DDigit,$dSep,$tSep);
		$rowData[] = $data->carabayar;
		$rowData[] = number_format($data->bayar,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->kembali,$DDigit,$dSep,$tSep);
		$rowData[] = $data->name;
		$rowData[] = $data->namapelanggan;
		$tombolprint = "<img title=\"Klik untuk mencetak nota penjualan\" onclick=\"print_penjualan(".$data->idpenjualan.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/print.png\" width=\"22\">";
		$rowData[] = $tombolprint;
        $tombolprint2 = "<img title=\"Klik untuk mencetak faktur penjualan\" onclick=\"print_faktur(".$data->idpenjualan.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/printer2.png\" width=\"22\">";
        $rowData[] = $tombolprint2;
        $TombolDelete = "<img title=\"Klik untuk menghapus penjualan\" onclick=\"delete_penjualan(".$data->idpenjualan.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/delete.ico\" width=\"22\">";
        $rowData[] = $TombolDelete;
		$rowData[] = $data->idpenjualan;
		$output[] = $rowData;
	}
	if (empty($idpelanggan)) {
		$recordsTotal = db_result(
			db_query(
				"SELECT COUNT(idpenjualan) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s'",
				$tglAwal,
				$tglAkhir
			)
		);
	}else{
		$recordsTotal = db_result(
			db_query(
				"SELECT COUNT(idpenjualan) FROM penjualan WHERE tglpenjualan BETWEEN '%s' AND '%s' AND idpelanggan=%d",
				$tglAwal,
				$tglAkhir,
				$idpelanggan
			)
		);
	}
	return array(
			"draw"            => isset ( $request['draw'] ) ?
				intval( $request['draw'] ) :
				0,
			"recordsTotal"    => intval( $recordsTotal ),
			"recordsFiltered" => intval( $recordsFiltered ),
			"data"            => $output,
			"sql"			  => $strSQL,
			"tglawal"		  => $tglAwal,
			"tglakhir"		  => $tglAkhir,
		);
}

function serverSidePenjualan2($request){
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
	get_number_format_server_side($currSym,$tSep,$dSep);
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$tglAwal = $_REQUEST['tglawal'].' 00:00';
	$tglAkhir = $_REQUEST['tglakhir'].' 23:59';
	$idSupplier = $_REQUEST['idsupplier'];
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		'prod.barcode','prod.namaproduct','supp.namasupplier', 
		'totalqty','minhargapokok','maxhargapokok','minhargajual','maxhargajual',
		'subtotal','totalmodal','laba'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
    if (is_null($pageLength) && $pageLength != -1){
        $pageLength = 100;
    }
    if ($pageLength != -1) {
        $firstRecord = $pageStart;
        $lastRecord = $pageStart + $pageLength;
    }
    $strSQL = "SELECT detail.idproduct,prod.barcode,prod.namaproduct,";
    $strSQL .= "(SELECT GROUP_CONCAT(namasupplier SEPARATOR '<br/>') FROM product_supplier ";
    $strSQL .= "AS ps LEFT JOIN supplier supp ON ps.idsupplier = supp.idsupplier ";
    $strSQL .= "WHERE ps.idproduct = detail.idproduct AND ps.idsupplier = detail.idsupplier GROUP BY ps.idproduct) ";
    $strSQL .= "AS namasupplier, ";
    $strSQL .= "SUM(detail.jumlah) AS totalqty,";
    $strSQL .= "MIN(detail.hargapokok) AS minhargapokok,MAX(detail.hargapokok) AS maxhargapokok,";
    $strSQL .= "MIN(detail.hargajual) AS minhargajual, MAX(detail.hargajual) AS maxhargajual, ";
    $strSQL .= "SUM(detail.hargapokok*detail.jumlah) AS totalmodal, ";
    $strSQL .= "SUM(((detail.hargajual - (detail.hargajual*detail.diskon/100)) + ";
    $strSQL .= "((detail.hargajual - (detail.hargajual*detail.diskon/100)) * detail.ppn/100))*detail.jumlah) AS subtotal,";
    $strSQL .= "SUM((((detail.hargajual - (detail.hargajual*detail.diskon/100)) + ((detail.hargajual - (detail.hargajual*detail.diskon/100)) * detail.ppn/100)) - detail.hargapokok)*detail.jumlah) AS laba ";
    $strSQL .= "FROM detailpenjualan AS detail ";
    $strSQL .= "LEFT JOIN penjualan AS penj ON detail.idpenjualan = penj.idpenjualan ";
    $strSQL .= "LEFT JOIN product AS prod ON detail.idproduct = prod.idproduct ";
    //$strSQL .= "LEFT JOIN supplier AS supp ON prod.idsupplier = supp.idsupplier ";
	if (!empty($idSupplier)){
		$strSQL .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' AND ";
        $strSQL .= "(SELECT MAX(idsupplier) FROM product_supplier ";
        $strSQL .= "AS ps ";
        $strSQL .= "WHERE ps.idproduct = detail.idproduct AND ps.idsupplier = detail.idsupplier ";
        $strSQL .= "GROUP BY ps.idproduct)  = %d ";
	}else{
		$strSQL .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' ";
	}
    $strSQLFilteredTotal = "SELECT COUNT(detail.idproduct) ";
    $strSQLFilteredTotal .= "FROM detailpenjualan AS detail ";
    $strSQLFilteredTotal .= "LEFT JOIN penjualan AS penj ON detail.idpenjualan = penj.idpenjualan ";
	$strSQLFilteredTotal .= "LEFT JOIN product AS prod ON detail.idproduct = prod.idproduct ";
	//$strSQLFilteredTotal .= "LEFT JOIN supplier AS supp ON prod.idsupplier = supp.idsupplier ";
	if (!empty($idSupplier)){
		$strSQLFilteredTotal .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' AND ";
        $strSQLFilteredTotal .= "(SELECT MAX(idsupplier) FROM product_supplier ";
        $strSQLFilteredTotal .= "AS ps ";
        $strSQLFilteredTotal .= "WHERE ps.idproduct = detail.idproduct AND ps.idsupplier = detail.idsupplier ";
        $strSQLFilteredTotal .= "GROUP BY ps.idproduct)  = %d ";
	}else {
		$strSQLFilteredTotal .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' ";
	}
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (prod.barcode LIKE '%%%s%%' OR prod.namaproduct LIKE '%%%s%%' ";

        $strCriteria .= "OR (SELECT GROUP_CONCAT(namasupplier SEPARATOR ',') FROM product_supplier ";
        $strCriteria .= "AS ps LEFT JOIN supplier supp ON ps.idsupplier = supp.idsupplier ";
        $strCriteria .= "WHERE ps.idproduct = detail.idproduct AND ps.idsupplier = detail.idsupplier ";
        $strCriteria .= "GROUP BY ps.idproduct) LIKE '%%%s%%' ";

		//$strCriteria .= "OR supp.namasupplier LIKE '%%%s%%' ";
		$strCriteria .= ")";
	}
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " GROUP BY detail.idproduct ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria . " GROUP BY detail.idproduct ORDER BY $orderColumn";
    }
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		if (!empty($idSupplier)){
			$result = db_query(
				$strSQL,
				$tglAwal,
				$tglAkhir,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$firstRecord,
				$lastRecord
			);
			$recordsFiltered = db_result(
				db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir, $searchQuery, $searchQuery, $searchQuery)
			);
		}else {
			$result = db_query(
				$strSQL,
				$tglAwal,
				$tglAkhir,
				$searchQuery,
				$searchQuery,
				$searchQuery,
				$firstRecord,
				$lastRecord
			);
			$recordsFiltered = db_result(
				db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir, $searchQuery, $searchQuery, $searchQuery)
			);
		}
	}else{
		if (!empty($idSupplier)) {
			$result = db_query($strSQL, $tglAwal, $tglAkhir, $idSupplier, $firstRecord, $lastRecord);
			$recordsFiltered = db_result(db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir, $idSupplier));
		}else{
			$result = db_query($strSQL, $tglAwal, $tglAkhir, $firstRecord, $lastRecord);
			$recordsFiltered = db_result(db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir));
		}
	}
	$output = array();
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$rowData[] = $data->barcode;
		$rowData[] = $data->namaproduct;
		$rowData[] = $data->namasupplier;
		$rowData[] = number_format($data->totalqty,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->minhargapokok,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->maxhargapokok,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->minhargajual,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->maxhargajual,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->subtotal,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->totalmodal,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->laba,$DDigit,$dSep,$tSep);
		$rowData[] = $data->idproduct;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(db_query("SELECT COUNT(idproduct) FROM detailpenjualan AS detail LEFT JOIN penjualan AS penj ON detail.idpenjualan=penj.idpenjualan WHERE penj.tglpenjualan BETWEEN '%s' AND '%s'",$tglAwal,$tglAkhir));
	return array(
			"draw"            => isset ( $request['draw'] ) ?
				intval( $request['draw'] ) :
				0,
			"recordsTotal"    => intval( $recordsTotal ),
			"recordsFiltered" => intval( $recordsFiltered ),
			"data"            => $output
		);
}

function serverSidePenjualan3($request){
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
	get_number_format_server_side($currSym,$tSep,$dSep);
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$tglAwal = $_REQUEST['tglawal'].' 00:00';
	$tglAkhir = $_REQUEST['tglakhir'].' 23:59';
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		'kateg.kodekategori','kateg.kategori','totaljual',
		'totalmodal','totallaba'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
    if (is_null($pageLength) || $pageLength != -1){
		$pageLength = 100;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT kateg.idkategori,kateg.kodekategori,kateg.kategori,";
	$strSQL .= "SUM((detail.hargajual - (detail.hargajual*detail.diskon/100))*detail.jumlah) AS totaljual,";
	$strSQL .= "SUM(detail.hargapokok*detail.jumlah) AS totalmodal, ";
	$strSQL .= "SUM(((detail.hargajual - (detail.hargajual * detail.diskon/100)) - detail.hargapokok) * detail.jumlah) AS totallaba ";
	$strSQL .= "FROM detailpenjualan AS detail ";
	$strSQL .= "LEFT JOIN penjualan AS penj ON detail.idpenjualan = penj.idpenjualan ";
	$strSQL .= "LEFT JOIN product AS prod ON detail.idproduct = prod.idproduct ";
	$strSQL .= "LEFT JOIN kategori AS kateg ON prod.idkategori = kateg.idkategori ";
	$strSQL .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' ";
	$strSQLFilteredTotal = "SELECT COUNT(kateg.idkategori) ";
	$strSQLFilteredTotal .= "FROM detailpenjualan AS detail ";
	$strSQLFilteredTotal .= "LEFT JOIN penjualan AS penj ON detail.idpenjualan = penj.idpenjualan ";
	$strSQLFilteredTotal .= "LEFT JOIN product AS prod ON detail.idproduct = prod.idproduct ";
	$strSQLFilteredTotal .= "LEFT JOIN kategori AS kateg ON prod.idkategori = kateg.idkategori ";
	$strSQLFilteredTotal .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' ";
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (kateg.kodekategori LIKE '%%%s%%' OR ";
		$strCriteria .= "kateg.kategori LIKE '%%%s%%' ) ";
	}
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " GROUP BY kateg.idkategori ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria . " GROUP BY kateg.idkategori ORDER BY $orderColumn";
    }
	$strSQLFilteredTotal .= $strCriteria." GROUP BY kateg.idkategori";
	if (!empty($searchQuery)){
		$result = db_query(
			$strSQL,
			$tglAwal,
			$tglAkhir,
			$searchQuery,
			$searchQuery,
			$firstRecord,
			$lastRecord
		);
		$recordsFiltered = db_result(
			db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir, $searchQuery, $searchQuery)
		);
	}else{
		$result = db_query($strSQL, $tglAwal, $tglAkhir, $firstRecord, $lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir));
	}
	$output = array();
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$rowData[] = $data->kodekategori;
		$rowData[] = $data->kategori;
		$rowData[] = number_format($data->totaljual,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->totalmodal,$DDigit,$dSep,$tSep);
		$rowData[] = number_format(($data->totallaba),$DDigit,$dSep,$tSep);
		$rowData[] = $data->idkategori;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(
		db_query("SELECT COUNT(kateg.idkategori) FROM detailpenjualan AS detail
				LEFT JOIN penjualan AS penj ON detail.idpenjualan=penj.idpenjualan
				LEFT JOIN product AS prod ON detail.idproduct = prod.idproduct
				LEFT JOIN kategori AS kateg ON prod.idkategori = kateg.idkategori
				WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' GROUP BY kateg.idkategori",
				$tglAwal,$tglAkhir
		)
	);
	return array(
		"draw"            => isset ( $request['draw'] ) ?
			intval( $request['draw'] ) :
			0,
		"recordsTotal"    => intval( $recordsTotal ),
		"recordsFiltered" => intval( $recordsFiltered ),
		"data"            => $output
	);
}

function serverSideLaundry($request){
    global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym,$tSep,$dSep);
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$tglAwal = $_REQUEST['tglawal'].' 00:00';
	$tglAkhir = $_REQUEST['tglakhir'].' 23:59';
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		'laundry.nonota','tanggal','totallaundry','laundry.carabayar','laundry.bayar',
		'plg.namapelanggan','laundry.status_laundry','perkiraan_ambil','laundry.keterangan'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
	if (is_null($pageLength)){
		$pageLength = 100;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT laundry.idtitipanlaundry,laundry.nonota,SUBSTR(laundry.tglpenjualan,1,10) AS tanggal,";
	$strSQL .= "SUBSTR(laundry.tglpenjualan,11,9) AS waktu, laundry.idpemakai,";
	$strSQL .= "(SELECT SUM(hargajual*jumlah) FROM detaillaundry WHERE ";
	$strSQL .= "idtitipanlaundry = laundry.idtitipanlaundry) AS totallaundry,";
	$strSQL .= "(SELECT MAX(perkiraan_ambil) FROM detaillaundry WHERE ";
	$strSQL .= "idtitipanlaundry = laundry.idtitipanlaundry) AS perkiraan_ambil,";
	$strSQL .= "laundry.carabayar, laundry.bayar, laundry.status_laundry, ";
	$strSQL .= "plg.namapelanggan, laundry.keterangan, user.name ";
	$strSQL .= "FROM titipanlaundry AS laundry ";
	$strSQL .= "LEFT JOIN cms_users AS user ON user.uid = laundry.idpemakai ";
	$strSQL .= "LEFT JOIN pelanggan AS plg ON plg.idpelanggan = laundry.idpelanggan ";
	$strSQL .= "WHERE laundry.tglpenjualan BETWEEN '%s' AND '%s' ";
	$strSQLFilteredTotal = "SELECT COUNT(laundry.idtitipanlaundry) ";
	$strSQLFilteredTotal .= "FROM titipanlaundry AS laundry ";
	$strSQLFilteredTotal .= "LEFT JOIN cms_users AS user ON user.uid = laundry.idpemakai ";
	$strSQLFilteredTotal .= "LEFT JOIN pelanggan AS plg ON plg.idpelanggan = laundry.idpelanggan ";
	$strSQLFilteredTotal .= "WHERE laundry.tglpenjualan BETWEEN '%s' AND '%s' ";
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (laundry.nonota LIKE '%%%s%%' OR SUBSTR(laundry.tglpenjualan,1,10) LIKE '%%%s%%' ";
		$strCriteria .= "OR SUBSTR(laundry.tglpenjualan,11,9) LIKE '%%%s%%' OR user.name LIKE '%%%s%%' ";
		$strCriteria .= "OR plg.namapelanggan LIKE '%%%s%%' OR laundry.carabayar LIKE '%%%s%%' ";
		$strCriteria .= ")";
	}
	$strSQL .= $strCriteria." ORDER BY $orderColumn LIMIT %d, %d";
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		$result = db_query($strSQL,$tglAwal,$tglAkhir,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$tglAwal,$tglAkhir,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery));
	}else{
		$result = db_query($strSQL,$tglAwal,$tglAkhir,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$tglAwal,$tglAkhir));
	}
    $arrayhari = arrayHariSS();
	$output = array();
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$tomboldetail = "<img title=\"Klik untuk melihat detail laundry\" onclick=\"view_detail(".$data->idtitipanlaundry.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/forward_enabled.ico\" width=\"22\">";
		$tombolambil = "<img title=\"Klik untuk mengisi form pengambilan laundry\" onclick=\"pickup_laundry(".$data->idtitipanlaundry.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/update.ico\" width=\"22\">";
		$tombolhapus = "<img title=\"Klik untuk menghapus laundry\" onclick=\"delete_laundry(".$data->idtitipanlaundry.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/del.ico\" width=\"22\">";
		$tombolprint = "<img title=\"Klik untuk mencetak titipan laundry\" onclick=\"print_laundry(".$data->idtitipanlaundry.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/print.png\" width=\"22\">";
		$tombolselesai = "<img title=\"Laundry sudah diambil\" src=\"$baseDirectory/misc/media/images/checks.png\" width=\"22\">";
		$rowData[] = $tomboldetail;
		if ($data->status_laundry == 0 || $data->status_laundry == 1){
			$rowData[] = $tombolambil;
		}else{
			$rowData[] = $tombolselesai;
		}
		$rowData[] = $tombolprint;
		$rowData[] = $tombolhapus;
		$rowData[] = $data->nonota;
        $indexhari = date('w', strtotime($data->tanggal));
        $rowData[] = $arrayhari[$indexhari];
		$rowData[] = date('d-m-Y', strtotime($data->tanggal));
		$rowData[] = $data->waktu;
		$rowData[] = number_format($data->totallaundry,$DDigit,$dSep,$tSep);
        $rowData[] = $data->carabayar;
        $rowData[] = number_format($data->bayar,$DDigit,$dSep,$tSep);
        $sisaPembayaran = $data->totallaundry - $data->bayar;
        $rowData[] = number_format($sisaPembayaran,$DDigit,$dSep,$tSep);
        $rowData[] = $data->name;
        $rowData[] = $data->namapelanggan;
        if ($data->status_laundry == 0){
            $rowData[] = 'BELUM DIAMBIL';
        }else if ($data->status_laundry == 1){
            $rowData[] = 'DIAMBIL SEBAGIAN';
        }else if ($data->status_laundry == 2){
            $rowData[] = 'SUDAH DIAMBIL';
        }
        $rowData[] = date('d-m-Y H:i', $data->perkiraan_ambil);
        $rowData[] = $data->keterangan;
        $rowData[] = $data->idtitipanlaundry;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(db_query("SELECT COUNT(idtitipanlaundry) FROM titipanlaundry WHERE tglpenjualan BETWEEN '%s' AND '%s'",$tglAwal,$tglAkhir));
	return array(
			"draw"            => isset ( $request['draw'] ) ?
				intval( $request['draw'] ) :
				0,
			"recordsTotal"    => intval( $recordsTotal ),
			"recordsFiltered" => intval( $recordsFiltered ),
			"data"            => $output,
            "order"           => $orderColumn
		);
}
function serverSideCustomerOrder($request){
	global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym,$tSep,$dSep);
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$tglAwal = $_REQUEST['tglawal'].' 00:00';
	$tglAkhir = $_REQUEST['tglakhir'].' 23:59';
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		5 => 'customerorder.nonota',
		7 => 'tanggal',
		9 => 'total',
        10 => 'ppn_value',
        11 => 'total_plus_ppn',
		12 => 'customerorder.carabayar',
		13 => 'customerorder.bayar',
		16 => 'plg.namapelanggan',
		17 => 'customerorder.status_order',
		18 => 'perkiraan_ambil',
		19 => 'customerorder.keterangan',
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
    if (is_null($pageLength) || $pageLength != -1){
		$pageLength = 100;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT customerorder.id,customerorder.nonota,SUBSTR(customerorder.tglorder,1,10) AS tanggal,";
	$strSQL .= "SUBSTR(customerorder.tglorder,11,9) AS waktu, customerorder.idpemakai,";
	$strSQL .= "customerorder.total,";
    $strSQL .= "(customerorder.total * (customerorder.ppn/100)) AS ppn_value, customerorder.total_plus_ppn, ";
	$strSQL .= "(SELECT MAX(perkiraan_ambil) FROM detailcustomerorder WHERE ";
	$strSQL .= "idcustomerorder = customerorder.id) AS perkiraan_ambil,";
	$strSQL .= "customerorder.carabayar, customerorder.bayar, customerorder.status_order, ";
	$strSQL .= "customerorder.idpelanggan, plg.namapelanggan, customerorder.keterangan, ";
    $strSQL .= "user.name, meja.meja, customerorder.idorderpanggil  ";
	$strSQL .= "FROM customer_order AS customerorder ";
	$strSQL .= "LEFT JOIN meja AS meja ON meja.id = customerorder.idmeja ";
	$strSQL .= "LEFT JOIN cms_users AS user ON user.uid = customerorder.idpemakai ";
	$strSQL .= "LEFT JOIN pelanggan AS plg ON plg.idpelanggan = customerorder.idpelanggan ";
	$strSQL .= "WHERE customerorder.tglorder BETWEEN '%s' AND '%s' ";
	$strSQLFilteredTotal = "SELECT COUNT(customerorder.id) ";
	$strSQLFilteredTotal .= "FROM customer_order AS customerorder ";
	$strSQLFilteredTotal .= "LEFT JOIN meja AS meja ON meja.id = customerorder.idmeja ";
	$strSQLFilteredTotal .= "LEFT JOIN cms_users AS user ON user.uid = customerorder.idpemakai ";
	$strSQLFilteredTotal .= "LEFT JOIN pelanggan AS plg ON plg.idpelanggan = customerorder.idpelanggan ";
	$strSQLFilteredTotal .= "WHERE customerorder.tglorder BETWEEN '%s' AND '%s' ";
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (customerorder.nonota LIKE '%%%s%%' OR SUBSTR(customerorder.tglorder,1,10) LIKE '%%%s%%' ";
		$strCriteria .= "OR SUBSTR(customerorder.tglorder,11,9) LIKE '%%%s%%' OR user.name LIKE '%%%s%%' ";
		$strCriteria .= "OR plg.namapelanggan LIKE '%%%s%%' OR customerorder.carabayar LIKE '%%%s%%' ";
		$strCriteria .= ")";
	}
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		$result = db_query($strSQL,$tglAwal,$tglAkhir,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$tglAwal,$tglAkhir,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery));
	}else{
		$result = db_query($strSQL,$tglAwal,$tglAkhir,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$tglAwal,$tglAkhir));
	}
	$arrayhari = arrayHariSS();
	$output = array();
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$tomboldetail = "<img title=\"Klik untuk melihat detail customer order\" onclick=\"view_detail(".$data->id.",'".$data->nonota."',".$data->idpelanggan.");\" src=\"$baseDirectory/misc/media/images/forward_enabled.ico\" width=\"22\">";
		$tombolambil = "<img title=\"Klik untuk mengisi form pengambilan customer order\" onclick=\"pickup_customerorder(".$data->id.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/update.ico\" width=\"22\">";
		$tombolhapus = "<img title=\"Klik untuk menghapus customer order\" onclick=\"delete_customerorder(".$data->id.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/del.ico\" width=\"22\">";
		$tombolprint = "<img title=\"Klik untuk mencetak customer order\" onclick=\"print_customerorder(".$data->id.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/print.png\" width=\"22\">";
		//$tombolselesai = "<img title=\"Customer order sudah diambil\" src=\"$baseDirectory/misc/media/images/checks.png\" width=\"22\">";
		$tombolprintproduksi = "<img title=\"Klik untuk mencetak keperluan produksi\" onclick=\"print_production(".$data->id.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/print-production.png\" width=\"22\">";
		$rowData[] = $tomboldetail;
		if ($data->status_order == 0 || $data->status_order == 1){
			$rowData[] = $tombolambil;
		}else{
			$rowData[] = $tombolselesai;
		}
		$rowData[] = $tombolprint;
		$rowData[] = $tombolhapus;
		$rowData[] = $data->meja;
		$rowData[] = '<div id="'.$data->nonota.'" class="barcode-place"></div>';
		$rowData[] = $data->nonota;
		$indexhari = date('w', strtotime($data->tanggal));
		$rowData[] = $arrayhari[$indexhari];
		$rowData[] = date('d-m-Y', strtotime($data->tanggal));
		$rowData[] = $data->waktu;
		$rowData[] = number_format($data->total,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->ppn_value,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->total_plus_ppn,$DDigit,$dSep,$tSep);
		$rowData[] = $data->carabayar;
		$rowData[] = number_format($data->bayar,$DDigit,$dSep,$tSep);
		$sisaPembayaran = ($data->total + $data->ppn_value) - $data->bayar;
		$rowData[] = number_format($sisaPembayaran,$DDigit,$dSep,$tSep);
		$rowData[] = $data->name;
		$rowData[] = $data->namapelanggan;
		if ($data->status_order == 0){
			$rowData[] = 'BELUM DIAMBIL';
		}else if ($data->status_order == 1){
			$rowData[] = 'DIAMBIL SEBAGIAN';
		}else if ($data->status_order == 2){
			$rowData[] = 'SUDAH DIAMBIL';
		}
		$rowData[] = date('d-m-Y H:i', $data->perkiraan_ambil);
		$rowData[] = $data->keterangan;
		//$rowData[] = $tombolprintproduksi;
        $NomerPanggil = '1'.str_pad($data->idorderpanggil,3,'0', STR_PAD_LEFT);
        $tombolselesai = "<img title=\"Klik untuk memanggil pelanggan\" onclick=\"panggil_pelanggan('".$NomerPanggil."');\" src=\"$baseDirectory/misc/media/images/complete-small.png\" width=\"22\">";
        $rowData[] = $tombolselesai;
		$rowData[] = $data->id;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(db_query("SELECT COUNT(id) FROM customer_order WHERE tglorder BETWEEN '%s' AND '%s'",$tglAwal,$tglAkhir));
	return array(
		"draw"            => isset ( $request['draw'] ) ?
			intval( $request['draw'] ) :
			0,
		"recordsTotal"    => intval( $recordsTotal ),
		"recordsFiltered" => intval( $recordsFiltered ),
		"data"            => $output,
		"order"           => $orderColumn,
	);
}
function arrayHariSS(){
    $hari_array = array('Minggu','Senin','Selasa','Rabu','Kamis','Jumat','Sabtu');
    return $hari_array;
}
function kategoriPengeluaran($request){
	global $baseDirectory;
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		'id','kategori','jeniskategori','keterangan'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
    if (is_null($pageLength) || $pageLength != -1){
		$pageLength = 100;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT id, kategori, jeniskategori, keterangan, created, changed, uid ";
	$strSQLFilteredTotal = "SELECT COUNT(id) ";
	$strSQL .= "FROM cms_kategoripengeluaran ";
	$strSQLFilteredTotal .= "FROM cms_kategoripengeluaran ";
	$strSQL .= "WHERE 1=1 ";
	$strSQLFilteredTotal .= "WHERE 1=1 ";
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (kategori LIKE '%%%s%%' OR keterangan LIKE '%%%s%%' )";
	}
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		$result = db_query($strSQL,$searchQuery,$searchQuery,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$searchQuery,$searchQuery));
	}else{
		$result = db_query($strSQL,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal));
	}
	$output = array();
	$jenisKategori = array('Pengeluaran', 'Pemasukan');
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$editbutton = '<img title="Klik untuk mengubah kategori pengeluaran" onclick="edit_kategori('.$data->id.', this.parentNode.parentNode);" src="'.$baseDirectory.'/misc/media/images/edit.ico" width="22">';
		$rowData[] = $editbutton;
		$rowData[] = $data->kategori;
		$rowData[] = $jenisKategori[$data->jeniskategori];
		$rowData[] = $data->keterangan;
		$rowData[] = $data->jeniskategori;
		$rowData[] = $data->id;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(db_query("SELECT COUNT(id) FROM cms_kategoripengeluaran"));
	return array(
		"draw"            => isset ( $request['draw'] ) ?
			intval( $request['draw'] ) :
			0,
		"recordsTotal"    => intval( $recordsTotal ),
		"recordsFiltered" => intval( $recordsFiltered ),
		"data"            => $output
	);
}
function pengeluaran($request){
	global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym,$tSep,$dSep);
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		3 => 'pengeluaran.tglpengeluaran',
		4 => 'pengeluaran.kategori',
		5 => 'pengeluaran.keterangan',
		6 => 'pengeluaran.nilai'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
    if (is_null($pageLength) || $pageLength != -1){
		$pageLength = 100;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT pengeluaran.id, pengeluaran.keterangan, pengeluaran.kategori, ";
	$strSQL .= "pengeluaran.nilai, pengeluaran.tglpengeluaran, pengeluaran.created, ";
	$strSQL .= "pengeluaran.changed, pengeluaran.uid, ";
	$strSQL .= "katpengeluaran.kategori AS kategori_title ";
	$strSQL .= "FROM cms_pengeluaran AS pengeluaran ";
	$strSQL .= "LEFT JOIN cms_kategoripengeluaran AS katpengeluaran ";
	$strSQL .= "ON pengeluaran.kategori=katpengeluaran.id ";
	$strSQL .= "WHERE 1=1 ";
	$strSQLFilteredTotal = "SELECT COUNT(pengeluaran.id) ";
	$strSQLFilteredTotal .= "FROM cms_pengeluaran AS pengeluaran ";
	$strSQLFilteredTotal .= "LEFT JOIN cms_kategoripengeluaran AS katpengeluaran ";
	$strSQLFilteredTotal .= "ON pengeluaran.kategori=katpengeluaran.id ";
	$strSQLFilteredTotal .= "WHERE 1=1 ";
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (katpengeluaran.kategori LIKE '%%%s%%' OR ";
		$strCriteria .= "katpengeluaran.keterangan LIKE '%%%s%%' OR ";
		$strCriteria .= "pengeluaran.keterangan LIKE '%%%s%%' OR ";
		$strCriteria .= "SUBSTR(pengeluaran.tglpengeluaran,1,10) LIKE '%%%s%%'  OR ";
		$strCriteria .= "pengeluaran.nilai LIKE '%%%s%%'";
		$strCriteria .= ")";
	}
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		$result = db_query($strSQL,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery));
	}else{
		$result = db_query($strSQL,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal));
	}
	$output = array();
	$arrayhari = arrayHariSS();
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$editbutton = '<img title="Klik untuk mengubah pengeluaran" onclick="edit_pengeluaran('.$data->id.', this.parentNode.parentNode);" src="'.$baseDirectory.'/misc/media/images/edit.ico" width="22">';
		$deletebutton = '<img title="Klik untuk menghapus pengeluaran" onclick="hapus_pengeluaran('.$data->id.');" src="'.$baseDirectory.'/misc/media/images/del.ico" width="22">';
		$rowData[] = $editbutton;
		$rowData[] = $deletebutton;
		$index_hari = date('w', $data->tglpengeluaran);
		$tglpengeluaran = date('Y-m-d', $data->tglpengeluaran);
		$rowData[] = $arrayhari[$index_hari];
		$rowData[] = $tglpengeluaran;
		$rowData[] = $data->kategori_title;
		$rowData[] = $data->keterangan;
		$rowData[] = number_format($data->nilai,$DDigit,$dSep,$tSep);
		$rowData[] = $data->kategori;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(db_query("SELECT COUNT(id) FROM cms_pengeluaran"));
	return array(
		"draw"            => isset ( $request['draw'] ) ?
			intval( $request['draw'] ) :
			0,
		"recordsTotal"    => intval( $recordsTotal ),
		"recordsFiltered" => intval( $recordsFiltered ),
		"data"            => $output
	);
}
function pemasukan($request){
	global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym,$tSep,$dSep);
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		3 => 'pemasukan.tglpemasukan',
		4 => 'pemasukan.kategori',
		5 => 'pemasukan.keterangan',
		6 => 'pemasukan.nilai'
	);
	$orderColumnArray = $_REQUEST['order'];
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
    if (is_null($pageLength) || $pageLength != -1){
		$pageLength = 100;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = "SELECT pemasukan.id, pemasukan.keterangan, pemasukan.kategori, ";
	$strSQL .= "pemasukan.nilai, pemasukan.tglpemasukan, pemasukan.created, ";
	$strSQL .= "pemasukan.changed, pemasukan.uid, ";
	$strSQL .= "katpemasukan.kategori AS kategori_title ";
	$strSQL .= "FROM cms_pemasukan AS pemasukan ";
	$strSQL .= "LEFT JOIN cms_kategoripengeluaran AS katpemasukan ";
	$strSQL .= "ON pemasukan.kategori=katpemasukan.id ";
	$strSQL .= "WHERE 1=1 ";
	$strSQLFilteredTotal = "SELECT COUNT(pemasukan.id) ";
	$strSQLFilteredTotal .= "FROM cms_pemasukan AS pemasukan ";
	$strSQLFilteredTotal .= "LEFT JOIN cms_kategoripengeluaran AS katpemasukan ";
	$strSQLFilteredTotal .= "ON pemasukan.kategori=katpemasukan.id ";
	$strSQLFilteredTotal .= "WHERE 1=1 ";
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (katpemasukan.kategori LIKE '%%%s%%' OR ";
		$strCriteria .= "katpemasukan.keterangan LIKE '%%%s%%' OR ";
		$strCriteria .= "pemasukan.keterangan LIKE '%%%s%%' OR ";
		$strCriteria .= "SUBSTR(pemasukan.tglpemasukan,1,10) LIKE '%%%s%%'  OR ";
		$strCriteria .= "pemasukan.nilai LIKE '%%%s%%'";
		$strCriteria .= ")";
	}
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)){
		$result = db_query($strSQL,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$searchQuery,$searchQuery,$searchQuery,$searchQuery,$searchQuery));
	}else{
		$result = db_query($strSQL,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal));
	}
	$output = array();
	$arrayhari = arrayHariSS();
	while ($data = db_fetch_object($result)){
		$rowData = array();
		$editbutton = '<img title="Klik untuk mengubah pemasukan" onclick="edit_pemasukan('.$data->id.', this.parentNode.parentNode);" src="'.$baseDirectory.'/misc/media/images/edit.ico" width="22">';
		$deletebutton = '<img title="Klik untuk menghapus pemasukan" onclick="hapus_pemasukan('.$data->id.');" src="'.$baseDirectory.'/misc/media/images/del.ico" width="22">';
		$rowData[] = $editbutton;
		$rowData[] = $deletebutton;
		$index_hari = date('w', $data->tglpemasukan);
		$tglpemasukan = date('Y-m-d', $data->tglpemasukan);
		$rowData[] = $arrayhari[$index_hari];
		$rowData[] = $tglpemasukan;
		$rowData[] = $data->kategori_title;
		$rowData[] = $data->keterangan;
		$rowData[] = number_format($data->nilai,$DDigit,$dSep,$tSep);
		$rowData[] = $data->kategori;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(db_query("SELECT COUNT(id) FROM cms_pemasukan"));
	return array(
		"draw"            => isset ( $request['draw'] ) ?
			intval( $request['draw'] ) :
			0,
		"recordsTotal"    => intval( $recordsTotal ),
		"recordsFiltered" => intval( $recordsFiltered ),
		"data"            => $output
	);
}
function serverSideDetailCustomerOrder($request){
	get_number_format_server_side($currSym,$tSep,$dSep);
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
	global $baseDirectory;
	$pageStart = $_GET['start'];
	$pageLength = $_GET['length'];
	$searchArray = $_REQUEST['search'];
	$idCustomerOrder = $_REQUEST['idcustomerorder'];
	$searchQuery = $searchArray['value'];
	$arrayColumn = array(
		1 => 'product.barcode',
		2 => 'product.namaproduct',
		3 => 'detord.jumlah',
		4 => '(detord.jumlah - detord.sisa)',
		5 => 'detord.sisa',
        6 => 'detord.hargajual',
        7 => '(detord.jumlah*detord.hargajual)',
		8 => 'detord.perkiraan_ambil',
		9 => 'detord.diambil',
	);
	$orderColumnArray = isset($_REQUEST['order']) && !empty($_REQUEST['order']) ? $_REQUEST['order'] : array( 0 => array('column' => 1, 'dir' => 'ASC'));
	$orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
	if (is_null($pageStart)){
		$pageStart = 0;
	}
	if (is_null($pageLength) || $pageLength == -1){
		$pageLength = 100;
	}
	$firstRecord = $pageStart;
	$lastRecord = $pageStart + $pageLength;
	$strSQL = 'SELECT detord.id,product.barcode, product.namaproduct, detord.jumlah,';
	$strSQL .= 'detord.hargajual,(detord.hargajual*detord.jumlah) AS subtotal,detord.sisa,';
	$strSQL .= 'detord.diambil,detord.perkiraan_ambil,detord.outstanding FROM ';
	$strSQL .= 'detailcustomerorder detord LEFT JOIN product product ';
	$strSQL .= 'ON detord.idproduct=product.idproduct ';
	$strSQL .= 'LEFT JOIN supplier supp ON product.idsupplier=supp.idsupplier ';
	$strSQL .= 'WHERE detord.idcustomerorder=%d ';
	$strSQLFilteredTotal = 'SELECT COUNT(detord.id) FROM ';
	$strSQLFilteredTotal .= 'detailcustomerorder detord LEFT JOIN product product ';
	$strSQLFilteredTotal .= 'ON detord.idproduct=product.idproduct ';
	$strSQLFilteredTotal .= 'LEFT JOIN supplier supp ON product.idsupplier=supp.idsupplier ';
	$strSQLFilteredTotal .= 'WHERE detord.idcustomerorder=%d ';
	$strCriteria = "";
	if (!empty($searchQuery)){
		$strCriteria .= "AND (product.barcode LIKE '%%%s%%' OR ";
		$strCriteria .= "product.namaproduct LIKE '%%%s%%'";
		$strCriteria .= ")";
	}
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }
	$strSQLFilteredTotal .= $strCriteria;
	if (!empty($searchQuery)) {
		$result = db_query($strSQL, $idCustomerOrder, $searchQuery, $searchQuery, $firstRecord, $lastRecord);
		$recordsFiltered = db_result(
			db_query($strSQLFilteredTotal, $idCustomerOrder, $searchQuery, $searchQuery)
		);
	}else{
		$result = db_query($strSQL,$idCustomerOrder,$firstRecord,$lastRecord);
		$recordsFiltered = db_result(db_query($strSQLFilteredTotal,$idCustomerOrder));
	}
	while($data = db_fetch_object($result)){
		$rowData = array();
		$deletebutton = '<img title="Klik untuk menghapus detail order" onclick="hapus_detail('.$data->id.',\''.$data->namaproduct.'\');" src="'.$baseDirectory.'/misc/media/images/del.ico" width="22">';
		$rowData[] = $deletebutton;
		$rowData[] = $data->barcode;
		$rowData[] = $data->namaproduct;
		$rowData[] = $data->jumlah;
		$rowData[] = $data->jumlah - $data->sisa;
		$rowData[] = $data->sisa;
		$rowData[] = number_format($data->hargajual,$DDigit,$dSep,$tSep);
		$rowData[] = number_format($data->subtotal,$DDigit,$dSep,$tSep);
		$rowData[] = date('d M H:i',$data->perkiraan_ambil);
		if (!empty($data->diambil)){
			$rowData[] = date('d M H:i',$data->diambil);
		}else{
			$rowData[] = '-';
		}
		$rowData[] = $data->id;
		$output[] = $rowData;
	}
	$recordsTotal = db_result(db_query("SELECT COUNT(id) FROM detailcustomerorder"));
	return array(
		"draw"            => isset ( $request['draw'] ) ?
			intval( $request['draw'] ) :
			0,
		"recordsTotal"    => intval( $recordsTotal ),
		"recordsFiltered" => intval( $recordsFiltered ),
		"data"            => $output
	);
}
function serverSideGetProduct($request){
	$items = array();
	if ($_GET["term"]){
		$KATACARI = '%'.$_GET["term"].'%';
		$result = db_query("SELECT idproduct,barcode, alt_code, namaproduct, stok, hargajual,hargapokok,
        idkategori FROM product WHERE (alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s')
        AND status_product = 1 LIMIT 50",
        $KATACARI,$KATACARI,$KATACARI);
		$items = array();
		while ($data = db_fetch_object($result)) {
            $diskon = 0;
            if ($data->idproduct) {
                $idpelanggan = 0;
                if (isset($_GET["idpelanggan"])){
                    $idpelanggan = $_GET["idpelanggan"];
                }
                $result2 = db_query(
                    "SELECT besardiskon FROM diskonkategori WHERE idpelanggan='%d' AND idkategori='%d'",
                    $idpelanggan,
                    $data->idkategori
                );
                $datadiskon = db_fetch_object($result2);
                if (!empty($datadiskon) && $datadiskon->besardiskon >= 0) {
                    $diskon = $datadiskon->besardiskon;
                }
            }
			$items[] = array(
				'value' => $data->namaproduct,
				'barcode'   => $data->barcode,
				'alt_code'  => $data->alt_code,
				'hargajual' => $data->hargajual,
				'hargapokok' => $data->hargapokok,
                'diskon' => $diskon,
				'id' => $data->idproduct,
			);
		}
	}
	return $items;
}
function serverSideGetOneProduct($request){
	$items = array();
	if ($_GET["term"]){
		$KATACARI = '%'.$_GET["term"].'%';
		$result = db_query("SELECT idproduct,barcode, alt_code, namaproduct, stok, hargajual,hargapokok FROM product 
        WHERE (alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s')
        AND status_product = 1 LIMIT 1",$KATACARI,$KATACARI,$KATACARI);
		$items = array();
		while ($data = db_fetch_object($result)) {
            $diskon = 0;
            if ($data->idproduct) {
                $idpelanggan = 0;
                if (isset($_GET["idpelanggan"])){
                    $idpelanggan = $_GET["idpelanggan"];
                }
                $result2 = db_query(
                    "SELECT besardiskon FROM diskonkategori WHERE idpelanggan='%d' AND idkategori='%d'",
                    $idpelanggan,
                    $data->idkategori
                );
                $datadiskon = db_fetch_object($result2);
                if ($datadiskon->besardiskon >= 0) {
                    $diskon = $datadiskon->besardiskon;
                }
            }
			$items[] = array(
				'value' => $data->namaproduct,
				'barcode'   => $data->barcode,
				'alt_code'  => $data->alt_code,
				'hargajual' => $data->hargajual,
				'hargapokok' => $data->hargapokok,
                'diskon' => $diskon,
				'id' => $data->idproduct,
			);
		}
	}
	return $items;
}
function serverSideDetailPenjualan($request){
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
	get_number_format_server_side($currSym,$tSep,$dSep);
    global $baseDirectory;
    $pageStart = $_GET['start'];
    $pageLength = $_GET['length'];
    $searchArray = $_REQUEST['search'];
    $idPenjualan = $_REQUEST['idpenjualan'];
    $searchQuery = $searchArray['value'];
    $arrayColumn = array(
        1 => 'product.barcode',
        2 => 'product.namaproduct',
        3 => 'detail.jumlah',
        4 => 'detail.hargajual',
        5 => 'detail.hargapokok',
        6 => '(detail.jumlah*detail.hargajual)',
        7 => '(detail.jumlah*detail.hargapokok)',
        8 => '(detail.jumlah*(detail.hargajual - detail.hargapokok))',
    );
    $orderColumnArray = isset($_REQUEST['order']) && !empty($_REQUEST['order']) ? $_REQUEST['order'] : array( 0 => array('column' => 1, 'dir' => 'ASC'));
    $orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
    if (is_null($pageStart)){
        $pageStart = 0;
    }
    if (is_null($pageLength) && $pageLength != -1){
        $pageLength = 25;
    }
    if ($pageLength != -1) {
        $firstRecord = $pageStart;
        $lastRecord = $pageStart + $pageLength;
    }
    $strSQL = 'SELECT detail.iddetail,product.barcode, product.namaproduct, detail.jumlah,detail.diskon, detail.ppn, ';
    $strSQL .= 'detail.hargapokok,detail.hargajual,';
    $strSQL .= '((detail.hargajual - (detail.hargajual*diskon/100))*detail.jumlah) AS subtotal,';
    $strSQL .= '((detail.hargajual - (detail.hargajual*diskon/100))*detail.jumlah) + ';
    $strSQL .= '(((detail.hargajual - (detail.hargajual*diskon/100))*detail.jumlah) * ppn/100)  AS subtotalppn,';
    $strSQL .= '(detail.hargapokok*detail.jumlah) AS modal,';
    $strSQL .= '((detail.hargajual-detail.hargapokok)*detail.jumlah) AS laba ';
    $strSQL .= 'FROM detailpenjualan detail LEFT JOIN product product ';
    $strSQL .= 'ON detail.idproduct=product.idproduct ';
    $strSQL .= 'LEFT JOIN supplier supp ON product.idsupplier=supp.idsupplier ';
    $strSQL .= 'WHERE detail.idpenjualan=%d ';
    $strSQLFilteredTotal = 'SELECT COUNT(detail.iddetail) FROM ';
    $strSQLFilteredTotal .= 'detailpenjualan detail LEFT JOIN product product ';
    $strSQLFilteredTotal .= 'ON detail.idproduct=product.idproduct ';
    $strSQLFilteredTotal .= 'LEFT JOIN supplier supp ON product.idsupplier=supp.idsupplier ';
    $strSQLFilteredTotal .= 'WHERE detail.idpenjualan=%d ';
    $strCriteria = "";
    if (!empty($searchQuery)){
        $strCriteria .= "AND (product.barcode LIKE '%%%s%%' OR ";
        $strCriteria .= "product.namaproduct LIKE '%%%s%%'";
        $strCriteria .= ")";
    }
    if ($pageLength == -1){
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }else {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    }
    $strSQLFilteredTotal .= $strCriteria;
    if (!empty($searchQuery)) {
        $result = db_query($strSQL, $idPenjualan, $searchQuery, $searchQuery, $firstRecord, $lastRecord);
        $recordsFiltered = db_result(
            db_query($strSQLFilteredTotal, $idPenjualan, $searchQuery, $searchQuery)
        );
    }else{
        $result = db_query($strSQL,$idPenjualan,$firstRecord,$lastRecord);
        $recordsFiltered = db_result(db_query($strSQLFilteredTotal,$idPenjualan));
    }
    $output = array();
    while($data = db_fetch_object($result)){
        $rowData = array();
        $deletebutton = '<img title="Klik untuk menghapus detail penjualan" onclick="hapus_detail('.$data->iddetail.',\''.$data->namaproduct.'\');" src="'.$baseDirectory.'/misc/media/images/del.ico" width="22">';
        $rowData[] = $deletebutton;
        $rowData[] = $data->barcode;
        $rowData[] = $data->namaproduct;
        $rowData[] = $data->jumlah;
        $rowData[] = number_format($data->hargajual,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->diskon,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->ppn,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->hargapokok,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->subtotal,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->modal,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->laba,$DDigit,$dSep,$tSep);
        $rowData[] = $data->iddetail;
        $output[] = $rowData;
    }
    $recordsTotal = db_result(db_query("SELECT COUNT(iddetail) FROM detailpenjualan"));
    return array(
        "draw"            => isset ( $request['draw'] ) ?
            intval( $request['draw'] ) :
            0,
        "recordsTotal"    => intval( $recordsTotal ),
        "recordsFiltered" => intval( $recordsFiltered ),
        "data"            => $output
    );
}
function serverSideGetCategoryProduct(){
	$result = db_query("SELECT idkategori, kodekategori, kategori, keterangan FROM kategori");
	$items = array();
	while ($data = db_fetch_object($result)) {
		$items[$data->idkategori] = $data;
	}
	return $items;
}
function serverSideGetProductByCategory($request, $SearchText = null){
	$dataCategory = serverSideGetCategoryProduct();
	if (!empty($SearchText)){
        $result = db_query("SELECT idproduct,barcode, alt_code, namaproduct, stok, 
        hargajual,hargapokok,idkategori 
        FROM product WHERE status_product = 1 AND 
        (barcode LIKE ('%".$SearchText."%') OR namaproduct LIKE ('%".$SearchText."%') 
        OR alt_code LIKE ('%".$SearchText."%'))
        ORDER BY idkategori, namaproduct");
    }else {
        $result = db_query("SELECT idproduct,barcode, alt_code, namaproduct, stok, 
        hargajual,hargapokok,idkategori 
        FROM product WHERE status_product = 1 
        ORDER BY idkategori, namaproduct");
    }
	$items = array();
	while ($data = db_fetch_object($result)) {
		$diskon = 0;
		if ($data->idproduct) {
			$idpelanggan = 0;
			if (isset($request["idpelanggan"])){
				$idpelanggan = $request["idpelanggan"];
			}
			$result2 = db_query(
				"SELECT besardiskon FROM diskonkategori WHERE idpelanggan='%d' AND idkategori='%d'",
				$idpelanggan,
				$data->idkategori
			);
			$datadiskon = db_fetch_object($result2);
			if (!empty($datadiskon) && $datadiskon->besardiskon >= 0) {
				$data->diskon = $datadiskon->besardiskon;
			}
		}
		if (!empty($data->idkategori)){
			$items[$dataCategory[$data->idkategori]->kategori][] = $data;
		}
	}
	return $items;
}
function serverSideGetProductByGrup($request){
	$dataCategory = serverSideGetCategoryProduct();
	$result = db_query("SELECT idproduct,barcode, alt_code, namaproduct, stok, hargajual,hargapokok,idkategori FROM product");
	$items = array();
	while ($data = db_fetch_object($result)) {
		$diskon = 0;
		if ($data->idproduct) {
			$idpelanggan = 0;
			if (isset($request["idpelanggan"])){
				$idpelanggan = $request["idpelanggan"];
			}
			$result2 = db_query(
				"SELECT besardiskon FROM diskonkategori WHERE idpelanggan='%d' AND idkategori='%d'",
				$idpelanggan,
				$data->idkategori
			);
			$datadiskon = db_fetch_object($result2);
			if (!empty($datadiskon) && $datadiskon->besardiskon >= 0) {
				$data->diskon = $datadiskon->besardiskon;
			}
		}
		if (!empty($data->idkategori)){
			$items[$dataCategory[$data->idkategori]->kategori][] = $data;
		}
	}
	return $items;
}
function serverSideGetDataMeja($request){
	$result = db_query("SELECT id,barcodemeja, kodemeja, meja, keterangan FROM meja");
	$items = array();
	while ($data = db_fetch_object($result)) {
		$items[] = $data;
	}
	return $items;
}
function serverSideGetAllProduct($request){
	$result = db_query("SELECT idproduct,barcode, alt_code, 
    namaproduct, stok, hargajual,hargapokok,idkategori FROM product 
    WHERE status_product = 1
    ORDER BY idkategori, namaproduct");
	$items = array();
	while ($data = db_fetch_object($result)) {
		$diskon = 0;
		if ($data->idproduct) {
			$idpelanggan = 0;
			if (isset($request["idpelanggan"])){
				$idpelanggan = $request["idpelanggan"];
			}
			$result2 = db_query(
				"SELECT besardiskon FROM diskonkategori WHERE idpelanggan='%d' AND idkategori='%d'",
				$idpelanggan,
				$data->idkategori
			);
			$datadiskon = db_fetch_object($result2);
			if (!empty($datadiskon) && $datadiskon->besardiskon >= 0) {
				$data->diskon = $datadiskon->besardiskon;
			}
		}
		$items[] = $data;
	}
	return $items;
}
function serverSidePostOrder($request){
	header('Access-Control-Allow-Origin: *');
	$postData = $request;
	$savedData = array();
	if (count($postData)){
		$newData = array();
		$Keterangan = '';
		foreach ($postData as $key => $dataVal){
			if ($key != 'request_data' && $key != 'keterangan'){
				$splitKey = explode('__', $key);
				$newData[$splitKey[1]][$splitKey[0]] = $dataVal;
			}else{
                $Keterangan = $dataVal;
            }
		}
		$savedData = saveCustomerOrderAndroid($newData, $Keterangan);
	}
	return $savedData;
}

function saveCustomerOrderAndroid($postData = null, $Keterangan = ''){
    $dataPremis = get_data_premis_server_side();
    date_default_timezone_set(date_default_timezone_get());
	if (!empty($postData) && count($postData)){
		$savedData = array();
		$totalBelanja = 0;
		$totalModal = 0;
		for ($i = 0;$i < count($postData);$i++){
			$totalBelanja = $totalBelanja + ($postData[$i]['hargajual'] * $postData[$i]['jumlah']);
			$totalModal = $totalModal + ($postData[$i]['hargamodal'] * $postData[$i]['jumlah']);
			$idMeja = $postData[$i]['idmeja'];
		}
		$no_nota = createEAN13CodeServerSide(getRandomStringServerSide(9));
		$carabayar = 'KEMUDIAN';
		$nokartu = '';
		//date_default_timezone_set('Asia/Jakarta');
		$waktujual = date("Y-m-d H:i:s");
		$splitTanggal = explode('-', date('Y-m-d'));
		$splitJam = explode(':',date("H:i:s"));
		$intTanggal = mktime($splitJam[0],$splitJam[1],$splitJam[2],$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
		$savedData['penjualan']['nonota'] = $no_nota;
		$savedData['penjualan']['idpemakai'] = 4;
		$savedData['penjualan']['total'] = $totalBelanja;
		$savedData['penjualan']['carabayar'] = $carabayar;
		$savedData['penjualan']['bayar'] = 0;
		$savedData['penjualan']['nokartu'] = $nokartu;
		$savedData['penjualan']['tglorder'] = $waktujual;
		$savedData['penjualan']['idpelanggan'] = 0;
		$savedData['penjualan']['idmeja'] = $idMeja;
		$savedData['penjualan']['totalmodal'] = $totalModals;
        $savedData['penjualan']['keterangan'] = $Keterangan;
        $savedData['penjualan']['ppn'] = $dataPremis->ppn_value;
        $savedData['penjualan']['total_plus_ppn'] = $totalBelanja + ($totalBelanja*($dataPremis->ppn_value/100));
        db_query("INSERT INTO customer_order (nonota, idpemakai, total, carabayar, bayar, nokartu, 
		tglorder, idpelanggan, keterangan, idmeja, totalmodal, android_order, ppn, total_plus_ppn)
		VALUES ('%s', '%d', '%f', '%s', '%f', '%s', '%s', '%d', '%s','%d','%f','%d','%f','%f')",
		$no_nota, 4, $totalBelanja, $carabayar, 0, $nokartu, $waktujual,0,$Keterangan.' => Android Order',
            $idMeja,$totalModals,1, $dataPremis->ppn_value, $savedData['penjualan']['total_plus_ppn']);
		$idOrder = db_result(db_query("SELECT id FROM customer_order WHERE nonota='%s'", $no_nota));
		for ($i = 0;$i < count($postData);$i++){
			$detailData = array();
			$IDPRODUK =	$postData[$i]['idproduct'];
			$detailData['idproduk'] = $IDPRODUK;
			$QTY = $postData[$i]['jumlah'];
			$detailData['qty'] = $QTY;
			$HARGAJUAL = $postData[$i]['hargajual'];
			$detailData['hargajual'] = $HARGAJUAL;
			$DISKON = 0;
			$detailData['diskon'] = $DISKON;
			$HARGAPOKOK = $postData[$i]['hargamodal'];
			$detailData['hargapokok'] = $HARGAPOKOK;
			$detailbarcode = createEAN13CodeServerSide(getRandomStringServerSide(9));
			$detailData['detailbarcode'] = $detailbarcode;
			db_query("INSERT INTO detailcustomerorder(idcustomerorder, idproduct, jumlah,
			hargapokok, hargajual, diskon, sisa, masuk, perkiraan_ambil, detailbarcode,
			outstanding) VALUES ('%d', '%d', '%f', '%f', '%f', '%f', '%f','%d','%d','%s','%d')",
				$idOrder,$IDPRODUK,$QTY,$HARGAPOKOK,$HARGAJUAL,$DISKON,$QTY,$intTanggal,$intTanggal,
				$detailbarcode,$QTY);
			$savedData['detailpenjualan'][] = $detailData;
		}
	}
	return $savedData;
}
function createEAN13CodeServerSide($number){
	$code = '899' . str_pad($number, 9, '0');
	$weightflag = true;
	$sum = 0;
	// Weight for a digit in the checksum is 3, 1, 3.. starting from the last digit.
	// loop backwards to make the loop length-agnostic. The same basic functionality
	// will work for codes of different lengths.
	for ($i = strlen($code) - 1; $i >= 0; $i--)
	{
		$sum += (int)$code[$i] * ($weightflag?3:1);
		$weightflag = !$weightflag;
	}
	$code .= (10 - ($sum % 10)) % 10;
	return $code;
}
function getRandomStringServerSide($length=22)
{
	$key = '';
	$keys = array_merge(range(0, 9));
	for ($i = 0; $i < $length; $i++) {
		mt_srand((double)microtime() * 10000000);
		$key .= $keys[array_rand($keys)];
	}
	return $key;
}

function get_data_premis_server_side(){
	$strSQL = 'SELECT id, zone, nama, alamat, telepon, whatsapp, bbm, telegram, ';
	$strSQL .= 'email, website, created, changed, uid, ppn_value, currency_code, ';
	$strSQL .= 'currency_symbol, thousand_separator, decimal_separator,ppn_symbol,';
    $strSQL .= 'bahasa,kritik_saran,decimal_digit FROM cms_datapremis LIMIT 1';
	$result = db_query($strSQL);
	$arrayData = db_fetch_object($result);
	return $arrayData;
}

function get_number_format_server_side(&$currencySym, &$thousandSep, &$decimalSep){
	$dataPremis = get_data_premis_server_side();
	if (!empty($dataPremis)){
		$currencySym = $dataPremis->currency_symbol;
		$thousandSep = $dataPremis->thousand_separator;
		$decimalSep = $dataPremis->decimal_separator;
	}else{
		$currencySym = 'Rp.';
		$thousandSep = '.';
		$decimalSep = ',';
	}
	return $dataPremis;
}
function set_default_time_zone_server_side(){
	$defaultTimeZone = date_default_timezone_get();
	if ($defaultTimeZone != 'Asia/Kuala_Lumpur') {
        date_default_timezone_set('Asia/Kuala_Lumpur');
    }
}
function serverSideArrayKategori($request){
    $strSQL = 'SELECT idkategori, kodekategori, kategori FROM kategori';
    $result = db_query($strSQL);
    $output = array();
    while($data = db_fetch_object($result)){
        $output[$data->idkategori] = $data->kodekategori.' => '.$data->kategori;
    }
    $strSQL = 'SELECT idkategori FROM product WHERE idproduct=%d';
    $idKategori = db_result(db_query($strSQL, $request['idproduk']));
    $output['selected'] =  $idKategori;
    return $output;
}
function serverSideArraySubKategori($request){
    $strSQL = 'SELECT idkategori FROM product WHERE idproduct=%d';
    $idKategori = db_result(db_query($strSQL, $request['idproduk']));
    $strSQL = 'SELECT idsubkategori, kodesubkategori, subkategori FROM subkategori WHERE idkategori=%d';
    $result = db_query($strSQL,$idKategori);
    $output = array();
    while($data = db_fetch_object($result)){
        $output[$data->idsubkategori] = $data->kodesubkategori.' => '.$data->subkategori;
    }
    $strSQL = 'SELECT idsubkategori FROM product WHERE idproduct=%d';
    $idSubKategori = db_result(db_query($strSQL, $request['idproduk']));
    $output['selected'] =  $idSubKategori;
    return $output;
}
function serverSideArrayGrupMenu($request){
    $strSQL = 'SELECT id, kode_grup, nama_grup, keterangan, weight FROM grup_menu';
    $result = db_query($strSQL);
    $output = array();
    while($data = db_fetch_object($result)){
        $output[$data->id] = $data->kode_grup.' => '.$data->nama_grup;
    }
    $strSQL = 'SELECT id_grup_menu FROM product WHERE idproduct=%d';
    $idGrup = db_result(db_query($strSQL, $request['idproduk']));
    $output['selected'] =  $idGrup;
    return $output;
}
function serverSideCheckLogin($request){
	$username = $request['userkonter'];
	$password = $request['passkonter'];
	$userID = db_result(db_query("SELECT uid FROM cms_users WHERE name='%s' AND pass='%s'", $username, md5($password)));
	$retArray = array();
	$retData = new stdClass();
	$retData->uservalid = 0;
	if ($userID > 0){
		$retData->uservalid = 1;
	}
	$retArray[] = $retData;
	return $retArray;
}
function serverSideArraySatuan($request){
    $strSQL = 'SELECT satuan FROM satuan ORDER BY satuan';
    $result = db_query($strSQL);
    $output = array();
    while($data = db_fetch_object($result)){
        $output[$data->satuan] = $data->satuan;
    }
    $strSQL = 'SELECT satuan FROM product WHERE idproduct=%d';
    $satuan = db_result(db_query($strSQL, $request['idproduk']));
    $output['selected'] =  $satuan;
    return $output;
}
function serverSideArrayHutang($request)
{
    global $baseDirectory;
    get_number_format_server_side($currencySym, $thousandSep, $decimalSep);
    $DataPremis = get_data_premis_server_side();
    $Digit = $DataPremis->decimal_digit;
    $pageStart = $_GET['start'];
    $pageLength = $_GET['length'];
    $IdPelanggan = $_GET['idpelanggan'];
    $searchArray = $_REQUEST['search'];
    $searchQuery = $searchArray['value'];
    $arrayColumn = array(
        '', 'jual.nonota', 'jual.total', 'jual.bayar', 'piutang.besarhutang'
    );
    $orderColumnArray = $_REQUEST['order'];
    $orderColumn = $arrayColumn[$orderColumnArray[0]['column']] . ' ' . $orderColumnArray[0]['dir'];
    if (is_null($pageStart)) {
        $pageStart = 0;
    }
    if (is_null($pageLength)) {
        $pageLength = 100;
    }
    $firstRecord = $pageStart;
    $lastRecord = $pageStart + $pageLength;

    $strSQL = "SELECT piutang.idpelanggan, piutang.idpenjualan, piutang.besarhutang, jual.nonota,";
    $strSQL .= "jual.total, jual.bayar, SUBSTR(jual.tglpenjualan,1,10) AS tgljual ";
    $strSQL .= "FROM detailpiutang AS piutang ";
    $strSQL .= "LEFT JOIN penjualan AS jual ON piutang.idpenjualan = jual.idpenjualan WHERE jual.idpelanggan='%d' AND 1=1 ";

    $strSQLFilteredTotal = "SELECT COUNT(piutang.idpelanggan) ";
    $strSQLFilteredTotal .= "FROM detailpiutang AS piutang ";
    $strSQLFilteredTotal .= "LEFT JOIN penjualan AS jual ON piutang.idpenjualan = jual.idpenjualan WHERE jual.idpelanggan='%d' AND 1=1 ";

    $StrSqlAllRecord = $strSQLFilteredTotal;

    $strCriteria = "";
    if (!empty($searchQuery)) {
        $strCriteria .= "AND (jual.nonota LIKE '%%%s%%') ";
    }
    if ($pageLength != '-1'){
        $strSQL .= $strCriteria." ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria." ORDER BY $orderColumn";
    }
    //$strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    $strSQLFilteredTotal .= $strCriteria;
    if (!empty($searchQuery)) {
        $result = db_query($strSQL, $IdPelanggan, $searchQuery, $firstRecord, $lastRecord);
        $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $IdPelanggan, $searchQuery));
    } else {
        $result = db_query($strSQL, $IdPelanggan, $firstRecord, $lastRecord);
        $recordsFiltered = db_result(db_query($strSQLFilteredTotal,$IdPelanggan));
    }
    $output = array();
    while ($data = db_fetch_object($result)) {
        $rowData = array();
        $lihatdetailpenjualan = '<img title="Klik untuk melihat detail penjualan" onclick="view_detail(\''.$data->idpenjualan.'\',\''.$data->nonota.'\');" src="'.$baseDirectory.'/misc/media/images/forward_enabled.ico">';
        $rowData[] = $lihatdetailpenjualan;
        $rowData[] = $data->nonota;
        $rowData[] = number_format($data->total,$Digit,$decimalSep,$thousandSep);
        $rowData[] = number_format($data->bayar,$Digit,$decimalSep,$thousandSep);
        $rowData[] = number_format($data->besarhutang,$Digit,$decimalSep,$thousandSep);
        $rowData[] = $data->tgljual;
        $output[] = $rowData;
    }
    $recordsTotal = db_result(db_query($StrSqlAllRecord, $IdPelanggan));
    return array(
        "draw" => isset ($request['draw']) ?
            intval($request['draw']) :
            0,
        "recordsTotal" => intval($recordsTotal),
        "recordsFiltered" => intval($recordsFiltered),
        "data" => $output
    );
}
function serverSideArraySupplier($request){
    $strSQL = 'SELECT idsupplier, kodesupplier, namasupplier FROM supplier';
    $result = db_query($strSQL);
    $output = array();
    while($data = db_fetch_object($result)){
        $output[$data->idsupplier] = $data->namasupplier;
    }
    $strSQL = 'SELECT idsupplier FROM product_supplier WHERE idproduct=%d';
    $result = db_query($strSQL, $request['idproduk']);
    $SelectedSupplier = array();
    while ($data = db_fetch_object($result)){
        $SelectedSupplier[$data->idsupplier] = $data->idsupplier;
    }
    $output['selected'] = $SelectedSupplier;
    return $output;
}

function serverSidePembelian($request){
    global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym,$tSep,$dSep);
    $pageStart = $_GET['start'];
    $pageLength = $_GET['length'];
    $searchArray = $_REQUEST['search'];
    $tglAwal = $_REQUEST['tglawal'].' 00:00';
    $tglAkhir = $_REQUEST['tglakhir'].' 23:59';
    $idsupplier = $_REQUEST['idsupplier'];
    $searchQuery = $searchArray['value'];
    $TypePelanggan = $_REQUEST['type_pelanggan'];
    $arrayhari = arrayHariServerSide();
    $arrayColumn = array(
        'pemb.idpembelian','pemb.nonota','pemb.tglpembelian','pemb.tglpembelian',
        'pemb.total','pemb.totalmodal','(pemb.total - pemb.totalmodal)','pemb.carabayar',
        'pemb.bayar','pemb.kembali','user.name','supp.namasupplier','pemb.jatuh_tempo'
    );
    $orderColumnArray = $_REQUEST['order'];
    $orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
    if (is_null($pageStart)){
        $pageStart = 0;
    }
    if (is_null($pageLength)){
        $pageLength = 100;
    }
    $firstRecord = $pageStart;
    $lastRecord = $pageStart + $pageLength;
    $strSQL = "SELECT pemb.idpembelian,pemb.nonota,SUBSTR(pemb.tglpembelian,1,10) AS tanggal,";
    $strSQL .= "SUBSTR(pemb.tglpembelian,12,5) AS waktu, pemb.idpemakai,pemb.total,";
    $strSQL .= "pemb.carabayar,pemb.bayar,pemb.kembali,";
    $strSQL .= "pemb.nokartu,pemb.keterangan,pemb.jatuh_tempo, user.name, supp.namasupplier, supp.idsupplier ";
    $strSQL .= "FROM pembelian AS pemb ";
    $strSQL .= "LEFT JOIN cms_users AS user ON user.uid = pemb.idpemakai ";
    $strSQL .= "LEFT JOIN supplier AS supp ON supp.idsupplier = pemb.idsupplier ";
    if (empty($idsupplier)){
        $strSQL .= "WHERE pemb.tglpembelian BETWEEN '%s' AND '%s' ";
    }else{
        $strSQL .= "WHERE pemb.tglpembelian BETWEEN '%s' AND '%s' AND pemb.idsupplier=%d ";
    }
    $strSQLFilteredTotal = "SELECT COUNT(pemb.idpembelian) ";
    $strSQLFilteredTotal .= "FROM pembelian AS pemb ";
    $strSQLFilteredTotal .= "LEFT JOIN cms_users AS user ON user.uid = pemb.idpemakai ";
    $strSQLFilteredTotal .= "LEFT JOIN supplier AS supp ON supp.idsupplier = pemb.idsupplier ";
    if (empty($idsupplier)){
        $strSQLFilteredTotal .= "WHERE pemb.tglpembelian BETWEEN '%s' AND '%s' ";
    }else{
        $strSQLFilteredTotal .= "WHERE pemb.tglpembelian BETWEEN '%s' AND '%s' AND pemb.idsupplier=%d ";
    }
    $strCriteria = "";
    if (!empty($searchQuery)){
        $strCriteria .= "AND (pemb.nonota LIKE '%%%s%%' OR SUBSTR(pemb.tglpembelian,1,10) LIKE '%%%s%%' ";
        $strCriteria .= "OR SUBSTR(pemb.tglpembelian,11,9) LIKE '%%%s%%' OR user.name LIKE '%%%s%%' ";
        $strCriteria .= "OR supp.namasupplier LIKE '%%%s%%' OR pemb.carabayar LIKE '%%%s%%' ";
        $strCriteria .= ")";
    }
    $strCriteria .= ' AND pemb.status_supplier = '.$TypePelanggan;
    if ($pageLength == -1){
        $strSQL .= $strCriteria." ORDER BY $orderColumn";
    }else{
        $strSQL .= $strCriteria." ORDER BY $orderColumn LIMIT %d, %d";
    }
    $strSQLFilteredTotal .= $strCriteria;
    if (!empty($searchQuery)){
        if (empty($idsupplier)) {
            if ($pageLength == -1) {
                $result = db_query(
                    $strSQL,
                    $tglAwal,
                    $tglAkhir,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery
                );
                $recordsFiltered = db_result(
                    db_query(
                        $strSQLFilteredTotal,
                        $tglAwal,
                        $tglAkhir,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery
                    )
                );
            }else{
                if ($pageLength == -1) {
                    $result = db_query(
                        $strSQL,
                        $tglAwal,
                        $tglAkhir,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery
                    );
                }else{
                    $result = db_query(
                        $strSQL,
                        $tglAwal,
                        $tglAkhir,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $firstRecord,
                        $lastRecord
                    );
                }
                $recordsFiltered = db_result(
                    db_query(
                        $strSQLFilteredTotal,
                        $tglAwal,
                        $tglAkhir,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery,
                        $searchQuery
                    )
                );
            }
        }else{
            if ($pageLength == -1) {
                $result = db_query(
                    $strSQL,
                    $tglAwal,
                    $tglAkhir,
                    $idsupplier,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery
                );
            }else{
                $result = db_query(
                    $strSQL,
                    $tglAwal,
                    $tglAkhir,
                    $idsupplier,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $firstRecord,
                    $lastRecord
                );
            }
            $recordsFiltered = db_result(
                db_query(
                    $strSQLFilteredTotal,
                    $tglAwal,
                    $tglAkhir,
                    $idsupplier,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery
                )
            );
        }
    }else{
        if (empty($idsupplier)) {
            $result = db_query($strSQL, $tglAwal, $tglAkhir, $firstRecord, $lastRecord);
            $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir));
        }else{
            $result = db_query($strSQL, $tglAwal, $tglAkhir, $idsupplier, $firstRecord, $lastRecord);
            $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir, $idsupplier));
        }
    }
    $output = array();
    while ($data = db_fetch_object($result)){
        $rowData = array();
        $imgDetail = "<img title=\"Klik untuk melihat detail pembelian\" onclick=\"view_detail(".$data->idpembelian.",'".$data->nonota."',".$data->idsupplier.");\" src=\"$baseDirectory/misc/media/images/forward_enabled.ico\" width=\"22\">";
        $tombolhapus = "<img title=\"Klik untuk menghapus pembelian\" onclick=\"delete_pembelian(".$data->idpembelian.",'".$data->nonota."');\" src=\"$baseDirectory/misc/media/images/del.ico\" width=\"22\">";
        $rowData[] = $imgDetail;
        $rowData[] = $tombolhapus;
        $rowData[] = $data->nonota;
        $indexhari = date('w', strtotime($data->tanggal));
        $rowData[] = $arrayhari[$indexhari];
        $rowData[] = $data->tanggal;
        $rowData[] = $data->waktu;
        $rowData[] = number_format($data->total,$DDigit,$dSep,$tSep);
        $rowData[] = $data->carabayar;
        $rowData[] = number_format($data->bayar,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->kembali,$DDigit,$dSep,$tSep);
        $rowData[] = $data->jatuh_tempo;
        $rowData[] = $data->namasupplier;
        $rowData[] = $data->name;
        $tombolprint = "<img title=\"Klik untuk mencetak barcode\" onclick=\"print_pembelian(".$data->idpembelian.");\" src=\"$baseDirectory/misc/media/images/print.png\" width=\"22\">";
        $rowData[] = $tombolprint;
        $rowData[] = $data->idpembelian;
        $output[] = $rowData;
    }
    if (empty($idsupplier)) {
        $recordsTotal = db_result(
            db_query(
                "SELECT COUNT(idpembelian) FROM pembelian WHERE tglpembelian BETWEEN '%s' AND '%s'",
                $tglAwal,
                $tglAkhir
            )
        );
    }else{
        $recordsTotal = db_result(
            db_query(
                "SELECT COUNT(idpembelian) FROM pembelian WHERE tglpembelian BETWEEN '%s' AND '%s' AND idsupplier=%d",
                $tglAwal,
                $tglAkhir,
                $idsupplier
            )
        );
    }
    return array(
        "draw"            => isset ( $request['draw'] ) ?
            intval( $request['draw'] ) :
            0,
        "recordsTotal"    => intval( $recordsTotal ),
        "recordsFiltered" => intval( $recordsFiltered ),
        "data"            => $output,
        "sql"			  => $strSQL,
        "tglawal"		  => $tglAwal,
        "tglakhir"		  => $tglAkhir,
    );
}

function serverSidePembelian2($request){
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym,$tSep,$dSep);
    $pageStart = $_GET['start'];
    $pageLength = $_GET['length'];
    $searchArray = $_REQUEST['search'];
    $tglAwal = $_REQUEST['tglawal'].' 00:00';
    $tglAkhir = $_REQUEST['tglakhir'].' 23:59';
    $idSupplier = $_REQUEST['idsupplier'];
    $searchQuery = $searchArray['value'];
    $arrayColumn = array(
        'prod.barcode','prod.namaproduct','supp.namasupplier',
        'totalqty','minhargapokok','maxhargapokok','totalmodal'
    );
    $orderColumnArray = $_REQUEST['order'];
    $orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
    if (is_null($pageStart)){
        $pageStart = 0;
    }
    if (is_null($pageLength)){
        $pageLength = 100;
    }
    $firstRecord = $pageStart;
    $lastRecord = $pageStart + $pageLength;
    $strSQL = "SELECT detail.idproduct,prod.barcode,prod.namaproduct,";
    $strSQL .= "GROUP_CONCAT(DISTINCT supp.namasupplier SEPARATOR '<br/>') AS namasupplier, SUM(detail.jumlah) AS totalqty,";
    $strSQL .= "MIN(detail.hargapokok) AS minhargapokok,MAX(detail.hargapokok) AS maxhargapokok,";
    $strSQL .= "SUM(detail.hargapokok*detail.jumlah) AS totalmodal ";
    $strSQL .= "FROM detailpembelian AS detail ";
    $strSQL .= "LEFT JOIN pembelian AS pemb ON detail.idpembelian = pemb.idpembelian ";
    $strSQL .= "LEFT JOIN product AS prod ON detail.idproduct = prod.idproduct ";
    $strSQL .= "LEFT JOIN supplier AS supp ON detail.idsupplier = supp.idsupplier ";
    if (!empty($idSupplier)){
        $strSQL .= "WHERE pemb.tglpembelian BETWEEN '%s' AND '%s' AND detail.idsupplier = %d ";
    }else{
        $strSQL .= "WHERE pemb.tglpembelian BETWEEN '%s' AND '%s' ";
    }
    $strSQLFilteredTotal = "SELECT COUNT(DISTINCT detail.idproduct) ";
    $strSQLFilteredTotal .= "FROM detailpembelian AS detail ";
    $strSQLFilteredTotal .= "LEFT JOIN pembelian AS pemb ON detail.idpembelian = pemb.idpembelian ";
    $strSQLFilteredTotal .= "LEFT JOIN product AS prod ON detail.idproduct = prod.idproduct ";
    $strSQLFilteredTotal .= "LEFT JOIN supplier AS supp ON detail.idsupplier = supp.idsupplier ";
    if (!empty($idSupplier)){
        $strSQLFilteredTotal .= "WHERE pemb.tglpembelian BETWEEN '%s' AND '%s' AND detail.idsupplier = %d ";
    }else {
        $strSQLFilteredTotal .= "WHERE pemb.tglpembelian BETWEEN '%s' AND '%s' ";
    }
    $strCriteria = "";
    if (!empty($searchQuery)){
        $strCriteria .= "AND (prod.barcode LIKE '%%%s%%' OR prod.namaproduct LIKE '%%%s%%' ";
        $strCriteria .= "OR supp.namasupplier LIKE '%%%s%%' ";
        $strCriteria .= ")";
    }
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " GROUP BY detail.idproduct ORDER BY $orderColumn LIMIT %d, %d";
    }else{
        $strSQL .= $strCriteria . " GROUP BY detail.idproduct ORDER BY $orderColumn";
    }
    $strSQLFilteredTotal .= $strCriteria;
    if (!empty($searchQuery)){
        if (!empty($idSupplier)){
            if ($pageLength != -1) {
                $result = db_query(
                    $strSQL,
                    $tglAwal,
                    $tglAkhir,
                    $idSupplier,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $firstRecord,
                    $lastRecord
                );
            }else{
                $result = db_query(
                    $strSQL,
                    $tglAwal,
                    $tglAkhir,
                    $idSupplier,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery
                );
            }
            $recordsFiltered = db_result(
                db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir, $idSupplier, $searchQuery, $searchQuery, $searchQuery)
            );
        }else {
            if ($pageLength != -1) {
                $result = db_query(
                    $strSQL,
                    $tglAwal,
                    $tglAkhir,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery,
                    $firstRecord,
                    $lastRecord
                );
            }else{
                $result = db_query(
                    $strSQL,
                    $tglAwal,
                    $tglAkhir,
                    $searchQuery,
                    $searchQuery,
                    $searchQuery
                );
            }
            $recordsFiltered = db_result(
                db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir, $searchQuery, $searchQuery, $searchQuery)
            );
        }
    }else{
        if (!empty($idSupplier)) {
            if ($pageLength != -1) {
                $result = db_query($strSQL, $tglAwal, $tglAkhir, $idSupplier, $firstRecord, $lastRecord);
            }else{
                $result = db_query($strSQL, $tglAwal, $tglAkhir, $idSupplier);
            }
            $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir, $idSupplier));
        }else{
            if ($pageLength != -1) {
                $result = db_query($strSQL, $tglAwal, $tglAkhir, $firstRecord, $lastRecord);
            }else{
                $result = db_query($strSQL, $tglAwal, $tglAkhir);
            }
            $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $tglAwal, $tglAkhir));
        }
    }
    $output = array();
    while ($data = db_fetch_object($result)){
        $rowData = array();
        $rowData[] = $data->barcode;
        $rowData[] = $data->namaproduct;
        $rowData[] = $data->namasupplier;
        $rowData[] = number_format($data->totalqty,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->minhargapokok,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->maxhargapokok,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->totalmodal,$DDigit,$dSep,$tSep);
        $rowData[] = $data->idproduct;
        $output[] = $rowData;
    }
    $recordsTotal = db_result(db_query("SELECT COUNT(DISTINCT idproduct) FROM detailpembelian AS detail LEFT JOIN pembelian AS pemb ON detail.idpembelian=pemb.idpembelian WHERE pemb.tglpembelian BETWEEN '%s' AND '%s'",$tglAwal,$tglAkhir));
    return array(
        "draw"            => isset ( $request['draw'] ) ?
            intval( $request['draw'] ) :
            0,
        "recordsTotal"    => intval( $recordsTotal ),
        "recordsFiltered" => intval( $recordsFiltered ),
        "data"            => $output,
        "query"           => $strSQL,
    );
}

function serverSideDetailPembelian($request){
    global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym,$tSep,$dSep);
    $pageStart = $_GET['start'];
    if (isset($_GET['asal']) && $_GET['asal'] == 'pembelian'){
        $pageLength = -1;
    }else {
        $pageLength = $_GET['length'];
    }
    $searchArray = $_REQUEST['search'];
    $idPembelian = $_REQUEST['idpembelian'];
    $searchQuery = $searchArray['value'];
    $arrayColumn = array(
        1 => 'product.barcode',
        2 => 'product.namaproduct',
        3 => 'detail.jumlah',
        4 => 'detail.hargapokok',
        5 => '(detail.jumlah*detail.hargapokok)',
    );
    $orderColumnArray = isset($_REQUEST['order']) && !empty($_REQUEST['order']) ? $_REQUEST['order'] : array( 0 => array('column' => 1, 'dir' => 'ASC'));
    $orderColumn = $arrayColumn[$orderColumnArray[0]['column']].' '.$orderColumnArray[0]['dir'];
    if (is_null($pageStart)){
        $pageStart = 0;
    }
    if (is_null($pageLength)){
        $pageLength = 25;
    }

    $firstRecord = $pageStart;
    $lastRecord = $pageStart + $pageLength;
    $strSQL = 'SELECT detail.iddetail,product.barcode, product.namaproduct, detail.jumlah,';
    $strSQL .= 'detail.hargapokok,(detail.hargapokok*detail.jumlah) AS subtotal ';
    $strSQL .= 'FROM detailpembelian detail LEFT JOIN product product ';
    $strSQL .= 'ON detail.idproduct=product.idproduct ';
    $strSQL .= 'LEFT JOIN supplier supp ON product.idsupplier=supp.idsupplier ';
    $strSQL .= 'WHERE detail.idpembelian=%d ';
    $strSQLFilteredTotal = 'SELECT COUNT(detail.iddetail) FROM ';
    $strSQLFilteredTotal .= 'detailpembelian detail LEFT JOIN product product ';
    $strSQLFilteredTotal .= 'ON detail.idproduct=product.idproduct ';
    $strSQLFilteredTotal .= 'LEFT JOIN supplier supp ON product.idsupplier=supp.idsupplier ';
    $strSQLFilteredTotal .= 'WHERE detail.idpembelian=%d ';
    $strCriteria = "";
    if (!empty($searchQuery)){
        $strCriteria .= "AND (product.barcode LIKE '%%%s%%' OR ";
        $strCriteria .= "product.namaproduct LIKE '%%%s%%'";
        $strCriteria .= ")";
    }
    if ($pageLength == -1){
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }else {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    }
    $strSQLFilteredTotal .= $strCriteria;
    if (!empty($searchQuery)) {
        if ($pageLength == -1){
            $result = db_query($strSQL, $idPembelian, $searchQuery, $searchQuery);
        }else {
            $result = db_query($strSQL, $idPembelian, $searchQuery, $searchQuery, $firstRecord, $lastRecord);
        }
        $recordsFiltered = db_result(
            db_query($strSQLFilteredTotal, $idPembelian, $searchQuery, $searchQuery)
        );
    }else{
        if ($pageLength == -1) {
            $result = db_query($strSQL, $idPembelian);
        }else {
            $result = db_query($strSQL, $idPembelian, $firstRecord, $lastRecord);
        }
        $recordsFiltered = db_result(db_query($strSQLFilteredTotal,$idPembelian));
    }
    $output = array();
    while($data = db_fetch_object($result)){
        $rowData = array();
        $deletebutton = '<img title="Klik untuk menghapus detail pembelian" onclick="hapus_detail('.$data->iddetail.',\''.$data->namaproduct.'\');" src="'.$baseDirectory.'/misc/media/images/del.ico" width="22">';
        $printbutton = '<img title="Klik untuk mencetak sticker batch detail pembelian" onclick="print_detail('.$idPembelian.','.$data->iddetail.',\''.$data->namaproduct.'\');" src="'.$baseDirectory.'/misc/media/images/print.png" width="22">';
        $rowData[] = $deletebutton;
        $rowData[] = $data->barcode;
        $rowData[] = $data->namaproduct;
        $rowData[] = $data->jumlah;
        $rowData[] = number_format($data->hargapokok,$DDigit,$dSep,$tSep);
        $rowData[] = number_format($data->subtotal,$DDigit,$dSep,$tSep);
        $rowData[] = $printbutton;
        $rowData[] = $data->iddetail;
        $output[] = $rowData;
    }
    $recordsTotal = db_result(db_query("SELECT COUNT(iddetail) FROM detailpembelian WHERE idpembelian=%d", $idPembelian));
    return array(
        "draw"            => isset ( $request['draw'] ) ?
            intval( $request['draw'] ) :
            0,
        "recordsTotal"    => intval( $recordsTotal ),
        "recordsFiltered" => intval( $recordsFiltered ),
        "data"            => $output
    );
}

function arrayHariServerSide(){
    $hari_array = array('Minggu','Senin','Selasa','Rabu','Kamis','Jumat','Sabtu');
    return $hari_array;
}

function serverSideDataPremis($request){
    $DataPremis = get_data_premis_server_side();
    $ReturnArray = array();
    $ReturnArray[] = $DataPremis;
    return $ReturnArray;
}

function serverSidePenjualanAndroid($request){
    header('Access-Control-Allow-Origin: *');
    $tglAwal = $_REQUEST['tglawal'].' 00:00';
    $tglAkhir = $_REQUEST['tglakhir'].' 23:59';
    $strSQL = "SELECT penj.idpenjualan,penj.nonota,SUBSTR(penj.tglpenjualan,1,10) AS tanggal,";
    $strSQL .= "SUBSTR(penj.tglpenjualan,11,9) AS waktu, penj.idpemakai,penj.total,penj.totalmodal,";
    $strSQL .= "(penj.total-penj.totalmodal) AS laba, penj.carabayar,penj.bayar,penj.kembali,";
    $strSQL .= "penj.nokartu,penj.keterangan,penj.insert_date, user.name, plg.namapelanggan ";
    $strSQL .= "FROM penjualan AS penj ";
    $strSQL .= "LEFT JOIN cms_users AS user ON user.uid = penj.idpemakai ";
    $strSQL .= "LEFT JOIN pelanggan AS plg ON plg.idpelanggan = penj.idpelanggan ";
    $strSQL .= "WHERE penj.tglpenjualan BETWEEN '%s' AND '%s' ";
    $strSQL .= "ORDER BY tglpenjualan";
    $result = db_query($strSQL,$tglAwal,$tglAkhir);
    $output = array();
    while ($data = db_fetch_object($result)){
        $output[] = $data;
    }
    return $output;
}

function serverSidePostSales($request){
    header('Access-Control-Allow-Origin: *');
    $postData = $request;
    $savedData = array();
    if (count($postData)){
        $newData = array();
        foreach ($postData as $key => $dataVal){
            if ($key != 'request_data'){
                $splitKey = explode('__', $key);
                $newData[$splitKey[1]][$splitKey[0]] = $dataVal;
            }
        }
        $savedData = saveSalesAndroid($newData);
    }
    return $savedData;
}

function saveSalesAndroid($postData = null){
    if (!empty($postData) && count($postData)){
        $savedData = array();
        $username = $postData['']['userkonter'];
        $password = $postData['']['passkonter'];
        $savedData['username'] = $username;
        $savedData['password'] = $password;
        $userID = db_result(db_query("SELECT uid FROM cms_users WHERE name='%s' AND pass='%s'", $username, md5($password)));
        $savedData['user_id'] = $userID;
        if (!empty($userID) && count($postData) && isset($postData[0])) {
            $userTimeZone = db_result(db_query("SELECT timezone_name FROM cms_users WHERE uid=%d", $userID));
            date_default_timezone_set($userTimeZone);
            $savedData['user_timezone'] = $userTimeZone;
            $totalBelanja = 0;
            $totalModal = 0;
            for ($i = 0; $i < count($postData); $i++) {
                $totalBelanja = $totalBelanja + ($postData[$i]['hargajual'] * $postData[$i]['jumlah']);
                $totalModal = $totalModal + ($postData[$i]['hargamodal'] * $postData[$i]['jumlah']);
            }
            $result = db_query("SELECT idpenjualan FROM penjualan ORDER BY idpenjualan DESC LIMIT 1");
            $data = db_fetch_object($result);
            if ($data->idpenjualan > 0) {
                $next_id = $data->idpenjualan + 1;
            } else {
                $next_id = 1;
            }
            $no_nota = buat_nota_server_side($next_id);
            $carabayar = 'TUNAI';
            $bayar = $totalBelanja;
            $nokartu = '';
            $waktujual = date("Y-m-d H:i:s");
            $savedData['penjualan']['nonota'] = $no_nota;
            $savedData['penjualan']['idpemakai'] = $userID;
            $savedData['penjualan']['total'] = $totalBelanja;
            $savedData['penjualan']['carabayar'] = $carabayar;
            $savedData['penjualan']['bayar'] = $bayar;
            $savedData['penjualan']['nokartu'] = $nokartu;
            $savedData['penjualan']['tglpenjualan'] = $waktujual;
            $savedData['penjualan']['idpelanggan'] = 0;
            $savedData['penjualan']['totalmodal'] = $totalModal;
            db_query("INSERT INTO penjualan (nonota, idpemakai, total, carabayar, bayar, nokartu, 
			tglpenjualan, idpelanggan, keterangan, totalmodal, android_order)
			VALUES ('%s', '%d', '%f', '%s', '%f', '%s', '%s', '%d', '%s','%f','%d')",
                $no_nota, $userID, $totalBelanja, $carabayar, $bayar, $nokartu, $waktujual, 0, 'Penjualan Via Android', $totalModal, 1);
            $idSales = db_result(db_query("SELECT idpenjualan FROM penjualan WHERE nonota='%s'", $no_nota));
            $savedData['penjualan']['id'] = $idSales;
            for ($i = 0; $i < count($postData); $i++) {
                $detailData = array();
                $idProduct = $postData[$i]['idproduct'];
                $detailData['idproduk'] = $idProduct;
                $qtyProduct = $postData[$i]['jumlah'];
                $detailData['qty'] = $qtyProduct;
                $hargaJual = $postData[$i]['hargajual'];
                $detailData['hargajual'] = $hargaJual;
                $diskon = 0;
                $detailData['diskon'] = $diskon;
                $hargaPokok = $postData[$i]['hargamodal'];
                $detailData['hargapokok'] = $hargaPokok;
                if (!empty($idProduct) && !empty($qtyProduct)) {
                    db_query("INSERT INTO detailpenjualan(idpenjualan, idproduct, jumlah,
					hargapokok, hargajual, diskon) VALUES ('%d', '%d', '%f', '%f', '%f', '%f')",
                        $idSales, $idProduct, $qtyProduct, $hargaPokok, $hargaJual, $diskon);
                    $savedData['detailpenjualan'][] = $detailData;
                    $result = db_query("SELECT type_product, hargapokok,stok FROM product WHERE idproduct='%d'",$idProduct);
                    $data = db_fetch_object($result);
                    $stokSebelum = $data->stok;
                    if ($data->type_product == 0){
                        $stokSekarang = $stokSebelum - $qtyProduct;
                        db_query("UPDATE product SET stok='%f' WHERE idproduct='%d'",$stokSekarang,$idProduct);
                        $keterangan = 'Penjualan';
                        db_query("INSERT INTO transaksistock (idproduk, idpenjualan, stocksebelum, keluar, stocksetelah, keterangan) VALUES 	
						('%d', '%d', '%f', '%f', '%f', '%s')",$idProduct,$idSales,$stokSebelum,$qtyProduct,$stokSekarang,$keterangan);
                    }
                }
            }
            do_upload_premisdata_server_side();
        }
    }
    return $savedData;
}
function buat_nota_server_side($idpenjualan){
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

function serverSidePerubahanHargaAndroid($request){
    $returnData = json_decode($request);
    for ($i = 0;$i < count($returnData);$i++){
        $sql_update = "UPDATE product SET hargajual='%f',uploaded=0, changed=1 WHERE idproduct='%d'";
        db_query($sql_update, $returnData[$i]->hargajualbaru,$returnData[$i]->idproduct);
        db_query("INSERT INTO historyhargajual (hargasebelum, hargasesudah, uid) VALUES('%f','%f','%d')",
            $returnData[$i]->hargajual,$returnData[$i]->hargajualbaru,1);
    }
    return $returnData;
}

function serverSidePerubahanStockAndroid($request){
    $returnData = json_decode($request);
    for ($i = 0;$i < count($returnData);$i++){
        $StokSebelum = db_result(db_query('SELECT stok FROM product WHERE idproduct=%d', $returnData[$i]->idproduct));
        $sql_update = "UPDATE product SET stok='%f',uploaded=0, changed=1 WHERE idproduct='%d'";
        db_query($sql_update, $returnData[$i]->updatedstok,$returnData[$i]->idproduct);
        $SelisihStok = ($returnData[$i]->updatedstok*1) - $StokSebelum;
        if ($SelisihStok < 0){
            $keluar = abs($SelisihStok);
            $keterangan = 'Stock Take -> Stok Berkurang';
            db_query("INSERT INTO transaksistock (idproduk, stocksebelum, keluar, stocksetelah, keterangan) VALUES 
			('%d', '%f', '%f', '%f', '%s')",$returnData[$i]->idproduct,$StokSebelum,$keluar,$returnData[$i]->updatedstok,$keterangan);
        }else{
            $masuk = $SelisihStok;
            $keterangan = 'Stock Take -> Stok Bertambah';
            db_query("INSERT INTO transaksistock (idproduk, stocksebelum, masuk, stocksetelah, keterangan) VALUES 
			('%d', '%f', '%f', '%f', '%s')",$returnData[$i]->idproduct,$StokSebelum,$masuk,$returnData[$i]->updatedstok,$keterangan);
        }
    }
    return $returnData;
}

function ServerSidePettyCash($request)
{
    global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym, $tSep, $dSep);
    $pageStart = $_GET['start'];
    $pageLength = $_GET['length'];
    $searchArray = $_REQUEST['search'];
    $searchQuery = $searchArray['value'];
    $arrayColumn = array(
        0 => 'pcash.id',
        1 => 'user.name',
        2 => 'pcash.login',
        3 => 'pcash.ip',
        4 => 'pcash.petty_cash',
    );
    $orderColumnArray = $_REQUEST['order'];
    $orderColumn = $arrayColumn[$orderColumnArray[0]['column']] . ' ' . $orderColumnArray[0]['dir'];
    if (is_null($pageStart)) {
        $pageStart = 0;
    }
    if (is_null($pageLength) || $pageLength != -1) {
        $pageLength = 100;
    }
    $firstRecord = $pageStart;
    $lastRecord = $pageStart + $pageLength;
    set_default_time_zone_server_side();
    $Now = mktime(date('H') + 1, date('i'), date('s'), date('n'), date('j'), date('Y'));
    $Today = mktime(0, 0, 0, date('n'), date('j'), date('Y'));
    $StrSqlCaraBayar = 'SELECT carabayar FROM carabayar';
    $result = db_query($StrSqlCaraBayar);
    $QueryCb = array();
    while ($data = db_fetch_object($result)) {
        $DataQueryCb = new stdClass();
        $StrSqlCb = "SELECT SUM(total_bayar) FROM penjualan_bayar AS pb LEFT JOIN ";
        $StrSqlCb .= "penjualan AS penj ON pb.idpenjualan = penj.idpenjualan WHERE ";
        $StrSqlCb .= "penj.idpemakai = pcash.uid AND ";
        $StrSqlCb .= "(UNIX_TIMESTAMP(tglpenjualan) >= pcash.login AND UNIX_TIMESTAMP(tglpenjualan) <= " . $Now . ") AND ";
        $StrSqlCb .= "pb.carabayar = '" . $data->carabayar . "'";
        $DataQueryCb->sql = $StrSqlCb;
        $DataQueryCb->name = strtolower(str_replace(' ', '_', $data->carabayar));
        $QueryCb[] = $DataQueryCb;
    }
    $StrSqlKembali = "SELECT SUM(kembali) FROM penjualan AS penj WHERE penj.idpemakai = pcash.uid AND ";
    $StrSqlKembali .= "(UNIX_TIMESTAMP(tglpenjualan) >= pcash.login AND UNIX_TIMESTAMP(tglpenjualan) <= " . $Now . ") ";
    $StrSqlKembali .= "AND penj.kembali >= 0";
    $strSQL = "SELECT pcash.id, user.name, pcash.login, pcash.ip, ";
    for ($i = 0; $i < count($QueryCb); $i++) {
        $strSQL .= "(" . $QueryCb[$i]->sql . ") AS " . $QueryCb[$i]->name . ", ";
    }
    $strSQL .= "($StrSqlKembali) AS kembali, ";
    $strSQL .= "pcash.petty_cash FROM user_petty_cash AS pcash ";
    $strSQL .= "LEFT JOIN {users} AS user ON pcash.uid = user.uid ";
    $strSQL .= "WHERE 1=1 AND pcash.log_out IS NULL AND pcash.login > %d ";
    $strSQLFilteredTotal = "SELECT COUNT(pcash.id) FROM user_petty_cash AS pcash ";
    $strSQLFilteredTotal .= "WHERE 1=1 AND pcash.log_out IS NULL AND pcash.login > %d ";
    $strCriteria = "";
    if (!empty($searchQuery)) {
        $strCriteria .= "AND (user.name LIKE '%%%s%%')";
    }
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    } else {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }
    $strSQLFilteredTotal .= $strCriteria;
    if (!empty($searchQuery)) {
        $result = db_query($strSQL, $Today, $searchQuery, $firstRecord, $lastRecord);
        $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $Today, $searchQuery));
    } else {
        $result = db_query($strSQL, $Today, $firstRecord, $lastRecord);
        $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $Today));
    }
    $output = array();
    while ($data = db_fetch_object($result)) {
        $rowData = array();
        $rowData[] = '['.$data->id.'] '.$data->name;
        $rowData[] = date('Y-m-d H:i', $data->login).' ['.$data->login.']';
        $rowData[] = $data->ip;
        $rowData[] = number_format($data->petty_cash, $DDigit, $dSep, $tSep);
        $TotalCash = 0;
        for ($i = 0; $i < count($QueryCb); $i++) {
            $FieldName = $QueryCb[$i]->name;
            if ($QueryCb[$i]->name == 'tunai'){
                $TotalCash = $data->$FieldName;
            }
            $rowData[] = number_format($data->$FieldName, $DDigit, $dSep, $tSep);
        }
        $rowData[] = number_format($data->kembali, $DDigit, $dSep, $tSep);
        $TotalCash = $data->petty_cash + $TotalCash - $data->kembali;
        $rowData[] = number_format($TotalCash, $DDigit, $dSep, $tSep);
        $deletebutton = '<img title="Klik untuk menghapus petty cash" onclick="hapus_pettycash('.$data->id.');" src="'.$baseDirectory.'/misc/media/images/del.ico" width="22">';
        $rowData[] = $deletebutton;
        $PrintButton = '<img title="Klik untuk print cashier log" onclick="print_log('.$data->id.');" src="'.$baseDirectory.'/misc/media/images/print.png" width="22">';
        $rowData[] = $PrintButton;
        $output[] = $rowData;
    }
    $recordsTotal = db_result(db_query("SELECT COUNT(pcash.id) FROM user_petty_cash AS pcash WHERE pcash.log_out IS NULL AND pcash.login > %d", $Today));
    return array(
        "draw" => isset ($request['draw']) ?
            intval($request['draw']) :
            0,
        "recordsTotal" => intval($recordsTotal),
        "recordsFiltered" => intval($recordsFiltered),
        "data" => $output,
        "StrSql" => $strSQL,
        "Today" => $Today,
        "Now" => $Now,
    );
}

function ServerSidePettyCashData($request)
{
    global $baseDirectory;
    $DataPremis = get_data_premis_server_side();
    $DDigit = $DataPremis->decimal_digit;
    get_number_format_server_side($currSym, $tSep, $dSep);
    set_default_time_zone_server_side();
    $pageStart = $_GET['start'];
    $pageLength = $_GET['length'];
    $searchArray = $_REQUEST['search'];
    $searchQuery = $searchArray['value'];
    $DateFrom = mktime(0,0,0, date('n'), date('j') - 3, date('Y'));
    if (isset($request['date_from']) && !empty($request['date_from'])){
        $SeparateDate = explode(' ', $request['date_from']);
        $XDate = explode('-',$SeparateDate[0]);
        $XTime = explode('-',$SeparateDate[1]);
        $DateFrom = mktime($XTime[0],$XTime[1],$XTime[2], $XDate[1], $XDate[2], $XDate[0]);
    }
    $DateThru = mktime(23,55,0, date('n'), date('j'), date('Y'));
    if (isset($request['date_thru']) && !empty($request['date_thru'])){
        $SeparateDate = explode(' ', $request['date_thru']);
        $XDate = explode('-',$SeparateDate[0]);
        $XTime = explode('-',$SeparateDate[1]);
        $DateThru = mktime($XTime[0],$XTime[1],$XTime[2], $XDate[1], $XDate[2], $XDate[0]);
    }
    $arrayColumn = array(
        0 => 'user.name',
        1 => 'pcash.login',
        2 => 'pcash.log_out',
        3 => 'pcash.ip',
        4 => 'pcash.petty_cash',
    );
    $orderColumnArray = $_REQUEST['order'];
    $orderColumn = $arrayColumn[$orderColumnArray[0]['column']] . ' ' . $orderColumnArray[0]['dir'];
    if (is_null($pageStart)) {
        $pageStart = 0;
    }
    if (is_null($pageLength) || $pageLength != -1) {
        $pageLength = 100;
    }
    $firstRecord = $pageStart;
    $lastRecord = $pageStart + $pageLength;

    $StrSqlCaraBayar = 'SELECT carabayar FROM carabayar';
    $result = db_query($StrSqlCaraBayar);
    $QueryCb = array();
    while ($data = db_fetch_object($result)) {
        $DataQueryCb = new stdClass();
        $StrSqlCb = "SELECT SUM(total_bayar) FROM penjualan_bayar AS pb LEFT JOIN ";
        $StrSqlCb .= "penjualan AS penj ON pb.idpenjualan = penj.idpenjualan WHERE ";
        $StrSqlCb .= "penj.idpemakai = pcash.uid AND ";
        $StrSqlCb .= "(UNIX_TIMESTAMP(tglpenjualan) >= pcash.login AND UNIX_TIMESTAMP(tglpenjualan) <= pcash.log_out) AND ";
        $StrSqlCb .= "pb.carabayar = '" . $data->carabayar . "'";
        $DataQueryCb->sql = $StrSqlCb;
        $DataQueryCb->name = strtolower(str_replace(' ', '_', $data->carabayar));
        $QueryCb[] = $DataQueryCb;
    }
    $StrSqlKembali = "SELECT SUM(kembali) FROM penjualan AS penj WHERE penj.idpemakai = pcash.uid AND ";
    $StrSqlKembali .= "(UNIX_TIMESTAMP(tglpenjualan) >= pcash.login AND UNIX_TIMESTAMP(tglpenjualan) <= pcash.log_out) ";
    $StrSqlKembali .= "AND penj.kembali >= 0";
    $strSQL = "SELECT pcash.id, user.name, pcash.login, pcash.ip, ";
    for ($i = 0; $i < count($QueryCb); $i++) {
        $strSQL .= "(" . $QueryCb[$i]->sql . ") AS " . $QueryCb[$i]->name . ", ";
    }
    $strSQL .= "($StrSqlKembali) AS kembali, ";
    $strSQL .= "pcash.petty_cash, pcash.log_out FROM user_petty_cash AS pcash ";
    $strSQL .= "LEFT JOIN {users} AS user ON pcash.uid = user.uid ";
    $strSQL .= "WHERE 1=1 AND pcash.log_out IS NOT NULL AND pcash.log_out != 1 ";
    $strSQL .= "AND pcash.login BETWEEN '%s' AND '%s' ";
    $strSQLFilteredTotal = "SELECT COUNT(pcash.id) FROM user_petty_cash AS pcash ";
    $strSQLFilteredTotal .= "WHERE 1=1 AND pcash.log_out IS NOT NULL AND pcash.log_out != 1 ";
    $strSQLFilteredTotal .= "AND pcash.login BETWEEN '%s' AND '%s' ";
    $strCriteria = "";
    if (!empty($searchQuery)) {
        $strCriteria .= "AND (user.name LIKE '%%%s%%')";
    }
    if ($pageLength != -1) {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn LIMIT %d, %d";
    } else {
        $strSQL .= $strCriteria . " ORDER BY $orderColumn";
    }
    $strSQLFilteredTotal .= $strCriteria;
    if (!empty($searchQuery)) {
        $result = db_query($strSQL, $searchQuery, $DateFrom, $DateThru, $firstRecord, $lastRecord);
        $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $DateFrom, $DateThru, $searchQuery));
    } else {
        $result = db_query($strSQL, $DateFrom, $DateThru, $firstRecord, $lastRecord);
        $recordsFiltered = db_result(db_query($strSQLFilteredTotal, $DateFrom, $DateThru));
    }
    $output = array();
    while ($data = db_fetch_object($result)) {
        $rowData = array();
        $rowData[] = '['.$data->id.'] '.$data->name;
        $rowData[] = date('Y-m-d H:i', $data->login).' ['.$data->login.']';
        $rowData[] = date('Y-m-d H:i', $data->log_out).' ['.$data->log_out.']';
        $rowData[] = $data->ip;
        $rowData[] = number_format($data->petty_cash, $DDigit, $dSep, $tSep);
        $TotalCash = 0;
        for ($i = 0; $i < count($QueryCb); $i++) {
            $FieldName = $QueryCb[$i]->name;
            if ($QueryCb[$i]->name == 'tunai'){
                $TotalCash = $data->$FieldName;
            }
            $rowData[] = number_format($data->$FieldName, $DDigit, $dSep, $tSep);
        }
        $rowData[] = number_format($data->kembali, $DDigit, $dSep, $tSep);
        $TotalCash = $data->petty_cash + $TotalCash - $data->kembali;
        $rowData[] = number_format($TotalCash, $DDigit, $dSep, $tSep);
        $PrintButton = '<img title="Klik untuk print cashier log" onclick="print_log('.$data->id.');" src="'.$baseDirectory.'/misc/media/images/print.png" width="22">';
        $rowData[] = $PrintButton;
        $output[] = $rowData;
    }
    $recordsTotal = db_result(
        db_query(
            "SELECT COUNT(pcash.id) FROM user_petty_cash AS pcash WHERE pcash.log_out IS NOT NULL AND 
            pcash.log_out != 1 AND pcash.login BETWEEN '%s' AND '%s'", $DateFrom, $DateThru
        )
    );
    return array(
        "draw" => isset ($request['draw']) ?
            intval($request['draw']) :
            0,
        "recordsTotal" => intval($recordsTotal),
        "recordsFiltered" => intval($recordsFiltered),
        "data" => $output,
        "StrSql" => $strSQL,
        "Today" => $Today,
        "Now" => $Now,
    );
}

if ($_GET['request_data'] == 'pelanggan'){
	$returnArray = serverSidePelanggan($_GET);
}else if($_GET['request_data'] == 'produk'){
	$returnArray = serverSideProduk($_GET);
}else if($_GET['request_data'] == 'penjualan'){
	$returnArray = serverSidePenjualan($_GET);
}else if($_GET['request_data'] == 'penjualan2'){
	$returnArray = serverSidePenjualan2($_GET);
}else if($_GET['request_data'] == 'penjualan3'){
	$returnArray = serverSidePenjualan3($_GET);
}else if($_GET['request_data'] == 'laundry'){
    $returnArray = serverSideLaundry($_GET);
}else if($_GET['request_data'] == 'kategoripengeluaran'){
	$returnArray = kategoriPengeluaran($_GET);
}else if($_GET['request_data'] == 'pengeluaran'){
	$returnArray = pengeluaran($_GET);
}else if($_GET['request_data'] == 'pemasukan'){
	$returnArray = pemasukan($_GET);
}else if($_GET['request_data'] == 'customerorder'){
	$returnArray = serverSideCustomerOrder($_GET);
}else if($_GET['request_data'] == 'detailcustomerorder'){
	$returnArray = serverSideDetailCustomerOrder($_GET);
}else if($_GET['request_data'] == 'getproduct'){
	$returnArray = serverSideGetProduct($_GET);
}else if($_GET['request_data'] == 'getproductbarcode'){
	$returnArray = serverSideGetOneProduct($_GET);
}else if($_GET['request_data'] == 'detailpenjualan'){
    $returnArray = serverSideDetailPenjualan($_GET);
}else if($_GET['request_data'] == 'kategoriproduct'){
	$returnArray = serverSideGetCategoryProduct($_GET);
}else if($_GET['request_data'] == 'productbykategori'){
    $SearchText = null;
    if (isset($_GET['search_text']) && !empty($_GET['search_text'])){
        $SearchText = $_GET['search_text'];
    }
    $returnArray = serverSideGetProductByCategory($_GET, $SearchText);
}else if($_GET['request_data'] == 'allproduct'){
	$returnArray = serverSideGetAllProduct($_GET);
}else if ($_GET['request_data'] == 'getdatameja'){
	$returnArray = serverSideGetDataMeja($_GET);
}else if ($_GET['request_data'] == 'postorder'){
	header('Access-Control-Allow-Origin: *');
	$returnArray = serverSidePostOrder($_GET);
}else if($_GET['request_data'] == 'kategori'){
    $returnArray = serverSideArrayKategori($_GET);
}else if($_GET['request_data'] == 'subkategori'){
    $returnArray = serverSideArraySubKategori($_GET);
}else if($_GET['request_data'] == 'grupmenu'){
    $returnArray = serverSideArrayGrupMenu($_GET);
}else if ($_GET['request_data'] == 'checkconnection'){
	header('Access-Control-Allow-Origin: *');
	$returnArray = serverSideCheckLogin($_GET);
}else if($_GET['request_data'] == 'satuan'){
    $returnArray = serverSideArraySatuan($_GET);
}else if($_GET['request_data'] == 'datahutang'){
    $returnArray = serverSideArrayHutang($_GET);
}else if($_GET['request_data'] == 'supplier'){
    $returnArray = serverSideArraySupplier($_GET);
}else if($_GET['request_data'] == 'pembelian'){
    $returnArray = serverSidePembelian($_GET);
}else if($_GET['request_data'] == 'pembelian2'){
    $returnArray = serverSidePembelian2($_GET);
}else if($_GET['request_data'] == 'detailpembelian'){
    $returnArray = serverSideDetailPembelian($_GET);
}else if($_GET['request_data'] == 'getdatapremis'){
    $returnArray = serverSideDataPremis($_GET);
}else if ($_GET['request_data'] == 'penjualanandroid'){
    header('Access-Control-Allow-Origin: *');
    $returnArray = serverSidePenjualanAndroid($_GET);
}else if ($_GET['request_data'] == 'postsales'){
    header('Access-Control-Allow-Origin: *');
    $returnArray = serverSidePostSales($_GET);
}else if ($_GET['request_data'] == 'perubahanhargadroid'){
    header('Access-Control-Allow-Origin: *');
    if (isset($_SERVER['HTTP_ORIGIN'])) {
        header("Access-Control-Allow-Origin: {$_SERVER['HTTP_ORIGIN']}");
        header('Access-Control-Allow-Credentials: true');
        header('Access-Control-Max-Age: 86400');    // cache for 1 day
    }
    // Access-Control headers are received during OPTIONS requests
    if ($_SERVER['REQUEST_METHOD'] == 'OPTIONS') {
        if (isset($_SERVER['HTTP_ACCESS_CONTROL_REQUEST_METHOD']))
            header("Access-Control-Allow-Methods: GET, POST, OPTIONS");

        if (isset($_SERVER['HTTP_ACCESS_CONTROL_REQUEST_HEADERS']))
            header("Access-Control-Allow-Headers: {$_SERVER['HTTP_ACCESS_CONTROL_REQUEST_HEADERS']}");
        exit(0);
    }
    //http://stackoverflow.com/questions/15485354/angular-http-post-to-php-and-undefined
    $postdata = file_get_contents("php://input");
    if (isset($postdata)) {
        $returnArray = serverSidePerubahanHargaAndroid($postdata);
    }
}else if ($_GET['request_data'] == 'perubahanstokdroid'){
    header('Access-Control-Allow-Origin: *');
    if (isset($_SERVER['HTTP_ORIGIN'])) {
        header("Access-Control-Allow-Origin: {$_SERVER['HTTP_ORIGIN']}");
        header('Access-Control-Allow-Credentials: true');
        header('Access-Control-Max-Age: 86400');    // cache for 1 day
    }
    // Access-Control headers are received during OPTIONS requests
    if ($_SERVER['REQUEST_METHOD'] == 'OPTIONS') {
        if (isset($_SERVER['HTTP_ACCESS_CONTROL_REQUEST_METHOD']))
            header("Access-Control-Allow-Methods: GET, POST, OPTIONS");

        if (isset($_SERVER['HTTP_ACCESS_CONTROL_REQUEST_HEADERS']))
            header("Access-Control-Allow-Headers: {$_SERVER['HTTP_ACCESS_CONTROL_REQUEST_HEADERS']}");
        exit(0);
    }
    //http://stackoverflow.com/questions/15485354/angular-http-post-to-php-and-undefined
    $postdata = file_get_contents("php://input");
    if (isset($postdata)) {
        $returnArray = serverSidePerubahanStockAndroid($postdata);
    }
}else if($_GET['request_data'] == 'monitorcash'){
    $returnArray = ServerSidePettyCash($_GET);
}else if($_GET['request_data'] == 'cashierlog'){
    $returnArray = ServerSidePettyCashData($_GET);
}
header('Access-Control-Allow-Origin: *');
echo json_encode($returnArray);
?>