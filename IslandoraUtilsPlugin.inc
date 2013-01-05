<?php

class IslandoraUtilsPlugin {

  protected $pid;
  protected $fedora_object;
  protected $form_state;
  protected $results = array();


  /**
   * Constructor
   */
  function IslandoraUtilsPlugin($pid = NULL, $form_state = NULL) {
    module_load_include('inc', 'islandora', 'includes/tuque');

    if (isset($pid)) {
      $this->pid = $pid;
      $this->fedora_object = islandora_object_load($pid);
    }

    if (isset($form_state)) {
      $this->form_state = $form_state;
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