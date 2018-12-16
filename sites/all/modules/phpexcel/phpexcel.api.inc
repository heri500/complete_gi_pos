<?php

/**
 * @file
 * Defines the phpexcel api functions that other modules can use.
 */

/**
 * Simple API function which will generate an XLS file and
 * save it in $path.
 *
 * @param array $headers
 * 						An array containing all headers. If given a two-dimensional array,
 *            each first dimension entry will be on a separate worksheet
 *						($headers[sheet][header]).
 * @param array $data
 * 						A two-dimensional array containing all data ($data[row][column]).
 *            If given a three-dimensional array, each first dimension
 * 						entry will be on a separate worksheet ($data[sheet][row][column]).
 * @param string $path
 * 						The path where the file must be saved. Must be writable.
 * @param array $options
 *            An array which allows to set some specific options.
 * 						Used keys:
 * 								[format] = The EXCEL format. Can be either 'xls' or 'xlsx'
 * 								[creator] = The name of the creator
 * 								[title] = The title
 * 								[subject] = The subject
 * 								[description] = The description
 *            The options array will always be passed to all the hooks. If
 *						developers need specific information for their own hooks, they
 *						can add any data to this array.
 * @return bool
 * 						TRUE on success, FALSE on error. Look in watchdog logs for information
 *            about errors.
 */
function phpexcel_export($headers = array(), $data = array(), $path = '', $options = NULL) {
  if (!count($headers) && (!isset($options['ignore_headers']) || (isset($options['ignore_headers']) && !$options['ignore_headers']))) {
    watchdog('phpexcel', "You must provide at lease one header entry!", array(), WATCHDOG_ERROR);
		
    return FALSE;
  }
  
  if (!count($data)) {
    watchdog('phpexcel', "No data provided!", array(), WATCHDOG_ERROR);
		
    return FALSE;
  }
  
	// Must assign to a variable, as file_check_path() will alter the passed argument
	$check_directory = $path;
	
  if (!file_check_path($check_directory)) {
    watchdog('phpexcel', "Path '@path' is not writable!", array('@path' => $path), WATCHDOG_ERROR);
		
    return FALSE;
  }
  
	if (!file_check_location('sites/all/libraries/PHPExcel/PHPExcel.php', 'sites/all/libraries/PHPExcel')) {
		watchdog('phpexcel', "Couldn't find the PHPExcel library! ", array(), WATCHDOG_ERROR);
		
		return FALSE;	
	}
	
  require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
	
	$path = file_munge_filename($path, 'xls xlsx');
  
	// Can we use Memcache ?
	/*$memcache = variable_get('phpexcel_memcache', '');
	
  if (empty($memcache) || !$cache_method = PHPExcel_CachedObjectStorageFactory::cache_to_memcache) {
		$cache_method = PHPExcel_CachedObjectStorageFactory::cache_in_memory_gzip;
		
		$cache_settings = array();
	}
	else {
		$cache_settings = array('memcacheServer'  => $memcache);
	}
  
  PHPExcel_Settings::setCacheStorageMethod($cache_method, $cache_settings);*/
	
	// Must we render from a template file ?
	if (!empty($options['template'])) {
		$xls_reader = PHPExcel_IOFactory::createReaderForFile($options['template']);
		
    $xls = $xls_reader->load($options['template']);
	}
	else {
		$xls = new PHPExcel();
	}
	
  _phpexcel_set_properties($xls->getProperties(), $options);
	
	// Must we ignore the headers ?
	if (empty($options['ignore_headers'])) {
		_phpexcel_set_headers($xls, $headers, $options);
	}
	
  _phpexcel_set_columns($xls, $data, count(reset(array_values($headers))), $options);
  
  if (!isset($options['format']) || $options['format'] == 'xls') {
		$writer = new PHPExcel_Writer_Excel5($xls);
	}
	else {
		$writer = new PHPExcel_Writer_Excel2007($xls);
	}
	
	$writer->save($path);
	
	return file_exists($path);
}

/**
 * Simple API function which allows to export a db_query() result to an Excel file.
 * The headers will be set to the names of the exported columns.
 *
 * @see phpexcel_export()
 *
 * @param result $result
 * 							The MySQL result object.
 * @param string $path
 * 							The path where the file should be saved. Must be writable.
 * @param array $options
 * 							An array which allows to set some specific options.
 *
 * @return bool
 * 							TRUE on success, FALSE on error. Look into watchdog logs for information
 *              about errors.
 */
function phpexcel_export_db_result($result, $path, $options = array()) {
	$data = array();
	
	while ($row = db_fetch_array($result)) {
		if (!isset($headers)) {
			$headers = array_keys($row);
		}
		$data[] = array_values($row);
	}
	
	return phpexcel_export($headers, $data, $path, $options);
}

/**
 * Sets the Excel file properties, like creator, title, etc.
 *
 * @see phpexcel_export()
 */
function _phpexcel_set_properties(&$properties, $options) {
  if (isset($options['creator'])) {
    $properties->setCreator($options['creator']);
  }
  else {
    $properties->setCreator("PHPExcel");
  }
  
  if (isset($options['title'])) {
		$properties->setTitle($options['title']);
  }
  
  if (isset($options['subject'])) {
		$properties->setSubject($options['subject']);
  }
  
  if (isset($options['description'])) {
		$properties->setDescription($options['description']);
  }
}

/**
 * Sets the Excel file headers.
 *
 * @see phpexcel_export()
 */
