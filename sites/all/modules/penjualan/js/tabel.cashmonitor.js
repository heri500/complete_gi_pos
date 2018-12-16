var oTable;
var currSym = '';
var tSep = '.';
var dSep = ',';
var dDigit = 0;

function TabelCashMonitor(){
    oTable = $('#tabel_cash_monitor').dataTable( {
        'bJQueryUI': true,
        'bAutoWidth': false,
        'sPaginationType': 'full_numbers',
        'bInfo': true,
        'aLengthMenu': [[100, 200, 300, -1], [100, 200, 300, 'All']],
        'iDisplayLength': 100,
        'aaSorting': [[urutan, 'desc']],
        'processing': true,
        'serverSide': true,
        'aoColumnDefs': [
            { 'bSortable': false, 'aTargets': [ 11 ] }
        ],
        'ajax': Drupal.settings.basePath + 'sites/all/modules/datapelanggan/server_processing.php?request_data=monitorcash',
        buttons: [
            'copy', 'excel', 'print'
        ],
        'sDom': '<"button-div"B><"H"lfr>t<"F"ip>',
        'createdRow': function ( row, data, index ) {
            row.id = data[(data.length - 1)];
            $('td', row).eq(1).addClass('center');
            $('td', row).eq(2).addClass('center');
            $('td', row).eq(3).addClass('center');
            $('td', row).eq(4).addClass('angka');
            $('td', row).eq(5).addClass('angka');
            $('td', row).eq(6).addClass('angka');
            $('td', row).eq(7).addClass('angka');
            $('td', row).eq(8).addClass('angka');
            $('td', row).eq(9).addClass('angka');
            $('td', row).eq(10).addClass('angka');
            $('td', row).eq(11).addClass('angka');
            $('td', row).eq(12).addClass('center');
        },
    });
}

function hapus_pettycash(idpettycash){
    if (idpettycash > 0){
        var konfirmasi = confirm('Yakin menghapus petty cash ini...??!');
        if (konfirmasi){
            var request = new Object();
            request.idpc = idpettycash;
            alamat = pathutama + 'penjualan/deletepettycash';
            $.ajax({
                type: 'POST',
                url: alamat,
                data: request,
                cache: false,
                success: function (data) {
                    var returnData = eval(data);
                    alert(returnData[0].message);
                    oTable.fnDraw();
                }
            });
        }
    }
}

function print_log(IdPettyCash){
    if (IdPettyCash > 0){
        window.open(pathutama + 'print/6?close_cashier=1&id_petty_cash='+ IdPettyCash);
    }
}

$(document).ready(function() {
    pathutama = Drupal.settings.basePath;
    urutan = Drupal.settings.urutan;
    currSym = Drupal.settings.currSym;
    tSep = Drupal.settings.tSep[0];
    dSep = Drupal.settings.dSep[0];
    dDigit = Drupal.settings.dDigit;
    TabelCashMonitor();
    setInterval( function () {
        oTable.fnDraw();
    }, 30000 );
})