 $(document).ready(function() {


 var name = '';
 
 	function go() {
 		name = $('#user-name').val();
    	$('#user-name').val('');
    	$('.user-form').hide();
    	$('.chat-box').show();
	};

    $('#user-name').keydown(function(e) {
	    if(e.keyCode == 13){ //Enter pressed 
	      go();
	    }
  	});

	$('.go-user').on('click', function(e) {
    	go();
  	});

	$('.chat-box textarea').keydown(function(e) {
	if(e.keyCode == 13){
	  $.ajax({
	    type: "POST",
	    url: "/msg",
	    data: JSON.stringify({"username" : name, "message" : $('#message-input').val().trim()}),
	    contentType: "application/json"
	  });
	  $(this).val('');
	  $('.jumbotron').hide();
	  e.preventDefault()
	}
	});



 });

