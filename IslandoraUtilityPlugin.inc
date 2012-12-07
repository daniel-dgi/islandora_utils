<?php

class IslandoraUtilityPlugin {
 
  protected $pid;
  
  protected $results = array();
  
  
  /**
   * Constructor
   */
  function IslandoraUtilityPlugin($pid) {
    $this->pid = $pid;
  }


  protected function save_result($result = array()) {
    if(isset($result['type']) && isset($result['pid']) && isset($result['message'])) {
      $this->results[] = $result;
    }
  }
  
  private function process($pid) {
    
  }
  
}