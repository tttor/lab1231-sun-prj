// script: google-images.js
var page = require('webpage').create();
page.settings.loadImages = false;
page.settings.userAgent = 'Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.117 Safari/537.36';
var fs = require('fs');
if (phantom.args.length != 4) {
	console.log('usage: phantomjs images.js URL out_file.html n_scrolls delay');
	console.log('suggest n_scrolls = 10 and delay=1000, for 10 scrolls each taking about 1 second');
	phantom.exit(1);
}
var query_url = phantom.args[0];
var out_file = phantom.args[1];
var n_scrolls = parseInt(phantom.args[2]);
var delay = parseInt(phantom.args[3]);

function collectImages(n) {
	if (n>1) {
		page.evaluate(function() {
			window.scrollTo(0, document.body.scrollHeight);
			var smb = document.getElementById("smb");
			if (smb) { smb.click(); }
		});
		setTimeout(function() { collectImages(n-1); }, delay);
	} else {
		fs.write(out_file, page.content, 'w');
//		page.render('export.png');
		phantom.exit();
	}
}

page.open(query_url, function (status) {
	if (status !== 'success') {
		console.log('Unable to access network');
		phantom.exit(1);
	}
	collectImages(n_scrolls);
});
