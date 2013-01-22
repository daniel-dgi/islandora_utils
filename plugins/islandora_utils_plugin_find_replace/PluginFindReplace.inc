<?php

module_load_include('inc', 'islandora_utils', 'IslandoraUtilsPlugin');
class PluginFindReplace extends IslandoraUtilsPlugin {

  /**
   * Constructor
   */
  function PluginFindReplace($pid = NULL, $form_state = NULL) {
    parent::__construct($pid, $form_state);
  }


  public function process() {
    dd($this->pid, 'pppppDDDDD');
    module_load_include('inc', 'islandora', 'includes/IslandoraTuque');
    module_load_include('inc', 'islandora', 'includes/utilities');


    $this->results[] = array(
      'type' => 'error',
      'pid' => $this->pid,
      'dsid' => 'n/a',
      'message' => 'find...replace... test',
    );
  }



}

