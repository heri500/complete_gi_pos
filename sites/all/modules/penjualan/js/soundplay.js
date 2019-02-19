function initAudio(element) {
    var song = element.attr('song');
    audio = new Audio(song);
    if ($(element).is(':last-child')){
        audio.playbackRate = 1;
    }else{
        audio.playbackRate = 1.7;
    }
    audio.play();
    $('#playlist li').removeClass('active');
    element.addClass('active');
    audio.onended = function() {
        var next = $('#playlist li.active').next();
        if (next.length == 0) {
            close();
        } else {
            initAudio(next);
            //audio.play();
        }
    };
}
$(document).ready(function(){
    initAudio($('#playlist li:first-child'));
})