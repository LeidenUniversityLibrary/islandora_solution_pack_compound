jQuery(window).ready( function($) {
  var viewer = jQuery('.content .islandora-viewer');
  var toc = jQuery('.islandora-compound-prev-next--hierarchical.table-of-contents');
  if (viewer.size() == 1 && toc.size() == 1) {
    toc.insertBefore(viewer);
  }
});
jQuery(window).load( function($) {
  var toc = jQuery('.table-of-contents');
  var content = toc.next();
  var tocheader = toc.find('.toc-header');

  var resizeFunc = function() {
		var tocposx = toc.position().left;
		var contentposx = content.position().left;
		if (contentposx != tocposx) {
			// toc is next to content
			var contentheight = content.height();
			toc.find('.toc-content').css({'max-height': contentheight+'px'});
		}
		else {
			toc.find('.toc-content').css({'max-height': ''});
		}
	};
	resizeFunc();
	jQuery(window).on('resize', resizeFunc);

  var tochwidth = tocheader.width();
  var contentwidth = content.width();
  var toggleToc = function(e) {
    if (toc.hasClass('aside')) {
			tocheader.animate({width: tochwidth + 'px'}, function() {
				 tocheader.css('width', '');
				 jQuery('#closetoc').one("click", toggleToc);
			});
			content.animate({width: contentwidth + 'px'}, function() {
				content.css('width', '');
			});
      setTimeout(function() {
        toc.removeClass('aside').addClass('expanding');
        setTimeout(function() {
          toc.removeClass('expanding');
        }, 500);
      }, 100);
    }
    else if (jQuery(this).is('#closetoc')) {
      var tocposx = toc.position().left;
      var contentposx = content.position().left;
			toc.addClass('aside');
      if (contentposx != tocposx) {
				tochwidth = tocheader.width();
				contentwidth = content.width();
				setTimeout(function() {
				  var contentheight = content.height();
					tocheader.animate({width: contentheight + 'px'}, function() {
						 tocheader.one("click", toggleToc);
					});
					content.animate({width: "95%"});
				}, 100);
			}
			else {
			  setTimeout(function() {
			    tocheader.one("click", toggleToc);
			  }, 400);
			}
    }
    e.preventDefault();
  };
  jQuery('#closetoc').one("click", toggleToc);
});
