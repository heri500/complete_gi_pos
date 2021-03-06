var oTable;
var pathutama = '';
var pathfile = '';
var alamatupdate = '';
var barcodesama = false;
var renderer = 'bmp';
var btype = 'ean13';
var settings = {
    output:renderer,
    barWidth: 1,
    barHeight: 50
};
var statusstok = 0;
var statusproduct = 1;
var currSym = '';
var tSep = '.';
var dSep = ',';
var dDigit = 0;
var DiscTable = [];
var ProductSize = false;

function tampilkantabelproduk(){
    var ArrayColumn = [
        { 'bSortable': false },null,null,null,null,null,null,null,
        null,{ 'bVisible': false },{ 'bVisible': false },
        { 'bVisible': false },null,null,null,{ 'bVisible': false },null,null,{ 'bSortable': false },
        { 'bSortable': false },{ 'bSortable': false },{ 'bSortable': false }
    ];
    if (ProductSize){
        ArrayColumn = [
            { 'bSortable': false },null,null,null,null,null,null,null,
            null,{ 'bVisible': false },{ 'bVisible': false },
            { 'bVisible': false },null,null,null,{ 'bVisible': false },null,null,{ 'bSortable': false },
            { 'bSortable': false },{ 'bSortable': false },{ 'bSortable': false }, { 'bSortable': false }
        ];
    }
    oTable = $('#tabel_produk').DataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'sPaginationType': 'full_numbers',
        'bInfo': true,
        'processing': true,
        'serverSide': true,
        'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=produk&statusstok='+ statusstok +'&status_product='+ statusproduct  +'&idkategori='+ Drupal.settings.idkategori +'&product_size='+ ProductSize,
        'aoColumns': ArrayColumn,
        'aLengthMenu': [[100, 200, 300, 500], [100, 200, 300,500]],
        'iDisplayLength': 100,
        'aaSorting': [[1, 'asc']],
        buttons: [
            {
                extend: 'colvis',
                columns: ':not(:first-child)'
            }, 'copy', 'excel', 'print'
        ],
        'sDom': '<"button-div"B><"H"l<"#katdiv"><"#katstock">fr>t<"F"ip>',
        'createdRow': function ( row, data, index ) {
            row.id = data[(data.length - 1)];
            var alamatKategori = Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=kategori&idproduk='+ row.id;
            $('td', row).eq(1).addClass('center').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 1 };
                },
                'loadurl' : alamatKategori,
                'width': '140px',
                'height': '20px',
                'submit': 'Ok',
                'type': 'select',
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...'
            });
            var alamatSubKategori = Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=subkategori&idproduk='+ row.id;
            $('td', row).eq(2).addClass('center').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 2 };
                },
                'loadurl' : alamatSubKategori,
                'width': '140px',
                'height': '20px',
                'submit': 'Ok',
                'type': 'select',
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...'
            });
            var alamatsupplier = Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=supplier&idproduk='+ row.id;
            $('td', row).eq(3).addClass('left').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 11 };
                },
                'loadurl' : alamatsupplier,
                'width': '200px',
                'height': '20px',
                'submit': 'Ok',
                'type': 'multiselect',
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...',
                'onblur' : 'ignore',
                'cancel' : 'Cancel'
            }).click(function(){
                $(this).find('select').chosen();
                $('.chosen-container').css('width', '100%');
            });
            $('td', row).eq(4).addClass('center').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 3 };
                },
                'width': '120px',
                'height': '20px',
                'submit': 'Ok',
                'select': true,
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...'
            });
            $('td', row).eq(5).addClass('center').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 4 };
                },
                'width': '80px',
                'height': '20px',
                'submit': 'Ok',
                'select': true,
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...'
            });
            $('td', row).eq(6).addClass('left').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 5 };
                },
                'width': '220px',
                'height': '20px',
                'submit': 'Ok',
                'select': true,
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...'
            });
            $('td', row).eq(7).addClass('angka').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 6 };
                },
                'data': function (value, settings) {
                    var newValue = value.replace(tSep,'');
                    newValue = newValue.replace(dSep,'.');
                    return newValue;
                },
                'width': '70px',
                'height': '20px',
                'submit': 'Ok',
                'select': true,
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...'
            });
            $('td', row).eq(8).addClass('angka').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 7 };
                },
                'data': function (value, settings) {
                    var newValue = value.replace(tSep,'');
                    newValue = newValue.replace(dSep,'.');
                    return newValue;
                },
                'width': '70px',
                'height': '20px',
                'submit': 'Ok',
                'select': true,
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...',
                'callback' : function(value, settings){
                    oTable.draw(false);
                    var AlamatCalculateNilai = Drupal.settings.basePath + 'dataproduk/calculatenilai';
                    $.ajax({
                        type: 'POST',
                        url: AlamatCalculateNilai,
                        cache: false,
                        success: function (data) {
                            var ReturnVal = parseFloat(data.trim());
                            $('#total-nilai-barang').html(currSym +" "+ number_format(ReturnVal,dDigit,dSep,tSep));
                        }
                    });
                }
            });
            $('td', row).eq(9).addClass('angka').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 8 };
                },
                'data': function (value, settings) {
                    var newValue = value.replace(tSep,'');
                    newValue = newValue.replace(dSep,'.');
                    return newValue;
                },
                'width': '60px',
                'height': '20px',
                'submit': 'Ok',
                'select': true,
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...'
            });
            $('td', row).eq(10).addClass('center');
            var alamatSatuan = Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=satuan&idproduk='+ row.id;
            $('td', row).eq(11).addClass('center').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 10 };
                },
                'loadurl' : alamatSatuan,
                'width': '80px',
                'height': '20px',
                'submit': 'Ok',
                'type': 'select',
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...',
                'callback' : function(value, settings) {
                    oTable.draw(false);
                }
            });
            //$('td', row).eq(11).addClass('center');
            $('td', row).eq(12).addClass('angka');
            //$('td', row).eq(13).addClass('center');
            var alamatGrup = Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=grupmenu&idproduk='+ row.id;
            $('td', row).eq(13).addClass('center').editable(alamatupdate, {
                'submitdata': function ( value, settings ) {
                    return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': 12 };
                },
                'loadurl' : alamatGrup,
                'width': '140px',
                'height': '20px',
                'submit': 'Ok',
                'type': 'select',
                'indicator': 'Menyimpan...',
                'tooltip': 'Klik untuk mengubah...'
            });
            $('td', row).eq(14).addClass('center');
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
                .column( 16 )
                .data()
                .reduce( function (a, b) {
                    return intVal(a) + intVal(b);
                }, 0 );
            // Update footer
            $( api.column( 16 ).footer() ).html(
                currSym +' '+ number_format(total,dDigit,dSep,tSep)
            ).addClass('angka');
        },
        'drawCallback': function( settings ) {
            $('.barcode-select').click(function(){
                if ($(this).is(':checked')){
                    $(this).parent().parent().addClass('selected');
                }else{
                    $(this).parent().parent().removeClass('selected');
                }
            });
        },
    });
}

