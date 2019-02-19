var pathutama = '';

function reset_android(){
    var Konfirmasi = confirm('Yakin ingin mereset monitoring Android Order??');
    if (Konfirmasi){
        alamat = pathutama + 'penjualan/resettablecoopen';
        $.ajax({
            type: 'POST',
            url: alamat,
            cache: false,
            success: function (data) {
                alert('Android Monitoring Berhasil Di Reset, Silahkan Tutup Browser Anda Dan Buka Kembali Aplikasi...!!!!');
            }
        });
    }
}

$(document).ready(function() {
    pathutama = Drupal.settings.basePath;
    alamat = pathutama + 'penjualan/gettablecoopen';
    $.ajax({
        type: 'POST',
        url: alamat,
        cache: false,
        success: function (data) {
            var totalOpen = parseInt(data);
            if (totalOpen == 1){
                var timeOutId = 0;
                var ajaxFn = function () {
                    alamat = pathutama + 'penjualan/getnewandroidorderandroid';
                    $.ajax({
                        type: 'POST',
                        url: alamat,
                        cache: false,
                        success: function (data) {
                            var idOrder = parseInt(data.trim());
                            if (idOrder > 0) {
                                $('select[name=tabel_customerorder_length]').val(200);
                                $('select[name=tabel_customerorder_length]').trigger('change');
                                //window.open(pathutama + 'print/6?idghorder='+ idOrder);
                                window.open(pathutama + "print/6?idghorderonly=" + idOrder +'&totalprint=1&print_category=1');
                            }
                            alamatcek = pathutama + 'penjualan/gettablecoopen';
                            $.ajax({
                                type: 'POST',
                                url: alamatcek,
                                cache: false,
                                success: function (data) {
                                    totalOpen = parseInt(data);
                                    $('#total-open').html(totalOpen);
                                    timeOutId = setTimeout(ajaxFn, 7000);
                                }
                            });
                        }
                    });
                }
                ajaxFn();
                $('#status-android').attr('src',pathutama + 'misc/media/images/on-icon.png');
                //$('#total-open').html(totalOpen);
            }else{
                $('#status-android').attr('src',pathutama + 'misc/media/images/off-icon.png');
                $('#total-open').html(totalOpen);
            }
        }
    });
    window.onunload = function(){
        alamat = pathutama + 'penjualan/updatetableco?count=-1';
        $.ajax({
            type: 'GET',
            url: alamat,
            cache: false,
            success: function (data) {

            }
        });
    }
    window.onbeforeunload = function(){
        alamat = pathutama + 'penjualan/updatetableco?count=-1';
        $.ajax({
            type: 'GET',
            url: alamat,
            cache: false,
            success: function (data) {

            }
        });
    }
});