<?php

module_load_include('inc', 'islandora_utils', 'IslandoraUtilsPlugin');
class ExportObjectPlugin extends IslandoraUtilsPlugin {

  protected $pid;
  protected $fedora_object;
  protected $form_state;


  /**
   * Constructor
   */
  function ExportObjectPlugin($pid = NULL, $form_state = NULL) {
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



    dsm($this->form_state,'ffffffffgggggg');
    $values = $this->form_state['values'];


    if($values['islandora_utils_plugin_export_download']) {
      //download object to computer

    }

    if($values['islandora_utils_plugin_export_ingest_remote']) {
      if($this->pid) {
        $this->fedora_object = islandora_object_load($this->pid);
      }

      //ingest local object in remote fedora
      if(variable_get('islandora_utils_remote_islandora_base_url', '')) {
        $url = variable_get('islandora_utils_remote_islandora_base_url', '');
        if(!variable_get('islandora_utils_remote_islandora_drupal_filter', '')) {
          $remote_user = new stdClass();
          $remote_user->name = variable_get('islandora_utils_remote_islandora_username', 'fedoraAdmin');
          $remote_user->pass = variable_get('islandora_utils_remote_islandora_password', 'fedoraAdmin');
          $remote_user->uid = -1;
        }
        else {
          $remote_user = NULL;
        }

        $remote_connection = new IslandoraTuque($remote_user, $url);
        $export = $this->fedora_object->repository->api->m->export($this->fedora_object->id);
        $params['string'] = $export;
        $bob = $remote_connection->api->m->ingest($params);
        dsm($bob,'bob response');
      }
      else {
        dsm("can't ingest");
      }
    }
    if($values['islandora_utils_plugin_export_ingest_local']) {
      dsm('ingest local');
      //functionize this bit of code
      if(variable_get('islandora_utils_remote_islandora_base_url', '')) {
        $url = variable_get('islandora_utils_remote_islandora_base_url', '');
        if(!variable_get('islandora_utils_remote_islandora_drupal_filter', '')) {
          $remote_user = new stdClass();
          $remote_user->name = variable_get('islandora_utils_remote_islandora_username', 'fedoraAdmin');
          $remote_user->pass = variable_get('islandora_utils_remote_islandora_password', 'fedoraAdmin');
          $remote_user->uid = -1;
        }
        else {
          $remote_user = NULL;
        }

        try {



        } catch (Exception $exc) {
          dsm($exc, 'opps');
        }

        $remote_connection = new IslandoraTuque($remote_user, $url);
        $export = $remote_connection->api->m->export('codearl:top');

        $local_connection = new IslandoraTuque();
        $params['string'] = $export;
        $bob = $local_connection->api->m->ingest($params);
        dsm($bob,'bobbbbbbbbbb');

      }
      else {
        dsm("can't ingest");
      }


    }
  }



}