function simpankategori(){
    if ($('#kodekategori').val() != '' && $('#kategori').val() != ''){
        var request = new Object();
        request.kodekategori = $('#kodekategori').val();
        request.kategori = $('#kategori').val();
        request.keterangan = $('#keterangan').val();
        request.asal = 'Subkategori';
        alamat = pathutama + 'dataproduk/simpankategori';
        $.ajax({
            type: 'POST',
            url: alamat,
            data: request,
            cache: false,
            success: function(data){
                $('#idkategori').append('<option value="'+ data +'">'+ request.kodekategori +'-'+ request.kategori +'</option>');
                $('#kodekategori').val('');
                $('#kategori').val('');
                $('#keterangan').val('');
                $('#dialogtambahkategori').dialog('close');
                $('#idkategori').val(data);
                filtersubkategori();
            }
        });
    }else{
        $('#tambahkategoriform').submit();
    }
}

function simpansubkategori(){
    if ($('#kodesubkategori').val() != '' && $('#subkategori').val() != ''){
        var request = new Object();
        request.idkategori = $('#idkategori2').val();
        request.kodesubkategori = $('#kodesubkategori').val();
        request.subkategori = $('#subkategori').val();
        request.keterangan = $('#keterangansub').val();
        request.asal = 'Produk';
        alamat = pathutama + 'dataproduk/simpansubkategori';
        $.ajax({
            type: 'POST',
            url: alamat,
            data: request,
            cache: false,
            success: function(data){
                $('#idsubkategori').append('<option value="'+ data +'">'+ request.kodesubkategori +'-'+ request.subkategori +'</option>');
                $('#kodesubkategori').val('');
                $('#subkategori').val('');
                $('#keterangansub').val('');
                $('#dialogtambahsubkategori').dialog('close');
                $('#idsubkategori').val(data);
                ubahkodebarang();
            }
        });
    }else{
        $('#tambahsubkategoriform').submit();
    }
}

