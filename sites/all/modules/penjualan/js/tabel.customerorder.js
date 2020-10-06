var oTable;
var oTable2;
var pathutama = '';
var urutan = '';
var alamatupdatedetailorder = '';
var selectedOrder = 0;
var selectedNota = '';
var selectedPelanggan = 0;
var currSym = '';
var tSep = '.';
var dSep = ',';
var dDigit = 0;
var alamatupdate = '';
var AlamatMeja = '';

function tampiltabelcustomerorder(){
	if (Drupal.settings.urutan == 1){
		oTable = $('#tabel_customerorder').dataTable( {
			'bJQueryUI': true,
			'bAutoWidth': false,
			'sPaginationType': 'full_numbers',
			'bInfo': true,
			'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
			'iDisplayLength': 100,
			'order': [[ 4, "desc" ]],
			'processing': true,
			'serverSide': true,
			'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=customerorder&tglawal='+ Drupal.settings.tglawal +'&tglakhir='+ Drupal.settings.tglakhir,
			buttons: [
				{
					extend: 'colvis',
					columns: [4,5,6,7,8,9,10,11,12,13,14,15,16]
				}, 'copy', 'excel', 'print'
			],
			'sDom': '<"button-div"B><"H"l<"#multidiv">fr>t<"F"ip>',
			'createdRow': function ( row, data, index ) {
				row.id = data[(data.length - 1)];
				$('td', row).eq(1).addClass('center');
				$('td', row).eq(2).addClass('center');
				$('td', row).eq(3).addClass('center');
				$('td', row).eq(4).addClass('center');
				$('td', row).eq(5).addClass('center').editable(alamatupdate, {
					'submitdata': function ( value, settings ) {
						return { 'row_id': this.parentNode.getAttribute('id'), 'ubah': 'meja' };
					},
					'loadurl' : AlamatMeja,
					'width': '100px',
					'height': '20px',
					'submit': 'Ok',
					'type': 'select',
					'indicator': 'Menyimpan...',
					'tooltip': 'Klik untuk mengubah meja...'
				});
				$('td', row).eq(6).addClass('center');
				$('td', row).eq(7).addClass('center');
				$('td', row).eq(8).addClass('center');
				$('td', row).eq(9).addClass('center');
				$('td', row).eq(10).addClass('angka');
				$('td', row).eq(11).addClass('angka');
				$('td', row).eq(12).addClass('angka');
				$('td', row).eq(13).addClass('angka');
				$('td', row).eq(14).addClass('center');
				$('td', row).eq(15).addClass('angka');
				$('td', row).eq(16).addClass('angka');
				$('td', row).eq(17).addClass('center');
				$('td', row).eq(18).addClass('center');
				$('td', row).eq(19).addClass('center');
				$('td', row).eq(20).addClass('center');
			},
			'drawCallback': function( settings ) {
				var renderer = "bmp";
				var btype = "ean13";
				var settings = {
					output:renderer,
					barWidth: 1,
					barHeight: 20
				};
				$(".barcode-place").each(function(){
					barcode_value = $(this).attr('id');
					$(this).barcode(barcode_value, btype, settings);
				});
			},
			"aoColumnDefs": [
                {
                    "bSortable": false,
                    "aTargets": [0, 1, 2, 3, 5, 9, 13, 14, 17, 19, 22, 23]
                },
                {
                    "targets": [6, 8, 17, 20],
                    "visible": false
                }
			],
			'footerCallback': function ( row, data, start, end, display ) {
				var api = this.api(), data;
				// Remove the formatting to get integer data for summation
                var intVal = function ( i ) {
                    if (typeof i === 'string') {
                        i = i.split(tSep).join('');
                        i = i.split(dSep).join('.');
                    }else if (typeof i === 'number'){
                        i = i;
                    }else{
                        i = 0;
                    }
                    return parseFloat(i);
                };
				// Total over all pages
				total = api
					.column( 11 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
                total = parseFloat(Math.abs(total)).toFixed(2);
				$( api.column( 11 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');

                total = api
                    .column( 12 )
                    .data()
                    .reduce( function (a, b) {
                        return intVal(a) + intVal(b);
                    }, 0 );
                // Update footer
                total = parseFloat(Math.abs(total)).toFixed(2);
                $( api.column( 12 ).footer() ).html(
                    currSym +' '+ number_format(total,dDigit,dSep,tSep)
                ).addClass('angka');

                total = api
                    .column( 13 )
                    .data()
                    .reduce( function (a, b) {
                        return intVal(a) + intVal(b);
                    }, 0 );
                // Update footer
                total = parseFloat(Math.abs(total)).toFixed(2);
                $( api.column( 13 ).footer() ).html(
                    currSym +' '+ number_format(total,dDigit,dSep,tSep)
                ).addClass('angka');


				total = api
					.column( 15 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				total = parseFloat(Math.abs(total)).toFixed(2);
				$( api.column( 15 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 16 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				total = parseFloat(Math.abs(total)).toFixed(2);
				$( api.column( 16 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
			},
		});
	}else{
		oTable = $('#tabel_customerorder').dataTable( {
			'bJQueryUI': true,
			'bAutoWidth': false,
			'sPaginationType': 'full_numbers',
			'bInfo': true,
			'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
			'iDisplayLength': 100,
			'order': [[ 1, "desc" ]],
			/*'processing': true,
			'serverSide': true,
			'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=customerorder&tglawal='+ Drupal.settings.tglawal +'&tglakhir='+ Drupal.settings.tglakhir,*/
			buttons: [
				'copy', 'excel', 'print'
			],
			'sDom': '<"button-div"B><"H"lfr>t<"F"ip>',
		});
	}
}
function tampiltabelcustomerorderdetail(selectedId){
	selectedOrder = selectedId;
	oTable2 = $("#tabel_detail_customerorder").dataTable( {
		'bJQueryUI': true,
		'bAutoWidth': false,
		'bPaginate': false,
		'bLengthChange': false,
		'bInfo': false,
		'aaSorting': [[1, 'asc']],
		'sDom': '<"H"<"toolbar">fr>t<"F"ip>',
		'aoColumnDefs': [
			{ 'bSortable': false, 'aTargets': [ 0 ] }
		],
		'processing': true,
		'serverSide': true,
		'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=detailcustomerorder&idcustomerorder=' + selectedId,
		'createdRow': function ( row, data, index ) {
			row.id = data[(data.length - 1)];
			$('td', row).eq(1).addClass('center');
			$('td', row).eq(3).addClass('angka').attr('id','qty-'+ data[(data.length - 1)]).editable(alamatupdatedetailorder,{
				name : 'qty',
				width : 60,
				height : 18,
				select: true,
				style   : 'margin: 0',
				tooltip   : 'Klik untuk mengubah jumlah barang',
				indicator : 'Saving...',
				callback : function(value, settings) {
					var QtyPrint = parseFloat(value) - $('#qtyprint-'+ row.id).val();
					oTable2.fnDraw();
					setTimeout(function(){
						if (QtyPrint > 0){
							$('#qtyprint-'+ row.id).val(QtyPrint);
						}else{
							$('#qtyprint-'+ row.id).val(value);
						}
					},500);
				}
			});
			$('td', row).eq(4).addClass('angka');
			$('td', row).eq(5).addClass('angka');
			$('td', row).eq(6).addClass('angka');
			$('td', row).eq(7).addClass('angka');
			$('td', row).eq(8).addClass('center');
			$('td', row).eq(9).addClass('center');
			$('td', row).eq(11).attr('id','keterangan-'+ data[(data.length - 1)]).editable(alamatupdatedetailorder,{
				name : 'keterangan',
				width : 100,
				height : 18,
				select: true,
				style   : 'margin: 0',
				tooltip   : 'Klik untuk mengisi keterangan',
				indicator : 'Saving...'
			});
		},
		'footerCallback': function ( row, data, start, end, display ) {
			var api = this.api(), data;
			// Remove the formatting to get integer data for summation
			var intVal = function ( i ) {
				if (typeof i === 'string') {
					i = i.split(tSep).join('');
					i = i.split(dSep).join('.');
				}else if (typeof i === 'number'){
					i = i;
				}else{
					i = 0;
				}
				return parseFloat(i);
			};
			// Total over all pages
			total = api
				.column( 7 )
				.data()
				.reduce( function (a, b) {
					return intVal(a) + intVal(b);
				}, 0 );
			// Update footer
			total = parseFloat(Math.abs(total)).toFixed(2);
			$( api.column( 7 ).footer() ).html(
				currSym +' '+ number_format(total,dDigit,dSep,tSep)
			).addClass('angka');
		},
	});
}
function view_detail(idcustomerorder,nonota,idpelanggan){
    selectedNota = nonota;
	selectedPelanggan = idpelanggan;
	var request = new Object();
	request.idcustomerorder = idcustomerorder;
	alamat = pathutama + 'penjualan/detailcustomerorder';
	$.ajax({
		type: 'POST',
		url: alamat,
		data: request,
		cache: false,
		success: function(data){
			$('#table-detail-wrapper').html(data);
			tampiltabelcustomerorderdetail(idcustomerorder);
			$('div.toolbar').html('No. Nota : '+ nonota);
			$('#dialogdetail').dialog('open');
		}
	});
}
function delete_customerorder(idcustomerorder,nonota){
	var konfirmasi = confirm('Yakin ingin menghapus customer order dengan no nota : '+ nonota +' ini...??!!');
	if (konfirmasi){
		window.location = pathutama + 'penjualan/deletecustomerorder/'+ idcustomerorder +'?destination=penjualan/viewcustomerorder';	
	}
}
function print_customerorder(idcustomerorder,nonota){
	var konfirmasi = confirm('Yakin ingin mencetak kembali customerorder dengan no nota : '+ nonota +' ini...??!!');	
	if (konfirmasi){
		window.open(pathutama + 'print/6?idghorderonly='+ idcustomerorder);
	}
}
function print_production(idcustomerorder,nonota){
	var konfirmasi = confirm('Yakin ingin mencetak detail customer order dengan no nota : '+ nonota +' untuk keperluan produksi...??!!');
	if (konfirmasi){
		window.open(pathutama + 'print/6?idorderlogo='+ idcustomerorder);
	}
}
function pickup_customerorder(idcustomerorder, nonota){
	window.open(Drupal.settings.basePath + 'penjualan/kasir/'+ idcustomerorder +'/viewcustomerorder');
}
function hapus_detail(id, namaproduct){
	var confirmation = confirm('Yakin ingin menghapus order '+ namaproduct +'...??!');
	if (confirmation){
		var request = new Object();
		request.iddetailorder = id;
		request.idcustomerorder = selectedOrder;
		alamat = pathutama + 'penjualan/deletedetailorder';
		$.ajax({
			type: 'POST',
			url: alamat,
			data: request,
			cache: false,
			success: function (data) {
				var returnData = eval(data);
				console.log(returnData);
				alert(returnData[0].message);
				oTable2.fnDraw();
			}
		});
	}
}

function panggil_pelanggan(nopanggil){
    var konfirmasi = confirm('Yakin order dengan no order : '+ nopanggil +' selesai dan memanggil pelanggan...??!!');
    if (konfirmasi){
        window.open(pathutama + 'panggilpelanggan/'+ nopanggil);
    }
}

function print_detail_order(id_order, id_detail_order, namaproduct){
    var konfirmasi = confirm('Yakin ingin mencetak order '+ namaproduct +' ini di tempat produksi nya..??!');
    if (konfirmasi){
    	var QtyPrint = $('#qtyprint-'+ id_detail_order).val();
        window.open(pathutama + "print/6?id_order=" + id_order +'&id_detail_order='+ id_detail_order +'&qtyprint='+ QtyPrint);
    }
}

$(document).ready(function(){
	pathutama = Drupal.settings.basePath;
	alamatupdatetanggaljual = pathutama + 'penjualan/updatecustomerorder';
	alamatupdatedetailorder = pathutama + 'penjualan/updatedetailcustomerorder';
	AlamatMeja = pathutama + 'dataproduk/mejaoption';
	alamatupdate = pathutama + 'penjualan/updatecustomerorder';
	currSym = Drupal.settings.currSym;
	tSep = Drupal.settings.tSep;
	dSep = Drupal.settings.dSep;
    dDigit = Drupal.settings.dDigit;

	urutan = Drupal.settings.urutan;
	$('#dialogdetail').dialog({
		modal: true,
		width: 1024,
		resizable: false,
		autoOpen: false,
		position: ['middle','center'],
		open: function(event, ui){
			$('.ui-dialog').css('overflow','inherit');
		},
		close: function(){
			oTable.fnDraw();
		}
	});
	$('button').button();
	//TableToolsInit.sSwfPath = pathutama +'misc/media/datatables/swf/ZeroClipboard.swf';
	if (urutan == 1){
		$('.edit-tanggal').editable(alamatupdatetanggaljual,{
			submitdata : function(value, settings) {
			 var idcustomerorder = $(this).attr('id');
			 var splitidcustomerorder = idcustomerorder.split('-');
			 idcustomerorder = splitidcustomerorder[1];
			 var jamcustomerorderupdate = $('#jamcustomerorder-'+ idcustomerorder).html();
			 return {jamcustomerorder: jamcustomerorderupdate,ubah: 'tanggal'};
   		},
			name : 'tanggaljual',
			width : 130,
			height : 18,
			style   : 'margin: 0',
			tooltip   : 'Klik untuk mengubah tanggal customerorder',
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
				var idcustomerorder = $(this).attr('id');
			 	var splitidcustomerorder = idcustomerorder.split('-');
			 	idcustomerorder = splitidcustomerorder[1];
			 	$('#haricustomerorder-'+ idcustomerorder).html(hari);
     	}
	  });
	  $('.edit-jam').editable(alamatupdatetanggaljual,{
			name : 'jampenjualan',
			width : 120,
			height : 18,
			style   : 'margin: 0;',
			type: "timepicker",
			submitdata : function(value, settings) {
			 var idcustomerorder = $(this).attr('id');
			 var splitidcustomerorder = idcustomerorder.split('-');
			 idcustomerorder = splitidcustomerorder[1];
			 var tglcustomerorderupdate = $('#tglpenjualan-'+ idcustomerorder).html();
			 return {tanggaljual: tglcustomerorderupdate,ubah: 'jam'};
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
			tooltip   : 'Klik untuk mengubah jam customerorder',
	    indicator : 'Saving...'
	  });
	}
	tampiltabelcustomerorder();
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
	$("#new-product").autocomplete({
		source: pathutama + 'sites/all/modules/datapelanggan/server_processing.php?request_data=getproduct&idpelanggan='+ selectedPelanggan,
		select: function (event, ui) {
			if (ui.item.barcode){
				$('#new-product').val(ui.item.barcode);
			}else if(!ui.item.barcode && ui.item.alt_code){
				$('#new-product').val(ui.item.alt_code);
			}else if(!ui.item.barcode && !ui.item.alt_code){
				$('#new-product').val(ui.item.value);
			}
			$('#idproduct').val(ui.item.id);
			var hargajual = ui.item.hargajual;
			var diskonview = '';
			if (ui.item.diskon > 0){
				hargajual = hargajual - (hargajual * ui.item.diskon/100);
				diskonview = '('+ ui.item.diskon +'%)';
			}
			$('#diskon').val(ui.item.diskon);
			$('#harga-view').val(currSym +' '+ number_format(hargajual,dDigit,dSep,tSep) +' '+ diskonview);
			$('#hargajual').val(hargajual);
			$('#hargapokok').val(ui.item.hargapokok);
			$('#subtotal-view').val(currSym +' '+ number_format(hargajual,dDigit,dSep,tSep));
			$('#qty-new').val('1');
			$('#qty-new').select();
		}
	});
	$('#qty-new').on('keyup',function(){
		var subTotal = $(this).val() * $('#hargajual').val();
		$('#subtotal-view').val(currSym +' '+ number_format(subTotal,dDigit,dSep,tSep));
	});
	$('#qty-new').on('keypress',function(e){
		if (e.keyCode == 13) {
			$('#add-new-button').click();
		}
	});
	$('#new-product').on('keypress', function(e){
		if (e.keyCode == 13){
			if ($("#new-product").val() != ''){
				var request = new Object();
				request.request_data = 'getproductbarcode';
				request.idpelanggan = selectedPelanggan;
				request.term = $("#new-product").val();
				alamat = pathutama + 'sites/all/modules/datapelanggan/server_processing.php';
				$.ajax({
					type: 'GET',
					url: alamat,
					data: request,
					cache: false,
					success: function(data){
						var returnData = eval(data);
						$('#new-product').val(returnData[0].value);
						$('#idproduct').val(returnData[0].id);
						var hargajual = returnData[0].hargajual;
						var diskonview = '';
						if (returnData[0].diskon > 0){
							hargajual = hargajual - (hargajual * returnData[0].diskon/100);
							diskonview = '('+ returnData[0].diskon +'%)';
						}
						$('#diskon').val(returnData[0].diskon);
						$('#hargajual').val(hargajual);
						$('#hargapokok').val(returnData[0].hargapokok);
						$('#harga-view').val(currSym +' '+ number_format(hargajual,dDigit,dSep,tSep) +' '+ diskonview);
						$('#subtotal-view').val(currSym +' '+ number_format(hargajual,dDigit,dSep,tSep));
						$('#qty-new').val('1');
						$('#qty-new').select();
					}
				});
			}
		}
	});
	$('#add-new-button').on('click', function(){
		var request = new Object();
		request.idcustomerorder = selectedOrder;
		request.idproduk = $('#idproduct').val();
		request.hargajual = $('#hargajual').val();
		request.hargapokok = $('#hargapokok').val();
		request.diskon = $('#diskon').val();
		request.qty = $('#qty-new').val();
		alamat = pathutama + 'penjualan/simpandetailorder';
		$.ajax({
			type: 'POST',
			url: alamat,
			data: request,
			cache: false,
			success: function (data) {
				oTable2.fnDraw();
				$('#new-product').val('');
				$('#idproduct').val('');
				$('#diskon').val('');
				$('#hargajual').val('');
				$('#hargapokok').val('');
				$('#harga-view').val('');
				$('#subtotal-view').val('');
				$('#qty-new').val('');
				$('#new-product').focus();
			}
		});
	});
    $('#print-slip').on('click', function(){
        print_customerorder(selectedOrder,selectedNota);
    });

	alamat = pathutama + 'datapremis/uploaddata';
	$.ajax({
		type: 'POST',
		url: alamat,
		cache: false,
		success: function (data) {

		}
	});
    $('#multidiv').append($('#bayar-multi-order'));
    $('#bayar-multi-order').on('click', function(){
        var selected_nota = '';
        var counterData = 0;
        $('.custord-select').each(function(){
            if ($(this).is(':checked')){
                var strID = $(this).val();
                selected_nota += '<option value="'+ strID +'" selected>'+ strID +'</option>';
                counterData++;
            }
        });
        if (selected_nota != ''){
            $('#idcustomerorder-select').empty();
            $('#idcustomerorder-select').append(selected_nota);
            $('#bayar-multi-form').submit();
        }
    });
})
