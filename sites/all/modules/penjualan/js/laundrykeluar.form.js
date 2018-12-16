$(document).ready(function() {
    $('#no-nota').focus();
    $('#no-nota').autocomplete({
        source: Drupal.settings.basePath + 'penjualan/carinotalaundry',
        minLength: 2,
        select: function( event, ui ) {
            window.location = Drupal.settings.basePath + 'penjualan/kasir/'+ ui.item.id +'/laundrykeluar';
        }
    });
})