function simpansupplier(){
    if ($('#kodesupplier').val() != '' && $('#supplier').val() != ''){
        var request = new Object();
        request.kodesupplier = $('#kodesupplier').val();
        request.namasupplier = $('#namasupplier').val();
        request.keterangan = $('#keterangan').val();
        request.asal = 'supplier';
        alamat = pathutama + 'datasupplier/simpansupplier';
        $.ajax({
            type: 'POST',
            url: alamat,
            data: request,
            cache: false,
            success: function(data){
                $('#idsupplier').append('<option value="'+ data +'">'+ request.kodesupplier +'-'+ request.namasupplier +'</option>');
                $('#kodesupplier').val('');
                $('#supplier').val('');
                $('#keterangan').val('');
                $('#dialogtambahsupplier').dialog('close');
                $('#idsupplier').val(data);
            }
        });
    }else{
        $('#tambahsupplierform').submit();
    }
}
function simpansatuan(){
    if ($('#namasatuan').val() != ''){
        var request = new Object();
        request.namasatuan = $('#namasatuan').val();
        alamat = pathutama + 'dataproduk/simpansatuan';
        $.ajax({
            type: 'POST',
            url: alamat,
            data: request,
            cache: false,
            success: function(data){
                if (data != 'error'){
                    $('#satuan').append('<option value="'+ request.namasatuan +'">'+ request.namasatuan +'</option>');
                    $('#namasatuan').val('');
                    $('#dialogtambahsatuan').dialog('close');
                    $('#satuan').val(data);
                }else{
                    alert('Satuan ini sudah ada..!!');
                }
            }
        });
    }else{
        $('#tambahsatuanform').submit();
    }
}
function tutupformtambahproduk(){
    $('#formproduk').validationEngine('hide');
    $('#formpenambahanproduk').fadeOut('slow');
}
function filtersubkategori(){
    var request = new Object();
    request.idkategori = 	$('#idkategori').val();
    var alamat = pathutama + 'dataproduk/filterkategori';
    $.ajax({
        type: 'POST',
        url: alamat,
        data: request,
        cache: false,
        success: function(data){
            $('#idsubkategori').empty();
            $('#idsubkategori').append(data);
            ubahkodebarang();
        }
    });
}
function ubahkodebarang(){
    var kategori = '';
    $('#idkategori option:selected').each(function () {
        kategori = $(this).text();
    });
    var pecahkategori = kategori.split('-');
    var subkategori = '';
    $('#idsubkategori option:selected').each(function () {
        subkategori = $(this).text();
    });
    pecahsubkategori = subkategori.split('-');
    kodebarang = pecahkategori[0] +'-'+ pecahsubkategori[0];
    var request = new Object();
    request.kodealternatif = 	kodebarang;
    var alamat = pathutama + 'dataproduk/cekkodealternatif';
    $.ajax({
        type: 'POST',
        url: alamat,
        data: request,
        cache: false,
        success: function(data){
            $('#kodepoduk').val(kodebarang +'-'+ data.trim());
        }
    });
}
function hitungmargin(asal){
    if (asal == 'hargapokok'){
        if ($('#rad-hargajual').is(':checked')) {
            if ($('#margin').val() > 0 && $('#hargapokok').val() > 0){
                var hargajual = $('#hargapokok').val() * (100/(100-$('#margin').val()));
                $('#hargajual').val(Math.Math.round10(hargajual,(dDigit * -1)));
            }
        }else if ($('#rad-margin').is(':checked')) {
            if ($('#hargajual').val() > 0 && $('#hargapokok').val() > 0){
                var margin = 100 - (100*($('#hargapokok').val()/$('#hargajual').val()));
                $('#margin').val(Math.round10(margin,(dDigit * -1)));
            }
        }
    }else if (asal == 'hargajual'){
        if ($('#rad-hargapokok').is(':checked')) {
            if($('#hargajual').val() > 0 && $('#margin').val() > 0){
                var hargapokok = $('#hargajual').val() * ((100 - $('#margin').val())/100);
                $('#hargapokok').val(Math.round10(hargapokok,(dDigit * -1)));
            }
        }else if ($('#rad-margin').is(':checked')) {
            if ($('#hargajual').val() > 0 && $('#hargapokok').val() > 0){
                var margin = 100 - (100*($('#hargapokok').val()/$('#hargajual').val()));
                $('#margin').val(Math.round10(margin,(dDigit * -1)));
            }
        }
    }else if (asal == 'margin'){
        if ($('#rad-hargajual').is(':checked')) {
            if ($('#hargapokok').val() > 0 && $('#margin').val() > 0){
                var hargajual = $('#hargapokok').val() * (100/(100-$('#margin').val()));
                $('#hargajual').val(Math.round10(hargajual,(dDigit * -1)));
            }
        }else if ($('#rad-hargapokok').is(':checked')) {
            if($('#hargajual').val() > 0 && $('#margin').val() > 0){
                var hargapokok = $('#hargajual').val() * ((100 - $('#margin').val())/100);
                $('#hargapokok').val(Math.round10(hargapokok,(dDigit * -1)));
            }
        }
    }
}
function cek_barcode(field, rules, i, options){
    if (field.val() != ''){
        $('#validating').html('<img src = "'+ pathutama +'misc/media/images/loading.gif">');
        $('#validating').fadeIn('fast',function(){
            var request = new Object();
            request.barcode = field.val();
            cekbarcode = pathutama +'dataproduk/cekbarcode';
            $.ajax({
                type: 'POST',
                url: cekbarcode,
                data: request,
                cache: false,
                success: function(data){
                    if (data == 'sama'){
                        $('#validating').fadeOut('fast',function(){
                            $('#barcode').validationEngine('showPrompt', 'Barcode ini sudah digunakan..!!', 'error', 'topRight', true);
                            $('#barcode').select();
                            barcodesama = true;
                        })
                    }else{
                        if (data == 'taksama'){
                            barcodesama = false;
                        }
                        $('#validating').fadeOut('fast');
                    }
                }
            });
        })
    }
}
function simpanproduk(namafile){
    if ($('#barcode').val() != '' && !barcodesama && $('#namaproduk').val() != '' && $('#hargapokok').val() > 0 && $('#hargajual').val() > 0){
        var request = new Object();
        request.barcode_type = $('#barcode_type').val();
        request.barcode = $('#barcode').val();
        request.alt_code = $('#kodepoduk').val();
        request.idkategori = $('#idkategori').val();
        request.idsubkategori = $('#idsubkategori').val();
        request.idsupplier = $('#idsupplier').val();
        request.namaproduk = $('#namaproduk').val();
        request.hargapokok = $('#hargapokok').val();
        request.hargajual = $('#hargajual').val();
        request.margin = $('#margin').val();
        request.minstok = $('#minstok').val();
        request.maxstok = $('#maxstok').val();
        request.stok = $('#stok').val();
        request.satuan = $('#satuan').val();
        request.namafile = namafile;
        request.keterangan = $('#keteranganproduk').val();
        request.type_product = $("#type_product").val();
        request.lead_time = $("#lead_time").val();
        request.jam_kerja = $("#ikut_jam_kerja").val();
        request.sebelum_zuhur = $("#sebelum_zuhur").val();
        alamatsimpan = pathutama +'dataproduk/simpanproduk';
        $.ajax({
            type: 'POST',
            url: alamatsimpan,
            data: request,
            cache: false,
            success: function(){
                $('#namaproduk').val('');
                $('#hargapokok').val('');
                $('#hargajual').val('');
                $('#margin').val('');
                $('#keteranganproduk').val('');
                $('#stok').val('');
                $('#minstok').val('');
                $('#maxstok').val('');
                ubahkodebarang();
                $('#barcode').val('');
                $('#refreshprodukbaru').click();
                alamatbarcode = pathutama +'penjualan/getrandomstring';
                $.ajax({
                    type: 'POST',
                    url: alamatbarcode,
                    cache: false,
                    success: function(data){
                        $('#barcode').val(data.trim());
                        $('#barcode').select();
                    }
                });
            }
        });
    }else{
        $('#formproduk').validationEngine('validate');
    }
}
function editproduk(idproduk){
    if (idproduk > 0){
        window.location = pathutama + 'dataproduk/editproduk?idproduk='+ idproduk;
    }
}
function view_status(kondisistok){
    if (kondisistok != 'all' && kondisistok != 'nonaktif') {
        window.location = pathutama + 'dataproduk/produk?statusstok=' + kondisistok +'&idkategori='+ Drupal.settings.idkategori;
    }else if(kondisistok == 'nonaktif'){
        window.location = pathutama + 'dataproduk/produk?status_product=0'+'&idkategori='+ Drupal.settings.idkategori;
    }else{
        window.location = pathutama + 'dataproduk/produk?idkategori='+ Drupal.settings.idkategori;
    }
}
function save_produk(){
    var jumlah_file = 0;
    $(".uploadifyQueueItem").each(function(){
        jumlah_file++;
    });
    if (jumlah_file > 0){
        $("#uploadify").uploadifyUpload();
    }else{
        simpanproduk("tanpafile");
    }
}
function edithargaproduk(idproduct){
    if (idproduct){
        SelectedProduct = idproduct;
        $('#dialoghargasupplier').dialog('open');
    }
}
function closehargasupplier(){
    $('#dialoghargasupplier').dialog('close');
}
function simpanhargasupplier(){
    var PostData = '';
    $('.input-number').each(function(){
        if (PostData == ''){
            PostData = $(this).attr('id') + '_' + $(this).val();
        }else{
            PostData += '__' + $(this).attr('id') + '_' + $(this).val();
        }
    });
    if (PostData != ''){
        var request = new Object();
        request.dataHarga = PostData;
        request.idproduct = SelectedProduct;
        var AlamatUpdateHarga = pathutama +'dataproduk/updatesupplierprice';
        $.ajax({
            type: 'POST',
            url: AlamatUpdateHarga,
            data: request,
            cache: false,
            success: function(){
                alert('Data harga supplier berhasil diupdate...!!!');
                $('#dialoghargasupplier').dialog('close');
            }
        });
    }
}

