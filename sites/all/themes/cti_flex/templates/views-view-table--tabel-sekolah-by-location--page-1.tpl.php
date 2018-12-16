<?php
// $Id: views-view-table.tpl.php,v 1.8 2009/01/28 00:43:43 merlinofchaos Exp $
/**
 * @file views-view-table.tpl.php
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $class: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 * @ingroup views_templates
 */
 	drupal_add_css('misc/media/datatables/themes/redmond/jquery-ui-1.8.14.custom.css');
	drupal_add_js('misc/media/jquery-1.5.1.min.js');
	drupal_add_js('misc/media/jquery-ui-1.8.14.custom.min.js');
	drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
	drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
	drupal_add_js('
	  var mainpath = "'.base_path().'";
		
		function fnFormatDetails ( oTable, nTr , nidsekolah){
			var detailbangun = $("#detail_bangun_ruang_"+ nidsekolah).html();
			sOut = detailbangun;
			return sOut;
		}
		function fnFormatDetails2 ( oTable, nTr , nidsekolah){
			var detailfurniture = $("#detail_furniture_"+ nidsekolah).html();
			sOut = detailfurniture;
			return sOut;
		}
		function see_details_bangun(nTr,iconbutton, nid){
			if ( iconbutton.src.match("details_close") ){
				/* This row is already open - close it */
				iconbutton.src = mainpath +"misc/media/images/house.png";
				oTable.fnClose( nTr );
			}else{
				/* Open this row */
				iconbutton.src = mainpath +"misc/media/images/details_close.png";
				oTable.fnOpen( nTr, fnFormatDetails(oTable, nTr, nid), "details" );
			}
		}
		function see_details_furniture(nTr,iconbutton, nid){
			if ( iconbutton.src.match("details_close") ){
				/* This row is already open - close it */
				iconbutton.src = mainpath +"misc/media/images/cubes.png";
				oTable.fnClose( nTr );
			}else{
				/* Open this row */
				iconbutton.src = mainpath +"misc/media/images/details_close.png";
				oTable.fnOpen( nTr, fnFormatDetails2(oTable, nTr, nid), "details" );
			}
		}
		$(document).ready(function(){
   		oTable = $("#tabel_sekolah").dataTable( {
				"bJQueryUI": true,
				"bPaginate": true,
				"sPaginationType": "full_numbers",
				"bLengthChange": true,
				"bFilter": true,
				"bSort": true,
				"bInfo": true,
				"bAutoWidth": false,
				"bStateSave": true,
				"aaSorting": [ [7,"asc"] , [4,"asc"] ]
			});
			$("tr.odd td, tr.even td").css("padding","0 10px");
			$("#edit-submit-tabel-sekolah-by-location").css("margin-top","1em");
   	})
	','inline');
?>
<table id="tabel_sekolah" class="<?php print $class; ?> display">
  <?php if (!empty($title)) : ?>
    <caption><?php print $title; ?></caption>
  <?php endif; ?>
  <thead>
    <tr>
    		<th>&nbsp;</th>
    		<th>&nbsp;</th>
      <?php foreach ($header as $field => $label): ?>
      	<?php
      	$added_style = '';
      	if ($field == 'nid'){
      		$added_style = 'style="width:25px;"';
      	}
      	?>
        <th class="views-field views-field-<?php print $fields[$field]; ?>" <?php print $added_style; ?> >
          <?php print $label; ?>
        </th>
      <?php endforeach; ?>
    </tr>
  </thead>
  <tbody>
  	<?php foreach ($rows as $count => $row): ?>
    	<?php
    		global $nid_sekolah;
    		$nid_sekolah = $rows[$count]['nid'];
    	?>
      <tr class="<?php print implode(' ', $row_classes[$count]); ?>">
      		<td style="width: 25px;">
      			<img onclick="see_details_bangun(this.parentNode.parentNode,this,'<?php print $nid_sekolah; ?>')"title="Klik untuk melihat detail bangun ruang yang dimiliki sekolah" src="<?php print base_path(); ?>misc/media/images/house.png" style="cursor:pointer;">
      		</td>
      		<td style="width: 25px;">
      			<img onclick="see_details_furniture(this.parentNode.parentNode,this,'<?php print $nid_sekolah; ?>')"title="Klik untuk melihat detail furniture yang dimiliki sekolah" src="<?php print base_path(); ?>misc/media/images/cubes.png" style="cursor:pointer;">
      		</td>
        <?php foreach ($row as $field => $content): ?>
          <td class="views-field views-field-<?php print $fields[$field]; ?>">
          	<?php
          		if ($field == 'nid'){
          			$content_view = '<a href="'.base_path().'node/'.$content.'/edit?destination=tabelsekolah?field_provinsi_ref_nid='.$_GET['field_provinsi_ref_nid'].'&field_kabupaten_ref_nid='.$_GET['field_kabupaten_ref_nid'].'"><img title="Klik untuk mengubah" src="'.base_path().'misc/media/images/edit.ico" width="20" style="cursor:pointer;"></a>';
          			$content = $content_view;
          			$detailbangunruang = views_embed_view("list_bangun_ruang_sekolah","default",$nid_sekolah);
	        			$tabeldetailbangunruang = '<div id="detail_bangun_ruang_'.$nid_sekolah.'" style="display:none">'.$detailbangunruang.'</div>';
	        			$content .= $tabeldetailbangunruang;
	        			$detailfurniture = views_embed_view("list_furniture_sekolah","default",$nid_sekolah);
	        			$tabeldetailfurniture = '<div id="detail_furniture_'.$nid_sekolah.'" style="display:none">'.$detailfurniture.'</div>';
	        			$content .= $tabeldetailfurniture;
          		}
          	?>
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>