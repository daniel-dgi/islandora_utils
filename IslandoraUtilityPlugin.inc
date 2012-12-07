<?php

class IslandoraUtilityPlugin {

  protected $fedora_object;

  protected $results = array();


  /**
   * Constructor
   */
  function IslandoraUtilityPlugin($fedora_object = NULL) {
    if (isset($fedora_object)) {
      $this->$fedora_object = $fedora_object;
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