function _phpexcel_set_headers(&$xls, &$headers, $options) {
	if (!is_array(reset(array_values($headers)))) {
		$headers = array($headers);
	}
	
	phpexcel_invoke('export', 'headers', $headers, $xls, $options);
	
	$sheet_id = 0;
	
	foreach ($headers as $sheet_name => $sheet_headers) {
		$xls->createSheet($sheet_id);
		
		phpexcel_invoke('export', 'new sheet', $sheet_id, $xls, $options);
		
		$sheet = $xls->setActiveSheetIndex($sheet_id);

		if (!is_numeric($sheet_name)) {
			$sheet->setTitle($sheet_name);
		}
		else {
			$sheet->setTitle("Worksheet $sheet_id");
		}

		for ($i = 0, $len = count($sheet_headers); $i < $len; $i++) {
			$value = trim($sheet_headers[$i]);
			
			phpexcel_invoke('export', 'pre cell', $value, $sheet, $options, $i, 1);
			
			$sheet->setCellValueByColumnAndRow($i, 1, $value);
			
			phpexcel_invoke('export', 'post cell', $value, $sheet, $options, $i, 1);
		}
		
		$sheet_id++;
	}
}

/**
 * Adds the data to the Excel file.
 *
 * @see phpexcel_export()
 */
function _phpexcel_set_columns(&$xls, &$data, $cols, $options) {
	$data = array_values($data);
	
	if (!is_array($data[0][0])) {
		$data = array($data);
	}
	
	phpexcel_invoke('export', 'data', $data, $xls, $options);
	
	foreach ($data as $sheet_id => $sheet_data) {
		$sheet = $xls->setActiveSheetIndex($sheet_id);
	
		for ($i = 0, $len = count($sheet_data); $i < $len; $i++) {
			for ($j = 0; $j < $cols; $j++) {
				$value = isset($sheet_data[$i][$j]) ? $sheet_data[$i][$j] : '';
				
				// We must offset the row count (by 2, because PHPExcel starts the count at 1), because the first row is used by the headers
				phpexcel_invoke('export', 'pre cell', $value, $sheet, $options, $j, $i + 2);
				
				$sheet->setCellValueByColumnAndRow($j, $i + 2, $value);
				
				phpexcel_invoke('export', 'post cell', $value, $sheet, $options, $j, $i + 2);
			}
		}
	}
}

/**
 * Simple API function that will load an Excel file from $path and parse it
 * as a multidimensional array.
 *
 * @param string $path
 * 						The path to the Excel file. Must be readable.
 * @param boolean $keyed_by_headers = TRUE
 * 						If TRUE, will key the row array with the header values and will
 * 						skip the header row. If FALSE, will contain the headers in the first
 * 						row and the rows will be keyed numerically.
 * @return array|boolean
 * 						The parsed data as an array on success, FALSE on error. Look into
 * 						watchdog logs for information about errors.
 */
function phpexcel_import($path, $keyed_by_headers = TRUE) {
	if (is_readable($path)) {
		require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
		
		$xls_reader = PHPExcel_IOFactory::createReaderForFile($path);
		
		$xls_reader->setReadDataOnly(TRUE);
		
		$xls_data = $xls_reader->load($path);
		
		$data = array();
		$headers = array();
		$i = 0;
		
		phpexcel_invoke('import', 'full', $xls_data, $xls_reader, array('keyed_by_headers' => $keyed_by_headers));
		
		foreach ($xls_data->getWorksheetIterator() as $worksheet) {
			$j = 0;
			
			phpexcel_invoke('import', 'sheet', $worksheet, $xls_reader, array('keyed_by_headers' => $keyed_by_headers));
			
			foreach ($worksheet->getRowIterator() as $row) {
				$k = 0;
				
				phpexcel_invoke('import', 'row', $row, $xls_reader, array('keyed_by_headers' => $keyed_by_headers));
				
				$cells = $row->getCellIterator();
				
				$cells->setIterateOnlyExistingCells(FALSE);
				
				foreach($cells as $cell) {					
					if (!$j && $keyed_by_headers) {
						$value = $cell->getValue() ? trim($cell->getValue()) : $k;
						
						phpexcel_invoke('import', 'pre cell', $value, $cell, array('keyed_by_headers' => $keyed_by_headers), $k, $j);
						
						$headers[$i][] = $value;
					}
					elseif ($keyed_by_headers) {
						$value = $cell->getValue() ? $cell->getValue() : '';
						
						phpexcel_invoke('import', 'pre cell', $value, $cell, array('keyed_by_headers' => $keyed_by_headers), $k, $j);
						
						$data[$i][$j - 1][$headers[$i][$k]] = $value;
						
						phpexcel_invoke('import', 'post cell', $data[$i][$j - 1][$headers[$i][$k]], $cell, array('keyed_by_headers' => $keyed_by_headers), $k, $j);
					}
					else {
						$value = $cell->getValue() ? $cell->getValue() : '';
						
						phpexcel_invoke('import', 'pre cell', $value, $cell, array('keyed_by_headers' => $keyed_by_headers), $k, $j);
						
						$data[$i][$j][] = $value;
						
						phpexcel_invoke('import', 'post cell', $data[$i][$j][$k], $cell, array('keyed_by_headers' => $keyed_by_headers), $k, $j);
					}
					
					$k++;
				}
				
				$j++;
			}
			
			$i++;
		}
		
		return $data;
	}
	else {
		watchdog('phpexcel', "The path '!path' is not readable !", array('!path' => $path));
		
		return FALSE;
	}
}

/**
 * Invokes phpexcel hooks
 * We need a custom hook-invoke method, because we need to pass parameters by
 * reference.
 */
function phpexcel_invoke($hook, $op, &$data, &$phpexcel, $options, $column = NULL, $row = NULL) {
	foreach(module_implements('phpexcel_' . $hook) as $module) {
		$function = $module . '_phpexcel_' . $hook;
		
		$function($op, $data, $phpexcel, $options, $column, $row);
	}
}
