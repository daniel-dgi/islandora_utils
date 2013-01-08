<?php

module_load_include('inc', 'islandora_utils', 'IslandoraUtilsPlugin');
class ExportObjectPlugin extends IslandoraUtilsPlugin {

  /**
   * Constructor
   */
  function ExportObjectPlugin($pid = NULL, $form_state = NULL, &$context = NULL) {
    parent::__construct($pid, $form_state, $context);
  }


  public function process() {
    module_load_include('inc', 'islandora', 'includes/IslandoraTuque');
    module_load_include('inc', 'islandora', 'includes/utilities');

    $values = $this->form_state['values'];

    if($values['islandora_utils_plugin_export_download']) {
      if(!isset($this->context['results']['plugin_export']['download_file_dir'])) {
        $file_dir =  'public://islandora_utils_plugin_export-' . date('Y-m-d-h-i-s');
        drupal_mkdir($file_dir);
        //$file_name = 'islandora_utils_export'.date('Y-m-d-h:i:s').'.txt';
        //$file_dir = variable_get('file_public_path', file_directory_temp());
        //$results_file = file_create_filename($file_name, $file_dir);

        $this->context['results']['plugin_export']['download_file_dir'] = $file_dir;
      }

      if(!isset($this->context['results']['plugin_export']['file_list'])) {
        $this->context['results']['plugin_export']['file_list'] = array();
      }
      $datastreams = $this->fedora_object->repository->api->a->listDatastreams($this->fedora_object->id);

      foreach ($datastreams as $dsid => $datastream) {
        $file_name = $this->fedora_object->id . '-' . $dsid;
        $this->context['results']['plugin_export']['file_list'][] = array(
          'pid' => $this->fedora_object->id,
          'dsid' => $dsid,
          'label' => $datastream['label'],
          'mimetype' => $datastream['mimetype'],
          'filename' => $file_name,
        );
        $ds_content = $this->fedora_object->repository->api->a->getDatastreamDissemination($this->fedora_object->id, $dsid);
        file_put_contents($this->context['results']['plugin_export']['download_file_dir'] . '/'. $file_name, $ds_content);
      }
    }
    /*
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
     *
     */
  }



}

