var oTable;
var oTable2;
var pathutama = '';
var tglAwal = '';
var tglAkhir = '';
var urutan = 0;
var tampilData = 0;
var idpelanggan = 0;
var idsupplier = 0;
var selectedPenjualan = 0;
var selectedNota = '';
var selectedPelanggan = 0;
var alamatupdatepenjualan = '';
var currSym = '';
var tSep = '.';
var dSep = ',';
var dDigit = 0;
var PilihanPelanggan = '';
function tampiltabeljual(){
	if (tampilData == 0){
        var TargetDisabled = [ 0,3,12,14,15,16, 17, 18 ];
        if (Drupal.settings.module_piutang_exists){
            var ArrayColumn = [1,2,3,4,5,6,7,8,9,10,11,12,13];
        }else{
            var ArrayColumn = [1,2,3,4,5,6,7,8,9,10,11,12];
        }
		oTable = $('#tabel_penjualan').dataTable( {
			'bJQueryUI': true,
			'bAutoWidth': false,
			'sPaginationType': 'full_numbers',
			'bInfo': true,
			'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
			'iDisplayLength': 100,
			'aaSorting': [[urutan, 'desc']],
			'processing': true,
			'serverSide': true,
			'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=penjualan&tglawal='+ tglAwal +'&tglakhir='+ tglAkhir +'&idpelanggan='+ Drupal.settings.filterId +'&gst_access='+ Drupal.settings.gst_access,
			buttons: [
				{
					extend: 'colvis',
					columns: ArrayColumn
				}, 'copy', 'excel', 'print'
			],
			'sDom': '<"button-div"B><"H"l<"#multidiv">fr>t<"F"ip>',
			'createdRow': function ( row, data, index ) {
				row.id = data[(data.length - 1)];
				row.idpelanggan = data[(data.length - 2)];
				$('td', row).eq(1).addClass('center');
				$('td', row).eq(2).addClass('center');
				$('td', row).eq(3).addClass('center');
				$('td', row).eq(4).addClass('angka');
				$('td', row).eq(5).addClass('angka');
				$('td', row).eq(6).addClass('angka');
				$('td', row).eq(7).addClass('angka');
				$('td', row).eq(8).addClass('angka');
				$('td', row).eq(9).addClass('center');
				$('td', row).eq(10).addClass('angka').editable(alamatupdate,{
                    submitdata : function(value, settings) {
                        var idpenjualan = row.id;
                        return { ubah: 'bayar' };
                    },
                    name   : 'bayar',
                    width  : 100,
                    height : 18,
                    style  : 'margin: 0',
                    submit : 'Ok',
                    select : true,
                    tooltip   : 'Klik untuk mengubah bayar',
                    indicator : 'Saving...',
                    callback : function(value, settings) {
                        oTable.fnDraw();
                    }
                }).attr('id','bayar-'+ row.id);
				$('td', row).eq(11).addClass('angka');
				$('td', row).eq(12).addClass('center');
				var PilihanPelanggan = Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=opsipelanggan&idpelanggan='+ row.idpelanggan;
				$('td', row).eq(13).addClass('center').editable(alamatupdate,{
					submitdata : function(value, settings) {
						var idpenjualan = row.id;
						var idpelanggan = row.idpelanggan;
						return { ubah: 'pelanggan' };
					},
					name    : 'pelanggan',
					loadurl : PilihanPelanggan,
					width   : 120,
					height  : 20,
					type    : 'select',
					style : 'width: 200px',
					cssclass : 'editable-select',
					submit  : 'Ok',
					tooltip   : 'Klik untuk mengubah pelanggan',
					indicator : 'Saving...',
					callback : function(value, settings) {
						oTable.fnDraw();
					}
				}).attr('id',row.id +'-'+ row.idpelanggan);
				$('td', row).eq(14).addClass('center');
                $('td', row).eq(15).addClass('center');
				$('td', row).eq(16).addClass('center');
			},
            'aoColumnDefs': [
                { 'bSortable': false, 'aTargets': TargetDisabled }
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
					.column( 4 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 4 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 5 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 5 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 6 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 6 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 7 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 7 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 8 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 8 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 10 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 10 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 11 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 11 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
			},
		});
	}else if (tampilData == 1){
		oTable = $('#tabel_penjualan').dataTable( {
			'bJQueryUI': true,
			'bAutoWidth': false,
			'sPaginationType': 'full_numbers',
			'bInfo': true,
			'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
			'iDisplayLength': -1,
			'aaSorting': [[urutan, 'desc']],
			'processing': true,
			'serverSide': true,
			'language' : {
				decimal   : dSep,
				thousands : tSep,
			},
			'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=penjualan2&tglawal='+ tglAwal +'&tglakhir='+ tglAkhir +'&idpelanggan='+ Drupal.settings.filterId +'&gst_access='+ Drupal.settings.gst_access,
			buttons: [
				{
					extend: 'colvis'
				}, 'copy', 'excel', 'print'
			],
			'sDom': '<"button-div"B><"H"lfr>t<"F"ip>',
			'createdRow': function ( row, data, index ) {
				row.id = data[(data.length - 1)];
				$('td', row).eq(0).addClass('center');
				$('td', row).eq(2).addClass('angka');
				$('td', row).eq(3).addClass('angka');
				$('td', row).eq(4).addClass('angka');
				$('td', row).eq(5).addClass('angka');
				$('td', row).eq(6).addClass('angka');
				$('td', row).eq(7).addClass('angka');
				$('td', row).eq(8).addClass('angka');
				$('td', row).eq(9).addClass('angka');
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
				$( api.column( 7 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 8 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 8 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 9 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 9 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
			},
		});
	}else if (tampilData == 2){
		oTable = $('#tabel_penjualan').dataTable( {
			'bJQueryUI': true,
			'bAutoWidth': false,
			'sPaginationType': 'full_numbers',
			'bInfo': true,
			'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
			'iDisplayLength': 100,
			'aaSorting': [[urutan, 'desc']],
			'processing': true,
			'serverSide': true,
			'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=penjualan3&tglawal='+ tglAwal +'&tglakhir='+ tglAkhir +'&gst_access='+ Drupal.settings.gst_access,
			buttons: [
				{
					extend: 'colvis'
				}, 'copy', 'excel', 'print'
			],
			'sDom': '<"button-div"B><"H"lfr>t<"F"ip>',
			'createdRow': function ( row, data, index ) {
				row.id = data[(data.length - 1)];
				$('td', row).eq(0).addClass('center');
				$('td', row).eq(2).addClass('angka');
				$('td', row).eq(3).addClass('angka');
				$('td', row).eq(4).addClass('angka');
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
					.column( 2 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 2 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 3 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 3 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
				total = api
					.column( 4 )
					.data()
					.reduce( function (a, b) {
						return intVal(a) + intVal(b);
					}, 0 );
				// Update footer
				$( api.column( 4 ).footer() ).html(
					currSym +' '+ number_format(total,dDigit,dSep,tSep)
				).addClass('angka');
			},
		});
	}
}
function tampiltabeljualdetail(){
    oTable2 = $("#tabel_detail_penjualan").dataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'bPaginate': false,
        'bLengthChange': false,
        'scrollY': '330px',
        'scrollCollapse': true,
        'bInfo': false,
        'aaSorting': [[1, 'asc']],
        'sDom': '<"H"<"toolbar">fr>t<"F"ip>',
        'aoColumnDefs': [
            { 'bSortable': false, 'aTargets': [ 0 ] }
        ],
        'processing': true,
        'serverSide': true,
        'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?asal=penjualan&request_data=detailpenjualan&idpenjualan=' + selectedPenjualan,
        'createdRow': function ( row, data, index ) {
            row.id = data[(data.length - 1)];
            $('td', row).eq(1).addClass('center');
            $('td', row).eq(3).addClass('angka').attr('id','qty-'+ data[(data.length - 1)]).editable(alamatupdatepenjualan,{
                name : 'qty',
                width : 60,
                height : 18,
                select: true,
                style   : 'margin: 0',
                tooltip   : 'Klik untuk mengubah jumlah barang',
                indicator : 'Saving...',
                callback : function(value, settings) {
                    oTable2.fnDraw();
                }
            });
            $('td', row).eq(4).addClass('angka').attr('id','hargajual-'+ data[(data.length - 1)]).editable(alamatupdatepenjualan,{
                name : 'hargajual',
                width : 80,
                height : 18,
                select: true,
                style   : 'margin: 0',
                tooltip   : 'Klik untuk mengubah harga jual barang',
                indicator : 'Saving...',
                data : function (value, settings) {
                    var newValue = value.replace(tSep,'');
                    newValue = newValue.replace(dSep,'.');
                    return newValue;
                },
                callback : function(value, settings) {
                    oTable2.fnDraw();
                }
            });
            $('td', row).eq(5).addClass('angka').attr('id','diskon-'+ data[(data.length - 1)]).editable(alamatupdatepenjualan,{
                name : 'diskon',
                width : 80,
                height : 18,
                select: true,
                style   : 'margin: 0',
                tooltip   : 'Klik untuk mengubah diskon barang',
                indicator : 'Saving...',
                callback : function(value, settings) {
                    oTable2.fnDraw();
                }
            });
            $('td', row).eq(6).addClass('angka').attr('id','ppn-'+ data[(data.length - 1)]).editable(alamatupdatepenjualan,{
                name : 'ppn',
                width : 60,
                height : 18,
                select: true,
                style   : 'margin: 0',
                tooltip   : 'Klik untuk mengubah ppn barang',
                indicator : 'Saving...',
                callback : function(value, settings) {
                    oTable2.fnDraw();
                }
            });
            $('td', row).eq(7).addClass('angka').attr('id','hargapokok-'+ data[(data.length - 1)]).editable(alamatupdatepenjualan,{
                name : 'hargapokok',
                width : 80,
                height : 18,
                select: true,
                style   : 'margin: 0',
                tooltip   : 'Klik untuk mengubah harga modal barang',
                indicator : 'Saving...',
                data : function (value, settings) {
                    var newValue = value.replace(tSep,'');
                    newValue = newValue.replace(dSep,'.');
                    return newValue;
                },
                callback : function(value, settings) {
                    oTable2.fnDraw();
                }
            });
            $('td', row).eq(8).addClass('angka');
            $('td', row).eq(9).addClass('angka');
            $('td', row).eq(10).addClass('angka');
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
                .column( 8 )
                .data()
                .reduce( function (a, b) {
                    return intVal(a) + intVal(b);
                }, 0 );
            // Update footer
            $( api.column( 8 ).footer() ).html(
                currSym +' '+ number_format(total,dDigit,dSep,tSep)
            ).addClass('angka');
            total = api
                .column( 9 )
                .data()
                .reduce( function (a, b) {
                    return intVal(a) + intVal(b);
                }, 0 );
            // Update footer
            $( api.column( 9 ).footer() ).html(
                currSym +' '+ number_format(total,dDigit,dSep,tSep)
            ).addClass('angka');
            total = api
                .column( 10 )
                .data()
                .reduce( function (a, b) {
                    return intVal(a) + intVal(b);
                }, 0 );
            // Update footer
            $( api.column( 10 ).footer() ).html(
                currSym +' '+ number_format(total,dDigit,dSep,tSep)
            ).addClass('angka');
        },
    });
}
function view_detail(idpenjualan,nonota,idpelanggan){
	selectedPenjualan = idpenjualan;
	selectedNota = nonota;
	selectedPelanggan = idpelanggan;
	var request = new Object();
	request.idpenjualan = idpenjualan;
	alamat = pathutama + 'penjualan/detailpenjualan';
	$.ajax({
		type: 'POST',
		url: alamat,
		data: request,
		cache: false,
		success: function(data){
			$('#table-detail-wrapper').html(data);
			tampiltabeljualdetail();
			$('div.toolbar').html('No. Nota : '+ nonota);
			$('#dialogdetail').dialog('open');
		}
	});
}
function print_penjualan(idpenjualan,nonota){
	var konfirmasi = confirm('Yakin ingin mencetak nota penjualan dengan no nota : '+ nonota +' ini...??!!');
	if (konfirmasi){
		window.open(pathutama + 'print/6?idpenjualangh='+ idpenjualan);
	}
}
function hapus_detail(id, namaproduct){
	var confirmation = confirm('Yakin ingin menghapus penjualan '+ namaproduct +'...??!');
	if (confirmation){
		var request = new Object();
		request.iddetail = id;
		request.idpenjualan = selectedPenjualan;
		alamat = pathutama + 'penjualan/deletedetailpenjualan';
		$.ajax({
			type: 'POST',
			url: alamat,
			data: request,
			cache: false,
			success: function (data) {
				var returnData = eval(data);
				alert(returnData[0].message);
				oTable2.fnDraw();
			}
		});
	}
}

function print_faktur(idpenjualan,nonota){
    var konfirmasi = confirm('Yakin ingin export faktur ke xls untuk penjualan dengan no nota : '+ nonota +' ini...??!!');
    if (konfirmasi){
        alamat = pathutama + 'penjualan/exportfaktur/'+ idpenjualan;
        $.ajax({
            type: 'POST',
            url: alamat,
            cache: false,
            success: function(data){
                var xlsFilename = data.trim();
                window.location = Drupal.settings.basePath +"sites/default/files/"+ xlsFilename;
            }
        });
    }
}

function delete_penjualan(idpenjualan, nonota){
	var konfirmasi = confirm(Drupal.settings.konfirmasi_text +'...??!');
	if (konfirmasi){
        var request = new Object();
        request.idpenjualan = idpenjualan;
        alamat = pathutama + 'penjualan/hapuspenjualan';
        $.ajax({
            type: 'POST',
            url: alamat,
            data: request,
            cache: false,
            success: function(data){
				alert('Data telah di hapus...!!!');
                oTable.fnDraw();
            }
        });
	}
}

function SyncRequest(){
    var AlamatSync = pathutama + 'penjualan/syncpenjualan';
    $.ajax({
        url: AlamatSync,
        type: 'POST',
        cache: false,
        success: function(data){
            oTable.fnDraw();
        },
    });
}

$(document).ready(function(){
	pathutama = Drupal.settings.basePath;
	alamatupdatepenjualan = pathutama + 'penjualan/updatedetailpenjualan';
	alamatupdate = pathutama + 'penjualan/updatepenjualan';
	urutan = Drupal.settings.urutan;

	currSym = Drupal.settings.currSym;
	tSep = Drupal.settings.tSep[0];
	dSep = Drupal.settings.dSep[0];
    dDigit = Drupal.settings.dDigit;

	tampilData = Drupal.settings.tampilData;
    tglAwal = Drupal.settings.tglAwal;
    tglAkhir = Drupal.settings.tglAkhir;
	$('#dialogdetail').dialog({
		modal: true,
		width: 1000,
		resizable: false,
		autoOpen: false,
		position: ['auto','auto'],
		close: function(){
			oTable.fnDraw();
		}
	});
	$('button').button();
	/*TableToolsInit.sSwfPath = pathutama +'misc/media/datatables/swf/ZeroClipboard.swf';*/
	tampiltabeljual();

	setInterval(oTable.fnDraw(), 120000);
	
	$('#tgl1').datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: 'yy-mm-dd'
	});
	$('#tgl2').datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: 'yy-mm-dd'
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
		request.idpenjualan = selectedPenjualan;
		request.idproduk = $('#idproduct').val();
		request.hargajual = $('#hargajual').val();
		request.hargapokok = $('#hargapokok').val();
		request.diskon = $('#diskon').val();
		request.qty = $('#qty-new').val();
		alamat = pathutama + 'penjualan/simpandetailpenjualan';
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
		print_penjualan(selectedPenjualan,selectedNota);
	});

	/* Upload data to report server */
	alamat = pathutama + 'datapremis/uploaddata';
	$.ajax({
		type: 'POST',
		url: alamat,
		cache: false,
		success: function (data) {

		}
	});
	var SyncButton = '<a onclick="SyncRequest()" class="dt-button" tabindex="0" aria-controls="tabel_penjualan"><span>Sync Penjualan</span></a>';
    $('.button-div .dt-buttons').append(SyncButton);
	/* End upload data */
    $('#idpelanggan').chosen();
	$('#multidiv').append($('#print-multi-nota'));
	$('#print-multi-nota').on('click', function(){
		var selected_nota = '';
		var counterData = 0;
		$('.penjualan-select').each(function(){
			if ($(this).is(':checked')){
				var strID = $(this).val();
				selected_nota += '<option value="'+ strID +'" selected>'+ strID +'</option>';
				counterData++;
			}
		});
		if (selected_nota != ''){
			$('#idpenjualan-select').empty();
			$('#idpenjualan-select').append(selected_nota);
			$('#print-multi-form').submit();
		}
	});
})