function export_to_xls(){
    alamatsimpan = pathutama +'dataproduk/exportxls';
    $.ajax({
        type: 'POST',
        url: alamatsimpan,
        cache: false,
        success: function (data) {
            window.open(pathutama + data.trim());
        }
    })
}

function discount_produk(idproduct) {
    $('#dialog-produk-diskon').dialog('open');
    var request = new Object();
    request.idproduct = idproduct;
    var AlamatProduct = pathutama + 'dataproduk/getproductinfo';
    $.ajax({
        type: 'POST',
        url: AlamatProduct,
        data: request,
        cache: false,
        success: function (data) {
            var ReturnData = eval(data);
            var NamaProduct = ReturnData[0].namaproduct;
            $('#product-title').html(NamaProduct);
            $('#harga_asal').val(number_format(ReturnData[0].hargajual, dDigit, dSep, tSep));
            $('#harga_diskon').val(ReturnData[0].hargajual);
            $('#idproduct_diskon').val(idproduct);
            $('#normal_price').val(ReturnData[0].hargajual);
            var AlamatTabelDiskon = pathutama + 'dataproduk/tablediskonproduct';
            $.ajax({
                type: 'POST',
                url: AlamatTabelDiskon,
                data: request,
                cache: false,
                success: function (data) {
                    var DiscountTable = data.trim();
                    $('#table-diskon-wrapper').html(DiscountTable);
                    DiscTable[idproduct] = $('#tabel-' + idproduct).dataTable({
                        'bJQueryUI': true,
                        'bAutoWidth': false,
                        'bInfo': true,
                        'bPaginate': false,
                        'bLengthChange': false,
                        'aoColumns': [
                            null, null, null, null, {'bSortable': false}
                        ],
                        'scrollY': '130px',
                        'scrollCollapse': true,
                        'processing': true,
                        'serverSide': true,
                        'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=detailproductdiscount&idproduct=' + idproduct,
                        'createdRow': function (row, data, index) {
                            $('td', row).eq(0).addClass('center');
                            $('td', row).eq(1).addClass('center');
                            $('td', row).eq(2).addClass('angka');
                            $('td', row).eq(3).addClass('angka');
                            $('td', row).eq(5).addClass('center');
                        },
                        'aaSorting': [[1, 'desc']],
                    });
                }
            });
        }
    });
}

