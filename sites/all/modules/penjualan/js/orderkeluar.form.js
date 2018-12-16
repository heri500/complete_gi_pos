$(document).ready(function() {
    $('#no-nota').focus();
    $('#no-nota').autocomplete({
        source: Drupal.settings.basePath + 'penjualan/carinotaorder',
        minLength: 2,
        select: function( event, ui ) {
            window.location = Drupal.settings.basePath + 'penjualan/kasir/'+ ui.item.id +'/orderkeluar';
        }
    });
    $("#no-nota").keydown(function(event){
        if(event.keyCode == 13) {
            if($("#no-nota").val().length == 0) {
                event.preventDefault();
                return false;
            }else{
                var request = new Object();
                request.term = $(this).val();
                alamat = Drupal.settings.basePath + 'penjualan/carinotaorder';
                $.ajax({
                    type: 'GET',
                    url: alamat,
                    data: request,
                    cache: false,
                    success: function(data){
                        var returnVal = eval(data);
                        if (parseInt(returnVal[0].id) > 0){
                            window.location = Drupal.settings.basePath + 'penjualan/kasir/'+ returnVal[0].id +'/orderkeluar';
                        }
                    }
                });
            }
        }
    });
})