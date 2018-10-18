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

(function(){

    /**
     * Decimal adjustment of a number.
     *
     * @param   {String}    type    The type of adjustment.
     * @param   {Number}    value   The number.
     * @param   {Integer}   exp     The exponent (the 10 logarithm of the adjustment base).
     * @returns {Number}            The adjusted value.
     */
    function decimalAdjust(type, value, exp) {
        // If the exp is undefined or zero...
        if (typeof exp === 'undefined' || +exp === 0) {
            return Math[type](value);
        }
        value = +value;
        exp = +exp;
        // If the value is not a number or the exp is not an integer...
        if (isNaN(value) || !(typeof exp === 'number' && exp % 1 === 0)) {
            return NaN;
        }
        // Shift
        value = value.toString().split('e');
        value = Math[type](+(value[0] + 'e' + (value[1] ? (+value[1] - exp) : -exp)));
        // Shift back
        value = value.toString().split('e');
        return +(value[0] + 'e' + (value[1] ? (+value[1] + exp) : exp));
    }

    // Decimal round
    if (!Math.round10) {
        Math.round10 = function(value, exp) {
            return decimalAdjust('round', value, exp);
        };
    }
    // Decimal floor
    if (!Math.floor10) {
        Math.floor10 = function(value, exp) {
            return decimalAdjust('floor', value, exp);
        };
    }
    // Decimal ceil
    if (!Math.ceil10) {
        Math.ceil10 = function(value, exp) {
            return decimalAdjust('ceil', value, exp);
        };
    }
})();

function tampilkantabelproduk(){
    oTable = $('#tabel_produk').DataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'sPaginationType': 'full_numbers',
        'bInfo': true,
        'processing': true,
        'serverSide': true,
        'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=produk&statusstok='+ statusstok +'&status_product='+ statusproduct  +'&idkategori='+ Drupal.settings.idkategori,
        'aoColumns': [
            { 'bSortable': false },null,null,null,null,null,null,null,
            null,{ 'bVisible': false },{ 'bVisible': false },
            { 'bVisible': false },null,null,null,{ 'bVisible': false },null,null,{ 'bSortable': false },
            { 'bSortable': false },{ 'bSortable': false }
        ],
        'aLengthMenu': [[100, 200, 300, 500], [100, 200, 300,500]],
        'iDisplayLength': 100,
        'aaSorting': [[1, 'asc']],
        buttons: [
            {
                extend: 'colvis',
                columns: ':not(:first-child)'
            }, 'copy', 'excel', 'print'
        ],
        'sDom': '<"button-div"B><"H"l<"#katdiv">fr>t<"F"ip>',
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
        if ($('#margin').val() > 0 && $('#hargapokok').val() > 0){
            var hargajual = $('#hargapokok').val() * (100/(100-$('#margin').val()));
            $('#hargajual').val(Math.Math.round10(hargajual,(dDigit * -1)));
        }
    }else if (asal == 'hargajual'){
        if ($('#hargapokok').val() > 0 && $('#hargajual').val() > 0){
            var margin = (($('#hargajual').val() - $('#hargapokok').val())/$('#hargajual').val())*100;
            $('#margin').val(Math.round10(margin,(dDigit * -1)));
        }
    }else if (asal == 'margin'){
        if ($('#hargapokok').val() > 0 && $('#margin').val() > 0){
            var hargajual = $('#hargapokok').val() * (100/(100-$('#margin').val()));
            $('#hargajual').val(Math.round10(hargajual,(dDigit * -1)));
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

function export_data(){
    var request = new Object();
    request.filetype = $('#file_type').val();
    request.idkategori = $('#kategori-filter').val();
    if (request.filetype == 'csv') {
        alamatsimpan = pathutama + 'dataproduk/exportcsv';
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
    $('#kategori-filter').on('change', function(){
        if ($('#idpelanggan').val()){
            window.location = pathutama + 'dataproduk/produk?statusstok=' + Drupal.settings.statusproduct +'&server='+ $('#idpelanggan').val() +'&idkategori='+ $(this).val();
        }else{
            window.location = pathutama + 'dataproduk/produk?statusstok=' + Drupal.settings.statusproduct +'&idkategori='+ $(this).val();
        }
    });
})