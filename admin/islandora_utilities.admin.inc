<?php

function islandora_utilities_settings_form($form, &$form_state) {
  global $user;
  module_load_include('inc', 'islandora', 'includes/IslandoraTuque');

  if(variable_get('islandora_utilities_remote_islandora_base_url', '')) {
    $url = variable_get('islandora_utilities_remote_islandora_base_url', '');
    if(!variable_get('islandora_utilities_remote_islandora_drupal_filter', '')) {
      $remote_user = new stdClass();
      $remote_user->name = variable_get('islandora_utilities_remote_islandora_username', 'fedoraAdmin');
      $remote_user->pass = variable_get('islandora_utilities_remote_islandora_password', 'fedoraAdmin');
      $remote_user->uid = -1;
    }
    else {
      $remote_user = NULL;
    }

    $remote_connection = new IslandoraTuque($remote_user, $url);

    try {
      $info = $remote_connection->api->a->describeRepository();

    }
    catch (RepositoryException $e) {
      $info = FALSE;
    }

    if ($info) {
      try {
        $dc = $remote_connection->api->m->getDatastream('fedora-system:ContentModel-3.0', 'DC');
      }
      catch (RepositoryException $e) {
        $dc = FALSE;
      }
    }

    if ($info) {
      if ($dc) {
        $confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>'
          . t('Successfully connected to Fedora Server (Version !version).', array('!version' => $info['repositoryVersion']));
      }
      else {
        $confirmation_message = '<img src="' . url('misc/watchdog-warning.png') . '"/>'
          . t('Unable to authenticate when connecting to to Fedora Server (Version !version). Please configure the !filter.', array('!version' => $info['repositoryVersion'], '!filter' => 'Drupal Filter'));
      }
    }
    else {
      $confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> '
        . t('Unable to connect to Fedora server at !islandora_url', array('!islandora_url' => $url));
    }
    unset($remote_connection);
  }
  else {
    $confirmation_message = '<img src="' . url('misc/watchdog-warning.png') . '"/>'
      . t('Settings are not set.');
  }

  $form['islandora_utilities_output_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for output'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );
  $form['islandora_utilities_output_settings']['islandora_utilities_output_settings_output_everything'] = array(
    '#type' => 'checkbox',
    '#title' => t('Output everything'),
    '#default_value' => variable_get('islandora_utilities_output_settings_output_everything', ''),
    '#description' => t("If not checked, only warnings will be in the output"),
  );

  $form['islandora_utilities_output_settings']['islandora_utilities_output_settings_output_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Output to drupal_set_message'),
    '#default_value' => variable_get('islandora_utilities_output_settings_output_screen', ''),
  );

  $form['islandora_utilities_output_settings']['islandora_utilities_output_settings_output_file'] = array(
    '#type' => 'checkbox',
    '#title' => t('Output to txt file'),
    '#default_value' => variable_get('islandora_utilities_output_settings_output_file', ''),
    '#description' => t("Dumps file into the drupal files dir"),
  );

  $form['islandora_utilities_validate_object_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Validate object settings --move this to alter form'),
    '#description' => t("Settings for validating objects and datastreams"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

   $form['islandora_utilities_validate_object_settings']['islandora_utilities_validate_object_settings_content_models'] = array(
    '#type' => 'textfield',
    '#title' => t('Content Model make this into a table form'),
    '#default_value' => variable_get('islandora_utilities_validate_object_settings_content_models', ''),
    );

   $form['islandora_utilities_validate_object_settings']['islandora_utilities_validate_object_settings_datastream_ids'] = array(
    '#type' => 'textfield',
    '#title' => t('Datastreams ids'),
    '#description' => t('Space seperated list of datastreams for the above content model'),
    '#default_value' => variable_get('islandora_utilities_validate_object_settings_datastream_ids', ''),
  );


   $form['islandora_utilities_validate_object_settings']['add_content_model'] = array(
    '#type' => 'button',
    '#value' => t('Add Content Model'),
    '#suffix' => '</div>',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'islandora_utilities_validate_object_settings_add_content_model',
      'wrapper' => 'islandora_utilities_validate_object_settings',
      'method' => 'replace',
    ),
  );


   $form['islandora_utilities_validate_object_settings']['islandora_utilities_validate_object_settings_datastream_ids_merge'] = array(
     '#type' => 'checkbox',
      '#title' => t('Merge datastream ids with requirments from object content models'),
      '#default_value' => variable_get('islandora_utilities_validate_object_settings_datastream_ids_merge', ''),
      '#description' => t('Checking this will validate against all DS ids from the content model, with the addional ones mentioned above '),
   );

   $form['islandora_utilities_validate_object_settings']['islandora_utilities_validate_object_settings_datastream_size_zero'] = array(
     '#type' => 'checkbox',
      '#title' => t('Check for zero byte datastreams (-1)'),
      '#default_value' => variable_get('islandora_utilities_validate_object_settings_datastream_size_zero', ''),
   );

  $query = "select \$object from <#ri>
    where
    \$object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
    and \$object <fedora-model:hasModel> <info:fedora/fedora-system:ContentModel-3.0

  order by \$object";

  $connection = new IslandoraTuque();

  $form['islandora_utilities_remote'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remote fedora settings'),
    '#description' => t("Used for importing and exporting objects and datastream between two fedora instances"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['islandora_utilities_remote']['islandora_utilities_remote_islandora_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable remote connection'),
    '#default_value' => variable_get('islandora_utilities_remote_islandora_enable', ''),
    '#description' => t('Do we need this? Recommend turing this off when not in use'),
  );

  $form['islandora_utilities_remote']['islandora_utilities_remote_islandora_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote fedora base URL'),
    '#default_value' => variable_get('islandora_utilities_remote_islandora_base_url', ''),
    '#description' => t('Rest API'),
  );

  $form['islandora_utilities_remote']['islandora_utilities_remote_islandora_drupal_filter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use drupal filter'),
    '#default_value' => variable_get('islandora_utilities_remote_islandora_drupal_filter', ''),
    '#description' => t('If checked, the import / export functionality will use your drupal user / password for authentication'),
  );
  $form['islandora_utilities_remote']['islandora_utilities_remote_islandora_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote fedora username'),
    '#default_value' => variable_get('islandora_utilities_remote_islandora_username', ''),
  );

  $form['islandora_utilities_remote']['islandora_utilities_remote_islandora_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote fedora password'),
    '#description' => t('WARNING! This password is stored in plain text form.'),
    '#default_value' => variable_get('islandora_utilities_remote_islandora_password', ''),
  );

  $form['islandora_utilities_remote']['islandora_utilities_remote_validate_connection'] = array(
    '#type' => 'item',
    '#title' => t('Validate remote connection'),
    '#markup' => $confirmation_message,
  );



  return system_settings_form($form);//$form;

}

function islandora_utilities_validate_object_settings_add_content_model($form, &$form_state) {
  //do some processing

}


function islandora_utilities_settings_submit($form, &$form_state) {
  /*
  dsm($form_state['input'],'ff');
  $values = $form_state['input'];
  dsm($values, 'values');

  variable_set('islandora_utilities_remote_fedora_base_url', $values['fedora_base_url']);

  if($values['fedora_base_url']) {
     variable_set('islandora_utilities_remote_fedora_base_url', $values['fedora_base_url']);
  }

  if($values['fedora_username']) {
     variable_set('islandora_utilities_remote_fedora_username', $values['fedora_username']);
  }

  if($values['drupal_filter']) {
     variable_set('islandora_utilities_remote_fedora_drupal_filter', $values['drupal_filter']);
  }
  else {
    variable_set('islandora_utilities_remote_fedora_drupal_filter', NULL);
  }

  if($values['fedora_password']) {
     variable_set('islandora_utilities_remote_fedora_password', $values['fedora_password']);
  }


  return $form;
  */
}