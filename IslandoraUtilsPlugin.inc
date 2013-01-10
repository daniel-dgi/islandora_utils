<?php

class IslandoraUtilsPlugin {

  protected $pid;
  protected $fedora_object;
  protected $form_state;
  protected $context;
  protected $results = array();
  protected $local_connection;
  protected $remote_connection;

  /**
   * Constructor
   */
  function IslandoraUtilsPlugin($pid = NULL, &$form_state = NULL, &$context = NULL) {
    module_load_include('inc', 'islandora', 'includes/tuque');

    if (isset($pid)) {
      $this->pid = $pid;
      $this->fedora_object = islandora_object_load($pid);
    }

    if (isset($form_state)) {
      $this->form_state = $form_state;
    }

    if (isset($context)) {
      $this->context = $context;
    }

    $this->local_connection = new IslandoraTuque();

    if(variable_get('islandora_utils_remote_islandora_base_url', '')) {
      $remote_url = variable_get('islandora_utils_remote_islandora_base_url', '');
      if(!variable_get('islandora_utils_remote_islandora_drupal_filter', '')) {
        $remote_user = new stdClass();
        $remote_user->name = variable_get('islandora_utils_remote_islandora_username', NULL);
        $remote_user->pass = variable_get('islandora_utils_remote_islandora_password', NULL);
        if(!$remote_user->name) {
          $remote_user->uid = 0;
        }
        else {
          $remote_user->uid = -1;
        }
      }
      else {
        $remote_user = NULL;
      }

      try {
        $this->remote_connection = new IslandoraTuque($remote_user, $remote_url);
      } catch (Exception $exc) {
        $this->remote_connection = NULL;
      }
    }
    else {
      $this->remote_connection = NULL;
    }
  }

  public function process() {
    dsm('empty processing function');
  }

  protected function save_result($result = array()) {
    if(isset($result['type']) && isset($result['pid']) && isset($result['message'])) {
      $this->results[] = $result;
    }
  }

  public function get_results() {
    return $this->results;
  }



}