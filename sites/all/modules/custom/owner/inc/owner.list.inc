<?php

/**
 * Show list of owners
 *
 * @return mixed
 */
function owner_lists_page()
{
  global $base_url;
  $header = array(
    array('data' => t('ID'), 'field' => 't.id'),
    array('data' => t('Name'), 'field' => 't.name'),
    array('data' => t('Address'), 'field' => 't.address_line_1'),
    array('data' => t('Phone'), 'field' => 't.phone'),
  );

  $query = db_select(TABLE_OWNER_DETAILS, 't')->extend('TableSort')->fields('t');

  $result = $query->orderByHeader($header)->execute();

  $rows = array();
  foreach($result as $row) {
    $row = (array)$row;
    $rows[] = array(
      'data' => array(
        'id' => '<a href="'.$base_url .'/owner/edit/'.$row['id'].'">'.$row['id'].'</a>',
        'name' => $row['name'],
        'address' => $row['address_line_1'],
        'phone' => $row['phone'],
      )
    );
  }

  // Build the table for the nice output.
  $build['tablesort_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}
