var oTable;
var oTable2;
var oTable3;
var oTable4;
var pathutama = '';
var pathfile = '';
var SelectedSupplier = 0;
var GroupInvoice = new Array;
function tampilkantabelsupplier(){
    oTable = $('#tabel_supplier').dataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'sPaginationType': 'full_numbers',
        'bInfo': true,
        'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
        'iDisplayLength': 100,
        'aaSorting': [[2, 'asc']],
        'sDom': '<"space"T><"clear"><"H"lfr>t<"F"ip>'
});
}
function tampiltabelhutangdetail(){
    oTable2 = $('#tabel_detail_hutang').dataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'bPaginate': false,
        'bLengthChange': false,
        'bFilter': false,
        'bInfo': false,
        'aaSorting': [[1, 'asc']],
        'sDom': '<"H"<"toolbar">fr>t<"F"ip>'
});
}
function view_detail_hutang(idsupplier,namasupplier,besarhutang){
    var tampilhutang = 'HUTANG KE SUPPLIER SAAT INI : Rp. '+ besarhutang;
    $('#tempatnilaihutang').html(tampilhutang);
    var request = new Object();
    request.idsupplier = idsupplier;
    alamat = pathutama + 'datasupplier/detailhutang';
    $.ajax({
        type: 'POST',
        url: alamat,
        data: request,
        cache: false,
        success: function(data){
            $('#detailpiutang').html(data);
            tampiltabelhutangdetail();
            $('div.toolbar').html('SUPPLIER : '+ namasupplier);
            alamat = pathutama + 'datasupplier/detailpembayaran';
            $.ajax({
                type: 'POST',
                url: alamat,
                data: request,
                cache: false,
                success: function(data2){
                    $('#detailpembayaran').html(data2);
                    tampiltabelpembayaran();
                    $('div.toolbar2').html('PEMBAYARAN');
                    $('#dialogdetailhutang').dialog('open');
                }
            });
        }
    });
}
function tampiltabelbelidetail(){
    oTable3 = $('#tabel_detail_pembelian').dataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'bPaginate': false,
        'bLengthChange': false,
        'bInfo': false,
        'aaSorting': [[0, 'asc']],
        'sDom': '<"H"<"toolbar">fr>t<"F"ip>'
});
}
function tampiltabelpembayaran(){
    oTable4 = $('#history_pembayaran').dataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'bPaginate': false,
        'bLengthChange': false,
        'bInfo': false,
        'bFilter': false,
        'aaSorting': [[1, 'asc']],
        'sDom': '<"H"<"toolbar">fr>t<"F"ip>'
});
}
function view_detail(idpembelian,nonota){
    var request = new Object();
    request.idpembelian = idpembelian;
    alamat = pathutama + 'pembelian/detailpembelian';
    $.ajax({
        type: 'POST',
        url: alamat,
        data: request,
        cache: false,
        success: function(data){
            $('#dialogdetail').html(data);
            tampiltabelbelidetail();
            $('div.toolbar').html('No. Nota : '+ nonota);
            $('#dialogdetail').dialog('open');
        }
    });
}
function pembayaran(idsupplier,namasupplier,besarhutang,nilaihutang){
    SelectedSupplier = idsupplier;
    if (nilaihutang > 0){
        var tampilhutang = 'HUTANG KE SUPPLIER '+ namasupplier +' SAAT INI : '+ Drupal.settings.currSym +' '+ besarhutang;
        $('#nilaipembayaran').val(parseFloat(nilaihutang));
        $('#tothutang').val(parseFloat(nilaihutang));
        $('#idsupplierbayar').val(idsupplier);
        $('#totalhutangpelanggan').html(tampilhutang);
        $('#dialogpembayaran').dialog('open');
    }else{
        alert('Tidak ada hutang ke supplier '+ namasupplier);
    }
}
function do_pembayaran(){
    /*idsupplier, nilaisebelumbayar, pembayaran, uid, tglbayar*/
    var request = new Object();
    request.idsupplier = $('#idsupplierbayar').val();
    request.hutang = $('#tothutang').val();
    request.pembayaran = $('#nilaipembayaran').val();
    request.tglbayar = $('#tglbayarkirim').val();
    request.related_nota = $('#related_nota').val();
    request.keterangan = $('#keterangan').val();
    request.discount_persen = $('#discount_persen').val();
    request.discount_value = $('#discount_value').val();
    alamat = pathutama + 'datasupplier/pembayaran';
    $.ajax({
        type: 'POST',
        url: alamat,
        data: request,
        cache: false,
        success: function(){
            window.location = pathutama + 'datasupplier/supplier';
        }
    });
}
function delete_pembayaran(idpembayaran, idsupplier){
    if (idpembayaran){
        var konfirmasi =  confirm('Apakah pembaaran ini benar benar ingin dihapus...??!!');
        if (konfirmasi){
            window.location = pathutama + "datasupplier/deletepembayaran?idpembayaran="+ idpembayaran +'&idsupplier='+ idsupplier;
        }
    }
}
$(document).ready(function() {
    pathutama = Drupal.settings.basePath;
    pathfile = Drupal.settings.basePath + Drupal.settings.filePath;
    TableToolsInit.sSwfPath = pathutama +'misc/media/datatables/swf/ZeroClipboard.swf';
    alamatupdate = pathutama + 'datasupplier/updatesupplier';
    $('#tabel_supplier tbody .editable').editable(alamatupdate, {
        'callback': function( sValue, y ) {
            var aPos = oTable.fnGetPosition( this );
            oTable.fnUpdate( sValue, aPos[0], aPos[1] );
        },
        'submitdata': function ( value, settings ) {
            var aPos = oTable.fnGetPosition( this );
            return { 'row_id': this.parentNode.getAttribute('id'), 'kol_id': aPos[1] };
        },
        'height': '20px',
        'width': '100px',
        'submit': 'Ok',
        'cancel': 'Batal',
        'indicator': 'Menyimpan...',
        'tooltip': 'Klik untuk mengubah...'
    });
    $("#tabel_supplier tbody .editable2").editable(alamatupdate, {
        "callback": function( sValue, y ) {
            var aPos = oTable.fnGetPosition( this );
            oTable.fnUpdate( sValue, aPos[0], aPos[1] );
        },
        "submitdata": function ( value, settings ) {
            var aPos = oTable.fnGetPosition( this );
            return { "row_id": this.parentNode.getAttribute("id"), "kol_id": aPos[1] };
        },
        "data"  : Drupal.settings.status_options,
        "type"  : "select",
        "height": "20px",
        "submit": "Ok",
        "indicator": "Menyimpan...",
        "tooltip": "Klik untuk mengubah..."
    });
    $("#tabel_supplier tbody .editable3").editable(alamatupdate, {
        "callback": function( sValue, y ) {
            var aPos = oTable.fnGetPosition( this );
            oTable.fnUpdate( sValue, aPos[0], aPos[1] );
        },
        "submitdata": function ( value, settings ) {
            var aPos = oTable.fnGetPosition( this );
            return { "row_id": this.parentNode.getAttribute("id"), "kol_id": aPos[1] };
        },
        "data"  : Drupal.settings.payment_type,
        "type"  : "select",
        "height": "20px",
        "submit": "Ok",
        "indicator": "Menyimpan...",
        "tooltip": "Klik untuk mengubah..."
    });
    tampilkantabelsupplier();
    $('#formsupplier').validationEngine();
    $('button').button();
    $('#kodesupplier').autotab_filter({
        format: 'alphanumeric',
        uppercase: true,
        nospace: true
    });
    $('#dialogdetailhutang').dialog({
        modal: true,
        width: 1100,
        resizable: false,
        autoOpen: false,
        position: ['auto','auto']
    });
    $('#dialogdetail').dialog({
        modal: true,
        width: 850,
        resizable: false,
        autoOpen: false,
        position: ['auto','auto']
    });
    $('#dialogpembayaran').dialog({
        modal: true,
        width: 550,
        resizable: true,
        autoOpen: false,
        position: ['auto','auto'],
        open: function(event, ui) {
            $('#nilaipembayaran').focus();
            $('#nilaipembayaran').select();
            var request = new Object();
            request.idsupplier = SelectedSupplier;
            alamat = pathutama + 'pembelian/getsupplierrelatednota';
            $.ajax({
                type: 'POST',
                url: alamat,
                data: request,
                cache: false,
                success: function(data){
                    try {
                        var RetData = eval(data);
                        GroupInvoice = [];
                        if (RetData.length > 0) {
                            $('#related_nota').empty();
                            for (var i = 0; i < RetData.length; i++) {
                                if (RetData[i].no_invoice == null) {
                                    RetData[i].no_invoice = '';
                                }
                                if (RetData[i].payment_paid == null) {
                                    RetData[i].payment_paid = 0;
                                }
                                var OptionAdd = '<option value="' + RetData[i].idpembelian + '">' + RetData[i].nonota + ' - ';
                                OptionAdd += RetData[i].no_invoice + ' Total : ' + RetData[i].total + ' Paid : ';
                                OptionAdd += RetData[i].payment_paid + ' </option>';
                                $('#related_nota').append(OptionAdd);
                                GroupInvoice[RetData[i].idpembelian] = parseFloat(RetData[i].total) - parseFloat(RetData[i].payment_paid);
                            }
                            $('#related_nota').trigger('chosen:updated');
                        }
                    } catch(err) {
                        alert(err.message);
                    }
                }
            });
        }
    });
    $('#tglbayar').datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: 'dd-mm-yy',
        altField: '#tglbayarkirim',
        altFormat: 'yy-mm-dd'
    });
    $('#nilaipembayaran').keypress(function(e) {
        if (e.keyCode == 13){
            $('#bayarhutang').click();
        }
    });
    $('#sync-hutang-supplier').click(function(){
        var konfirmasi = confirm('Yakin ingin sync data hutang ke supplier...??!');
        if (konfirmasi) {
            var alamat = pathutama + 'datasupplier/synchutang';
            $.ajax({
                type: 'POST',
                url: alamat,
                cache: false,
                success: function () {
                    alert('Sync Data Hutang Ke Supplier Berhasil...!');
                    window.location = pathutama + 'datasupplier/supplier';
                }
            });
        }
    });
    $('#related_nota').chosen({ width : '350px', search_contains : true }).change(function(){
        var SelectedNota = $(this).val();
        var TotalPayment = 0;
        if (SelectedNota.length > 0){
            for (var i = 0;i < SelectedNota.length;i++){
                TotalPayment = TotalPayment + parseFloat(GroupInvoice[SelectedNota[i]]);
            }
        }
        $('#nilaipembayaran').val(number_format(TotalPayment, Drupal.settings.dec_digit,'.',''));
        $('#discount_persen').keyup();
    });
    $('#discount_persen').on('keyup', function(){
        var DiscountValue = 0;
        if ($(this).val() != ''){
            DiscountValue = $(this).val();
        }
        var Pembayaran = parseFloat($('#nilaipembayaran').val());
        var NilaiDiskon = Pembayaran * (parseInt(DiscountValue)/100);
        var PembayarafAfterDiskon = Pembayaran - NilaiDiskon;
        $('#discount_value').val(number_format(NilaiDiskon,Drupal.settings.dec_digit,'.',''));
        $('#value_after_discount').val(number_format(PembayarafAfterDiskon,Drupal.settings.dec_digit,'.',''));
    });
})