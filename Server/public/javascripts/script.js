(function () {
  function updateTemp() {
    if ($('input#dev').val()) {
      $.ajax({
        url: 'http://localhost:3000/api/data/' + $('input#dev').val(),
        type: 'get',
        dataType: 'json',
        async: true,
        success: function (data) {
          $('span#temp').html(data.data);
          console.log(data.data);
        },
        error: function(XMLHttpRequest, textStatus, errorThrown) {
          console.log('error', errorThrown);
        }
      });
    }
  }
  setInterval(updateTemp, 3000);
})();
