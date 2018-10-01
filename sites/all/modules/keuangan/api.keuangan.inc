<?php

function getKategoriPengeluaran($restapi = null){
	$object_rs = db_query("SELECT `id`, `kategori`, `jeniskategori`, `keterangan`, `created`, `changed`, `uid`
	FROM `cms_kategoripengeluaran` WHERE jeniskategori = 0");
	$object_array = fetchObjectToArray($object_rs);
	if (is_null($restapi)){
		return $object_array;
	}else{
		print json_encode($object_array);
		exit();
	}
}
function getKategoriPemasukan($restapi = null){
	$object_rs = db_query("SELECT `id`, `kategori`, `jeniskategori`, `keterangan`, `created`, `changed`, `uid`
	FROM `cms_kategoripengeluaran` WHERE jeniskategori = 1");
	$object_array = fetchObjectToArray($object_rs);
	if (is_null($restapi)){
		return $object_array;
	}else{
		print json_encode($object_array);
		exit();
	}
}
function insertKategoriPengeluaran($restapi = null,$variables = null){
	global $user;
	if (!is_null($restapi)){
		$variables['kategori'] = $_POST['kategori'];
		$variables['keterangan'] = $_POST['keterangan'];
		$variables['jeniskategori'] = $_POST['jeniskategori'];
	}
	$variables['uid'] = $user->uid;
	$variables['created'] = time();
	$variables['changed'] = $variables['created'];
	if ($variables['kategori'] != ''){
		db_query("INSERT INTO `cms_kategoripengeluaran` (kategori, jeniskategori, keterangan, created, changed, uid) VALUES ('%s',%d,'%s',%d,%d,%d)",
				 $variables['kategori'],$variables['jeniskategori'],$variables['keterangan'],$variables['created'],$variables['changed'], $variables['uid']);
		$variables['id'] = db_last_insert_id('cms_kategoripengeluaran', 'id');
	}
	if (is_null($restapi)){
		return $variables;
	}else{
		print json_encode($variables);
		exit();
	}
}
function updateKategoriPengeluaran($restapi = null,$variables = null){
	global $user;
	if (!is_null($restapi)){
		if (isset($_POST['kategori'])){
			$variables['kategori'] = $_POST['kategori'];
		}
		if (isset($_POST['keterangan'])){
			$variables['keterangan'] = $_POST['keterangan'];
		}
		if (isset($_POST['id'])){
			$variables['id'] = $_POST['id'];
		}
		if (isset($_POST['jeniskategori'])){
			$variables['jeniskategori'] = $_POST['jeniskategori'];
		}
	}
	$variables['uid'] = $user->uid;
	$variables['changed'] = time();
	if (isset($variables['id'])){
		if (isset($variables['kategori']) && $variables['kategori'] != ''){
			db_query("UPDATE `cms_kategoripengeluaran` SET kategori='%s',changed=%d,uid=%d WHERE id=%d",
					 $variables['kategori'],$variables['changed'],$variables['uid'],$variables['id']);
		}
		if (isset($variables['keterangan'])){
			db_query("UPDATE `cms_kategoripengeluaran` SET keterangan='%s',changed=%d,uid=%d WHERE id=%d",
					 $variables['keterangan'],$variables['changed'],$variables['uid'],$variables['id']);
		}
		if (isset($variables['jeniskategori'])){
			db_query("UPDATE `cms_kategoripengeluaran` SET jeniskategori='%d',changed=%d,uid=%d WHERE id=%d",
					 $variables['jeniskategori'],$variables['changed'],$variables['uid'],$variables['id']);
		}
	}
	if (is_null($restapi)){
		return $variables;
	}else{
		print json_encode($variables);
		exit();
	}
}
function getPengeluaranByPeriode($restapi = null,$variables){
	if (is_null($restapi)){
		$variables['awal'] = $_POST['tglawal'];
		$variables['akhir'] = $_POST['tglakhir'];
	}
	if (isset($variables['awal']) && isset($variables['akhir'])){
		$variables['awal'] = $variables['awal'].' 00:00';
		$variables['akhir'] = $variables['akhir'].' 23:59';
		$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
		FROM `cms_pengeluaran` WHERE `tglpengeluaran` >= %d AND `tglpengeluaran` <= %d", $variables['awal'],$variables['akhir']);
	}else{
		$variables['awal'] = strtotime(date('Y-m').'-01 00:00');
		$tglakhir = geLastDay(date('n'), date('Y'));
		$variables['akhir'] = strtotime(date('Y-m').'-'.$tglakhir.' 23:59');
		$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
		FROM `cms_pengeluaran` WHERE `tglpengeluaran` >= %d AND `tglpengeluaran` <= %d", $variables['awal'],$variables['akhir']);
	}
	$object_array = fetchObjectToArray($object_rs);
	if (is_null($restapi)){
		return $object_array;
	}else{
		print json_encode($object_array);
		exit();
	}
}
function getPengeluaranByKategori($restapi = null,$variables){
	if (is_null($restapi)){
		$variables['idkategori'] = $_POST['idkategori'];
	}
	if (isset($variables['idkategori'])){
		$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
		FROM `cms_pengeluaran` WHERE `kategori` = %d", $variables['idkategori']);
	}else{
		$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
		FROM `cms_pengeluaran`");
	}
	$object_array = fetchObjectToArray($object_rs);
	if (is_null($restapi)){
		return $object_array;
	}else{
		print json_encode($object_array);
		exit();
	}
}
function getPengeluaranByPeriodeKategori($restapi = null,$variables){
	if (is_null($restapi)){
		$variables['awal'] = $_POST['tglawal'];
		$variables['akhir'] = $_POST['tglakhir'];
		$variables['idkategori'] = $_POST['idkategori'];
	}
	if (isset($variables['awal']) && isset($variables['akhir'])){
		$variables['awal'] = $variables['awal'].' 00:00';
		$variables['akhir'] = $variables['akhir'].' 23:59';
		if (isset($variables['idkategori'])){
			$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
			FROM `cms_pengeluaran` WHERE (`tglpengeluaran` >= %d AND `tglpengeluaran` <= %d) AND `kategori`=%d",
								  strtotime($variables['awal']),strtotime($variables['akhir']),$variables['idkategori']);
		}else{
			$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
			FROM `cms_pengeluaran` WHERE (`tglpengeluaran` >= %d AND `tglpengeluaran` <= %d) ",
								  strtotime($variables['awal']),strtotime($variables['akhir']));
		}
	}else{
		$variables['awal'] = strtotime(date('Y-m').'-01 00:00');
		$tglakhir = geLastDay(date('n'), date('Y'));
		$variables['akhir'] = strtotime(date('Y-m').'-'.$tglakhir.' 23:59');
		if (isset($variables['idkategori'])){
			$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
			FROM `cms_pengeluaran` WHERE (`tglpengeluaran` >= %d AND `tglpengeluaran` <= %d)  AND `kategori`=%d ORDER BY kategori",
								  $variables['awal'],$variables['akhir'],$variables['idkategori']);
		}else{
			$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
			FROM `cms_pengeluaran` WHERE (`tglpengeluaran` >= %d AND `tglpengeluaran` <= %d) ORDER BY kategori",
								  $variables['awal'],$variables['akhir']);
		}
	}
	$object_array = fetchObjectToArray($object_rs);
	if (is_null($restapi)){
		return $object_array;
	}else{
		print json_encode($object_array);
		exit();
	}
}
function getPemasukanByPeriode($restapi = null,$variables){
	if (is_null($restapi)){
		$variables['awal'] = $_POST['tglawal'];
		$variables['akhir'] = $_POST['tglakhir'];
	}
	if (isset($variables['awal']) && isset($variables['akhir'])){
		$variables['awal'] = $variables['awal'].' 00:00';
		$variables['akhir'] = $variables['akhir'].' 23:59';
		$object_rs = db_query("SELECT `id`, `keterangan`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
		FROM `cms_pemasukan` WHERE (`tglpengeluaran` >= %d AND `tglpengeluaran` <= %d) AND `kategori`=%d",
							  strtotime($variables['awal']),strtotime($variables['akhir']),$variables['idkategori']);
	}else{
		$variables['awal'] = strtotime(date('Y-m').'-01 00:00');
		$tglakhir = geLastDay(date('n'), date('Y'));
		$variables['akhir'] = strtotime(date('Y-m').'-'.$tglakhir.' 23:59');
		$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpengeluaran`, `created`, `changed`, `uid`
		FROM `cms_pemasukan` WHERE (`tglpengeluaran` >= %d AND `tglpengeluaran` <= %d)",
							  $variables['awal'],$variables['akhir']);
	}
	$object_array = fetchObjectToArray($object_rs);
	if (is_null($restapi)){
		return $object_array;
	}else{
		print json_encode($object_array);
		exit();
	}
}
function getPemasukanByPeriodeKategori($restapi = null,$variables){
	if (!is_null($restapi)){
		$variables['awal'] = $_POST['tglawal'];
		$variables['akhir'] = $_POST['tglakhir'];
		$variables['idkategori'] = $_POST['idkategori'];
	}
	if (isset($variables['awal']) && isset($variables['akhir'])){
		$variables['awal'] = $variables['awal'].' 00:00';
		$variables['akhir'] = $variables['akhir'].' 23:59';
		if (isset($variables['idkategori'])) {
			$object_rs = db_query(
				"SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpemasukan`, `created`, `changed`, `uid`
			FROM `cms_pemasukan` WHERE (`tglpemasukan` >= %d AND `tglpemasukan` <= %d) AND `kategori`=%d ORDER BY kategori",
				strtotime($variables['awal']),
				strtotime($variables['akhir']),
				$variables['idkategori']
			);
		}else{
			$object_rs = db_query(
				"SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpemasukan`, `created`, `changed`, `uid`
				FROM `cms_pemasukan` WHERE (`tglpemasukan` >= %d AND `tglpemasukan` <= %d) ORDER BY kategori",
				strtotime($variables['awal']),
				strtotime($variables['akhir'])
			);
		}
	}else{
		$variables['awal'] = strtotime(date('Y-m').'-01 00:00');
		$tglakhir = geLastDay(date('n'), date('Y'));
		$variables['akhir'] = strtotime(date('Y-m').'-'.$tglakhir.' 23:59');
		if (isset($variables['idkategori'])){
			$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpemasukan`, `created`, `changed`, `uid`
			FROM `cms_pemasukan` WHERE (`tglpemasukan` >= %d AND `tglpemasukan` <= %d)  AND `kategori`=%d ORDER BY kategori",
								  $variables['awal'],$variables['akhir'],$variables['idkategori']);
		}else{
			$object_rs = db_query("SELECT `id`, `keterangan`, `kategori`, `nilai`, `tglpemasukan`, `created`, `changed`, `uid`
			FROM `cms_pemasukan` WHERE (`tglpemasukan` >= %d AND `tglpemasukan` <= %d) ORDER BY kategori",
								  $variables['awal'],$variables['akhir']);
		}
	}
	$object_array = fetchObjectToArray($object_rs);
	if (is_null($restapi)){
		return $object_array;
	}else{
		print json_encode($object_array);
		exit();
	}
}
function fetchObjectToArray($object_rs){
	$object_array = array();
	while ($object_dt = db_fetch_object($object_rs)){
		$object_array[] = $object_dt;
	}
	return $object_array;
}
function geLastDay($mon, $year) {
	$daysinmonth = array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
	$days = $daysinmonth[$mon-1];
	if ($mon == 2 && ($year % 4) == 0 && (($year % 100) != 0 ||
			($year % 400) == 0)) $days++;
	//if ($mon == 2 && ($year % 4) == 0 && ($year % 1000) != 0) $days++;
	$lastday = $days;
	return $lastday;
}
function arrayHari(){
	$hari_array = array('Minggu','Senin','Selasa','Rabu','Kamis','Jumat','Sabtu');
	return $hari_array;
}
function arrayBulanSingkat(){
	$namabulan[1] = 'Jan';
	$namabulan[2] = 'Feb';
	$namabulan[3] = 'Mar';
	$namabulan[4] = 'Apr';
	$namabulan[5] = 'Mei';
	$namabulan[6] = 'Jun';
	$namabulan[7] = 'Jul';
	$namabulan[8] = 'Agt';
	$namabulan[9] = 'Sep';
	$namabulan[10] = 'Okt';
	$namabulan[11] = 'Nov';
	$namabulan[12] = 'Des';
	return $namabulan;
}
function arrayBulan(){
	$namabulan[1] = 'Januari';
	$namabulan[2] = 'Februari';
	$namabulan[3] = 'Maret';
	$namabulan[4] = 'April';
	$namabulan[5] = 'Mei';
	$namabulan[6] = 'Juni';
	$namabulan[7] = 'Juli';
	$namabulan[8] = 'Agustus';
	$namabulan[9] = 'September';
	$namabulan[10] = 'Oktober';
	$namabulan[11] = 'November';
	$namabulan[12] = 'Desember';
	return $namabulan;
}
function insertPengeluaran($restapi = null,$variables = null){
	global $user;
	if (!is_null($restapi)){
		$variables['idkategori'] = $_POST['idkategori'];
		$variables['jumlah'] = $_POST['jumlah'];
		$variables['keterangan'] = $_POST['keterangan'];
		$splitTanggal = explode('-',$_POST['tanggal']);
		$jamPengeluaran = date('H');
		$menitPengeluaran = date('m');
		$variables['tanggal'] = mktime($jamPengeluaran,$menitPengeluaran,0,$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
	}
	$variables['uid'] = $user->uid;
	$variables['created'] = time();
	$variables['changed'] = $variables['created'];
	if ($variables['idkategori'] != '' && $variables['jumlah'] > 0 && $variables['tanggal'] != ''){
		db_query("INSERT INTO cms_pengeluaran (kategori, nilai, tglpengeluaran, keterangan, created, changed, uid) VALUES (%d,'%f',%d,'%s',%d,%d,%d)",
				 $variables['idkategori'],$variables['jumlah'],$variables['tanggal'],
				 $variables['keterangan'],$variables['created'],$variables['changed'], $variables['uid']);
		$variables['id'] = db_last_insert_id('cms_pengeluaran', 'id');
	}
	if (is_null($restapi)){
		return $variables;
	}else{
		print json_encode($variables);
		exit();
	}
}
function updatePengeluaran($restapi = null,$variables = null){
	global $user;
	if (!is_null($restapi)){
		if (isset($_POST['idkategori'])){
			$variables['idkategori'] = $_POST['idkategori'];
		}
		if (isset($_POST['jumlah'])){
			$variables['jumlah'] = $_POST['jumlah'];
		}
		if (isset($_POST['tanggal'])){
			$splitTanggal = explode('-',$_POST['tanggal']);
			$jamPengeluaran = date('H');
			$menitPengeluaran = date('m');
			$variables['tanggal'] = mktime($jamPengeluaran,$menitPengeluaran,0,$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
		}
		if (isset($_POST['keterangan'])){
			$variables['keterangan'] = $_POST['keterangan'];
		}
		if (isset($_POST['id'])){
			$variables['id'] = $_POST['id'];
		}
	}
	$variables['uid'] = $user->uid;
	$variables['changed'] = time();
	if (isset($variables['id'])){
		if (isset($variables['idkategori']) && $variables['idkategori'] != ''){
			db_query("UPDATE `cms_pengeluaran` SET kategori='%d',changed=%d,uid=%d WHERE id=%d",
					 $variables['idkategori'],$variables['changed'],$variables['uid'],$variables['id']);
		}
		if (isset($variables['keterangan'])){
			db_query("UPDATE `cms_pengeluaran` SET keterangan='%s',changed=%d,uid=%d WHERE id=%d",
					 $variables['keterangan'],$variables['changed'],$variables['uid'],$variables['id']);
		}
		if (isset($variables['tanggal'])){
			db_query("UPDATE `cms_pengeluaran` SET tglpengeluaran='%s',changed=%d,uid=%d WHERE id=%d",
					 $variables['tanggal'],$variables['changed'],$variables['uid'],$variables['id']);
		}
		if (isset($variables['jumlah'])){
			db_query("UPDATE `cms_pengeluaran` SET nilai='%f',changed=%d,uid=%d WHERE id=%d",
					 $variables['jumlah'],$variables['changed'],$variables['uid'],$variables['id']);
		}
	}
	if (is_null($restapi)){
		return $variables;
	}else{
		print json_encode($variables);
		exit();
	}
}
function deletePengeluaran($nidpengeluaran = null){
	if (!empty($nidpengeluaran)){
		db_query("DELETE FROM cms_pengeluaran WHERE id='%d'",$nidpengeluaran);
		print 'delete success '.$nidpengeluaran;
	}
	exit();
}
function insertPemasukan($restapi = null,$variables = null){
	global $user;
	if (!is_null($restapi)){
		$variables['idkategori'] = $_POST['idkategori'];
		$variables['jumlah'] = $_POST['jumlah'];
		$variables['keterangan'] = $_POST['keterangan'];
		$splitTanggal = explode('-',$_POST['tanggal']);
		$jamPengeluaran = date('H');
		$menitPengeluaran = date('m');
		$variables['tanggal'] = mktime($jamPengeluaran,$menitPengeluaran,0,$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
	}
	$variables['uid'] = $user->uid;
	$variables['created'] = time();
	$variables['changed'] = $variables['created'];
	if ($variables['idkategori'] != '' && $variables['jumlah'] > 0 && $variables['tanggal'] != ''){
		db_query("INSERT INTO cms_pemasukan (kategori, nilai, tglpemasukan, keterangan, created, changed, uid) VALUES (%d,'%f',%d,'%s',%d,%d,%d)",
				 $variables['idkategori'],$variables['jumlah'],$variables['tanggal'],
				 $variables['keterangan'],$variables['created'],$variables['changed'], $variables['uid']);
		$variables['id'] = db_last_insert_id('cms_pemasukan', 'id');
	}
	if (is_null($restapi)){
		return $variables;
	}else{
		print json_encode($variables);
		exit();
	}
}
function updatePemasukan($restapi = null,$variables = null){
	global $user;
	if (!is_null($restapi)){
		if (isset($_POST['idkategori'])){
			$variables['idkategori'] = $_POST['idkategori'];
		}
		if (isset($_POST['jumlah'])){
			$variables['jumlah'] = $_POST['jumlah'];
		}
		if (isset($_POST['tanggal'])){
			$splitTanggal = explode('-',$_POST['tanggal']);
			$jamPengeluaran = date('H');
			$menitPengeluaran = date('m');
			$variables['tanggal'] = mktime($jamPengeluaran,$menitPengeluaran,0,$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
		}
		if (isset($_POST['keterangan'])){
			$variables['keterangan'] = $_POST['keterangan'];
		}
		if (isset($_POST['id'])){
			$variables['id'] = $_POST['id'];
		}
	}
	$variables['uid'] = $user->uid;
	$variables['changed'] = time();
	if (isset($variables['id'])){
		if (isset($variables['idkategori']) && $variables['idkategori'] != ''){
			db_query("UPDATE `cms_pemasukan` SET kategori='%d',changed=%d,uid=%d WHERE id=%d",
					 $variables['idkategori'],$variables['changed'],$variables['uid'],$variables['id']);
		}
		if (isset($variables['keterangan'])){
			db_query("UPDATE `cms_pemasukan` SET keterangan='%s',changed=%d,uid=%d WHERE id=%d",
					 $variables['keterangan'],$variables['changed'],$variables['uid'],$variables['id']);
		}
		if (isset($variables['tanggal'])){
			db_query("UPDATE `cms_pemasukan` SET tglpemasukan='%s',changed=%d,uid=%d WHERE id=%d",
					 $variables['tanggal'],$variables['changed'],$variables['uid'],$variables['id']);
		}
		if (isset($variables['jumlah'])){
			db_query("UPDATE `cms_pemasukan` SET nilai='%f',changed=%d,uid=%d WHERE id=%d",
					 $variables['jumlah'],$variables['changed'],$variables['uid'],$variables['id']);
		}
	}
	if (is_null($restapi)){
		return $variables;
	}else{
		print json_encode($variables);
		exit();
	}
}
function deletePemasukan($nidpemasukan = null){
	if (!empty($nidpemasukan)){
		db_query("DELETE FROM cms_pemasukan WHERE id='%d'",$nidpemasukan);
		print 'delete success '.$nidpemasukan;
	}
	exit();
}