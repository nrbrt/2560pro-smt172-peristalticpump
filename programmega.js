var Avrgirl = require('avrgirl-arduino');

var avrgirl = new Avrgirl({
  board: 'mega'
});

avrgirl.flash('mega2560-dual-smt172-peristalticpump.ino.mega.hex', function (error) {
  if (error) {
    console.error(error);
  } else {
    console.info('done.');
  }
});
