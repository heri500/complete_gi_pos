$(document).ready(function(){
    var pathutama = Drupal.settings.basePath;
	jsPrintSetup.setOption('orientation', jsPrintSetup.kPortraitOrientation);
	jsPrintSetup.setOption('marginTop', 0);
	jsPrintSetup.setOption('marginBottom', 0);
	jsPrintSetup.setOption('marginLeft', 0);
	jsPrintSetup.setOption('marginRight', 0);
	// Suppress print dialog
	jsPrintSetup.setSilentPrint(true);/** Set silent printing */
	// Do Print
	jsPrintSetup.print();
	if (Drupal.settings.print_category == 1){
        var request = new Object();
        request.idorder = Drupal.settings.id_order;
        alamat = pathutama + "penjualan/getcoarrayprinter";
        $.ajax({
            type: "POST",
            url: alamat,
            data: request,
            cache: false,
            success: function (data) {
                var ArrayPrinter = eval(data);
                var print_idx = 0;
                if (print_idx <= ArrayPrinter.length){
                    window.open(pathutama + 'print/6?idghordermultiprint='+ Drupal.settings.id_order +'&printername='+ ArrayPrinter[print_idx].trim() +'&print_idx='+ print_idx);
                    close();
                }else{
                    close();
                }
            }
        });
	}else{
        close();
    }
	//
	// Restore print dialog
	//jsPrintSetup.setSilentPrint(false);
});