function export_data(){
    var request = new Object();
    request.filetype = $('#file_type').val();
    request.idkategori = $('#kategori-filter').val();
    if (request.filetype == 'csv') {
        alamatsimpan = pathutama + 'dataproduk/exportcsv';
    }else if(request.filetype == 'csv-rongta'){
        alamatsimpan = pathutama + 'dataproduk/exportcsvrongta';
    }else{
        alamatsimpan = pathutama + 'dataproduk/exportxls';
    }
    $.ajax({
        type: 'POST',
        url: alamatsimpan,
        cache: false,
        data: request,
        success: function (data) {
            window.open(pathutama + data.trim());
        }
    })
}

function hapus_diskon(id_discount, idproduct){
    if (id_discount > 0){
        var konfirmasi = confirm('Yakin menghapus diskon??');
        if (konfirmasi){
            var request = new Object();
            request.id = id_discount;
            var AlamatHapus = pathutama + 'dataproduk/hapusdiskon';
            $.ajax({
                type: 'POST',
                url: AlamatHapus,
                cache: false,
                data: request,
                success: function (data) {
                    DiscTable[idproduct].fnDraw();
                }
            })
        }
    }
}

$(document).ready(function() {
    //jsPrintSetup.getPrintersList();
    //jsPrintSetup.setPrinter();
    currSym = Drupal.settings.currSym[0];
    tSep = Drupal.settings.tSep[0];
    dSep = Drupal.settings.dSep[0];
    dDigit = Drupal.settings.dec_digit;

    pathutama = Drupal.settings.basePath;
    pathfile = Drupal.settings.filePath;
    alamatupdate = Drupal.settings.basePath + 'dataproduk/updateproduk';
    statusstok = Drupal.settings.statusstokfilter;
    statusproduct = Drupal.settings.statusproduct;

    ProductSize = Drupal.settings.product_size;


    $('#dialogtambahkategori').dialog({
        modal: true,
        width: 350,
        resizable: false,
        autoOpen: false,
        position: ['auto','auto'],
        open: function(event, ui) {
            $('#kodekategori').focus();
        },
        close: function(){
            $('#tambahkategoriform').validationEngine('hide');
        }
    });
    $('#dialogtambahsubkategori').dialog({
        modal: true,
        width: 350,
        resizable: false,
        autoOpen: false,
        position: ['auto','auto'],
        open: function(event, ui) {
            $('#kodesubkategori').focus();
        },
        close: function(){
            $('#tambahsubkategoriform').validationEngine('hide');
        }
    });
    $('#dialogtambahsupplier').dialog({
        modal: true,
        width: 350,
        resizable: false,
        autoOpen: false,
        position: ['auto','auto'],
        open: function(event, ui) {
            $('#kodesupplier').focus();
        },
        close: function(){
            $('#tambahsupplierform').validationEngine('hide');
        }
    });
    $('#dialogtambahsatuan').dialog({
        modal: true,
        width: 350,
        resizable: false,
        autoOpen: false,
        position: ['auto','auto'],
        open: function(event, ui) {
            $('#namasatuan').select();
        },
        close: function(){
            $('#tambahsatuanform').validationEngine('hide');
        }
    });
    //Init dialog harga supplier
    $('#dialoghargasupplier').dialog({
        modal: false,
        width: 450,
        resizable: false,
        autoOpen: false,
        open : function(event, ui){
            $('.ui-dialog-title').css('font-size','14px');
            $('#dialoghargasupplier .ui-button').css('font-size','12px').css('font-weight','bold');
            $('#dialoghargasupplier').css('padding','.5em .1em');
            var request = new Object();
            request.idproduct = SelectedProduct;
            alamattabelproduk = pathutama +'dataproduk/getsupplierprice';
            $.ajax({
                type: 'POST',
                url: alamattabelproduk,
                data: request,
                cache: false,
                success: function(data){
                    $('#table-harga-wrapper').html(data.trim());
                    $('.input-number:first').select();
                }
            });
        }
    });

    //Init dialog diskon produk
    $('#dialog-produk-diskon').dialog({
        modal: false,
        width: 500,
        resizable: false,
        autoOpen: false,
        open : function(event, ui){
            $('.ui-dialog-title').css('font-size','14px');
            $('#dialog-produk-diskon .ui-button').css('font-size','12px').css('font-weight','bold');
            $('#dialog-produk-diskon').css('padding','.5em .1em');
            $('#harga_diskon').select();
        }
    });

    $('#formsubkategori').validationEngine();
    $('#tambahkategoriform').validationEngine();
    $('#tambahsubkategoriform').validationEngine();
    $('button').button();
    $('#addkategori').click(function(){
        $('#dialogtambahkategori').dialog('open');
        $('.ui-dialog-title').css('font-size','15px');
        $('#formproduk').validationEngine('hide');
        return false;
    });
    $('#addsubkategori').click(function(){
        $('#dialogtambahsubkategori').dialog('open');
        $('#idkategori2').val($('#idkategori').val());
        $('.ui-dialog-title').css('font-size','15px');
        $('#formproduk').validationEngine('hide');
        return false;
    });
    $('#addsupplier').click(function(){
        $('#dialogtambahsupplier').dialog('open');
        $('.ui-dialog-title').css('font-size','15px');
        $('#formproduk').validationEngine('hide');
        return false;
    });
    $('#addsatuan').click(function(){
        $('#dialogtambahsatuan').dialog('open');
        $('.ui-dialog-title').css('font-size','15px');
        $('#formproduk').validationEngine('hide');
        return false;
    });
    $('#tambahprodukbaru').button({
        icons: {
            primary: 'ui-icon-plusthick'
        },
        text: false
    }).click(function(){
        $('#formpenambahanproduk').fadeIn('slow',function(){
            alamatbarcode = pathutama +'penjualan/getrandomstring';
            $.ajax({
                type: 'POST',
                url: alamatbarcode,
                cache: false,
                success: function(data){
                    $('#barcode').val(data.trim());
                    $('#barcode').select();
                    $('#idsupplier').chosen();
                    $('.chosen-container').css('float', 'left');
                    $('.chosen-container').css('margin-right', '4px');
                    $('.chosen-container').css('margin-top', '4px');
                }
            });
        });
    });
    $('#refreshprodukbaru').button({
        icons: {
            primary: 'ui-icon-refresh'
        },
        text: false
    }).click(function(){
        $('#tempattabel').html('Memuat Data Produk...<img src = "'+ pathutama +'misc/media/images/loading.gif">');
        var request = new Object();
        request.asal = 'produk';
        alamattabelproduk = pathutama +'dataproduk/isitableproduk';
        $.ajax({
            type: 'POST',
            url: alamattabelproduk,
            data: request,
            cache: false,
            success: function(data){
                $('#tempattabel').fadeOut('fast',function(){
                    $('#tempattabel').html(data);
                    tampilkantabelproduk();
                    $('#tempattabel').fadeIn('fast');
                });
            }
        });
    });
    $('#kodekategori, #kodesubkategori, #kodesupplier, #namasatuan').autotab_filter({
        format: 'alphanumeric',
        uppercase: true,
        nospace: true
    });
    $('#hargapokok,#hargajual,#margin,#minstok,#maxstok,#stok').autotab_filter({
        format: 'custom',
        nospace: true,
        pattern: '[^0-9\.]',
    });
    //$('#main h2').css('width','100%');
    //$('#main h2').css('float','left');
    $('#barcode').keypress(function(e) {
        if(e.keyCode == 13) {
            $('#formproduk').validationEngine('validateField', '#barcode');
            ubahkodebarang();
            $('#idkategori').focus();
        }
    });
    $('#formproduk').validationEngine();
    $("#uploadify").uploadify({
        "uploader"       : pathutama + "sites/all/libraries/uploadify/uploadify.swf",
        "script"         : pathutama + "sites/all/libraries/uploadify/uploadxls.php",
        "cancelImg"      : pathutama + "sites/all/libraries/uploadify/images/cancel.png",
        "folder"         : pathfile,
        "fileExt"     	 : "*.jpg;*.gif;*.png",
        "fileDesc"    	 : "Image Files",
        "multi"          : false,
        "onComplete"  : function(event, ID, fileObj, response, data) {
            simpanproduk(fileObj.name);
        },
        "onError" : function (event,ID,fileObj,errorObj) {
            console.log(fileObj.name);
        }
    });
    tampilkantabelproduk();
    /*$('#print-barcode-biasa').on('click', function(){
        var selected_product = '';
        var counterData = 0;
        $('.barcode-select').each(function(){
            if ($(this).is(':checked')){
                if (counterData > 0){
                    selected_product += '__'+ $(this).val();
                }else{
                    selected_product = $(this).val();
                }
                counterData++;
            }
            if (selected_product.length > 1900){
                return false;
            }
        });
        if (selected_product != ''){
            window.open(pathutama + 'print/6?idproduct='+ selected_product);
        }
    });
    $('#print-barcode-logo').on('click', function(){
        var selected_product = '';
        var counterData = 0;
        $('.barcode-select').each(function(){
            if ($(this).is(':checked')){
                if (counterData > 0){
                    selected_product += '__'+ $(this).val();
                }else{
                    selected_product = $(this).val();
                }
                counterData++;
            }
            if (selected_product.length > 1900){
                return false;
            }
        });
        if (selected_product != ''){
            window.open(pathutama + 'print/6?idproductlogo='+ selected_product);
        }
    });*/
    $('#nonaktifkan').on('click', function(){
        var confirmation = confirm('Yakin ingin menonaktifkan produk yang dipilih...??!')
        if (confirmation){
            var selected_product = new Array;
            var counterData = 0;
            $('.barcode-select').each(function(){
                if ($(this).is(':checked')){
                    selected_product.push($(this).val());
                    counterData++;
                }
            });
            if (counterData > 0){
                var request = new Object();
                request.selectedproduct = selected_product;
                request.statusproduct = 0;
                alamatupdatestatus = pathutama +'dataproduk/ubahstatusproduct';
                $.ajax({
                    type: 'POST',
                    url: alamatupdatestatus,
                    data: request,
                    cache: false,
                    success: function(data){
                        oTable.draw(false);
                        alert('Produk berhasil dinonaktifkan');
                    }
                });
            }else{
                alert('Mohon pilih produk terlebih dulu...!!?');
            }
        }
    });

    $('#aktifkan').on('click', function(){
        var confirmation = confirm('Yakin ingin mengaktifkan produk yang dipilih...??!')
        if (confirmation){
            var selected_product = new Array;
            var counterData = 0;
            $('.barcode-select').each(function(){
                if ($(this).is(':checked')){
                    selected_product.push($(this).val());
                    counterData++;
                }
            });
            if (counterData > 0){
                var request = new Object();
                request.selectedproduct = selected_product;
                request.statusproduct = 1;
                alamatupdatestatus = pathutama +'dataproduk/ubahstatusproduct';
                $.ajax({
                    type: 'POST',
                    url: alamatupdatestatus,
                    data: request,
                    cache: false,
                    success: function(data){
                        oTable.draw(false);
                        alert('Produk berhasil diaktifkan');
                    }
                });
            }else{
                alert('Mohon pilih produk terlebih dulu...!!?');
            }
        }
    });

    $('#print-price-label').on('click', function(){
        var selected_product = '';
        var counterData = 0;
        $('.barcode-select').each(function(){
            if ($(this).is(':checked')){
                var strID = $(this).val();
                if (counterData > 0){
                    selected_product += '||'+ $(this).val() +'__'+ $('#print-'+ strID).val();
                }else{
                    selected_product = $(this).val() +'__'+ $('#print-'+ strID).val();
                }
                counterData++;
            }

        });
        if (selected_product != ''){
            $('#selected-product-print').val(selected_product);
            $('#sticker-type').val(2);
            $('#sticker-size').val($('#price-label').val());
            $('#form-print').submit();
        }
    });
    $('#print-barcode').on('click', function(){
        var selected_product = '';
        var counterData = 0;

        $('.barcode-select').each(function(){
            if ($(this).is(':checked')){
                var strID = $(this).val();
                if (counterData > 0){
                    if (Drupal.settings.product_size){
                        selected_product += '||'+ $(this).val() +'__'+ $('#print-'+ strID).val() +'__'+ $('#saiz-selection-'+ strID).val();
                    }else{
                        selected_product += '||'+ $(this).val() +'__'+ $('#print-'+ strID).val();
                    }
                }else{
                    if (Drupal.settings.product_size){
                        selected_product = $(this).val() +'__'+ $('#print-'+ strID).val() +'__'+ $('#saiz-selection-'+ strID).val();
                    }else{
                        selected_product = $(this).val() +'__'+ $('#print-'+ strID).val();
                    }
                }
                counterData++;
            }

        });
        if (selected_product != ''){
            $('#selected-product-print').val(selected_product);
            $('#sticker-type').val(1);
            $('#sticker-size').val($('#barcode-select').val());
            $('#form-print').submit();
        }
    });
    $('#print-barcode-50x15').on('click', function(){
        var selected_product = '';
        var counterData = 0;
        $('.barcode-select').each(function(){
            if ($(this).is(':checked')){
                var strID = $(this).val();
                if (counterData > 0){
                    selected_product += '||'+ $(this).val() +'__'+ $('#print-'+ strID).val();
                }else{
                    selected_product = $(this).val() +'__'+ $('#print-'+ strID).val();
                }
                counterData++;
            }

        });
        if (selected_product != ''){
            $('#selected-product-print').val(selected_product);
            $('#sticker-type').val(1);
            $('#form-print').submit();
        }
    });

    $('#print-barcode-tanpa-harga').on('click', function(){
        var selected_product = '';
        var counterData = 0;
        $('.barcode-select').each(function(){
            if ($(this).is(':checked')){
                var strID = $(this).val();
                if (counterData > 0){
                    selected_product += '||'+ $(this).val() +'__'+ $('#print-'+ strID).val();
                }else{
                    selected_product = $(this).val() +'__'+ $('#print-'+ strID).val();
                }
                counterData++;
            }

        });
        if (selected_product != ''){
            $('#selected-product-print').val(selected_product);
            $('#sticker-type').val(0);
            $('#form-print').submit();
        }
    });
    $('#katdiv').append($('#kategori-filter'));
    $('#katstock').append($('#date_from_view'));
    $('#katstock').append($('#date_thru_view'));
    $('#katstock').append($('#print-stock-card'));
    //$('#katstock').append($('#print-stock-carddate_from_view'));
    $('#kategori-filter').on('change', function(){
        if ($('#idpelanggan').val()){
            window.location = pathutama + 'dataproduk/produk?statusstok=' + Drupal.settings.statusproduct +'&server='+ $('#idpelanggan').val() +'&idkategori='+ $(this).val();
        }else{
            window.location = pathutama + 'dataproduk/produk?statusstok=' + Drupal.settings.statusproduct +'&idkategori='+ $(this).val();
        }
    });
    $('#rad-hargapokok').on('click', function () {
        if ($('#rad-hargapokok').is(':checked')) {
            $('#hargapokok').attr('readonly', 'readonly');
            $('#hargajual').removeAttr('readonly');
            $('#margin').removeAttr('readonly');
        }
    });
    $('#rad-hargajual').on('click', function () {
        if ($('#rad-hargajual').is(':checked')) {
            $('#hargajual').attr('readonly', 'readonly');
            $('#hargapokok').removeAttr('readonly');
            $('#margin').removeAttr('readonly');
        }
    });
    $('#rad-margin').on('click', function () {
        if ($('#rad-margin').is(':checked')) {
            $('#margin').attr('readonly', 'readonly');
            $('#hargapokok').removeAttr('readonly');
            $('#hargajual').removeAttr('readonly');
        }
    });
    $('#close-diskon').on('click', function(){
        $('#dialog-produk-diskon').dialog('close');
    });
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
    $('#dialog-produk-diskon button').on('click', function(e){
       e.preventDefault();
    });
    $('#simpan-diskon').on('click', function(){
        var request = new Object();
        request.idproduct = $('#idproduct_diskon').val();
        request.normal_price = $('#normal_price').val();
        request.price = $('#harga_diskon').val();
        request.date_from = $('#tgl1').val();
        request.date_thru = $('#tgl2').val();
        var AlamatSimpanDiskon = pathutama +'dataproduk/simpandiskonproduct';
        $.ajax({
            type: 'POST',
            url: AlamatSimpanDiskon,
            data: request,
            cache: false,
            success: function(data){
                DiscTable[request.idproduct].fnDraw();
            }
        });
    });
    $('#print-stock-card').on('click', function(){
        var selected_product = '';
        var counterData = 0;
        $('.barcode-select').each(function(){
            if ($(this).is(':checked')){
                var strID = $(this).val();
                selected_product += '<option value="'+ strID +'" selected>'+ strID +'</option>';
                counterData++;
            }
        });
        if (selected_product != ''){
            $('#idproduct-stockcard').empty();
            $('#idproduct-stockcard').append(selected_product);
            $('#stock-card-form').submit();
        }
    });
    $('#date_from_view').datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: 'yy-mm-dd',
        altField: '#date_from'
    });
    $('#date_thru_view').datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: 'yy-mm-dd',
        altField: '#date_thru'
    });
})