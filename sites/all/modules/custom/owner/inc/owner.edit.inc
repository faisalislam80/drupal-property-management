<?php

/**
 * @helper
 * owner register form
 */
function owner_edit_form($form, &$form_state, $owner_id)
{
  $owner_id = intval($owner_id);
  $owner_details = owner_load_by_id($owner_id);

  if(empty($owner_details)) {
    drupal_access_denied();
  }

  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $form['name'] = array(
    '#type'   => 'textfield',
    '#title'  => t('Owner full name'),
    '#maxlength' => 128,
    '#description' => t('Example : Faisal Islam'),
    '#required' => TRUE,
    '#default_value' => $owner_details['name']
  );
  $form['company'] = array(
    '#type'   => 'textfield',
    '#title'  => t('Owner Company Name'),
    '#maxlength' => 128,
    '#description' => t('Example : Royal Inc.'),
    '#required' => TRUE,
    '#default_value' => $owner_details['company']
  );
  $form['address_line_1'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 255,
    '#title'  => t('Owner address line 1'),
    '#required' => TRUE,
    '#default_value' => $owner_details['address_line_1']
  );
  $form['address_line_2'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 255,
    '#title'  => t('Owner address line 2 (optional)'),
    '#default_value' => $owner_details['address_line_2']
  );
  $form['city'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 128,
    '#title'  => t('City'),
    '#required' => TRUE,
    '#default_value' => $owner_details['city']
  );
  $form['state'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 128,
    '#title'  => t('State'),
    '#required' => TRUE,
    '#default_value' => $owner_details['state']
  );
  $form['zip'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 5,
    '#title'  => t('Zip'),
    '#required' => TRUE,
    '#element_validate' => array('owner_zip_element_validate'),
    '#default_value' => $owner_details['zip']
  );

  $countries = country_get_list();
  $form['country'] = array(
    '#type' => 'select',
    '#title' => t('Default country'),
    '#empty_value' => '',
    '#options' => $countries,
    '#description' => t('Select owner country.'),
    '#default_value' => $owner_details['country']
  );

  $form['phone'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 20,
    '#title'  => t('Owner Phone Number'),
    '#required' => TRUE,
    '#element_validate' => array('owner_phone_element_validate'),
    '#description' => t('EX: 000-0000-0000'),
    '#default_value' => $owner_details['phone']
  );
  $form['cell'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 20,
    '#title'  => t('Owner Cell Number'),
    '#required' => TRUE,
    '#element_validate' => array('owner_phone_element_validate'),
    '#description' => t('EX: 000-0000-0000'),
    '#default_value' => $owner_details['cell']
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Edit Owner Information!')
  );

  return $form;
}

function owner_edit_form_validate($form, &$form_state)
{
  //nothing extra validation required
}

function owner_edit_form_submit($form, &$form_state)
{
  //read owner id from url : fixme - implement better way to handle read owner id
  $owner_id = intval(arg(2));
  $owner_edit_info = array(
    'name' => $form_state['values']['name'],
    'company' => $form_state['values']['company'],
    'address_line_1' => $form_state['values']['address_line_1'],
    'address_line_2' => $form_state['values']['address_line_2'],
    'city' => $form_state['values']['city'],
    'state' => $form_state['values']['state'],
    'zip' => $form_state['values']['zip'],
    'country' => $form_state['values']['country'],
    'phone' => $form_state['values']['phone'],
    'cell' => $form_state['values']['cell'],
  );
  //update info
  db_update(TABLE_OWNER_DETAILS)->fields($owner_edit_info)->condition('id', $owner_id, '=')->execute();

  drupal_set_message(t('Owner information updated successfully'));
}
