var oTable;
var pathutama = '';
var pathfile = '';
function tampilkantabelmeja(){
    oTable = $('#tabel_meja').dataTable( {
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
$(document).ready(function() {
    TableToolsInit.sSwfPath = pathutama +'misc/media/datatables/swf/ZeroClipboard.swf';
    pathutama = Drupal.settings.basePath;
    pathfile = Drupal.settings.filePath;
    alamatupdate = pathutama + 'dataproduk/updatemeja';
    $('#tabel_meja tbody .editable').editable(alamatupdate, {
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
    tampilkantabelmeja();
    $('#formmeja').validationEngine();
    $('button').button();
    $('#kodemeja').autotab_filter({
        format: 'alphanumeric',
        uppercase: true,
        nospace: true
    });
    $('#formplace').css('width','950px');
    $('#tambahmeja').css('margin-top','10px');
    $('#genbarcodemeja').click(function(e){
        e.preventDefault();
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
    });
    $('.space').css('width','100%').prepend('<div style="float: left"><button id="print-barcode">Print Barcode Meja</button></div>')
    $('#print-barcode').button();
    $('#print-barcode').click(function(){
        var selected_meja = '';
        var counterData = 0;
        $('.selected-meja').each(function(){
            if ($(this).is(':checked')){
                if (counterData > 0){
                    selected_meja += '__'+ $(this).val();
                }else{
                    selected_meja = $(this).val();
                }
                counterData++;
            }
            if (selected_meja.length > 1900){
                return false;
            }
        });
        if (selected_meja != ''){
            window.open(pathutama + 'print/6?idmeja='+ selected_meja);
        }
    });
})