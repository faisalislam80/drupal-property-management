<?php

/**
 * @helper
 * owner register form
 */
function owner_register_form($form, &$form_state)
{
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $form['username'] = array(
  '#type'   => 'textfield',
  '#title'  => t('Owner username'),
  '#description' => t('Unique username - will be used for login to the site'),
  '#required' => TRUE
);
  $form['email'] = array(
    '#type'   => 'textfield',
    '#title'  => t('Owner email address'),
    '#description' => t('Unique and valid email'),
    '#required' => TRUE,
    '#element_validate' => array('owner_email_element_validate'),
  );
  $form['password'] = array(
    '#type'   => 'password_confirm',
    '#description' => t('This will used for login to owner portal'),
    '#required' => TRUE
  );
  $form['name'] = array(
    '#type'   => 'textfield',
    '#title'  => t('Owner full name'),
    '#maxlength' => 128,
    '#description' => t('Example : Mr. Mohammad Faisal Islam'),
    '#required' => TRUE
  );
  $form['company'] = array(
    '#type'   => 'textfield',
    '#title'  => t('Owner Company Name'),
    '#maxlength' => 128,
    '#description' => t('Example : Royal Inc.'),
    '#required' => TRUE
  );
  $form['address_line_1'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 255,
    '#title'  => t('Owner address 1'),
    '#required' => TRUE
  );
  $form['address_line_2'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 255,
    '#title'  => t('Owner address 2'),
  );
  $form['city'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 128,
    '#title'  => t('Owner City'),
    '#required' => TRUE
  );
  $form['state'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 128,
    '#title'  => t('Owner State'),
    '#required' => TRUE
  );
  $form['zip'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 5,
    '#title'  => t('Owner Zip'),
    '#required' => TRUE,
    '#element_validate' => array('owner_zip_element_validate'),
  );

  $countries = country_get_list();
  $form['country'] = array(
    '#type' => 'select',
    '#title' => t('Default country'),
    '#empty_value' => '',
    '#default_value' => variable_get('site_default_country', NULL),
    '#options' => $countries,
    '#description' => t('Select owner country.'),
  );

  $form['phone'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 20,
    '#title'  => t('Owner Phone Number'),
    '#required' => TRUE,
    '#element_validate' => array('owner_phone_element_validate'),
    '#description' => t('EX: 000-0000-0000'),
  );
  $form['cell'] = array(
    '#type'   => 'textfield',
    '#maxlength' => 20,
    '#title'  => t('Owner Cell Number'),
    '#required' => TRUE,
    '#element_validate' => array('owner_phone_element_validate'),
    '#description' => t('EX: 000-0000-0000'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Edit Owner Information!')
  );

  return $form;
}

function owner_register_form_validate($form, &$form_state)
{
  $username = $form_state['values']['username'];
  $email    = $form_state['values']['email'];
  //check unique username on users table
  $valid = TRUE;
  if(!owner_check_unique('users', 'name', $username)) {
    form_set_error('username', t('Username already taken, please choose another one.'));
    $valid = FALSE;
  }
  //check unique email on users table
  if(!owner_check_unique('users', 'mail', $email)) {
    form_set_error('email', t('Email already taken, please choose another one.'));
    $valid = FALSE;
  }

  return $valid;
}

function owner_register_form_submit($form, &$form_state)
{
  //save user to users table
  $user_owner_details = array(
    'name' => $form_state['values']['username'],
    'mail' => $form_state['values']['email'],
    'pass' => $form_state['values']['password'],
    'status' => 1,
    'created' => time(),
    'timezone' => 'Asia/Dhaka',
    'signature_format' => 'filtered_html',
    'roles' => array(
      DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      3 => 'property owner',
    ),
  );
  $account = user_save(NULL, $user_owner_details);

  if(!$account || !$account->uid) {
    drupal_set_message(t('User account not created successfully, please try again later.'), 'warning');
    return FALSE;
  }

  //save user to owner_details table
  db_insert(TABLE_OWNER_DETAILS)
            ->fields(array(
              'uid' => $account->uid,
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
            ))
            ->execute();
  //todo : send email to owner that his/her account has been created

  drupal_set_message(t('Owner created successfully'));
}
