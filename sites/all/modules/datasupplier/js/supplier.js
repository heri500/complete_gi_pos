var oTable;
var oTable2;
var oTable3;
var oTable4;
var pathutama = '';
var pathfile = '';
function tampilkantabelsupplier(){
    oTable = $('#tabel_supplier').dataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'sPaginationType': 'full_numbers',
        'bInfo': false,
        'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
        'iDisplayLength': 100,
        'aaSorting': [[0, 'asc']],
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
        'aaSorting': [[0, 'asc']],
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
        'aaSorting': [[0, 'asc']],
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
    if (nilaihutang > 0){
        var tampilhutang = 'HUTANG KE SUPPLIER '+ namasupplier +' SAAT INI : '+ Drupal.settings.currSym +' '+ besarhutang;
        $('#nilaipembayaran').val(parseInt(nilaihutang));
        $('#tothutang').val(parseInt(nilaihutang));
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
        'submit': 'Ok',
        'cancel': 'Batal',
        'indicator': 'Menyimpan...',
        'tooltip': 'Klik untuk mengubah...'
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
        width: 450,
        resizable: false,
        autoOpen: false,
        position: ['auto','auto'],
        open: function(event, ui) {
            $('#nilaipembayaran').focus();
            $('#nilaipembayaran').select();
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
})