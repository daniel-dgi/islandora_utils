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
    if (isset($pid)) {
      $this->pid = $pid;
    }

    if (isset($form_state)) {
      $this->form_state = $form_state;
    }
  }

  public function process() {
    dsm('getting here and there');
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