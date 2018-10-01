var oTable;
var qtyInput = new Array;
function tampilkantabelproduction(){
    oTable = $("#tabel_finished_order").DataTable( {
        'bJQueryUI': true,
        'bPaginate': false,
        'bLengthChange': false,
        'bFilter': true,
        'bInfo': true,
        'sScrollY': "400px",
        'aoColumns': [
            { "bSortable": false },null,null,null,null,null,null,null,null,null
        ],
        'bAutoWidth': false,
        'scrollCollapse': true,
        'createdRow': function ( row, data, index ) {
            row.id = data[(data.length - 1)];
            $('td', row).eq(1).addClass('center');
            $('td', row).eq(2).addClass('center');
            $('td', row).eq(3).addClass('center');
            $('td', row).eq(4).addClass('left');
            $('td', row).eq(5).addClass('angka');
            $('td', row).eq(6).addClass('angka');
            $('td', row).eq(7).addClass('angka');
            $('td', row).eq(8).addClass('angka');
            $('td', row).eq(9).addClass('center');
        }
    });
}
function hapus_produk(noNota){
    var confirmation = confirm('Apakah yakin ingin menghapus input produksi ini ...?!!');
    if (confirmation){
        var baris = $("#"+ noNota).get(0);
        var posisibaris = oTable.row(baris).index();
        oTable.row(posisibaris).remove();
        oTable.draw();
        qtyInput[noNota] = 0;
        $('#detailbarcode').select();
    }
}
$(document).ready(function() {
    $('#detailbarcode').focus();
    tampilkantabelproduction();
    $('#detailbarcode').autocomplete({
        source: Drupal.settings.basePath + 'penjualan/carinotaproduksi',
        minLength: 2,
        select: function( event, ui ) {
            var returnVal = ui;
            var idrow = returnVal[0].id;
            var icondelete = '<img onclick="hapus_produk(\''+ returnVal.detailbarcode +'\')" title="Klik untuk menghapus" src="'+ Drupal.settings.basePath +'misc/media/images/close.ico" width="24">';
            var newRow = new Array;
            newRow.push(icondelete);
            newRow.push(returnVal.nonota);
            newRow.push(returnVal.tglorder);
            newRow.push(returnVal.namapelanggan);
            newRow.push(returnVal.namaproduct);
            newRow.push(returnVal.jumlah);
            newRow.push(returnVal.outstanding);
            newRow.push('1');
            var outstandingVal = parseInt(returnVal.outstanding.trim()) - 1;
            newRow.push(outstandingVal);
            newRow.push(returnVal.detailbarcode);
            if (typeof $('#'+ returnVal.detailbarcode.trim()).attr('id') != 'undefined'){
                var nTr = $("#"+ returnVal.detailbarcode.trim()).get(0);
                var posisibaris = oTable.row(nTr).index();
                console.log(posisibaris);
                qtyInput[returnVal.detailbarcode]++;
                outstandingVal = parseInt(returnVal.outstanding.trim()) - qtyInput[returnVal.detailbarcode];
                if (outstandingVal >= 0){
                    oTable.cell(posisibaris,7).data(qtyInput[returnVal.detailbarcode]);
                    oTable.cell(posisibaris,8).data(outstandingVal);
                }else{
                    alert('Produksi kelebihan ....!!?');
		    qtyInput[returnVal.detailbarcode]--;
                }
            }else{
                qtyInput[returnVal[0].detailbarcode] = 1;
                oTable.row.add(newRow).draw( false );
            }
            $("#detailbarcode").select();
        }
    });
    $("#detailbarcode").keydown(function(event){
        if(event.keyCode == 13) {
            if($("#detailbarcode").val().length == 0) {
                event.preventDefault();
                return false;
            }else{
                var request = new Object();
                request.term = $(this).val();
                alamat = Drupal.settings.basePath + 'penjualan/carinotaproduksi';
                $.ajax({
                    type: 'GET',
                    url: alamat,
                    data: request,
                    cache: false,
                    success: function(data){
                        var returnVal = eval(data);
                        var idrow = returnVal[0].id;
                        var icondelete = '<img onclick="hapus_produk('+ idrow +',this.parentNode.parentNode,\''+ returnVal[0].detailbarcode.trim() +'\')" title="Klik untuk menghapus" src="'+ Drupal.settings.basePath +'misc/media/images/close.ico" width="24">';
                        var newRow = new Array;
                        newRow.push(icondelete);
                        newRow.push(returnVal[0].nonota);
                        newRow.push(returnVal[0].tglorder);
                        newRow.push(returnVal[0].namapelanggan);
                        newRow.push(returnVal[0].namaproduct);
                        newRow.push(returnVal[0].jumlah);
                        newRow.push(returnVal[0].outstanding);
                        newRow.push('1');
                        var outstandingVal = parseInt(returnVal[0].outstanding.trim()) - 1;
                        newRow.push(outstandingVal);
                        newRow.push(returnVal[0].detailbarcode);
                        if (typeof $('#'+ returnVal[0].detailbarcode.trim()).attr('id') != 'undefined'){
                            var nTr = $("#"+ returnVal[0].detailbarcode.trim()).get(0);
                            var posisibaris = oTable.row(nTr).index();
                            console.log(posisibaris);
                            qtyInput[returnVal[0].detailbarcode]++;
                            outstandingVal = parseInt(returnVal[0].outstanding.trim()) - qtyInput[returnVal[0].detailbarcode];
                            if (outstandingVal >= 0){
                                oTable.cell(posisibaris,7).data(qtyInput[returnVal[0].detailbarcode]);
                                oTable.cell(posisibaris,8).data(outstandingVal);
                            }else{
                                alert('Produksi kelebihan ....!!?');
				qtyInput[returnVal[0].detailbarcode]--;
                            }
                        }else{
                            qtyInput[returnVal[0].detailbarcode] = 1;
                            oTable.row.add(newRow).draw( false );
                        }
                        $("#detailbarcode").select();
                    }
                });
            }
        }
    });
    $('#simpan-produksi').button().on('click', function(){
        for (key in qtyInput) {
            if (qtyInput.hasOwnProperty(key)) {
                var request = new Object();
                request.detailbarcode = key;
                request.qtyupdate = qtyInput[key];
                alamat = Drupal.settings.basePath + 'penjualan/simpanhasilproduksi';
                $.ajax({
                    type: 'POST',
                    url: alamat,
                    data: request,
                    cache: false,
                    success: function (data) {
                        alert('Produksi berhasil disimpan');
                    }
                })
            }
        }
    });
})