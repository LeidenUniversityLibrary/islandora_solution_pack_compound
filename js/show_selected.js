/**
 * @file
 * show_selected
 */

/**
 * Scrolls the selected (active) child of a compound into view.
 */
jQuery(window).load(function() {
  // Find the compounds container and the active child. 
  // Do this after the whole page loads, because the thumbs of the compound children need to be loaded
  // for this to work correctly.
  var activeCompoundChild;
  var compoundsContainer = jQuery('.islandora-compound-prev-next .islandora-compound-thumbs');
  if (compoundsContainer.size() > 0) {
    activeCompoundChild = compoundsContainer.find('.active').closest('.islandora-compound-thumb');
  }
  else {
    compoundsContainer = jQuery('#block-islandora-compound-object-compound-jail-display');
    activeCompoundChild = compoundsContainer.find('.islandora-compound-object-jail-active, .active').first().parent();
  }

  // Find the scrollable container the active child is in (this might not be the compound container)
  activeCompoundChild.parentsUntil(compoundsContainer.parent()).each(function(index, container) {
    if ((container.scrollHeight > container.clientHeight) || (container.scrollWidth > container.clientWidth)) {
      // This container is scrollable, check if the active child is not visible or almost not visible;
      // if so, scroll the active child to almost the top of the container (or as close as possible).
      var containerOffset = jQuery(container).offset();
      var containerHeight = jQuery(container).innerHeight();
      var containerWidth = jQuery(container).innerWidth();
      var activeOffset = activeCompoundChild.offset();
      var activeHeight = activeCompoundChild.outerHeight();
      var activeWidth = activeCompoundChild.outerWidth();
      var newScrollTop = (activeOffset.top - containerOffset.top);
      var newScrollLeft = (activeOffset.left - containerOffset.left);
      
      if ((container.scrollHeight > container.clientHeight) && (newScrollTop > (containerHeight - 2*activeHeight))) {
        if ((newScrollTop + 2*activeHeight) < container.scrollHeight) { 
          newScrollTop -= activeHeight;
        }
        container.scrollTop = newScrollTop;
      }
      if ((container.scrollWidth > container.clientWidth) && (newScrollLeft > (containerWidth - 2*activeWidth))) {
        if ((newScrollLeft + 2*activeWidth) < container.scrollWidth) { 
          newScrollLeft -= activeWidth;
        }
        container.scrollLeft = newScrollLeft;
      }
      
      return false;
    }
  });
});
