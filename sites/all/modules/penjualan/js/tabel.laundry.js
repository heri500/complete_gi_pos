var oTable;
var pathutama = '';
var urutan = '';
var alamatupdatedetaillaundry = '';
var alamatupdaterak = '';
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
function tampiltabellaundry(){
	oTable = $('#tabel_laundry').dataTable( {
		'bJQueryUI': true,
		'bAutoWidth': false,
		'sPaginationType': 'full_numbers',
		'bInfo': true,
		'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
		'iDisplayLength': 100,
		'ordering': false,
		/*'aaSorting': [[6, 'desc']],*/
		'processing': true,
		'serverSide': true,
		'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=laundry&tglawal='+ Drupal.settings.tglawal +'&tglakhir='+ Drupal.settings.tglakhir,
		buttons: [
			{
				extend: 'colvis',
				columns: [4,5,6,7,8,9,10,11,12,13,14,15]
			}, 'copy', 'excel', 'print'
		],
		'sDom': '<"button-div"B><"H"lfr>t<"F"ip>',
		'createdRow': function ( row, data, index ) {
			row.id = data[(data.length - 1)];
			$('td', row).eq(1).addClass('center');
			$('td', row).eq(2).addClass('center');
			$('td', row).eq(3).addClass('center');
			$('td', row).eq(4).addClass('center');
			$('td', row).eq(5).addClass('center');
			$('td', row).eq(6).addClass('center');
			$('td', row).eq(7).addClass('center');
			$('td', row).eq(8).addClass('angka');
			$('td', row).eq(9).addClass('center');
			$('td', row).eq(10).addClass('angka');
			$('td', row).eq(11).addClass('angka');
			$('td', row).eq(12).addClass('center');
			$('td', row).eq(13).addClass('center');
			$('td', row).eq(14).addClass('center');
			$('td', row).eq(15).addClass('center');
			$('td', row).eq(17).addClass('center').editable(alamatupdaterak, {
				'submitdata': function ( value, settings ) {
					return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 16 };
				},
				'width': '40px',
				'height': '20px',
				'submit': 'Ok',
				'select': true,
				'indicator': 'Save..',
				'tooltip': 'Edit'
			});
		},
		/*"aoColumnDefs": [
		 { "bSortable": false, "aTargets": [ 0,1,2,3,5,7,11 ] }
		 ]*/
	});
}
function tampiltabellaundrydetail(){
	oTable = $("#tabel_detail_laundry").dataTable( {
		"bJQueryUI": true,
		"bAutoWidth": false,
		"bPaginate": false,
		"bLengthChange": false,
		"bInfo": false,
		"aaSorting": [[0, "asc"]],
		"sDom": "<'H'<'toolbar'>fr>t<'F'ip>"
	});
}
function view_detail(idlaundry,nonota){
	var request = new Object();
	request.idtitipanlaundry = idlaundry;
	alamat = pathutama + 'penjualan/detaillaundry';
	$.ajax({
		type: 'POST',
		url: alamat,
		data: request,
		cache: false,
		success: function(data){
			$('#dialogdetail').html(data);
			tampiltabellaundrydetail();
			$('div.toolbar').html('No. Nota : '+ nonota);
			$('#dialogdetail').dialog('open');
			/*$('.edit-jumlah').editable(alamatupdatedetaillaundry,{
			 name : 'jumlahproduk',
			 width : 60,
			 height : 18,
			 style   : 'margin: 0',
			 tooltip   : 'Klik untuk mengubah jumlah barang',
			 indicator : 'Saving...'
			 });
			 $('.edit-hargajual').editable(alamatupdatedetaillaundry,{
			 name : 'hargajual',
			 width : 90,
			 height : 18,
			 style   : 'margin: 0',
			 tooltip   : 'Klik untuk mengubah hargajual',
			 indicator : 'Saving...'
			 });*/
		}
	});
}
function delete_laundry(idlaundry,nonota){
	var konfirmasi = confirm('Yakin ingin menghapus laundry dengan no nota : '+ nonota +' ini...??!!');
	if (konfirmasi){
		window.location = pathutama + 'penjualan/deletelaundry/'+ idlaundry +'?destination=penjualan/viewlaundry';
	}
}
function print_laundry(idlaundry,nonota){
	var konfirmasi = confirm('Yakin ingin mencetak kembali laundry dengan no nota : '+ nonota +' ini...??!!');
	if (konfirmasi){
		window.open(pathutama + 'print/6?nidlaundry='+ idlaundry);
	}
}
function pickup_laundry(idtitipan, nonota){
	window.open(Drupal.settings.basePath + 'penjualan/kasir/'+ idtitipan);
}
$(document).ready(function(){
	pathutama = Drupal.settings.basePath;
	alamatupdatetanggaljual = pathutama + 'penjualan/updatelaundry';
	alamatupdaterak = pathutama + 'penjualan/updateraklaundry';
	//alamatupdatedetaillaundry = pathutama + 'laundry/updatedetaillaundry';
	urutan = Drupal.settings.urutan;
	$('#dialogdetail').dialog({
		modal: true,
		width: 850,
		resizable: false,
		autoOpen: false,
		position: ['auto','auto']
	});
	$('button').button();
	//TableToolsInit.sSwfPath = pathutama +'misc/media/datatables/swf/ZeroClipboard.swf';
	if (urutan == 1){
		$('.edit-tanggal').editable(alamatupdatetanggaljual,{
			submitdata : function(value, settings) {
				var idlaundry = $(this).attr('id');
				var splitidlaundry = idlaundry.split('-');
				idlaundry = splitidlaundry[1];
				var jamlaundryupdate = $('#jamlaundry-'+ idlaundry).html();
				return {jamlaundry: jamlaundryupdate,ubah: 'tanggal'};
			},
			name : 'tanggaljual',
			width : 130,
			height : 18,
			style   : 'margin: 0',
			tooltip   : 'Klik untuk mengubah tanggal laundry',
			indicator : 'Saving...',
			type: "datepicker",
			datepicker: {
				changeMonth: true,
				changeYear: true,
				dateFormat: "dd-mm-yy"
			},
			callback : function(value, settings) {
				var split_tanggal = value.split('-');
				var tanggaljual = new Date();
				var bulan = parseInt(split_tanggal[1]) - 1;
				tanggaljual.setFullYear(split_tanggal[2],bulan,split_tanggal[0]);
				var indexhari = tanggaljual.getDay();
				var hari = Drupal.settings.namahari[indexhari];
				var idlaundry = $(this).attr('id');
				var splitidlaundry = idlaundry.split('-');
				idlaundry = splitidlaundry[1];
				$('#harilaundry-'+ idlaundry).html(hari);
			}
		});
		$('.edit-jam').editable(alamatupdatetanggaljual,{
			name : 'jampenjualan',
			width : 120,
			height : 18,
			style   : 'margin: 0;',
			type: "timepicker",
			submitdata : function(value, settings) {
				var idlaundry = $(this).attr('id');
				var splitidlaundry = idlaundry.split('-');
				idlaundry = splitidlaundry[1];
				var tgllaundryupdate = $('#tglpenjualan-'+ idlaundry).html();
				return {tanggaljual: tgllaundryupdate,ubah: 'jam'};
			},
			timepicker: {
				timeOnlyTitle: "PILIH WAKTU",
				timeText: "Waktu",
				hourText: "Jam",
				minuteText: "Menit",
				secondText: "Detik",
				currentText: "Saat Ini",
				showButtonPanel: false
			},
			submit		: "Ok",
			tooltip   : 'Klik untuk mengubah jam laundry',
			indicator : 'Saving...'
		});
	}
	tampiltabellaundry();
	$('#tgl1').datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: 'dd-mm-yy'
	});
	$('#tgl2').datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: 'dd-mm-yy'
	});
})