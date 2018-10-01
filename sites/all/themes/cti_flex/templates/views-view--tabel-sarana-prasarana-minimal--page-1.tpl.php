<?php
/**
 * @file views-view.tpl.php
 * Main view template
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 * - $admin_links: A rendered list of administrative links
 * - $admin_links_raw: A list of administrative links suitable for theme('links')
 *
 * @ingroup views_templates
 */
 	$path = drupal_get_path('theme', 'cti_flex');
	$form_style = $path.'/css/custom-css.css';
	drupal_add_css($form_style, 'theme', 'all', FALSE);
	$variables['styles'] = drupal_get_css();
	drupal_add_css('misc/media/datatables/themes/redmond/jquery-ui-1.8.14.custom.css');
	drupal_add_js('misc/media/jquery-1.5.1.min.js');
	drupal_add_js('misc/media/jquery-ui-1.8.14.custom.min.js');
	drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
	drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
	drupal_add_js('
	  var mainpath = "'.base_path().'";
		
		$(document).ready(function(){
			oTable = $("#tabel_sarana_prasarana").dataTable( {
				"bJQueryUI": true,
				"bPaginate": true,
				"sPaginationType": "full_numbers",
				"bLengthChange": true,
				"bFilter": true,
				"bSort": true,
				"bInfo": true,
				"bAutoWidth": false,
				"bStateSave": true
			});
			$("tr.odd td, tr.even td").css("padding","0 10px");
			$("#edit-submit-tabel-sarana-prasarana-minimal").css("margin-top","1em");
			$("button").button();
   	})
	','inline');
	$title = t('COMPLETENESS OF THE INFRASTRUCTURE AND FACILITIES');
	drupal_set_title($title);
 	$buttontitle = t('Add New Infrastructure & Facilities');
 	$tooltip = t('Click this button to add new Infrastructure & Facilities');
 	$header = '<button title="'.$tooltip.'" onclick="window.location=\''.base_path().'node/add/sdn-kelengkapan-sar-pras?destination=saranaprasaranaminimal\'">'.$buttontitle.'</button>';
?>
<div class="<?php print $classes; ?>">
  <?php if ($admin_links): ?>
    <div class="views-admin-links views-hide">
      <?php print $admin_links; ?>
    </div>
  <?php endif; ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($exposed): ?>
    <div class="view-filters">
      <?php print $exposed; ?>
    </div>
  <?php endif; ?>

  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>

  <?php if ($rows): ?>
    <div class="view-content">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
  <?php endif; ?>

  <?php if ($attachment_after): ?>
    <div class="attachment attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div> <?php /* class view */ ?>