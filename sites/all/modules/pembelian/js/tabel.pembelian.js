var oTable;
var oTable2;
var pathutama = '';
var tglAwal = '';
var tglAkhir = '';
var urutan = 0;
var tampilData = 0;
var idsupplier = 0;
var selectedPembelian = 0;
var selectedNota = '';
var selectedSupplier = 0;
var alamatupdatepembelian = '';
var alreadyInit = 0;
var currSym = '';
var tSep = '.';
var dSep = ',';
var dDigit = 0;

function addCommas(nStr){
    nStr += "";
    x = nStr.split(".");
    x1 = x[0];
    x2 = x.length > 1 ? "," + x[1] : "";
    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1)) {
        x1 = x1.replace(rgx, "$1" + "." + "$2");
    }
    return x1 + x2;
}

function tampiltabelbeli(){
    if (tampilData == 0){
        oTable = $('#tabel_pembelian').dataTable( {
            'bJQueryUI': true,
            'bAutoWidth': false,
            'sPaginationType': 'full_numbers',
            'bInfo': true,
            'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
            'iDisplayLength': 100,
            'aaSorting': [[urutan, 'desc']],
            'processing': true,
            'serverSide': true,
            'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=pembelian&tglawal='+ tglAwal +'&tglakhir='+ tglAkhir +'&idsupplier='+ Drupal.settings.filterId +'&type_pelanggan=' + Drupal.settings.type_pelanggan,
            buttons: [
                {
                    extend: 'colvis',
                    columns: [1,2,3,4,5,6,7,8,9,10]
                }, 'copy', 'excel', 'print'
            ],
            'sDom': '<"button-div"B><"H"lfr>t<"F"ip>',
            'createdRow': function ( row, data, index ) {
                row.id = data[(data.length - 1)];
                $('td', row).eq(1).addClass('center');
                $('td', row).eq(2).addClass('center');
                $('td', row).eq(3).addClass('center').attr('id','haripembelian-'+ row.id);
                $('td', row).eq(4).addClass('center').editable(alamatupdatetanggalbeli,{
                    submitdata : function(value, settings) {
                        var idpembelian = row.id;
                        var jampembelianupdate = $('#jampembelian-'+ idpembelian).html();
                        return {jampembelian: jampembelianupdate,ubah: 'tanggal'};
                    },
                    name : 'tanggalbeli',
                    width : 130,
                    height : 18,
                    style   : 'margin: 0',
                    tooltip   : 'Klik untuk mengubah tanggal pembelian',
                    indicator : 'Saving...',
                    type: "datepicker",
                    datepicker: {
                        changeMonth: true,
                        changeYear: true,
                        dateFormat: "yy-mm-dd"
                    },
                    callback : function(value, settings) {
                        var split_tanggal = value.split('-');
                        var tanggalbeli = new Date();
                        var bulan = parseInt(split_tanggal[1]) - 1;
                        tanggalbeli.setFullYear(split_tanggal[2],bulan,split_tanggal[0]);
                        var indexhari = tanggalbeli.getDay();
                        var hari = Drupal.settings.namahari[indexhari];
                        var idpembelian = row.id;
                        $('#haripembelian-'+ idpembelian).html(hari);
                    }
                }).attr('id','tglpembelian-'+ row.id);
                $('td', row).eq(5).addClass('center').editable(alamatupdatetanggalbeli,{
                    name : 'jampembelian',
                    width : 120,
                    height : 18,
                    style   : 'margin: 0;',
                    type: "timepicker",
                    submitdata : function(value, settings) {
                        var idpembelian = row.id;
                        var tglpembelianupdate = $('#tglpembelian-'+ idpembelian).html();
                        return {tanggalbeli: tglpembelianupdate,ubah: 'jam'};
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
                    tooltip   : 'Klik untuk mengubah jam pembelian',
                    indicator : 'Saving...'
                }).attr('id','jampembelian-'+ row.id);
                $('td', row).eq(6).addClass('angka');
                $('td', row).eq(7).addClass('center');
                $('td', row).eq(8).addClass('angka');
                $('td', row).eq(9).addClass('angka');
                $('td', row).eq(10).addClass('center');
                $('td', row).eq(11).addClass('center');
            },
            'aoColumnDefs': [
                { 'bSortable': false, 'aTargets': [ 0,1,4,10 ] }
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
    }else if (tampilData == 1){
        oTable = $('#tabel_pembelian').dataTable( {
            'bJQueryUI': true,
            'bAutoWidth': false,
            'sPaginationType': 'full_numbers',
            'bInfo': true,
            'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
            'iDisplayLength': 100,
            'aaSorting': [[urutan, 'desc']],
            'processing': true,
            'serverSide': true,
            'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=pembelian2&tglawal='+ tglAwal +'&tglakhir='+ tglAkhir +'&idsupplier='+ Drupal.settings.filterId,
            buttons: [
                {
                    extend: 'colvis'
                }, 'copy', 'excel', 'print'
            ],
            'sDom': '<"button-div"B><"H"lfr>t<"F"ip>',
            'createdRow': function ( row, data, index ) {
                row.id = data[(data.length - 1)];
                $('td', row).eq(0).addClass('center');
                $('td', row).eq(3).addClass('angka');
                $('td', row).eq(4).addClass('angka');
                $('td', row).eq(5).addClass('angka');
                $('td', row).eq(6).addClass('angka');
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
                    .column( 6 )
                    .data()
                    .reduce( function (a, b) {
                        return intVal(a) + intVal(b);
                    }, 0 );
                // Update footer
                $( api.column( 6 ).footer() ).html(
                    currSym +' '+ number_format(total,dDigit,dSep,tSep)
                ).addClass('angka');
            },
        });
    }
}
function tampiltabelbelidetail(){
    oTable2 = $("#tabel_detail_pembelian").dataTable( {
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
        'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?asal=pembelian&request_data=detailpembelian&idpembelian=' + selectedPembelian,
        'createdRow': function ( row, data, index ) {
            row.id = data[(data.length - 1)];
            $('td', row).eq(1).addClass('center');
            $('td', row).eq(3).addClass('angka').attr('id','qty-'+ data[(data.length - 1)]).editable(alamatupdatepembelian,{
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
            $('td', row).eq(4).addClass('angka').attr('id','harga-'+ data[(data.length - 1)]).editable(alamatupdatepembelian,{
                name : 'harga',
                width : 80,
                height : 18,
                select: true,
                style   : 'margin: 0',
                tooltip   : 'Klik untuk mengubah harga modal',
                indicator : 'Saving...',
                'data': function (value, settings) {
                    var newValue = value.replace(tSep,'');
                    newValue = newValue.replace(dSep,'.');
                    return newValue;
                },
                callback : function(value, settings) {
                    oTable2.fnDraw();
                }
            });
            $('td', row).eq(5).addClass('angka');
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
                .column( 5 )
                .data()
                .reduce( function (a, b) {
                    return intVal(a) + intVal(b);
                }, 0 );
            // Update footer
            $( api.column( 5 ).footer() ).html(
                currSym +' '+ number_format(total,dDigit,dSep,tSep)
            ).addClass('angka');
        },
    });
}
function view_detail(idpembelian,nonota,idsupplier){
    selectedPembelian = idpembelian;
    selectedNota = nonota;
    selectedSupplier = idsupplier;
    if (alreadyInit > 0) {
        $("#new-product").autocomplete('destroy');
    }
    $("#new-product").autocomplete({
        source: pathutama + 'sites/all/modules/datapelanggan/server_processing.php?request_data=getproduct&idsupplier='+selectedSupplier,
        select: function (event, ui) {
            if (ui.item.barcode){
                $('#new-product').val(ui.item.barcode);
            }else if(!ui.item.barcode && ui.item.alt_code){
                $('#new-product').val(ui.item.alt_code);
            }else if(!ui.item.barcode && !ui.item.alt_code){
                $('#new-product').val(ui.item.value);
            }
            $('#idproduct').val(ui.item.id);
            $('#harga-view').val(currSym +' '+ number_format(ui.item.hargapokok,dDigit,dSep,tSep));
            $('#hargapokok').val(ui.item.hargapokok);
            $('#subtotal-view').val(currSym +' '+ number_format(ui.item.hargapokok,dDigit,dSep,tSep));
            $('#qty-new').val('1');
            $('#expired-date').select();
        }
    });
    var request = new Object();
    request.idpembelian = idpembelian;
    alamat = pathutama + 'pembelian/detailpembelian';
    $.ajax({
        type: 'POST',
        url: alamat,
        data: request,
        cache: false,
        success: function(data){
            $('#table-detail-wrapper').html(data);
            tampiltabelbelidetail();
            $('div.toolbar').html('No. Nota : '+ nonota);
            $('#dialogdetail').dialog('open');
        }
    });
    alreadyInit++;
}
function delete_pembelian(idpembelian,nonota){
	var konfirmasi = confirm('Yakin ingin menghapus pembelian dengan no nota : '+ nonota +' ini...??!!');	
	if (konfirmasi){
		window.location = pathutama + 'pembelian/deletepembelian/'+ idpembelian +'?destination=pembelian/viewpembelian';	
	}
}
function hapus_detail(id, namaproduct){
    var confirmation = confirm('Yakin ingin menghapus detail pembelian untuk product '+ namaproduct +'...??!');
    if (confirmation){
        var request = new Object();
        request.iddetail = id;
        request.idpembelian = selectedPembelian;
        alamat = pathutama + 'pembelian/deletedetailpembelian';
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
function print_detail(idpembelian, iddetail){
    window.open(pathutama + 'print/6?idpembelian='+ idpembelian +'&iddetail='+ iddetail);
}
function print_pembelian(idpembelian){
    window.open(pathutama + 'print/6?idpembelian='+ idpembelian);
}
$(document).ready(function(){
    pathutama = Drupal.settings.basePath;
    alamatupdatetanggalbeli = pathutama + 'pembelian/updatepembelian';
	urutan = Drupal.settings.urutan;
    tampilData = Drupal.settings.tampilData;
    tglAwal = Drupal.settings.tglAwal;
    tglAkhir = Drupal.settings.tglAkhir;
    alamatupdatepembelian = pathutama + 'pembelian/updatedetailpembelian';

    currSym = Drupal.settings.currSym;
    tSep = Drupal.settings.tSep[0];
    dSep = Drupal.settings.dSep[0];
    dDigit = Drupal.settings.dDigit;

	$("#dialogdetail").dialog({
		modal: true,
		width: 850,
		resizable: false,
		autoOpen: false,
		position: ["auto","auto"],
        open: function(){
            $('.ui-dialog.ui-widget').css('overflow','inherit');
        },
        close: function(){
            oTable.fnDraw();
        }
	});
	$("button").button();
	//TableToolsInit.sSwfPath = pathutama +"misc/media/datatables/swf/ZeroClipboard.swf";
	if (urutan == 1){
		$('.edit-tanggal').editable(alamatupdatetanggalbeli,{
			submitdata : function(value, settings) {
			 var idpembelian = $(this).attr('id');
			 var splitidpembelian = idpembelian.split('-');
			 idpembelian = splitidpembelian[1];
			 var jampembelianupdate = $('#jampembelian-'+ idpembelian).html();
			 return {jampembelian: jampembelianupdate,ubah: 'tanggal'};
   		},
			name : 'tanggalbeli',
			width : 130,
			height : 18,
			style   : 'margin: 0',
			tooltip   : 'Klik untuk mengubah tanggal pembelian',
	    indicator : 'Saving...',
	    type: "datepicker",
			datepicker: {
	      changeMonth: true,
	      changeYear: true,
	      dateFormat: "dd-mm-yy"
	    },
	    callback : function(value, settings) {
      	var split_tanggal = value.split('-');
      	var tanggalbeli = new Date();
      	var bulan = parseInt(split_tanggal[1]) - 1;
				tanggalbeli.setFullYear(split_tanggal[2],bulan,split_tanggal[0]);
				var indexhari = tanggalbeli.getDay();
				var hari = Drupal.settings.namahari[indexhari];
				var idpembelian = $(this).attr('id');
			 	var splitidpembelian = idpembelian.split('-');
			 	idpembelian = splitidpembelian[1];
			 	$('#haripembelian-'+ idpembelian).html(hari);
     	}
	  });
	  $('.edit-jam').editable(alamatupdatetanggalbeli,{
			name : 'jampembelian',
			width : 120,
			height : 18,
			style   : 'margin: 0;',
			type: "timepicker",
			submitdata : function(value, settings) {
			 var idpembelian = $(this).attr('id');
			 var splitidpembelian = idpembelian.split('-');
			 idpembelian = splitidpembelian[1];
			 var tglpembelianupdate = $('#tglpembelian-'+ idpembelian).html();
			 return {tanggalbeli: tglpembelianupdate,ubah: 'jam'};
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
			tooltip   : 'Klik untuk mengubah jam pembelian',
	    indicator : 'Saving...'
	  });
	}
	tampiltabelbeli();
	$("#tgl1").datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: "yy-mm-dd"
	});
	$("#tgl2").datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: "yy-mm-dd"
	});
    $('#add-new-button').on('click', function(){
        var request = new Object();
        request.idpembelian = selectedPembelian;
        request.idproduk = $('#idproduct').val();
        request.hargapokok = $('#hargapokok').val();
        request.qty = $('#qty-new').val();
        request.expired_date = $('#expired-date').val();
        alamat = pathutama + 'pembelian/simpandetailpembelian';
        $.ajax({
            type: 'POST',
            url: alamat,
            data: request,
            cache: false,
            success: function (data) {
                oTable2.fnDraw();
                $('#new-product').val('');
                $('#idproduct').val('');
                $('#hargapokok').val('');
                $('#harga-view').val('');
                $('#subtotal-view').val('');
                $('#qty-new').val('');
                $('#new-product').focus();
            }
        });
    });
    $('#expired-date').datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: 'yy-mm-dd',
        onClose: function(dateText, inst) {
            $('#qty-new').select();
        }
    });
    $('#qty-new').on('keyup',function(){
        var subTotal = $(this).val() * $('#hargapokok').val();
        $('#subtotal-view').val('Rp. '+ addCommas(subTotal));
    });
    $('#qty-new').on('keypress',function(e){
        if (e.keyCode == 13) {
            $('#add-new-button').click();
        }
    });
    $('#print-slip').on('click', function(){
        print_pembelian(selectedPembelian,selectedNota);
    });
})