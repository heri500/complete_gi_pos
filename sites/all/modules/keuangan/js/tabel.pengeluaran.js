var oTableKategori;
var oTablePengeluaran;
var oTablePemasukan;
function addCommas(nStr){
	nStr += "";
	x = nStr.split(",");
	x1 = x[0];
	x2 = x.length > 1 ? "," + x[1] : "";
	var rgx = /(\d+)(\d{3})/;
	while (rgx.test(x1)) {
		x1 = x1.replace(rgx, "$1" + "." + "$2");
	}
	return x1 + x2;
}
function tampiltabelkategori(){
	oTableKategori = $('#'+ Drupal.settings.idtabelkategori).dataTable( {
		'retrieve': true,
		'bJQueryUI': true,
		'bAutoWidth': false,
		'sPaginationType': 'full_numbers',
		'bInfo': true,
		'processing': true,
		'serverSide': true,
		"aoColumnDefs": [
			{ "bSortable": false, "aTargets": [ 0 ] },
			{ "bVisible": false, "aTargets": [ 4,5 ] }
		],
		'aaSorting': [[1, 'asc']],
		'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=kategoripengeluaran'
	});
}
function tampiltabelpengeluaran(){
	oTablePengeluaran = $('#'+ Drupal.settings.idtabelpengeluaran).dataTable( {
		'bJQueryUI': true,
		'bAutoWidth': false,
		'sPaginationType': 'full_numbers',
		'bInfo': true,
		'aaSorting': [[3, 'desc']],
		"aoColumnDefs": [
			{ "bSortable": false, "aTargets": [ 0,1,2 ] },
			{ "bVisible": false, "aTargets": [ 7 ] }
		],
		'processing': true,
		'serverSide': true,
		'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=pengeluaran',
		'createdRow': function ( row, data, index ) {
			row.id = data[(data.length - 1)];
			$('td', row).eq(2).addClass('center');
			$('td', row).eq(3).addClass('center');
			$('td', row).eq(6).addClass('angka');
		}
	});
}
function tampiltabelpemasukan(){
	oTablePemasukan = $('#'+ Drupal.settings.idtabelpemasukan).dataTable( {
		'bJQueryUI': true,
		'bAutoWidth': false,
		'sPaginationType': 'full_numbers',
		'bInfo': true,
		'aaSorting': [[3, 'desc']],
		"aoColumnDefs": [
			{ "bSortable": false, "aTargets": [ 0,1,2 ] },
			{ "bVisible": false, "aTargets": [ 7 ] }
		],
		'processing': true,
		'serverSide': true,
		'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=pemasukan',
		'createdRow': function ( row, data, index ) {
			row.id = data[(data.length - 1)];
			$('td', row).eq(2).addClass('center');
			$('td', row).eq(3).addClass('center');
			$('td', row).eq(6).addClass('angka');
		}
	});
}
function edit_kategori(nid, nTr){
	var aPos = oTableKategori.fnGetPosition( nTr );
	var aData = oTableKategori.fnGetData( aPos );
	console.log(aData);
	$('#edit-idkategori-2').val(aData[5]);
	$('#edit-kategori').val(aData[1]);
	$('#edit-keterangan').val(aData[3]);
	$('#edit-jeniskategori').val(aData[4]);
	$('#edit-kategori').select();
}
function edit_pemasukan(nid, nTr){
	var aPos = oTablePemasukan.fnGetPosition( nTr );
	var aData = oTablePemasukan.fnGetData( aPos );
	var tglsplit = aData[3].split('-');
	var tgledit = tglsplit[2] +'-'+ tglsplit[1] +'-'+ tglsplit[0];
	$('#edit-idkategori').val(aData[7]);
	$('#edit-tanggal').val(tgledit);
	var tglkirim = aData[3];
	$('#edit-tglkirim').val(tglkirim);
	$('#edit-keteranganpemasukan').val(aData[5]);
	console.log(aData[6]);
	var splitjumlahkirim = aData[6].split('.');
	var jumlahkirim = '';
	for (var i = 0;i < splitjumlahkirim.length;i++){
		jumlahkirim = jumlahkirim + splitjumlahkirim[i];
	}
	$('#edit-jumlah').val(jumlahkirim);
	$('#edit-idpemasukan').val(nid);
	$('#edit-jumlah').select();
}
function edit_pengeluaran(nid, nTr){
	var aPos = oTablePengeluaran.fnGetPosition( nTr );
	var aData = oTablePengeluaran.fnGetData( aPos );
	$('#edit-idkategori-1').val(aData[7]);
	var tglsplit = aData[3].split('-');
	var tgledit = tglsplit[2] +'-'+ tglsplit[1] +'-'+ tglsplit[0];
	$('#edit-tanggal-1').val(tgledit);
	var tglkirim = aData[3];
	$('#edit-tglkirim-1').val(tglkirim);
	$('#edit-keteranganpengeluaran').val(aData[5]);
	console.log(aData[6]);
	var splitjumlahkirim = aData[6].split('.');
	var jumlahkirim = '';
	for (var i = 0;i < splitjumlahkirim.length;i++){
		jumlahkirim = jumlahkirim + splitjumlahkirim[i];
	}
	$('#edit-jumlah-1').val(jumlahkirim);
	$('#edit-idpengeluaran').val(nid);
	$('#edit-jumlah-1').select();
}
function hapus_pengeluaran(nid){
	var konfirmasi = confirm('Yakin ingin menghapus pengeluaran ini...?!!');
	if (konfirmasi){
		var alamat = Drupal.settings.basePath + 'keuangan/deletepengeluaran/'+ nid;
		$.ajax({
			type: 'POST',
			url: alamat,
			cache: false,
			success: function(data){
				oTablePengeluaran.fnDraw();
			}
		});
	}
}
function hapus_pemasukan(nid){
	var konfirmasi = confirm('Yakin ingin menghapus pemasukan ini...?!!');
	if (konfirmasi){
		var alamat = Drupal.settings.basePath + 'keuangan/deletepemasukan/'+ nid;
		$.ajax({
			type: 'POST',
			url: alamat,
			cache: false,
			success: function(data){
				oTablePemasukan.fnDraw();
			}
		});
	}
}
jQuery(function ($) {
	$('#tabs').tabs();
	tampiltabelkategori();
	tampiltabelpengeluaran();
	tampiltabelpemasukan();
	$('#edit-tanggal-1').datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: 'dd-mm-yy',
		altField: '#edit-tglkirim-1',
		altFormat: 'yy-mm-dd'
	});
	$('#edit-tanggal').datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: 'dd-mm-yy',
		altField: '#edit-tglkirim',
		altFormat: 'yy-mm-dd'
	});

	$('#edit-submitpemasukan').on('click', function(e){
		if ($('#edit-idkategori').val() != '' && $('#edit-jumlah').val() != 0 && $('#edit-tglkirim').val() != ''){
			e.preventDefault();
			$('#form-input-pemasukan').block({ message: '<p style="color: 808080;padding: .2em;font-size: 18px;">Simpan pemasukan...<img src="/misc/media/images/loading.gif"></p>',css: { border: "3px solid #a00" } });
			var request = new Object();
			request.idkategori = $('#edit-idkategori').val();
			request.jumlah = $('#edit-jumlah').val();
			request.tanggal = $('#edit-tglkirim').val();
			request.keterangan = $('#edit-keteranganpemasukan').val();
			alamat = Drupal.settings.basePath + 'keuangan/insertPemasukan/1';
			if (parseInt($('#edit-idpemasukan').val()) > 0){
				request.id = $('#edit-idpemasukan').val();
				alamat = Drupal.settings.basePath + 'keuangan/updatePemasukan/1';
			}
			$.ajax({
				type: 'POST',
				url: alamat,
				data: request,
				cache: false,
				success: function(data){
					$('#edit-keteranganpemasukan').val('');
					$('#edit-jumlah').val('');
					$('#edit-idpemasukan').val('');
					oTablePemasukan.fnDraw();
					$('#form-input-pemasukan').unblock();
					$('#edit-jumlah').focus();
				}
			});
		}else{
			alert('Mohon pilih kategori pemasukan/isi jumlah pemasukan...!!?');
		}
	});

	$('#edit-submitpengeluaran').on('click', function(e){
		e.preventDefault();
		if ($('#edit-idkategori-1').val() != '' && $('#edit-jumlah-1').val() != 0 && $('#edit-tglkirim-1').val() != ''){
			$('#form-input-pengeluaran').block({ message: '<p style="color: 808080;padding: .2em;font-size: 18px;">Simpan pengeluaran...<img src="/misc/media/images/loading.gif"></p>',css: { border: "3px solid #a00" } });
			var request = new Object();
			request.idkategori = $('#edit-idkategori-1').val();
			request.jumlah = $('#edit-jumlah-1').val();
			request.tanggal = $('#edit-tglkirim-1').val();
			request.keterangan = $('#edit-keteranganpengeluaran').val();
			alamat = Drupal.settings.basePath + 'keuangan/insertPengeluaran/1';
			if (parseInt($('#edit-idpengeluaran').val()) > 0){
				request.id = $('#edit-idpengeluaran').val();
				alamat = Drupal.settings.basePath + 'keuangan/updatePengeluaran/1';
			}
			$.ajax({
				type: 'POST',
				url: alamat,
				data: request,
				cache: false,
				success: function(data){
					$('#edit-keteranganpengeluaran').val('');
					$('#edit-jumlah-1').val('');
					$('#edit-idpengeluaran').val('');
					oTablePengeluaran.fnDraw();
					$('#form-input-pengeluaran').unblock();
					$('#edit-jumlah-1').focus();
				}
			});
		}else{
			alert('Mohon pilih kategori pengeluaran/isi jumlah pengeluaran...!!?');
		}
	});

	$('#edit-submit').on('click', function(e){
		e.preventDefault();
		$('#form-input-kategori').block({ message: '<p style="color: 808080;padding: .2em;font-size: 18px;">Simpan kategori pengeluaran...<img src="/misc/media/images/loading.gif"></p>',css: { border: "3px solid #a00" } });
		var request = new Object();
		request.kategori = $('#edit-kategori').val();
		request.jeniskategori = $('#edit-jeniskategori').val();
		request.keterangan = $('#edit-keterangan').val();
		alamat = Drupal.settings.basePath + 'keuangan/insertKategoriPengeluaran/1';
		if (parseInt($('#edit-idkategori-2').val()) > 0){
			request.id = $('#edit-idkategori-2').val();
			alamat = Drupal.settings.basePath + 'keuangan/updateKategoriPengeluaran/1';
		}
		$.ajax({
			type: 'POST',
			url: alamat,
			data: request,
			cache: false,
			success: function(data){
				$('#edit-jeniskategori').val('');
				$('#edit-kategori').val('');
				$('#edit-keterangan').val('');
				$('#edit-idkategori').val('');
				oTableKategori.fnDraw();
				$('#form-input-kategori').unblock();
				$('#edit-kategori').focus();
			}
		});
	});
	$('#edit-batalpemasukan').on('click', function(e){
		e.preventDefault();
		$('#edit-idkategori').val('');
		$('#edit-keteranganpemasukan').val('');
		$('#edit-jumlah').val('');
		$('#edit-tanggal').val('');
		$('#edit-idpemasukan').val('');
		$('#edit-jumlah').focus();
	});
	$('#edit-batal').on('click', function(e){
		e.preventDefault();
		$('#edit-kategori-2').val('');
		$('#edit-jeniskategori').val('');
		$('#edit-keterangan').val('');
		$('#edit-idkategori').val('');
		$('#edit-kategori').focus();
	});
	$('#edit-batalpengeluaran').on('click', function(e){
		e.preventDefault();
		$('#edit-idkategori-1').val('');
		$('#edit-keteranganpengeluaran').val('');
		$('#edit-jumlah-1').val('');
		$('#edit-tanggal-1').val('');
		$('#edit-idpengeluaran').val('');
		$('#edit-jumlah-1').focus();
	});
	$("button").button();
	$("#tgl1").datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: "dd-mm-yy",
		altField: '#edit-tgl1',
		altFormat: 'yy-mm-dd'
	});
	$("#tgl2").datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: "dd-mm-yy",
		altField: '#edit-tgl2',
		altFormat: 'yy-mm-dd'
	});
	$('#view-report').on('click', function(e){
		e.preventDefault();
		var request = new Object();
		request.tglawal = $('#edit-tgl1').val();
		request.tglakhir = $('#edit-tgl2').val();
		alamat = Drupal.settings.basePath + 'keuangan/viewkeuanganajax';
		$.ajax({
			type: 'POST',
			url: alamat,
			data: request,
			cache: false,
			success: function (data) {
				$('#tempat-report').html(data.trim());
				$('#laporan-pemasukan').dataTable({
					'bJQueryUI': true,
					'bAutoWidth': false,
					'paging':   false,
					'ordering': false,
					'filter': false,
					'bInfo': false,
					'dom': '<"H"<"toolbar1">>frtip<"F">'
				});
				$("div.toolbar1").html('<b>PEMASUKAN</b>');
				$('#laporan-pengeluaran').dataTable({
					'bJQueryUI': true,
					'bAutoWidth': false,
					'paging':   false,
					'ordering': false,
					'filter': false,
					'bInfo': false,
					'dom': '<"H"<"toolbar2">>frtip<"F">'
				});
				$("div.toolbar2").html('<b>PENGELUARAN</b>');
			}
		});
	});
});