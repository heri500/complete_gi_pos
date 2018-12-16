$(document).ready(function() {
    var alamatupdate = Drupal.settings.basePath +'datapremis/generatepremisid';
    $('#edit-zone').on('change', function(){
        var request = new Object();
        request.zone = $(this).val();
        $.ajax({
            type: "POST",
            url: alamatupdate,
            data: request,
            cache: false,
            success: function(data){
                var returnVal = eval(data);
                $('#edit-id').val(returnVal[0]);
            }
        });
    });
})