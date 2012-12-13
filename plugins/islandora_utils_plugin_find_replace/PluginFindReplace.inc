<?php

module_load_include('inc', 'islandora_utils', 'IslandoraUtilsPlugin');
class PluginFindReplace extends IslandoraUtilsPlugin {

  protected $pid;
  protected $fedora_object;
  protected $form_state;


  /**
   * Constructor
   */
  function PluginFindReplace($pid = NULL, $form_state = NULL) {
    //parent::__construct($fedora_object); //?? need this
    if (isset($pid)) {
      $this->pid = $pid;
    }

    if (isset($form_state)) {
      $this->form_state = $form_state;
    }
  }


  public function process() {
    module_load_include('inc', 'islandora', 'includes/IslandoraTuque');
    module_load_include('inc', 'islandora', 'includes/utilities');


    dsm('find replace process function');
    dsm($this->form_state,'ffffffffgggggg');
    $values = $this->form_state['values'];
  }



}

